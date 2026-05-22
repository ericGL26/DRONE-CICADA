#include <SPI.h>
#include <arduino.h>
#include <RF24.h>
#include "receptorRadio.h"

void configurarReceptorRadio(){
  RF24 radio(9, 10); //CE, CSN (CORRIGIR PARA OS PINOS CORRETOS DO ESP32)

  const byte endereco[6] = "3301";

  SPI.begin();

  if (!radio.begin()) {
    Serial.println("NRF nao detectado");
    while(true);
  }

  radio.setChannel(108);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);

  radio.openReadingPipe(1, endereco);
  radio.startListening();
  
  Serial.println("Receptor iniciado");
}

void receberValores(){
  if(radio.available()){
    radio.read(&comandosRecebidos, sizeof(comandosRecebidos));
    
    Serial.print("Ligado/Desligado: ");
    Serial.println(comandosRecebidos.ligadoDesligado);
    Serial.print("Subir/Descer: ");
    Serial.println(comandosRecebidos.subirDescer);
    Serial.print("Giro Esquerda/Direita: ");
    Serial.println(comandosRecebidos.giroEsquerdaDireita);
    Serial.print("Ir Para Frente/Tras: ");
    Serial.println(comandosRecebidos.irParaFrenteTras);
    Serial.print("Inclinacao Lateral Esquerda/Direita: ");
    Serial.println(comandosRecebidos.inclinacaoLateralEsquerdaDireita);
  }
}