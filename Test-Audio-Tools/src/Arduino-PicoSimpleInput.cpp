#include <I2S.h>
#include <Arduino.h>
/* Notes:  This works for about one second, and then stops converting data (data goes to zero).  This
was working fine before for longer than that at least.  There is a note in the PCM1808 datasheet that:
In slave mode, the PCM1808 device operates under LRCK (pin 7), synchronized with system clock SCKI (pin 6).
The PCM1808 device does not require a specific phase relationship between LRCK and SCKI, but does require
the synchronization of LRCK and SCKI.
If the relationship between LRCK and SCKI changes more than ±6 BCKs for 64 BCK/frame (±5 BCKs for 48
BCK/frame) during one sample period due to LRCK or SCKI jitter, internal operation of the ADC halts within 1 / fS
and digital output goes to zero data (BPZ code) until resynchronization between LRCK and SCKI occurs.
In the case of changes less than ±5 BCKs for 64 BCK/frame (±4 BCKs for 48 BCK/frame), resynchronization
does not occur, and the previously described digital output control and discontinuity do not occur. */

#define RATE 48000
#define MCLK_MULT 256

I2S i2s(INPUT);

void setup() {
  Serial.begin(115200);
  i2s.setDATA(2); // These are the pins for the data on the SDR-TRX
  i2s.setBCLK(0);
  i2s.setMCLK(3);
  // Note: LRCK pin is BCK pin plus 1 (1 in this case).
  i2s.setSysClk(RATE);
  i2s.setBitsPerSample(24);
  i2s.setFrequency(RATE);
  i2s.setMCLKmult(MCLK_MULT);
  i2s.setBuffers(32, 0, 0);
  i2s.begin();
  delay(1000); // Wait for the I2S to start up

  while (1) {
    int32_t l, r;
    i2s.read32(&l, &r);
    l = l << 8;
    r = r << 8;
    Serial.printf("%d %d\r\n", l, r);
    // delayMicroseconds(100); // Give system time to breathe
  }
}

void loop() {}