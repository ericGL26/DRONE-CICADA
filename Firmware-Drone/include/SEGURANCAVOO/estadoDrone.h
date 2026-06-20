#ifndef estadoDrone_H
#define estadoDrone_H

enum estadoDrone {
  INICIALIZANDO,
  DESARMADO,
  ARMADO,
  FALHA,
};

class EstadoDrone { 
  public:
    static void iniciar();
    static void atualizar();

    static void armar();
    static void desarmar();
  
    private:
      static estadoDrone estadoAtual;

      static void atualizarInicializando();
      static void atualizarDesarmado();
      static void atualizarArmado();
      static void atualizarFalha();
};


#endif