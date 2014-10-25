#include "eVY1.h"
#include "eVY1Data.h"
#include "midiData.h"
#include "ColorLCDShield.h"
#include "eVocaloidPhoneticAlphabets.h"

eVY1 evy1(&Serial);
LCDShield lcd;

void setup() {
	Serial.begin(31250);
	lcd.init(PHILLIPS);
	lcd.contrast(-64);
	lcd.clear(GREEN);
	delay(2000); // wait for the shield to wake up
}
uint8_t note_nums[] = {
	48,50,52,53,55,57,59,
	60,62,64,65,67,69,71,//60=440Hz
	72,74,76,77,79,81,83,
	84,86,88,89,91,83,85,
};

void loop () {
	int channel = 0;
	char** pas[] = {
		pa_base,pa_mark,pa_halfmark,
		pa_extra_a,pa_extra_i,pa_extra_u,pa_extra_e,pa_extra_o,
		pa_markextra_vowel,pa_markextra_y,
		pa_halfmarkextra_vowel,pa_halfmarkextra_y
	};
	evy1.controlChange(channel, MIDI_CC_PORTAMENTO, 0x7f); // Portamento On
	evy1.controlChange(channel, MIDI_CC_PORTAMENTO_TIME, 0x7f); // Portament Time
	//evy1.pitchBend(0,i);
	for(int j = 0 ; j < 12 ; ++j){
		for(int i = 0 ; i < PA_BASE_SIZE; ++i){
			evy1.eVocaloid(0,pa_extra_yu[i]);
			evy1.noteOn(channel, 0x3c, 0x3f);//3c
			delay(250);
			evy1.noteOn(channel,0x3c,0x0);
			delay(100);
		}
	}
}
