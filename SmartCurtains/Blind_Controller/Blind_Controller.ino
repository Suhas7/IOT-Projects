#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "config.h"

char auth[] = BLYNK_KEY;
char ssid[] = WIFI_NAME;
char pass[] = WIFI_PASS;

WidgetTerminal terminal(V1);

int cycle=0;
float state;

void setState(int newState){
  Serial.print("old state is ");
  Serial.print(state);
  Serial.print(" and new state is ");
  Serial.println(newState);
  if(newState<state){
    digitalWrite(27, HIGH);
    digitalWrite(25, LOW);
    digitalWrite(26, HIGH);
  }else if(newState>state){
    digitalWrite(27, HIGH);
    digitalWrite(25, HIGH);
    digitalWrite(26, LOW);
  }else{return;}
  delay(11000);
  digitalWrite(25, LOW);
  digitalWrite(26, LOW);
  digitalWrite(27, LOW);
  state=newState; 
}

BLYNK_WRITE(V1){
  setState(param.asInt());
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  while (!Blynk.connected()) {
    delay(500);
    Serial.print(".");
  }
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  digitalWrite(15, LOW);
}

void loop() { 
  Blynk.run();
}
