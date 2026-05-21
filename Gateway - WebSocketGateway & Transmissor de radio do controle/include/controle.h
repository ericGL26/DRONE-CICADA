#pragma once

struct controleDados {
  int ligadoDesligado;
  int subirDescer;
  int giroEsquerdaDireita;
  int irParaFrenteTras;
  int inclinacaoLateralEsquerdaDireita;

  bool dadoChegou = false;
};

extern controleDados controle;