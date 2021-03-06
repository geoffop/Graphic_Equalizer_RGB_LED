#include <FastGPIO.h>
#include "FastLED.h"
// How many leds in your strip?
#define NUM_LEDS 39 //define how many LEDs are in series
#define DATA_PIN 3 //what pin does data go out
byte analogPin = 0; // read from multiplexer using analog input 0
byte strobePin = 2; // strobe is attached to digital pin 2
byte resetPin = 3; // reset is attached to digital pin 3
byte spectrumValue[8]; // to hold a2d values


CRGB leds[NUM_LEDS];//
byte lead_dot = 0;
void setup() {
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);//add TYPE,DATA PIN, RGB(COLOR CONFIG, ARRAY LEDS)
  FastGPIO::Pin<2>::setOutput(LOW);//start LOW
}
void loop() {
  FastGPIO::Pin<2>::setOutput(HIGH);//HIGH reset
  FastGPIO::Pin<2>::setOutput(LOW);//LOW reset
  for (byte j = 0; j <= 7; j++) {
    FastGPIO::Pin<4>::setOutput(LOW);
    delayMicroseconds(30); // to allow the output to settle
    spectrumValue[j] = map(analogRead(analogPin), 20, 1023, 0, 255);//Map analog input to 8bit from 10bit
    FastGPIO::Pin<4>::setOutput(HIGH);//increase count in IC
  }
  //if below noise level don't show on LED array
  if (spectrumValue[0] <= 17 && spectrumValue[1] <= 17 && spectrumValue[2] <= 17 && spectrumValue[3]  <= 17 && spectrumValue[4] <= 17 && spectrumValue[5] <= 17) {
    blank();
  }
  else {
    for (byte i = 0; i <= 5; i++)//first color
    {
      leds[i] = CRGB::Gold;
      leds[i].nscale8_video(spectrumValue[0]);//scale brightness
    }
    for (byte i = 6; i <= 11 ; i++)//second color
    {
      leds[i] = CRGB::Blue;
      leds[i].nscale8_video(spectrumValue[1]);//scale brightness
    }
    for (byte i = 12; i <= 18 ; i++)//third color
    {
      leds[i] = CRGB::Red;
      leds[i].nscale8_video(spectrumValue[2]);//scale brightness
      FastLED.show();//update output
    }
    for (byte i = 19; i <= 24 ; i++)//forth color
    {
      leds[i] = CRGB::Green;
      leds[i].nscale8_video(spectrumValue[3]);//scale brightness
    }
    for (byte i = 25; i <= 30 ; i++)//fifth color
    {
      leds[i] = CRGB::Purple;
      leds[i].nscale8_video(spectrumValue[4]);//scale brightness
    }
    for (byte i = 31; i <= 39 ; i++)//sixth color
    {
      leds[i] = CRGB::Yellow;
      leds[i].nscale8_video(constrain(spectrumValue[5] + spectrumValue[6] - 20, 0, 255));//scale brightness & combine two
      FastLED.show();
    }
  }
}
void blank() {
  for (int i = 0; i <= 39 ; i++)//set all to black
  {
    leds[i] = CRGB::Black;
    leds[i].nscale8_video(0);
    FastLED.show();
  }
  FastLED.show();
}
void sine() {//sine wave moving
  int bpm = 20;
  leds[lead_dot].b = 50;
  lead_dot = beatsin8(bpm, 0, NUM_LEDS);
  FastLED.show();
  EVERY_N_MILLISECONDS(100)
  {
    fadeToBlackBy(leds, NUM_LEDS, 64);
  }
}
