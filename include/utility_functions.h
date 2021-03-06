#ifndef __UTILITYFUNC_H_INCLUDED_
#define __UTILITYFUNC_H_INCLUDED_

#include "osc.h"
#include "main.h"

void updateLEDs();
void incrementIndex(int *program_index1, uint16_t *total_steps1, int *increment_by1);

void setHue1(uint8_t h);
void setSaturation1(uint8_t s);
void setValue1(uint8_t v);
void setInterval(uint8_t i);
void setSlope(float s);

void changeLEDProgram(OSCMessage &msg, int addrOffset );
void changeValue(OSCMessage &msg, int addrOffset );
void changePreset(OSCMessage &msg, int addrOffset );
void changeColorPreset(OSCMessage &msg, int addrOffset );
void setColorPreset(uint8_t color_preset);
void setColor1(uint8_t x, uint8_t y);
void allLedsOff();
void allLedsOn();
void setGlobalBrightness(uint8_t brightness);

#endif
