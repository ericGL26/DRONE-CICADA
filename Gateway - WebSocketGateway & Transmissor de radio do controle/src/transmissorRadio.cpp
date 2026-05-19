#include "transmissorRadio.h"
#include "pacoteComandos.h"

// CE, CSN
transmissorRadio::transmissorRadio()
: radio(5,4)
{
}


void transmissorRadio::iniciar(){
    Serial.println("Iniciando transmissor ESP8266...");
    
    SPI.begin();

    if(!radio.begin()){
        Serial.println(" NRF NAO DETECTADO");
    }

    Serial.println(" NRF OK");
    radio.setChannel(108);
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_LOW);
    radio.openWritingPipe(endereco);
    radio.stopListening();
}

void transmissorRadio::enviar(){
     Comandos dados;
        dados.cima = 10;
        dados.baixo = 0;
        dados.esquerda = 0;
        dados.direita = 5;

    bool ok = radio.write(&dados, sizeof(dados));

    if(ok){
        Serial.print("PACOTE ENVIADO");
    }
    else{
        //Serial.println(" Falha ao enviar");
    }
}