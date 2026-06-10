#ifndef MPU6050_H
#define MPU6050_H

#include <Arduino.h>

class MPU6050 {
    public:
        bool iniciar();
        void atualizar();

        int16_t aceleracaox, aceleracaoy, aceleracaoz;
        int16_t rotacaox, rotacaoy, rotacaoz;

    private:
        const uint8_t MPU_ADDRESS = 0x68; //0x68 é o endereco padrao I²C do mpu5060
};

#endif