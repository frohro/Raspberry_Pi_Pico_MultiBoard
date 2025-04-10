#include "Adafruit_TinyUSB.h"
#include "AudioTools.h"  // https://github.com/pschatzmann/arduino-audio-tools

Adafruit_USBD_Audio usb;
AudioInfo info(48000, 1, 16);
SawToothGenerator<int16_t> sawtooth;               
GeneratedSoundStream<int16_t> sawthooth_stream(sawtooth);

void setup() {
  Serial.begin(115200);
  //while(!Serial);  // wait for serial

  // generate 10 hz 
  sawtooth.begin(info, 10.0f);

  // Start USB device as Audio Source
  usb.setInput(sawthooth_stream);
  usb.begin(info.sample_rate, info.channels, info.bits_per_sample);
}

void loop() {
  // optional: use LED do display status
  usb.updateLED();
}