#ifndef FILTRO_COMPLEMENTAR_h
#define FILTRO_COMPLEMENTAR_h

#include <stdint.h>

class filtroComplementarMPU6050{
  public:
    bool iniciar();
    void atualizar(
      int16_t aceleracaoX,
      int16_t aceleracaoY,
      int16_t aceleracaoZ,
      int16_t rotacaoX,
      int16_t rotacaoY,
      int16_t rotacaoZ
    );
    
    float getRoll();
    float getPitch();
    
  private:
    float roll = 0;
    float pitch = 0;

    unsigned long ultimoTempo = 0;
};

#endif