#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include "DHT.h"
#include <BlynkSimpleEsp32.h>
#include "config.h"

char auth[] = BLYNK_KEY;
char ssid[] = WIFI_NAME;
char pass[] = WIFI_PASS

WidgetTerminal terminal(V1);

#define DHTPIN A1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
int cycle=0;
bool active;
int state;
bool buttState=false;

BLYNK_WRITE(V1){
  active = true;
  Serial.println("api req"):
  setState(param.asInt());
}

void setState(int newState){
  Serial.print("old state is ");
  Serial.print(state);
  Serial.print(" and new state is ");
  Serial.println(newState);
  if(state<newState){
    digitalWrite(15, HIGH);
    digitalWrite(14, LOW);
    digitalWrite(32, HIGH);
  }else if(state>newState){
    digitalWrite(15, HIGH);
    digitalWrite(14, HIGH);
    digitalWrite(32, LOW);
  }
  else{return;}
  delay(3350*abs(state-newState));
  digitalWrite(14, LOW);
  digitalWrite(32, LOW);
  digitalWrite(15, LOW);
  state=newState; 
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  while (!Blynk.connected()) {
    delay(500);
    Serial.print(".");
  }
  pinMode(14, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(15, OUTPUT);
  digitalWrite(15, LOW);
  active=true;
//  Blynk.syncVirtual(V1);
//  esp_sleep_enable_timer_wakeup(10e6);
//  esp_deep_sleep_start();
}

void loop() { 
  Blynk.run();
  //Serial.println("Button is reading");
  if(!buttState && digitalRead(27)){
    setState((state==0)?2:0);
  }
  buttState = (bool) digitalRead(27);
  float temp = dht.readTemperature(true);
  if(temp<70){
      setState(0);
  }
//  Serial.println(temp);
}
