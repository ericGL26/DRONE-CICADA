#include <Arduino.h>
#include "receptorRadio.h"
#include <SPI.h>

void setup() {
  Serial.begin(115200);

  configurarReceptorRadio();
}

void loop() {
    receberValores();
}