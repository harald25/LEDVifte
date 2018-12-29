#ifndef __SPECTRUMANALYZER_H_INCLUDED_
#define __SPECTRUMANALYZER_H_INCLUDED_
#include "main.h"

void spectrumAnalyzer();
void spectrumAnalyzerUpdate();
void computeVerticalLevels();
void changePalette(uint8_t palette_number);
enum spectrum_mode { RAINBOW, GRADIENT};
extern spectrum_mode spec_mode;


#endif
