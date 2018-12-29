#include "main.h"

// DEGUGGING
// If set to true, debug messages will be output to serial device
bool debug = false;

int led_array [NUM_LEDS_PER_STRIP*NUM_STRIPS];
CRGB leds [NUM_LEDS_PER_STRIP*NUM_STRIPS];
display_mode displaymode;
direction dir;
program active_program;
int interval;
int last_update;
int program_index1;
int program_index2;
int increment_by1;
int increment_by2;
uint16_t total_steps1;
uint16_t total_steps2;
uint8_t saturation1;
uint8_t value1;
uint8_t hue1;
uint8_t saturation2;
uint8_t value2;
uint8_t hue2;
bool update;
char osc_message_address[18];
float delta;

void setup() {
  Serial.begin(9600);         //Teensy <=> Computer
  SLIPSerial.begin(115200);   //Teensy <=> ESP
  Serial.println("Started");

  FastLED.addLeds<OCTOWS2811>(leds, NUM_LEDS_PER_STRIP);
  FastLED.setCorrection(COLOR_CORRECTION);
  FastLED.setBrightness(GLOBAL_BRIGHTNESS);

  // the audio library needs to be given memory to start working
  AudioMemory(12);
  spectrumAnalyzer();
}

void loop() {
    OSCMsgReceive();
    updateLEDs();
}
