
/**
 * @file streams-generator-serial.ino
 * @author Phil Schatzmann
 * @brief see https://github.com/pschatzmann/arduino-audio-tools/blob/main/examples/examples-stream/streams-generator-serial/README.md 
 * @copyright GPLv3
 **/
 
 #include "AudioTools.h"


 SineWaveGenerator<int24_t> sineWave(32000);              // subclass of SoundGenerator with max amplitude of 32000
 GeneratedSoundStream<int24_t> sound(sineWave);             // Stream generated from sine wave
 AudioInfo info(48000,2,24);                   // get the default configuration
 CsvOutput<int24_t> out(Serial); 
 StreamCopy copier(out, sound);                             // copies sound to out
 
 // Arduino Setup
 void setup(void) {  
   // Open Serial 
   Serial.begin(115200);
   //AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Warning);
 
   // Define CSV Output
   out.begin(sound);
 
   // Setup sine wave
   sineWave.begin(info, 620);
   Serial.println("started...");
 }
 
 // Arduino loop - copy sound to out 
 void loop() {
   copier.copy();
 }
 