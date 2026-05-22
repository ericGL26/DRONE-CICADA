#ifndef receptorRadio_h
#define receptorRadio_h

#include <SPI.h>
#include <Arduino.h>
#include <RF24.h>

extern RF24 radio;
extern const byte endereco[6];

struct comandosDrone {
    float ligadoDesligado;
    float subirDescer;
    float giroEsquerdaDireita;
    float irParaFrenteTras;
    float inclinacaoLateralEsquerdaDireita;
};

void configurarReceptorRadio();
void receberValores();

extern comandosDrone comandosRecebidos;

#endif // receptorRadio_h