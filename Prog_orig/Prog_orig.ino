#include <MyConfig.h>
#include <MySensors.h>

#define MY_DEBUG
#define MY_GATEWAY_SERIAL // Enable serial gateway
#if F_CPU == 8000000L
#define MY_BAUD_RATE 115200
#endif
// Enable inclusion mode
#define MY_INCLUSION_MODE_FEATURE
// Set inclusion mode duration (in seconds)
#define MY_INCLUSION_MODE_DURATION 60
// Set blinking period
#define MY_DEFAULT_LED_BLINK_PERIOD 300

#include <MySensors.h>

#define CHILD_ID_G 1
#define CHILD_ID_R 2
#define CHILD_ID_Y 3
#define CHILD_ID_B 4

MyMessage msg_G(CHILD_ID_G, V_TRIPPED);
MyMessage msg_R(CHILD_ID_R, V_TRIPPED);
MyMessage msg_Y(CHILD_ID_Y, V_TRIPPED);
MyMessage msg_B(CHILD_ID_B, V_TRIPPED);

#define RELAY_PIN_G 2 // LED VERDE
#define RELAY_PIN_R 3 // LED VERMELHO
#define RELAY_PIN_Y 4 // LED AMARELO
#define RELAY_PIN_B 5 // LED AZUL
#define NUMBER_OF_RELAYS 4 // Total number of attached relays
#define RELAY_ON 1  // GPIO value to write to turn on attached relay
#define RELAY_OFF 0 // GPIO value to write to turn off attached relay


uint8_t value_G = RELAY_OFF;
uint8_t value_R = RELAY_OFF;
uint8_t value_Y = RELAY_OFF;
uint8_t value_B = RELAY_OFF;
int BT_S = 6; //botao sensor temperatura
int BT_T = 7; //botao trip
byte ST_LED_G = LOW;
byte ST_LED_Y = LOW;
byte ST_LED_B = LOW;
byte ST_LED_R = LOW;
byte ST_BT_S = 0;
byte ST_BT_T = 0;
byte ST_ALARM = 0;


void before()
{

    //LED VERDE
  for (int sensor=1, pin=RELAY_PIN_G; sensor<=NUMBER_OF_RELAYS; sensor++, pin++) {
    // Then set relay pins in output mode
    pinMode(pin, OUTPUT);
    // Set relay to last known state (using eeprom storage)
    digitalWrite(pin, loadState(sensor)?RELAY_ON:RELAY_OFF);
  }
 
  //LED VERMELHO
  for (int sensor=2, pin=RELAY_PIN_R; sensor<=NUMBER_OF_RELAYS; sensor++, pin++) {
    // Then set relay pins in output mode
    pinMode(pin, OUTPUT);
    // Set relay to last known state (using eeprom storage)
    digitalWrite(pin, loadState(sensor)?RELAY_ON:RELAY_OFF);
  }

  //LED AMARELO
  for (int sensor=3, pin=RELAY_PIN_Y; sensor<=NUMBER_OF_RELAYS; sensor++, pin++) {
    // Then set relay pins in output mode
    pinMode(pin, OUTPUT);
    // Set relay to last known state (using eeprom storage)
    digitalWrite(pin, loadState(sensor)?RELAY_ON:RELAY_OFF);
  }

  //LED AZUL
  for (int sensor=4, pin=RELAY_PIN_B; sensor<=NUMBER_OF_RELAYS; sensor++, pin++) {
    // Then set relay pins in output mode
    pinMode(pin, OUTPUT);
    // Set relay to last known state (using eeprom storage)
    digitalWrite(pin, loadState(sensor)?RELAY_ON:RELAY_OFF);
  }

MOTOR_OFF();
}

void setup()
{
pinMode(BT_S , INPUT);  // pinMode = INPUT vai receber o sinal do BT
pinMode(BT_T , INPUT);
}

void presentation()
{
 
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("TCC -  Inovador", "ANDRE E RONALDO");

// LED VERDE
  for (int sensor=1, pin=RELAY_PIN_G; sensor<=NUMBER_OF_RELAYS; sensor++, pin++) {
    // Register all sensors to gw (they will be created as child devices)
    present(sensor, S_LIGHT);
    present(CHILD_ID_G, S_LIGHT);
  }
 
// LED VERMELHO
  for (int sensor=2, pin=RELAY_PIN_R; sensor<=NUMBER_OF_RELAYS; sensor++, pin++) {
    // Register all sensors to gw (they will be created as child devices)
    present(sensor, S_LIGHT);
    present(CHILD_ID_R, S_LIGHT);
  }

 // LED AMARELO
   for (int sensor=3, pin=RELAY_PIN_Y; sensor<=NUMBER_OF_RELAYS; sensor++, pin++) {
    // Register all sensors to gw (they will be created as child devices)
    present(sensor, S_LIGHT);
    present(CHILD_ID_Y, S_LIGHT);
  }

  // LED AZUL
   for (int sensor=4, pin=RELAY_PIN_B; sensor<=NUMBER_OF_RELAYS; sensor++, pin++) {
    // Register all sensors to gw (they will be created as child devices)
    present(sensor, S_LIGHT);
    present(CHILD_ID_B, S_LIGHT);
  }
}

void loop()
{
delay(50);
ST_BT_S = digitalRead(BT_S); //Realizo  leitura do botao
if (ST_BT_S == 1)
 {
  delay(50);
  TEMP_ALTA();
  delay(100);
 }

ST_BT_T = digitalRead(BT_T); //Realizo  leitura do botao
if (ST_BT_T == 1)
 {
  delay(50);
  TRIP();
  delay(100);
 }

}

// Função de inicio do motor
void MOTOR_OFF(){
  ST_ALARM = 0;
  digitalWrite(RELAY_PIN_G, 1);
  digitalWrite(RELAY_PIN_R, 0);
  value_R = 0;
  send(msg_R.set(value_R)); // envia mensagem para o domoticz para delisgar o motor
  digitalWrite(RELAY_PIN_Y, 0);
   value_Y = 0;
  send(msg_Y.set(value_Y)); // envia mensagem para o domoticz para delisgaro sinal de temp
  digitalWrite(RELAY_PIN_B, 0);
   value_B = 0;
  send(msg_B.set(value_B)); // envia mensagem para o domoticz para delisgar o sinal de trip
}


// Função de reconhecimento do alarme de temperatura
void TEMP_ALTA(){
    digitalWrite(RELAY_PIN_Y, 1); // ativa o led de temperatura
    ST_LED_Y = 1;
    value_Y = RELAY_ON;
    send(msg_Y.set(value_Y)); // envia mensagem para o domoticz o valor de temperatura
    delay(100);
    value_R = RELAY_OFF;
    send(msg_R.set(value_R)); // envia mensagem para o domoticz para delisgar o motor
    delay(100);
    digitalWrite(RELAY_PIN_R, 0);// desativa o led vermelho
    digitalWrite(RELAY_PIN_G, 1);// ativo o led verde
}


// Função de reconhecimento do alarme de trip
void TRIP(){
    digitalWrite(RELAY_PIN_B, 1); // ativa o led de temperatura
    ST_LED_B = 1;
    value_B = RELAY_ON;
    send(msg_B.set(value_B)); // envia mensagem para o domoticz o valor de trip
    delay(100);
    value_R = RELAY_OFF;
    send(msg_R.set(value_R)); // envia mensagem para o domoticz para delisgar o motor
    delay(100);
    digitalWrite(RELAY_PIN_R, 0); // desativa o led vermelho
    digitalWrite(RELAY_PIN_G, 1); // ativo o led verde
}

void MOTOR_ALARM(){
    delay(100);
    value_R = RELAY_OFF;
    send(msg_R.set(value_R)); // envia mensagem para o domoticz para delisgar o motor
    digitalWrite(RELAY_PIN_R, 0);// desativa o led vermelho
    digitalWrite(RELAY_PIN_G, 1);// ativo o led verde
    ST_ALARM = 0;
}

void receive(const MyMessage &message)
{
  // Atuação remota do domoticz


  if (message.getType()==V_STATUS) {
    // Leitura do botao remoto

    // -----------------------------------
    // FUNCIONAMENTO DO MOTOR REMOTO
    // -----------------------------------
    if(message.getSensor() == 2){ // IDENTIFICAO DO BOTAO REMOTO

    if (ST_LED_Y == 1 or ST_LED_B == 1) //verifico a situacao do motor
    {
       ST_ALARM = 1;
    }
    else{
      // VERIFICAÇÃO DO SINAL DO BOTAO (SE = 1  OU 0)
     if( message.getBool()?RELAY_ON:RELAY_OFF == 1) {


        digitalWrite(RELAY_PIN_R, message.getBool()?RELAY_ON:RELAY_OFF); // ATIVO LED VERMELHO (MOTOR LIGADO)
        ST_LED_R = 1;
        value_R = 1;
        ST_LED_G = 0;
        digitalWrite(RELAY_PIN_G, 0); // DESATIVO LED VERDE
        value_G = 0;
      }
      else{
         ST_LED_R = 0;
         value_R = value_R == RELAY_ON ? RELAY_OFF: RELAY_ON; // DESATIVO LED VERMELHO
         ST_LED_G = 1;
         digitalWrite(RELAY_PIN_G, message.getBool()?RELAY_ON:RELAY_OFF); // ATIVO LED VERDE (MOTOR DESLIGADO)
         value_G = value_G == RELAY_ON ? RELAY_OFF: RELAY_ON;
      }
    }
    }

    // -----------------------------------
    // FUNCIONAMENTO DO ALARME DE TEMPERATURA REMOTO E RECONHECIMENTO
    // -----------------------------------

   
    else if (message.getSensor() == 3){ // IDENTIFICAO DO BOTAO REMOTO

      if( message.getBool()?RELAY_ON:RELAY_OFF == 1) {
       TEMP_ALTA(); // BUSCO A FUNCAO DE TEMPERATURA
      }
      else{
         ST_LED_Y = 0;
         value_Y = value_Y == RELAY_ON ? RELAY_OFF: RELAY_ON;
         digitalWrite(RELAY_PIN_Y, message.getBool()?RELAY_ON:RELAY_OFF);  // DESATIVO LED AMARELO
      }
    }

    // -----------------------------------
    // FUNCIONAMENTO DO ALARME DE TRIP REMOTO E RECONHECIMENTO
    // -----------------------------------

    else if (message.getSensor() == 4){ // IDENTIFICAO DO BOTAO REMOTO

      if( message.getBool()?RELAY_ON:RELAY_OFF == 1) {
       TRIP();  // BUSCO A FUNCAO DE TRIP
      }
      else{
         ST_LED_B = 0;
         value_B = value_B == RELAY_ON ? RELAY_OFF: RELAY_ON;
         digitalWrite(RELAY_PIN_B, message.getBool()?RELAY_ON:RELAY_OFF); // DESATIVO LED AZUL
      }
    }
   
    // Store state in eeprom
   if (ST_ALARM == 1){
   MOTOR_ALARM(); // Situação do motor em interlock de falha
   }
   else{
    saveState(message.getSensor(), message.getBool());
    // Write some debug info
    Serial.print("Incoming change for sensor:");
    Serial.print(message.getSensor());
    Serial.print(", New status: ");
    Serial.println(message.getBool());
   }
   
  }
}