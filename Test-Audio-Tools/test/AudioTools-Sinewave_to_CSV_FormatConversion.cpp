
/**
 * @file streams-generator-serial.ino
 * @author Phil Schatzmann
 * @brief see https://github.com/pschatzmann/arduino-audio-tools/blob/main/examples/examples-stream/streams-generator-serial/README.md
 * @copyright GPLv3
 **/

#include "AudioTools.h"

SineWaveGenerator<int24_t> sineWave(32000);    // subclass of SoundGenerator with max amplitude of 32000
GeneratedSoundStream<int24_t> sound(sineWave); // Stream generated from sine wave
CsvOutput<int16_t> out(Serial);                // Output to Serial
AudioInfo from(48000, 2, 24);
AudioInfo to(48000, 2, 16);
FormatConverterStream conv(out);
StreamCopy copier(conv, sound); // copies sound to out

// Arduino Setup
void setup()
{
  // Open Serial
  Serial.begin(115200);
  // AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Warning);

  sineWave.begin(from, N_B4);
  sound.begin(from);
  conv.begin(from, to);

  out.begin(to);
}

// Arduino loop - copy sound to out
void loop()
{
  copier.copy();
}
