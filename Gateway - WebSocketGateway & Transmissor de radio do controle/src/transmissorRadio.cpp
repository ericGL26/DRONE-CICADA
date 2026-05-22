#include "transmissorRadio.h"
#include "controle.h"

// CE, CSN
transmissorRadio::transmissorRadio()
: radio(D1,D2)
{
}

void transmissorRadio::iniciar(){
    Serial.println("Iniciando transmissor ESP8266...");
    
    SPI.begin();
    if(!radio.begin()){
        Serial.println(" NRF NAO DETECTADO");
    }

    radio.setAutoAck(false); //desabilita a confirmação ACK
    radio.setRetries(0,0); //desabilita as tentativas de reenvio

    Serial.println("NRF OK");
    radio.setChannel(108);
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_LOW);
    radio.openWritingPipe(endereco);
    radio.stopListening();
}

void transmissorRadio::enviar(){
    if(!controle.dadoChegou){return;} //se nenhum dado tiver chegado ainda, não envia nada
    
    bool dadosEnviados = radio.write(&controle, sizeof(controle)); //retorna apenas se o envio foi bem sucedido ou não (POIS O ACK ESTÁ DESABILITADO)

    if(!dadosEnviados){
        Serial.println("Falha ao enviar dados");
    }
}