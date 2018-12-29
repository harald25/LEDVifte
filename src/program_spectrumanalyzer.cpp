#include "program_spectrumanalyzer.h"

// The display size and color to use
const unsigned int matrix_width = 11;
const unsigned int matrix_height = 32;

float level_old[matrix_width];
float smoothing_coeff_positive = 1.0; // experiment with different coefficients; --> 0.0 < smoothing_coeff < 1.0
float smoothing_coeff_negative = 0.1;

// These parameters adjust the vertical thresholds
const float maxLevel = 0.5;      // 1.0 = max, lower is more "sensitive"
const float dynamicRange = 40.0; // total range to display, in decibels
const float linearBlend = 0.3;   // useful range is 0 to 0.7


// Audio library objects
AudioInputAnalog         adc1(A3);       //xy=99,55
AudioAnalyzeFFT1024      fft;            //xy=265,75
AudioConnection          patchCord1(adc1, fft);


// This array holds the volume level (0 to 1.0) for each
// vertical pixel to turn on.  Computed in setup() using
// the 3 parameters above.
float thresholdVertical[matrix_height];

// This array specifies how many of the FFT frequency bin
// to use for each horizontal pixel.  Because humans hear
// in octaves and FFT bins are linear, the low frequencies
// use a small number of bins, higher frequencies use more.
int frequencyBinsHorizontal[matrix_width] = {
   1, 1, 2, 3, 4, 5, 6, 8, 10, 16, 25
};


// A simple xy() function to turn display matrix coordinates
// into the index numbers OctoWS2811 requires.  If your LEDs
// are arranged differently, edit this code...
unsigned int xy(unsigned int x, unsigned int y) {
  switch(x)
  {
    case(0):
      return 31 - y;
      break;
    case(1):
      return 32 + y;
      break;
    case(2):
      return 95 - y;
      break;
    case(3):
      return 96 + y;
      break;
    case(4):
      return 159 - y;
      break;
    case(5):
      return 160 + y;
      break;
    case(6):
      return 223 - y;
      break;
    case(7):
      return 224 + y;
      break;
    case(8):
      return 287 - y;
      break;
    case(9):
      return 288 + y;
      break;
    case(10):
      return 351 - y;
      break;
    default:
      break;

  }
}

void spectrumAnalyzer()
{
  active_program = SPECTRUM_ANALYZER;
  // compute the vertical thresholds before starting
  computeVerticalLevels();
  interval = 0;
  update = true;
  saturation1 = 255;
  value1 = 175;
}

void spectrumAnalyzerUpdate()
{
  unsigned int x, y, freqBin;
  float level;
  uint8_t gradientIndex;



  if (fft.available()) {
    // freqBin counts which FFT frequency data has been used,
    // starting at low frequency
    freqBin = 0;

    for (x=0; x < matrix_width; x++) {
      // get the volume for each horizontal pixel position
      level = fft.read(freqBin, freqBin + frequencyBinsHorizontal[x] - 1);
      //level = smoothing_coeff * level + (1.0 - smoothing_coeff) * level_old[x];

      float diff = level - level_old[x];
      if (diff > 0.0) {
        level = smoothing_coeff_positive * level + (1.0 - smoothing_coeff_positive) * level_old[x];
        //level = level + diff * smoothing_coeff_positive;
      } else {
        level = smoothing_coeff_negative * level + (1.0 - smoothing_coeff_negative) * level_old[x];
        //level = level + diff * smoothing_coeff_negative;
      }


      // uncomment to see the spectrum in Arduino's Serial Monitor
      // Serial.print(level);
      // Serial.print("  ");

      for (y=0; y < matrix_height; y++) {
        // for each vertical pixel, check if above the threshold
        // and turn the LED on or off
        gradientIndex = (matrix_height-y-2)*7.9;
        if (((matrix_height-y-2)*7.9) > 254) {
          gradientIndex = 254;
        }
        if (level >= thresholdVertical[y]) {
          leds[xy(x,y)] = CHSV(gradientIndex,saturation1,value1);
          leds[xy(x,y)+352] = CHSV(gradientIndex,saturation1,value1);
          //leds[xy(x,y)] = ColorFromPalette(spectrumGradient2,gradientIndex);
          //leds[xy(x,y)+352] = ColorFromPalette(spectrumGradient2,gradientIndex);
        } else {
          leds[xy(x,y)] = CRGB::Black;
          leds[xy(x,y)+352] = CRGB::Black;
        }
      }
      // increment the frequency bin count, so we display
      // low to higher frequency from left to right
      freqBin = freqBin + frequencyBinsHorizontal[x];
      level_old[x] = level;
    }
    // after all pixels set, show them all at the same instant
    FastLED.show();
    // Serial.println();
  }
}

// Run once from setup, the compute the vertical levels
void computeVerticalLevels() {
  unsigned int y;
  float n, logLevel, linearLevel;

  for (y=0; y < matrix_height; y++) {
    n = (float)y / (float)(matrix_height - 1);
    logLevel = pow10f(n * -1.0 * (dynamicRange / 20.0));
    linearLevel = 1.0 - n;
    linearLevel = linearLevel * linearBlend;
    logLevel = logLevel * (1.0 - linearBlend);
    thresholdVertical[y] = (logLevel + linearLevel) * maxLevel;
  }
}
