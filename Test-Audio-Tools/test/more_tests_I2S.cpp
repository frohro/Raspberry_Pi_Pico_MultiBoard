#include <I2S.h>

I2S i2s(INPUT);

void setup() {
  Serial.begin(115200);

  i2s.setDATA(2);
  i2s.setBCLK(0); 
  i2s.setMCLK(3);
  // Note: LRCK pin is BCK pin plus 1 (1 in this case).
  i2s.setBitsPerSample(32);
  i2s.setFrequency(8000);
  i2s.setMCLKmult(256);
  
  i2s.begin();

  while (1) {
    int16_t l, r;
    i2s.read16(&l, &r);
    Serial.printf("%d %d\r\n", l, r);
  }
}

void loop() {
  /* Nothing here */
}