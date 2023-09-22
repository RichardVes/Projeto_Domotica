#include <ESP8266WiFi.h>        
#include <PubSubClient.h>
//Rede local
const char* ssid = "Server_Domotica";     //Nome da sua rede wifi
const char* password = "122244444";    //Senha da rede wifi
//MQTT servidor (PUBLICO)
const char* BROKER_MQTT = "iot.eclipse.org";   //Endereco Do Seu Broker MQTT
int BROKER_PORT=1883;    // porta do broker mqtt

#define ID_MQTT "BCI01" //  ID unico para o client. 
#define TOPIC_PUBLISH "BCIBotão1" // Tópico unico.
PubSubClient MQTT(wifiClient);

//Declaração das Funções
void mantemConexoes(); //Garante que as conexoes com WIfi e MQTT Broker se mantenham
void conectaWifi();    // Se conecta ao Wifi
void conectaMQTT();    // Se cibecta ao MQTT
void enviaPacote();

void setup() {
  pinMode(pinBotao1, INPUT_PULLUP);
  Serial.begin(115200);
  conectaWifi();
  MQTT.setServer(BROKER_MQTT, BROKER_PORT)
}

void loop() {
 mantemConexoes();
 enviaValores();
 MQTT.loop();
}

void mantemCOnexoes(){
  if(!MQTT.connected(){
    conectaMQTT();
  }
  conectaWifi(); //se não hã conexão com o WIfi, reconecta
  }

void conectaWIfi(){
  if (wiFI.status() == WLCONECCTED) {
    return;
  }
  Serial.print("COnectando -se na rede");
  Serial.print(SSID);
  Serial.println(" Aguarde!");

  Wifi.begin(SSID, PASSWORD); // conecta na rede WIFI
  while (WiFi.status() !=WL_CONNECTED){
    delay(100);
    Serial.print(".");
  }
  Serial.print();
  Serial.print("Conectado com sucesso, na rede: ");
  Serial.print(SSID);
  Serial.print(" IP obtido:");
  Serial.print(WiFi.localIP());
}
void connectaMQTT(){
while (!MQTT.connected()){
  Serial.print("Conectando ao Broker MQTT: ");
  Serial.print(BROKER_MQTT);
  if (MQTT_connect(ID_MQTT)){
    Serial.println("Conectado ao Broker com sucesso!");
    }
else{
  Serial.println("Não foi possivel se conectar ao BROKER MQTT.")
  Serial.println("Nova tentativa de conexao em 5s");
  delay(5000);
}
}
}

void enviaValores(){
  static bool estadoBotao1=HIGH;
  static bool estadoBotao1Ant = HIGH;
  static unsigned long debounceBotao1;

  estadoBotao1=digitalRead(pinBotao1);
  if( (millis() - debounceBotao1)>30){  //Elimina efeito bouncing}
if (!estadoBotao1&&estadoBotao1Ant){
  //botao apertado
  MQTT.publish(TOPIC_PUBLISH,"1");
  Serial.println("Botao1 APERTADO. Playload enviado.");

  debounceBotao1=millis();
}else if (estadoBotao1)&&!estadoBotao1Ant){
  //Botao solto
MQTT.publish(TOPIC_PUBLISH,"0");
Serial.println("Botao1 SOLTO. playload enviado.");
debounceBotao1=millis();
}
}
estadoBotao1Ant=estadoBotao1;
}



