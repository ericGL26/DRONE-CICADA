#include <Arduino.h>
#include "RADIO/receptorRadio.h"
#include "SENSORES/MPU6050.h"

MPU6050 mpu6050;

void setup() {
  Serial.begin(115200);

  if(mpu6050.iniciar()){
    Serial.println("MPU6050 iniciado com sucesso!");
  }else{
    Serial.println("Erro ao iniciar MPU6050!"); 
    while(true)
      ;};

  configurarReceptorRadio();
}

void loop() {
    receberValores();
    mpu6050.atualizar();
}