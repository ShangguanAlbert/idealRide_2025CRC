#ifndef __BSP_SPEAKER_H
#define __BSP_SPEAKER_H
#include "stm32f4xx.h"

void GPIO_VOICE_Init(void);
void sound(uint8_t x);
void sound_platform(uint8_t platform_num);
void sound_number(uint8_t number);
void sound_ready(void);
void sound_vertical_spot(void);
void sound_clue_a(void);
void sound_clue_b(void);
void sound_find_treasure(void);

#endif
