#include "eVY1.h"
#include "eVY1Data.h"
#include "midiData.h"
#include "eVocaloidPhoneticAlphabets.h"
#include <SoftwareSerial.h>

#define SCAN_DELAY 1

#define EVY1_BAUD 31250
#define DEBUG_BAUD 19200

#define BUTTON_INPUT_BUFFER 8

#define FALSE 0
#define TRUE 1

#define TRIGGER_NONE 0
#define TRIGGER_PUSH 1
#define TRIGGER_RELEASE 2

#define TONE_NONE 0

SoftwareSerial swserial(2,3);
eVY1 evy1(&swserial);


uint8_t cols[10] = {
	22,24,26,28,30,
	32,34,36,38,40,
};

uint8_t rows[6] = { 31,33,35,37,39,41 };
//各列のデータを保持しておく（送信前に変換
uint8_t button_inputs[10];

uint8_t button_index = 0;
uint8_t button_datas[60];//button_datas[i] <=  {button_datas[i][7:1], new_data};
uint8_t button_triggers[60];

uint8_t cols_size = 10;
uint8_t rows_size = 6;
uint8_t button_size = 60;

void button_init(){
	for(int i = 0 ; i < cols_size ; ++i){
		pinMode(cols[i],OUTPUT);
		digitalWrite(cols[i],LOW);
	}
	for (int i = 0 ; i < rows_size ; ++i){
		pinMode(rows[i],INPUT);//外部にプルダウン抵抗が必要
	}
}
void button_scan(){
	for(int j = 0 ; j < cols_size ; ++j){
		digitalWrite(cols[j],HIGH);
		delayMicroseconds(SCAN_DELAY);
		uint8_t scan_data = 0x0;
		for (int i = 0 ; i < rows_size ; ++i){
			scan_data |= (digitalRead(rows[i]) & 0x1) << i;
		}
		button_inputs[j] = scan_data;
		digitalWrite(cols[j],LOW);
	}
}
/* After */
//50 51 52 53 54 55 56 57 58 59
//
//45 40 35 30 25 20 15 10  5  0
//46 41 36 31 26 31 16 11  6  1
//47 42 37 32 27 32 17 12  7  2
//48 43 38 33 28 33 18 13  8  3
//49 44 39 34 29 32 19 14  9  4
/* Before */
// 9 8 7 6 5 4 3 2 1 0 :button_states
//======================LSB
// . . . . . . . . . .
//
// . . . . . . . . . .
// . . . . . . . . . .
// . . . . . . . . . .
// . . . . . . . . . .
// . . . . . . . . . .
// . . . . . . . . . .
//======================MSB

void button_decode(){
	for(int j = 0 ; j < cols_size ; ++j){
		//あいう～～～～わをまでの5*10
		for(int i = 0; i < 5 ; ++i){
			uint8_t index = i + j * 5;
			button_datas[index] = (button_datas[index] << 1) | ((button_inputs[j] >> (i + 1)) & 0x1);//i + 1で最上段を読み飛ばす
		}
		//上段特殊ボタン(インデックスの方向が逆（実機左から50,51,...59)
		uint8_t up_index = button_size - 1 - j;
		button_datas[up_index] = (button_datas[up_index] << 1) | (button_inputs[j] & 0b1);
	}
}
void button_create_trigger(){
	for(uint8_t i = 0 ; i < button_size ; ++i){
		if(button_triggers[i] == TRIGGER_NONE && button_datas[i] == 0xff){
			button_triggers[i] = TRIGGER_PUSH;
		} else if(button_triggers[i] == TRIGGER_PUSH && button_datas[i] != 0){
			button_triggers[i] = TRIGGER_RELEASE;
		} else if(button_triggers[i] == TRIGGER_RELEASE && button_datas[i] == 0){
			button_triggers[i] = TRIGGER_NONE;
		}
	}
}

void setup() {
	Serial.begin(DEBUG_BAUD);
	Serial.println("wakeup");
	
	swserial.begin(EVY1_BAUD);
	button_init();
	delay(5000); // wait for the shield to wake up
	
	evy1.eVocaloid(0,pa_base[7]);
	evy1.noteOn(0, 0x3c, 0x3f);//3c
	delay(250);
	evy1.noteOn(0,0x3c,0x0);
	delay(100);

}
uint8_t note_nums[] = {
	48,50,52,53,55,57,59,
	60,62,64,65,67,69,71,//60=440Hz
	72,74,76,77,79,81,83,
	84,86,88,89,91,83,85,
};
#define BUTTON_NONE 0xff
uint8_t button_input = BUTTON_NONE;
uint8_t button_input_last = 60;

char* talk_data = NULL;

uint8_t talk_tone = TONE_NONE;

unsigned long last_talk_tick = 0;
unsigned long talk_release_span = 1000;

void talk_release( int channel )
{
	evy1.noteOn(channel,talk_tone,0x0);
	talk_tone = TONE_NONE;
}

void loop () {
	
	int channel = 0;
	evy1.controlChange(channel, MIDI_CC_PORTAMENTO, 0x7f); // Portamento On
	evy1.controlChange(channel, MIDI_CC_PORTAMENTO_TIME, 0x7f); // Portament Time
	//evy1.pitchBend(0,i);
	
	/* key read command */
	button_scan();
	button_decode();
	button_create_trigger();
	
	for(uint8_t i = 0 ; i < button_size ; ++i){
		//一番番号のでかいボタンを優先
		if(button_triggers[i] == TRIGGER_PUSH) {
			button_input = i;
			Serial.println(i,DEC);
		}
	}
	
	
	/* set data */
	if(button_input_last != button_input && talk_data == NULL && button_input != BUTTON_NONE){
		//test
		if(button_input < 50) talk_data = pa_base[button_input];
		//Memory
		button_input_last = button_input;
		button_input = BUTTON_NONE;
	}
	/* talk */
	if(talk_data != NULL){
		if(talk_tone != TONE_NONE){
			talk_release(channel);
		}
		/* Tone *///62-85
		talk_tone = (analogRead(A15) >> 5) + 56;//0~31
		/* Talk */
		evy1.eVocaloid(0,talk_data);
		evy1.noteOn(channel, talk_tone, 0x3f);//3c

		Serial.print("tone=");
		Serial.print(talk_tone,DEC);
		Serial.print("data=");
		Serial.print(talk_data);
		Serial.println();
		
		//Memory
		last_talk_tick = millis();
		talk_data = NULL;
	}
	/* talk release */
	if(talk_tone != TONE_NONE && millis() - last_talk_tick > talk_release_span){
		talk_release(channel);
		button_input_last = BUTTON_NONE;
	}
	
}
