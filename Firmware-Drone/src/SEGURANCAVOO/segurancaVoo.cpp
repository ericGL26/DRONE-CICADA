#include "RADIO/receptorRadio.h"

extern comandosDrone comandosdrone;


bool sinalPerdido(){
  return millis() - getUltimoPacoteRecebido() > 2000; //considera o sinal perdido se não receber um pacote por mais de 2 segundos millis é o relogio interno da esp32
};

void PousoSeguroEmergencia(){
  if(sinalPerdido() == true && comandosdrone.droneEmVoo == true){
    Serial.println("Sinal perdido! Iniciando pouso de emergencia...");
    comandosDrone comandosEmergencia;
     comandosEmergencia.ligadoDesligado = 1;
     comandosEmergencia.subirDescer = -40; //comando para descer o drone em segurança (valor -40 é aproximado, ainda deve ser corrijido com testes reais)
     comandosEmergencia.giroEsquerdaDireita = 0;
     comandosEmergencia.irParaFrenteTras = 0;
     comandosEmergencia.inclinacaoLateralEsquerdaDireita = 0;
  }
}