#include <WiFi.h>
#include <WiFiUdp.h>

WiFiUDP udp;

const char* ssid = "DroneControl";
const char* password = "12345678";

unsigned int localPort = 4210;

// +1 espaço para o '\0'
char pacote[256];

// Variáveis de controle
float subirDescer = 0;
float  giroEsquerdaDireita = 0;
float irParaFrenteTras = 0;
float inclinacaoLateralEsquerdaDireita = 0;

void setupUDP() {
  Serial.begin(9600);
  WiFi.softAP(ssid, password);
  udp.begin(localPort);

  Serial.println("UDP iniciado");
}

void loopUDP() {
  int tamanhoPacote = udp.parsePacket(); //verifica se chegou um pacote UDP e retorna o tamanho do pacote recebido. Se não houver pacotes, retorna 0.
  if (tamanhoPacote) {
    int bytesLidosPeloUDP = udp.read(pacote, sizeof(pacote) - 1); //quantidades de bytes lidos do pacote UDP e armazenados no buffer "pacote". O segundo argumento é o tamanho máximo a ser lido, que é o tamanho do buffer menos 1 para garantir espaço para o caractere nulo de terminação. 

    if (bytesLidosPeloUDP > 0) {
      pacote[bytesLidosPeloUDP] = '\0';
    }

    Serial.print("RAW: ");
    Serial.println(pacote);

    String data = String(pacote); //cria uma nova string chamada data com o mesmo conteúdo de pacote

    // Buscar posições das chaves
    int subirDescer = data.indexOf("subirDescer:"); //busca no DATA onde esta localizado o começo do texto com valor subirDescer
    int giroEsquerdaDireita = data.indexOf("giroEsquerdaDireita:");
    int irParaFrenteTras = data.indexOf("irParaFrenteTras:");
    int inclinacaoLateralEsquerdaDireita = data.indexOf("inclinacaoLateralEsquerdaDireita:");

    if (subirDescer != -1 && giroEsquerdaDireita != -1 && irParaFrenteTras != -1 && inclinacaoLateralEsquerdaDireita != -1) {

      int subirDescer_END = data.indexOf(",", subirDescer); //procura esse texto, mas começando por uma posicao especifica, nessa caso o valor da varavel subirDescer
      int giroEsquerdaDireita_END = data.indexOf(",", giroEsquerdaDireita);
      int irParaFrenteTras_END = data.indexOf(",", irParaFrenteTras);
      int inclinacaoLateralEsquerdaDireita_END = data.indexOf("}", inclinacaoLateralEsquerdaDireita);

      if (subirDescer_END != -1 && giroEsquerdaDireita_END != -1 && irParaFrenteTras_END != -1 && inclinacaoLateralEsquerdaDireita_END != -1) { //se o valor de indexOf retornar -1 significa que ele nao achou nada.

        subirDescer = data.substring(subirDescer + 13, subirDescer_END).toFloat(); // recorta a variavel data em o texto subirDescer, restando apenas o valor da variavel, entre 0 e 100
        giroEsquerdaDireita = data.substring(giroEsquerdaDireita + 23, giroEsquerdaDireita_END).toFloat();
        irParaFrenteTras = data.substring(irParaFrenteTras + 17, irParaFrenteTras_END).toFloat();
        inclinacaoLateralEsquerdaDireita = data.substring(inclinacaoLateralEsquerdaDireita + 37, inclinacaoLateralEsquerdaDireita_END).toFloat();

        // 🔒 Limites de segurança
        subirDescer = constrain(subirDescer, 0, 100); //constrain(valor, minimo, maximo). constrain significa restringir. se o valor ultrapassar 100 por exemplo ele limita no 100
        giroEsquerdaDireita = constrain(giroEsquerdaDireita, -100, 100);
        irParaFrenteTras = constrain(irParaFrenteTras, -100, 100);
        inclinacaoLateralEsquerdaDireita = constrain(inclinacaoLateralEsquerdaDireita, -100, 100);

        // DEBUG FINAL
        Serial.print("SUBIR/DESCER: "); Serial.print(subirDescer);
        Serial.print(" | GIRO: "); Serial.print(giroEsquerdaDireita);
        Serial.print(" | FRENTE/TRÁS: "); Serial.print(irParaFrenteTras);
        Serial.print(" | LATERAL: "); Serial.println(inclinacaoLateralEsquerdaDireita);
      }
    }
  }
}