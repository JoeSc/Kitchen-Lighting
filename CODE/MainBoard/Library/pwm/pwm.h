/*
 *  pwm.h
 *  TEST
 *
 *  Created by Joe  Schaack on 8/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#include "lpc111x.h"

void initPWM ( uint8_t bits_of_timer , uint8_t timerNum , uint8_t bitmask, uint32_t period , uint32_t prescaler);
void setPWM ( uint8_t bits_of_timer , uint8_t timerNum , uint8_t chn , uint32_t val);



   //