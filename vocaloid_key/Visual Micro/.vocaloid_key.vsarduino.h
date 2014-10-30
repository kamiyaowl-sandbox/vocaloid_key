/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Arduino Mega 2560 or Mega ADK, Platform=avr, Package=arduino
*/

#define __AVR_ATmega2560__
#define ARDUINO 101
#define ARDUINO_MAIN
#define F_CPU 16000000L
#define __AVR__
#define __cplusplus
extern "C" void __cxa_pure_virtual() {;}

void button_init();
void button_scan();
void button_decode();
void get_button_trigger(uint8_t source,uint8_t* input_buf,uint8_t* result);
void matrix_button_trigger();
//
void talk_release( int channel );
void update_matrixkey();
//

#include "C:\Program Files (x86)\Arduino\hardware\arduino\variants\mega\pins_arduino.h" 
#include "C:\Program Files (x86)\Arduino\hardware\arduino\cores\arduino\arduino.h"
#include "C:\Users\e10108\repos\vocaloid_key\vocaloid_key\vocaloid_key.ino"
#include "C:\Users\e10108\repos\vocaloid_key\vocaloid_key\eVY1.cpp"
#include "C:\Users\e10108\repos\vocaloid_key\vocaloid_key\eVY1.h"
#include "C:\Users\e10108\repos\vocaloid_key\vocaloid_key\eVY1Data.h"
#include "C:\Users\e10108\repos\vocaloid_key\vocaloid_key\eVocaloidPhoneticAlphabets.h"
#include "C:\Users\e10108\repos\vocaloid_key\vocaloid_key\midiClass.cpp"
#include "C:\Users\e10108\repos\vocaloid_key\vocaloid_key\midiClass.h"
#include "C:\Users\e10108\repos\vocaloid_key\vocaloid_key\midiData.h"
