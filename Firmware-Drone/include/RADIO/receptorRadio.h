#ifndef receptorRadio_h
#define receptorRadio_h

#include <SPI.h>
#include <Arduino.h>
#include <RF24.h>

extern RF24 radio;
extern const byte endereco[6];

struct comandosDrone {
    int ligadoDesligado;
    int subirDescer;
    int giroEsquerdaDireita;
    int irParaFrenteTras;
    int inclinacaoLateralEsquerdaDireita;
        
    //variaveis de controle e segurança
    bool droneEmVoo = false;
};

void configurarReceptorRadio();
void receberValores();
bool radioEstaFuncionando();

unsigned long getUltimoPacoteRecebido();
extern comandosDrone comandosRecebidos;

#endif // receptorRadio_h

