#include "Adafruit_TinyUSB.h"
#include "AudioTools.h"
#include "AudioTools/Concurrency/RP2040.h"

AudioInfo info(48000, 2, 16);
Adafruit_USBD_Audio usb;
BufferRP2040 buffer(256, 20); 
QueueStream queue(buffer);
I2SStream i2s;  // We need to make sure the I2S pins are correctly set.

size_t readCB(const uint8_t* data, size_t len, Adafruit_USBD_Audio& ref) {
  usb.setFeedbackPercent(buffer.size()*100 / buffer.available());
  return queue.write(data, len);
}

void setup() {  
  Serial.begin(115200);
  AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Warning);
  while(!Serial);  // wait for serial
  Serial.println("starting...");
  // start queue
  queue.begin(80);

  // Start USB device as Audio Sink w/o automatic feedback
  usb.setFeedbackMethod(AUDIO_FEEDBACK_METHOD_DISABLED);
  usb.readCBCallback(readCB);
  if (!usb.begin(info.sample_rate, info.channels, info.bits_per_sample)){
    Serial.println("USB error");
  }

  // If already enumerated, additional class driverr begin() e.g msc, hid, midi won't take effect until re-enumeration
  if (TinyUSBDevice.mounted()) {
    TinyUSBDevice.detach();
    delay(10);
    TinyUSBDevice.attach();
  }
}

void loop() {
  // just for fun: we blink the LED
  usb.updateLED();
}

void setup1(){
  //start i2s
  auto cfg = i2s.defaultConfig(RX_MODE);
  cfg.copyFrom(info);
  cfg.buffer_size = 256;
  cfg.buffer_count = 3;
  if (!i2s.begin(cfg)){
    Serial.print("i2s error");
  }
}

void loop1() {
  copier.copy();
}