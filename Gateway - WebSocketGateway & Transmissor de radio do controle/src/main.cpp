#include <Arduino.h>
#include "transmissorRadio.h"
#include "PONTE_WEBSOCKET_RECEPTOR.h"

transmissorRadio radioTx;

void setup(){

Serial.begin(115200);
delay(1000);

radioTx.iniciar();
iniciarWebSocket(); // 👈 inicia WebSocket
}

void loop(){
  atualizarWebSocket(); // 👈 recebe dados WebSocket
}