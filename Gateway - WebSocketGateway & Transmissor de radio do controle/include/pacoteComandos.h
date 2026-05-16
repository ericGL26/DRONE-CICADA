#ifndef PROTOCOLOCOMANDOS_H
#define PROTOCOLOCOMANDOS_H

#include <Arduino.h>

struct Comandos {
    byte cima;
    byte baixo;
    byte esquerda;
    byte direita;
};

#endif