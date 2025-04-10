/* This tests the I2S using only the Ardunio-Pico core, not AudioTools*/
#include <Arduino.h>
#include <I2S.h>

// I2S Configuration
#define SAMPLE_RATE 48000
#define BITS_PER_SAMPLE 24
#define MCLK_MULT 256
#define I2S_DATA_PIN 2
#define I2S_BCLK_PIN 0
#define I2S_MCLK_PIN 3

I2S i2s(INPUT); // Create I2S instance
int32_t r, l;

void setup()
{
    Serial.begin(115200);

    // This runs on Core0.  It is the I2S setup.
    i2s.setDATA(2); // These are the pins for the data on the SDR-TRX
    i2s.setBCLK(0);
    i2s.setMCLK(3);
    // Note: LRCK pin is BCK pin plus 1 (1 in this case).
    i2s.setSysClk(SAMPLE_RATE);
    i2s.setBitsPerSample(24);
    i2s.setFrequency(SAMPLE_RATE);
    i2s.setMCLKmult(MCLK_MULT);
    i2s.setBuffers(32, 0, 0);
    i2s.begin();
}

void loop()
{
    if (i2s.read(&l, &r) == 0)
    {
        Serial.println("No data");
    }
    else
    {
        Serial.print("Left: ");
        Serial.print(l);
        Serial.print(" Right: ");
        Serial.println(r);
    }
}