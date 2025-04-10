#include <cmath>
#include "Adafruit_TinyUSB.h"

const float sampleRate = 48000.0;
const float frequencyLeft = 1000.0;
const float frequencyRight = 500.0;
const float amplitude = 32000.0;
const float twoPi = 2.0 * M_PI;

Adafruit_USBD_Audio usb;

bool core1_separate_stack = true;

size_t readCB(uint8_t* data, size_t len, Adafruit_USBD_Audio& ref) {
  int16_t* data16 = (int16_t*)data;
  size_t samples = len / sizeof(int16_t);
  size_t result = 0;
  static float phaseLeft = 0.0;
  static float phaseRight = 0.0;
  float phaseIncrementLeft = twoPi * frequencyLeft / sampleRate;
  float phaseIncrementRight = twoPi * frequencyRight / sampleRate;

  for (size_t j = 0; j < samples; j += 2) {
    data16[j] = amplitude * sin(phaseLeft); // Left channel
    data16[j + 1] = amplitude * sin(phaseRight); // Right channel
    phaseLeft += phaseIncrementLeft;
    phaseRight += phaseIncrementRight;
    if (phaseLeft >= twoPi) phaseLeft -= twoPi;
    if (phaseRight >= twoPi) phaseRight -= twoPi;
    result += sizeof(int16_t) * 2;
  }
  return result;
}

void setup1() {
  Serial.begin(115200);

  // Start USB device as Microphone
  usb.setReadCallback(readCB);
  usb.begin(sampleRate, 2, 16); // Set sample rate to 48000 Hz

  if (TinyUSBDevice.mounted()) {
    TinyUSBDevice.detach();
    delay(10);
    TinyUSBDevice.attach();
  }
}

void loop1() {
  // optionally use LED to display status
  usb.updateLED();
}

void setup() {}
void loop() {}