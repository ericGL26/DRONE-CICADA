#ifndef TRANSMISSOR_RADIO_H
#define TRANSMISSOR_RADIO_H

#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

class transmissorRadio {
  private:
    RF24 radio;
     const uint8_t endereco[6] =
    {
        '3','3','0','1','\0'
    };
    
  public:
    transmissorRadio();
    void iniciar();
    void enviar();
};

#endif