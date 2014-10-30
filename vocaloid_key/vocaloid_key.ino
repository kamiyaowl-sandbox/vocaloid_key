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

#define PORTAMENT_ON 0x7f
#define PORTAMENT_OFF 0x0

SoftwareSerial swserial(2,3);
eVY1 evy1(&swserial);


uint8_t cols[10] = {
	32,34,36,38,40,
	42,44,46,48,50,
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

const uint8_t tone_fader_pin = A15;
const uint8_t portament_fader_pin = A14;

const uint8_t pedal2_pin = 45;
const uint8_t holdpedal_pin = 47;
const uint8_t portsw_pin = 49;
const uint8_t portswled_pin = 51;

uint8_t holdpedal_input = 0;
uint8_t holdpedal_trigger = TRIGGER_NONE;

uint8_t portament_enable = PORTAMENT_ON;
uint8_t portsw_input = 0;
uint8_t portsw_trigger = TRIGGER_NONE;

uint8_t portament_time = 0x0;

#define PA_MODE_NONE			0
#define PA_MODE_EX_A			50
#define PA_MODE_EX_I			51
#define PA_MODE_EX_U			52
#define PA_MODE_EX_E			53
#define PA_MODE_EX_O			54
#define PA_MODE_EX_YA			55
#define PA_MODE_EX_YU			56
#define PA_MODE_EX_YO			57
#define PA_MODE_MARK			58
#define PA_MODE_HALFMARK		59

uint16_t pa_mode = PA_MODE_NONE;
uint16_t pa_mode_sub = PA_MODE_NONE;
char **single_pa_table[] = {
    pa_extra_a,
    pa_extra_i,
    pa_extra_u,
    pa_extra_e,
    pa_extra_o,
    pa_extra_ya,
    pa_extra_yu,
    pa_extra_yo,
    pa_mark,
    pa_halfmark,    
};
char **double_pa_table[][8] = {
    {
        pa_markextra_vowel,//a
        pa_markextra_vowel,//i
        pa_markextra_vowel,//u
        pa_markextra_vowel,//e
        pa_markextra_vowel,//o
        pa_markextra_y,//ya
        pa_markextra_y,//yu
        pa_markextra_y,//yo
    },    {
        pa_halfmarkextra_vowel,//a
        pa_halfmarkextra_vowel,//i
        pa_halfmarkextra_vowel,//u
        pa_halfmarkextra_vowel,//e
        pa_halfmarkextra_vowel,//o
        pa_halfmarkextra_y,//ya
        pa_halfmarkextra_y,//yu
        pa_halfmarkextra_y,//yo
    },      
};

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
void get_button_trigger(uint8_t source,uint8_t* input_buf,uint8_t* result){
	*input_buf = (*input_buf << 1) | source;
	if(*result == TRIGGER_NONE && *input_buf == 0xff){
		*result = TRIGGER_PUSH;
		
		} else if(*result == TRIGGER_PUSH && *input_buf != 0){
		*result = TRIGGER_RELEASE;

		} else if(*result == TRIGGER_RELEASE && *input_buf == 0){
		*result = TRIGGER_NONE;
	}
}


void matrix_button_trigger(){
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
	
	pinMode(pedal2_pin,INPUT_PULLUP);
	pinMode(holdpedal_pin,INPUT_PULLUP);
	pinMode(portsw_pin,INPUT_PULLUP);
	pinMode(portswled_pin,OUTPUT);
	
	
	swserial.begin(EVY1_BAUD);
	button_init();
	delay(5000); // wait for the shield to wake up
	
	/* wakeup call */
	const int duration = 500;
	evy1.eVocaloid(0,"S i");
	evy1.noteOn(0, 0x3c, 0x3f);//3c
	delay(duration);
	
	evy1.eVocaloid(0,"m a");
	evy1.noteOn(0, 0x40, 0x3f);//3c
	delay(duration);

	evy1.eVocaloid(0,"k e");
	evy1.noteOn(0, 0x43, 0x3f);//3c
	delay(duration);
	evy1.noteOn(0,0x3c,0x0);

	evy1.eVocaloid(0,"M");
	evy1.noteOn(0, 0x4f, 0x3f);//3c
	delay(duration);
	evy1.noteOn(0,0x4f,0x0);

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
unsigned long talk_release_span = 10000;

void talk_release( int channel )
{
	evy1.noteOn(channel,talk_tone,0x0);
	talk_tone = TONE_NONE;
}

void update_matrixkey()
{
	button_scan();
	button_decode();
	matrix_button_trigger();
}

void loop () {
	
	int channel = 0;
	evy1.controlChange(channel, MIDI_CC_PORTAMENTO, portament_enable); // Portamento On
	evy1.controlChange(channel, MIDI_CC_PORTAMENTO_TIME, portament_time); // Portament Time
	//evy1.pitchBend(0,i);
	
	/* key read command */
	update_matrixkey();
	
	/* portament */
	get_button_trigger(!digitalRead(portsw_pin),&portsw_input,&portsw_trigger);
	if(portsw_trigger == TRIGGER_PUSH){
		portament_enable = (portament_enable == PORTAMENT_OFF) ? PORTAMENT_ON : PORTAMENT_OFF;
	}
	portament_time = analogRead(portament_fader_pin) >> 3;
	digitalWrite(portswled_pin,!portament_enable);
	
	/* pedal */
	get_button_trigger(!digitalRead(holdpedal_pin),&holdpedal_input,&holdpedal_trigger);
	
	for(uint8_t i = 0 ; i < button_size ; ++i){
		//一番番号のでかいボタンを優先
		if(button_triggers[i] == TRIGGER_PUSH) {
			button_input = i;
			Serial.println(i,DEC);
		}
	}
	
	/* set data */
	if(talk_data == NULL && button_input != BUTTON_NONE){
		//五十音->そのまま返す
		//小子音or濁点->テーブルを該当子音に置換
		//小子音and濁点->更にテーブルを置換
		if(button_input > 49) {
            if(pa_mode == PA_MODE_NONE){
                pa_mode = button_input;
            } else if(pa_mode != button_input){
                if(pa_mode_sub != button_input 
                    //組み合わせ不能なペアの除去
					&& (!((pa_mode == PA_MODE_MARK && button_input == PA_MODE_HALFMARK) || (button_input != PA_MODE_MARK || pa_mode == PA_MODE_HALFMARK)))
                    && (((pa_mode == PA_MODE_MARK || pa_mode == PA_MODE_HALFMARK) && (button_input != PA_MODE_MARK || button_input != PA_MODE_HALFMARK))
						|| ((pa_mode != PA_MODE_MARK || pa_mode != PA_MODE_HALFMARK) && (button_input == PA_MODE_MARK || button_input == PA_MODE_HALFMARK)))
                    ){
                    pa_mode_sub = button_input;
                } else {
                    pa_mode_sub = PA_MODE_NONE;
                }
            } else {
                pa_mode = PA_MODE_NONE;
            }
        }else if(button_input < 50) {
            if(pa_mode == PA_MODE_NONE && pa_mode_sub == PA_MODE_NONE){
                talk_data = pa_base[button_input];
            } else if(pa_mode != PA_MODE_NONE && pa_mode_sub == PA_MODE_NONE){
                talk_data = single_pa_table[pa_mode - PA_MODE_EX_A][button_input];
                pa_mode = PA_MODE_NONE;
            } else if(pa_mode != PA_MODE_NONE && pa_mode_sub != PA_MODE_NONE){
				uint8_t mark = pa_mode > pa_mode_sub ? pa_mode : pa_mode_sub;//濁点のほうがボタン番号が後とかいうゴミ実装
				uint8_t index = ((pa_mode > pa_mode_sub) ? pa_mode_sub : pa_mode) - PA_MODE_EX_A;

				talk_data = double_pa_table[mark == PA_MODE_HALFMARK][index][button_input];

				pa_mode = PA_MODE_NONE;
				pa_mode_sub = PA_MODE_NONE;
            }                
        }            
		//Memory
		button_input_last = button_input;
		button_input = BUTTON_NONE;
	}
	/* talk */
	if(talk_data != NULL){
		if(talk_tone != TONE_NONE){
			talk_release(channel);
		}
		/* Tone */
		talk_tone = (analogRead(tone_fader_pin) >> 5) + 56;//音階は適当に計算してる
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
	if((talk_tone != TONE_NONE && holdpedal_trigger == TRIGGER_RELEASE) ||
	(talk_tone != TONE_NONE && millis() - last_talk_tick > talk_release_span)){
		talk_release(channel);
		button_input_last = BUTTON_NONE;
	}
	
}
