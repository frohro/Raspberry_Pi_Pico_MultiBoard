#include "AudioTools.h"

SineWaveGenerator<int24_t> sine_wave;                   // subclass of SoundGenerator with max amplitude of 32000
GeneratedSoundStream<int24_t> in_stream(sine_wave);     // Stream generated from sine wave
CsvOutput<int16_t> out(Serial);                         // Output to Serial
AudioInfo from(48000, 2, 24);                                           
AudioInfo to(48000, 2, 16);                                           
FormatConverterStream conv(out);
StreamCopy copier(conv, in_stream);                     // copies sound to out

void setup(){
  Serial.begin(115200);
//   AudioLogger::instance().begin(Serial, AudioLogger::Info);  

  sine_wave.begin(from, N_B4);
  in_stream.begin(from);
  conv.begin(from, to);

  out.begin(to);
}

void loop(){
    copier.copy();
}