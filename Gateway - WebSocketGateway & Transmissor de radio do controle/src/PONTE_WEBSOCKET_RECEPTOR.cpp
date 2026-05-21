#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include "controle.h"

const char* ssid = "DRONE-CICADA";
const char* password = "12345678";

WebSocketsServer webSocket(81);

controleDados controle;

void webSocketEvent(
  uint8_t cliente,
  WStype_t tipoEvento,
  uint8_t * payload,
  size_t tamanho
){
  switch(tipoEvento){

    case WStype_CONNECTED:
      Serial.println("Cliente conectado");
      break;

    case WStype_DISCONNECTED:
      Serial.println("Cliente desconectado");
      break;

    case WStype_TEXT: {
      StaticJsonDocument<128> doc;
      DeserializationError error = deserializeJson(doc, payload, tamanho);

      if(error){
        Serial.println("Falha ao interpretar JSON");
        return;
      }

      controle.ligadoDesligado = doc["ligadoDesligado"].as<float>();
      controle.subirDescer = doc["subirDescer"].as<float>();
      controle.giroEsquerdaDireita = doc["giroEsquerdaDireita"].as<float>();
      controle.irParaFrenteTras = doc["irParaFrenteTras"].as<float>();
      controle.inclinacaoLateralEsquerdaDireita = doc["inclinacaoLateralEsquerdaDireita"].as<float>();
      controle.dadoChegou = true; //flag para iniciar o drone desarmado.
    } break;

    default:
      break;
  }
}

void iniciarWebSocket() {
  IPAddress local_IP(192, 168, 4, 1);
  IPAddress gateway(192, 168, 4, 1);
  IPAddress subnet(255, 255, 255, 0);

  WiFi.mode(WIFI_AP);

  bool ok = WiFi.softAPConfig(local_IP, gateway, subnet);
  if(!ok){
    Serial.println("Falha ao configurar IP fixo");
  }

  WiFi.softAP(ssid, password);

  Serial.println("Ponto de acesso criado");
  Serial.print("IP fixo: ");
  Serial.println(WiFi.softAPIP());

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  Serial.println("Servidor WebSocket iniciado");
}

void atualizarWebSocket() {
  webSocket.loop();
}