#include "MPU6050.h"
#include <Wire.h> //biblioteca para comunicacao I²C (forma que o mpu se comunica)

bool MPU6050::iniciar(){
    Wire.begin(21, 22); //portas que o SDA e SCL estao conectadas.

    //Inicia o MPU6050
    Wire.beginTransmission(MPU_ADDRESS);
    Wire.write(0x6B);
    Wire.write(0x00); //Tire o mpu do modo SLEEP (o valor 0x00 desativa esse modo)
    if(Wire.endTransmission(true) != 0){
        return false
    }
    return true
 }

 void MPU6050::atualizar(){
    Wire.beginTransmission(MPU_ADRESS);
    Wire.write(0x3B); //primeiro registrador de dados
    Wire.endTransmission(false); //mantem a comunicação aberta para receber os dados

    Wire.requestFrom(MPU_ADDRESS, (uint8_t)14, (uint8_t)true)

    aceleracaox = (Wire.read() << 8 | Wire.read()); //O mpu manda os dados em duas sequencias de bytes (HIGH e LOW), entao juntamos eles com o operador <<
    aceleracaoy = (Wire.read() << 8 | Wire.read());
    aceleracaoz = (Wire.read() << 8 | Wire.read());

    //Saltamos os bytes de temperatura (0x41 e 0x42) pois nao vamos usa-los agora (se nao saltarmos, o mpu os enviará na proxima sequencia de dados, desalinhando a estrutura.)
    Wire.read();
    Wire.read();

    rotacaox    = (Wire.read() << 8 | Wire.read());
    rotacaoy    = (Wire.read() << 8 | Wire.read());
    rotacaoz    = (Wire.read() << 8 | Wire.read());
 }