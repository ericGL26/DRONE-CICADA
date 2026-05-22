#include <Arduino.h>
#include "transmissorRadio.h"
#include "PONTE_WEBSOCKET_RECEPTOR.h"

transmissorRadio radio;

void setup(){
  Serial.begin(115200);
  delay(1000);
  iniciarWebSocket(); // 👈 inicia WebSocket
  radio.iniciar(); // 👈 inicia rádio
}

void loop(){
  atualizarWebSocket(); // 👈 recebe dados WebSocket
  radio.enviar(); // 👈 envia dados recebidos pelo para o drone via rádio
}