#include "SENSORES/MPU6050.h"
#include <Wire.h>

bool MPU6050::iniciar() {
    Wire.begin(21, 22);
    Wire.setClock(100000); // 100 kHz

    // Tira o MPU do modo sleep
    Wire.beginTransmission(MPU_ADDRESS);
    Wire.write(0x6B);
    Wire.write(0x00);

    if (Wire.endTransmission(true) != 0) {
        return false;
    }

    return true;
}

void MPU6050::atualizar() {

    // Seleciona o primeiro registrador dos dados
    Wire.beginTransmission(MPU_ADDRESS);
    Wire.write(0x3B);

    if (Wire.endTransmission(false) != 0) {
        Serial.println("Erro ao acessar MPU6050");
        return;
    }

    // Solicita os 14 bytes
    int bytesRecebidos = Wire.requestFrom(MPU_ADDRESS, (uint8_t)14, (uint8_t)true);

    if (bytesRecebidos != 14) {
        Serial.print("Recebi apenas ");
        Serial.print(bytesRecebidos);
        Serial.println(" bytes do MPU6050");
        return;
    }

    // Acelerometro
    aceleracaox = (Wire.read() << 8) | Wire.read();
    aceleracaoy = (Wire.read() << 8) | Wire.read();
    aceleracaoz = (Wire.read() << 8) | Wire.read();

    // Ignora temperatura
    Wire.read();
    Wire.read();

    // Giroscopio
    rotacaox = (Wire.read() << 8) | Wire.read();
    rotacaoy = (Wire.read() << 8) | Wire.read();
    rotacaoz = (Wire.read() << 8) | Wire.read();
}