#include <Arduino.h>
#include <math.h>
#include "SENSORES/filtroComplementarMPU6050.h"

bool filtroComplementarMPU6050::iniciar() {
  ultimoTempo = micros();
  return true;
}

void filtroComplementarMPU6050::atualizar(
  int16_t aceleracaoX,
  int16_t aceleracaoY,
  int16_t aceleracaoZ,
  int16_t rotacaoX,
  int16_t rotacaoY,
  int16_t rotacaoZ
){
  unsigned long tempoAtual = micros();

  float deltaTime = (tempoAtual - ultimoTempo) / 1000000.0; // Convertendo para segundos
  ultimoTempo = tempoAtual;

  //conversao do giroscopio para graus por segundo
  float giroX = rotacaoX / 131.0;
  float giroY = rotacaoY / 131.0;

  //Angulos calculados pelo acelerometro
  float rollAcelerometro = atan2(aceleracaoY, aceleracaoZ) * 180 / 3.14159265;
  float pitchAcelerometro = atan2(-aceleracaoX, sqrt(aceleracaoY * aceleracaoY + aceleracaoZ * aceleracaoZ)) * 180 / 3.14159265;

  //Integracao do giroscopio
  roll += giroX * deltaTime;
  pitch += giroY * deltaTime;

  //filtro complementar
  roll = 0.98 * roll + 0.02 * rollAcelerometro;
  pitch = 0.98 * pitch + 0.02 * pitchAcelerometro;
}

float filtroComplementarMPU6050::getRoll() {
  return roll;
}

float filtroComplementarMPU6050::getPitch() {
  return pitch;
}