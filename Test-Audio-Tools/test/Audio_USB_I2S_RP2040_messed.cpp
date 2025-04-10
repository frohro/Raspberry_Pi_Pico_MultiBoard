#include "Adafruit_TinyUSB.h"
#include "AudioTools.h"
#include "AudioTools/Concurrency/RP2040.h"
#include "hardware/clocks.h" // Add this for clock functions
#include "hardware/vreg.h"   // Add this for voltage regulator functions

#define RATE 48000
#define MCLK_MULT 256

AudioInfo info(RATE, 2, 24);
Adafruit_USBD_Audio usb;
BufferRP2040 buffer(256, 20);
QueueStream queue(buffer);
I2SStream i2s;     // We need to make sure the I2S pins are correctly set.
StreamCopy copier; // Add the missing copier declaration

// Changed parameter from const uint8_t* to uint8_t* to match expected callback signature
size_t readCB(uint8_t *data, size_t len, Adafruit_USBD_Audio &ref)
{
    usb.setFeedbackPercent(buffer.size() * 100 / buffer.available());
    return queue.write(data, len);
}

void setup()
{
    AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Debug);
    while (!Serial)
        ; // wait for serial
    Serial.println("starting...");
    // start queue
    queue.begin(80);

    // Start USB device as Audio Sink w/o automatic feedback
    usb.setFeedbackMethod(AUDIO_FEEDBACK_METHOD_DISABLED);
    usb.setReadCallback(readCB);
    if (!usb.begin(info.sample_rate, info.channels, info.bits_per_sample))
    {
        Serial.println("USB error");
    }

    // If already enumerated, additional class driverr begin() e.g msc, hid, midi won't take effect until re-enumeration
    if (TinyUSBDevice.mounted())
    {
        TinyUSBDevice.detach();
        delay(10);
        TinyUSBDevice.attach();
    }
}

void loop()
{
    // just for fun: we blink the LED
    usb.updateLED();
}

void setup1()
{
    // start i2s
    auto cfg = i2s.defaultConfig(RX_MODE);
    // i2s.setDATA(2); // These are the pins for the data on the SDR-TRX
    // i2s.setBCLK(0);
    // i2s.setMCLK(3);
    cfg.pin_mck = 3;          // MCLK pin
    cfg.pin_bck = 0;          // BCK pin
    cfg.pin_ws = 1;           // LRCK pin
    cfg.pin_data = 2;         // DATA pin
    cfg.bits_per_sample = 24; // 24 bits per sample
    cfg.sample_rate = RATE;
    cfg.mck_multiplier = MCLK_MULT; // MCLK multiplier
    // // Note: LRCK pin is BCK pin plus 1 (1 in this case).
    // i2s.setSysClk(RATE);
    // i2s.setBitsPerSample(24);
    // i2s.setFrequency(RATE);
    // i2s.setMCLKmult(MCLK_MULT);
    cfg.copyFrom(info);
    cfg.buffer_size = 256;
    cfg.buffer_count = 3;
    if (!i2s.begin(cfg))
    {
        Serial.print("i2s error");
    }

    // Setup the copier to move data between queue and i2s
    copier.begin(queue, i2s);
}

void loop1()
{
    copier.copy();
}