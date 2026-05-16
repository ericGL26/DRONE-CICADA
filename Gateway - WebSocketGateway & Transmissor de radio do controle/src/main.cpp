#include <Arduino.h>
#include "transmissorRadio.h"
#include "PONTE-UDP-RECEPTOR.h"

transmissorRadio radioTx;

void setup(){

Serial.begin(9600);
delay(1000);

radioTx.iniciar();
setupUDP(); // 👈 inicia UDP

}

void loop(){

radioTx.enviar();
loopUDP(); // 👈 recebe dados UDP

delay(50);
}