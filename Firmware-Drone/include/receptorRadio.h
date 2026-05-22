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
};

void configurarReceptorRadio();
void receberValores();

extern comandosDrone comandosRecebidos;

#endif // receptorRadio_h