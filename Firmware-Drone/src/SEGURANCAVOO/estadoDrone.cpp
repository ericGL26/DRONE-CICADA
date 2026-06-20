#include "SEGURANCAVOO/estadoDrone.h"
#include "RADIO/receptorRadio.h"

estadoDrone EstadoDrone::estadoAtual = INICIALIZANDO;

void EstadoDrone::atualizar(){
  switch (estadoAtual){
    case INICIALIZANDO:
         atualizarInicializando();
         // Lógica para inicialização
    break;

    case DESARMADO:
         atualizarDesarmado();
         // Lógica para estado desarmado
    break;

    case ARMADO:
         atualizarArmado();
         // Lógica para estado armado 
    break;

    case FALHA:
        atualizarFalha();
        // Lógica para estado de falha
    break;
  };
};

void EstadoDrone::armar(){
  estadoAtual = ARMADO;
}

void EstadoDrone::desarmar(){
  estadoAtual = DESARMADO;
}


void EstadoDrone::iniciar(){
  estadoAtual = INICIALIZANDO;
}


void EstadoDrone::atualizarInicializando(){
  if(radioEstaFuncionando() == true /* outras condições para inicialização */){
    estadoAtual = DESARMADO;
  }
}

void EstadoDrone::atualizarDesarmado(){
  // Lógica para atualizar o estado de desarmado
}

void EstadoDrone::atualizarArmado(){
  // Lógica para atualizar o estado de armado
}

void EstadoDrone::atualizarFalha(){
  // Lógica para atualizar o estado de falha
}