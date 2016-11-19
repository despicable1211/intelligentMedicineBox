#ifndef _MP3_CONTROL_H_
#define _MP3_CONTROL_H_

#include "include.h"

#define MP3_UART		UART5

#define MEDICINE_SOUND	0x02
#define BT_SOUND		0x01

typedef enum
{
	STOP,
	PLAY,
	PAUS,
	FF,
	FR,
	UNKNOW
} PlayState;

void mp3_init();
bool play_music(uint8 index);
bool stop_music();
PlayState getPlayState();

#endif