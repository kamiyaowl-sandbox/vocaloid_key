/*
* eVocaloidPhoneticAlphabets.h
*
* Created: 2014/10/25 17:05:07
*  Author: kamiya
*/


#ifndef EVOCALOIDPHONETICALPHABETS_H_
#define EVOCALOIDPHONETICALPHABETS_H_

#define PA_A 0
#define PA_KA 5
#define PA_GA 5

#define PA_SA 10
#define PA_ZA 10

#define PA_TA 15
#define PA_DA 15

#define PA_NA 20
#define PA_HA 25
#define PA_BA 25
#define PA_PA 25

#define PA_MA 30
#define PA_YA 35
#define PA_RA 40
#define PA_WA 45

#define PA_BASE_SIZE 50

// http://yamaha-webmusic.github.io/nsx1-apps/specs/ANMW820A-001-10-j.pdf
char* pa_base[] = {
	"a",			"i",			"M",			"e",			"o",
	"k a",			"k\' i",		"k M",			"k e",			"k o",
	"s a",			"S i",			"s M",			"s e",			"s o",
	"t a",			"tS i",			"ts M",			"t e",			"t o",
	"n a",			"J i",			"n M",			"n e",			"n o",
	"h a",			"C i",			"p\\ M",		"h e",			"h o",
	"m a",			"m\' i",		"m M",			"m e",			"m o",
	"j a",			"",				"j M",			"",				"j o",
	"4 a",			"4\' i",		"4 M",			"4 e",			"4 o",
	"w a",			"w i",			"",				"w e",			"o",
};
char* pa_mark[] = {
	"",				"",				"",				"",				"",
	"g a",			"g\' i",		"g M",			"g e",			"g o",
	"dz a",			"dZ i",			"dz M",			"dz e",			"dz o",
	"d a",			"dZ i",			"dz M",			"d e",			"d o",
	"",				"",				"",				"",				"",
	"b a",			"b\' i",		"b M",			"b e",			"b o",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
};
char* pa_halfmark[] = {
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"p a",			"p\' i",		"p M",			"p e",			"p o",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
};
char* pa_extra_a[] = {
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"ts a",			"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"p\\ a",		"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
};

char* pa_extra_i[] = {
	"",				"",				"w i",			"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"s i",			"",				"",
	"",				"",				"ts i",			"t\' i",		"",
	"",				"",				"",				"",				"",
	"",				"",				"p\\\' i",		"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
};

char* pa_extra_u[] = {
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"t M",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
};


char* pa_extra_e[] = {
	"",				"j e",			"w e",			"",				"",
	"",				"k\' e",		"",				"",				"",
	"",				"S e",			"",				"",				"",
	"",				"tS e",			"ts e",			"t\' e",			"",
	"",				"J e",			"",				"",				"",
	"",				"C e",			"p\\ e",		"",				"",
	"",				"m\' e",		"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"4\' e",		"",				"",				"",
	"",				"",				"",				"",				"",
};

char* pa_extra_o[] = {
	"",				"",				"w o",			"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"ts o",			"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"p\\ o",		"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
};

char* pa_extra_ya[] = {
	"",				"",				"",				"",				"",
	"",				"k\' a",		"",				"",				"",
	"",				"S a",			"",				"",				"",
	"",				"tS a",			"",			"t\' a",				"",
	"",				"J a",			"",				"",				"",
	"",				"C a",			"p\\\' a",		"",				"",
	"",				"m\' a",		"",				"",				"",
	"",				"4\' a",		"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
};

char* pa_extra_yu[] = {
	"",				"",				"",				"",				"",
	"",				"k\' M",		"",				"",				"",
	"",				"S M",			"",				"",				"",
	"",				"tS M",			"",				"t\' M",		"",
	"",				"J M",			"",				"",				"",
	"",				"C M",			"p\\\' M",		"",				"",
	"",				"m\' M",		"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"4\' M",		"",				"",				"",
	"",				"",				"",				"",				"",
};




char* pa_extra_yo[] = {
	"",				"",				"",				"",				"",
	"",				"k\' o",		"",				"",				"",
	"",				"S o",			"",				"",				"",
	"",				"tS o",			"",				"t\' o",		"",
	"",				"J o",			"",				"",				"",
	"",				"C o",			"",				"",				"",
	"",				"m\' o",		"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"4\' o",		"",				"",				"",
	"",				"",				"",				"",				"",
};

//���ꉹ+���_
char* pa_markextra_vowel[] = {
	"",				"",				"",				"",				"",
	"",				"",				"",				"g\' e",		"",
	"",				"",				"dz i",			"",				"",
	"",				"",				"",				"dZ e",			"",
	"",				"d\' i",		"",				"d\' e",		"d M",//�ǂ�(�ł�)
	"",				"",				"",				"b\' e",		"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
};

//���q��+���_
char* pa_markextra_y[] = {
	"",				"",				"",				"",				"",
	"N\' a",		"",				"g\' M",		"",				"N\' o",
	"dZ a",			"",				"dZ M",			"",				"dZ o",
	"d\' a",		"",				"d\' M",		"",				"d\' o",
	"",				"",				"",				"",				"",
	"b\' a",		"",				"b\' M",		"",				"b\' o",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
};

//���ꉹ+�����_
char* pa_halfmarkextra_vowel[] = {
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"p\' e",		"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
};
//���q��+�����_
char* pa_halfmarkextra_y[] = {
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"p\' a",		"",				"p\' M",		"",				"p\' o",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
	"",				"",				"",				"",				"",
};

#endif /* EVOCALOIDPHONETICALPHABETS_H_ */