// NJR4262 radar program for ESP32
// Development environment : vscode + platformio
// graph : teleplot(function of vscode)

#include <Arduino.h>
#include "../lib/NJR4262_ESP32.h"

NJR4262_ESP32 radar;

void setup() {
  Serial.begin(9600);
  delay(30);
}

void loop() {
  radar.updateVariable();
  // plot to teleplot
  Serial.print(">Respiratory Waveform:");
  Serial.println(radar.getpositionLPF());
  delay(DELTA_TIME);
}
