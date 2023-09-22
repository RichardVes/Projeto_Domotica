#include <ESP8266WiFi.h>        
#include <PubSubClient.h>
//Rede local
const char* ssid = "Server_Domotica";     //Nome da sua rede wifi
const char* password = "122244444";    //Senha da rede wifi
//MQTT servidor (PUBLICO)
const char* mqtt_server = "iot.eclipse.org";   //Endereco Do Seu Broker MQTT
int BROKER_PORT=1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  delay(10);

  // Conecte-se à rede Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado à rede WiFi");

  // Conecte-se ao servidor MQTT
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  // Envie uma mensagem para um tópico MQTT
  client.publish("meu_topico", "Hello, MQTT!");
  delay(5000); // Espere 5 segundos antes de enviar outra mensagem
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Tentando se reconectar ao MQTT...");
    if (client.connect("ESP8266Client")) {
      Serial.println("Conectado ao servidor MQTT");
    } else {
      Serial.print("Falha na conexão com MQTT, rc=");
      Serial.print(client.state());
      Serial.println("Tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}
