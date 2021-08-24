#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include "DHT.h"
#include <BlynkSimpleEsp32.h>
#include "config.h"
#include "HomeSpan.h"

char auth[] = BLYNK_KEY;
char ssid[] = WIFI_NAME;
char pass[] = WIFI_PASS;

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
  setState(param.asInt());
}

void setState(int newState){
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
struct AirConditioner : Service::Fan{
  SpanCharacteristic *state;
  SpanCharacteristic *state2;
  AirConditioner() : Service::Fan(){       // constructor() method for TableLamp defined with one parameter.  Note we also call the constructor() method for the LightBulb Service.
    state=new Characteristic::Active();
    state2=(new Characteristic::RotationSpeed(0))->setRange(0,2,1);
    pinMode(14, OUTPUT);
    pinMode(32, OUTPUT);
    pinMode(15, OUTPUT);
    digitalWrite(15, LOW);   
  }
  boolean update(){                          // update() method
    Serial.println(state->getNewVal());
    setState(2*(state->getNewVal()));
    return(true);
  }
};
void setup() {
  Serial.begin(115200);
  homeSpan.begin();
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
  new SpanAccessory();           // Table Lamp Accessory
    new Service::AccessoryInformation();            // HAP requires every Accessory to implement an AccessoryInformation Service, with 6 *required* Characteristics
      new Characteristic::Name("My Air Conditioner");      // Name of the Accessory, which shows up on the HomeKit "tiles", and should be unique across Accessories                                                    
      new Characteristic::Manufacturer("Suhas Raja");   // Manufacturer of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::SerialNumber("123-ABC");    // Serial Number of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::Model("AC");     // Model of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::FirmwareRevision("0.1");    // Firmware of the Accessory (arbitrary text string, and can be the same for every Accessory)  
      new Characteristic::Identify();                 // Provides a hook that allows a HomeKit Client to identify the device
    new Service::HAPProtocolInformation();          // HAP requires every Accessory (except those in a bridge) to implement a Protcol Information Service  
      new Characteristic::Version("1.1.0");           // Set the Version Characteristic to "1.1.0," which is required by HAP
  new AirConditioner();
}

void loop() { 
  Blynk.run();
  //Serial.println("Button is reading");
  if(!buttState && digitalRead(27)){
    setState((state==0)?2:0);
  }
  buttState = (bool) digitalRead(27);
  float temp = dht.readTemperature(true);
  if(temp<71){
      setState(0);
  }
  homeSpan.poll();
}
