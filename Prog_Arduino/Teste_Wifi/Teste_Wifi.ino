/*******************************************************************************
* ESP8266 Primeiros Passos com Arduino
* Codigo utilizado para enviar comandos AT atraves do monitor serial da Arduino
* IDE.
*******************************************************************************/  

#include "SoftwareSerial.h"

SoftwareSerial ESP_Serial(10, 11); // RX, TX

void setup(){
  Serial.begin(9600);
  ESP_Serial.begin(9600);
}

void loop(){
  if (ESP_Serial.available()){
    Serial.write(ESP_Serial.read());
  }
  if (Serial.available()){
    ESP_Serial.write(Serial.read());
  }
}