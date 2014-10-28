/*
* matrix_key.ino
*
* Created: 10/25/2014 8:32:30 PM
* Author: kamiya
*/
#define SCAN_DELAY 1

#define COMMAND_READALL 'r'

uint8_t cols[10] = {
	3,4,5,6,7,
	8,9,10,11,12,
};
uint8_t rows[6] = {14,15,16,17,18,19};//A0~A5
//各列のデータを保持しておく（送信前に変換
uint8_t button_inputs[10];
//
uint8_t button_datas[60];//押されている番号がtrue

uint8_t cols_size = 10;
uint8_t rows_size = 6;


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
			button_datas[index] = (button_inputs[j] >> (i + 1)) & 0x1;//i + 1で最上段を読み飛ばす
		}
		//上段特殊ボタン(インデックスの方向が逆（実機左から50,51,...59)
		uint8_t up_index = 59 - j;
		button_datas[up_index] = button_inputs[j] & 0b1;
	}
}
void command_readall(){
	for(uint8_t i = 0 ; i < 60 ; ++i){
		if(button_datas[i]){
			Serial.println(i,DEC);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void setup(){
	button_init();
	Serial.begin(BAUDRATE);
}
void loop(){
	button_scan();
	button_decode();
	if(Serial.available()){
		switch(Serial.read()){
			case COMMAND_READALL:
				command_readall();
				break;
			default:
				break;
		}
	}
}
