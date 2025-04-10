#include <I2S.h>
#include <Arduino.h>

#define RATE 48000
#define MCLK_MULT 256

I2S i2s(INPUT);
unsigned long startTime;
bool dataActive = true;
int zeroCount = 0;

void setup() {
  Serial.begin(115200);
  delay(1000); // Wait for serial
  
  Serial.println("\n\n--- PCM1808 I2S Test ---");
#ifdef ARDUINO_PICO_VERSION
  Serial.printf("Arduino-Pico Version: %s\n", ARDUINO_PICO_VERSION);
#endif
  
  i2s.setDATA(2);
  i2s.setBCLK(0);
  i2s.setMCLK(3);
  i2s.setSysClk(RATE);
  i2s.setBitsPerSample(32);
  i2s.setFrequency(RATE);
  i2s.setMCLKmult(MCLK_MULT);
  i2s.setBuffers(32, 8, 8);
  
  if (!i2s.begin()) {
    Serial.println("I2S initialization failed");
    while(1);
  }
  
  Serial.println("I2S started successfully");
  startTime = millis();

  // Your original code loop
  while (1) {
    int32_t l, r;
    i2s.read32(&l, &r);
    
    // Check for data dropout
    if (l == 0 && r == 0) {
      zeroCount++;
      if (zeroCount > 100 && dataActive) {
        unsigned long dropoutTime = millis() - startTime;
        Serial.printf("\n*** DATA DROPOUT at %lu ms (%0.1f seconds) ***\n\n", 
                      dropoutTime, dropoutTime/1000.0);
        dataActive = false;
      }
    } else {
      zeroCount = 0;
      if (!dataActive) {
        Serial.println("\n*** DATA SIGNAL RECOVERED ***\n");
        dataActive = true;
      }
    }
    
    // Only print occasional samples to reduce serial load
    if ((random(100) < 5)&(dataActive)) { // Print ~5% of samples
      Serial.printf("%d %d\r\n", l << 8, r << 8);
    }
    
    delayMicroseconds(100);
  }
}

void loop() {}