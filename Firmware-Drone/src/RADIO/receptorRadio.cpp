#include <SPI.h>
#include <arduino.h>
#include <RF24.h>
#include "RADIO/receptorRadio.h"

RF24 radio(13, 14); //CE, CSN (CORRIGIR PARA OS PINOS CORRETOS DO ESP32)
const byte endereco[6] = "3301";

comandosDrone comandosRecebidos;

void configurarReceptorRadio(){
  SPI.begin();

  if (!radio.begin()) {
    Serial.println("NRF nao detectado");
    while(true);
  }

  radio.setAutoAck(false); //desabilita a confirmação de recebimento (ACK)

  radio.setChannel(108);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);

  radio.openReadingPipe(1, endereco);
  radio.startListening();
  
  Serial.println("Receptor iniciado");
}

unsigned long ultimoPacoteRecebido = 0; //guarda o tempo que o ultimo pacote foi recebido para verificar se o sinal foi perdido (timeout)

void receberValores(){
  if(radio.available()){
    radio.read(&comandosRecebidos, sizeof(comandosRecebidos));
    
    //Serial.println(comandosRecebidos.ligadoDesligado);
    Serial.println(comandosRecebidos.subirDescer);
    ultimoPacoteRecebido = millis();
    //Serial.println(comandosRecebidos.giroEsquerdaDireita);
    //Serial.println(comandosRecebidos.irParaFrenteTras);
    //Serial.println(comandosRecebidos.inclinacaoLateralEsquerdaDireita);
  }
}


// Função para obter o tempo do último pacote recebido
unsigned long getUltimoPacoteRecebido()
{
  return ultimoPacoteRecebido;
}