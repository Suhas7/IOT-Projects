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
bool buttState=false;

struct DEV_TempSensor : Service::TemperatureSensor {     // A standalone Temperature sensor
  SpanCharacteristic *temp;                         // reference to the Current Temperature Characteristic
  DEV_TempSensor() : Service::TemperatureSensor(){       // constructor() method
    temp=new Characteristic::CurrentTemperature(-10.0);        // instantiate the Current Temperature Characteristic
    temp->setRange(-50,110);                                  // expand the range from the HAP default of 0-100 to -50 to 100 to allow for negative temperatures
    temp->setVal(dht.readTemperature(true));
    Serial.print("Configuring Temperature Sensor");           // initialization message
    Serial.print("\n");
  }
  void loop(){
    if(temp->timeVal()>15000){
      temp->setVal(dht.readTemperature()-4);
      Serial.print("Current temp: ");
      Serial.println(temp->getVal());
    }
  }
};

DEV_TempSensor* TS;
struct AirConditioner : Service::Fan{
  SpanCharacteristic *active;
  SpanCharacteristic *level;
  AirConditioner() : Service::Fan(){       // constructor() method for TableLamp defined with one parameter.  Note we also call the constructor() method for the LightBulb Service.
    active=new Characteristic::Active();
    level=(new Characteristic::RotationSpeed(0))->setRange(0,2,1);
    pinMode(14, OUTPUT);
    pinMode(32, OUTPUT);
    pinMode(15, OUTPUT);
    digitalWrite(15, LOW);   
  }
  boolean update(){                          // update() method
    Serial.println("hk update");
    if(active->getNewVal()) setState(level->getNewVal());
    else setState(0);
    return(true);
  }
  void setState(int newState){
    if((level->getVal())<newState){
        digitalWrite(15, HIGH);
        digitalWrite(14, LOW);
        digitalWrite(32, HIGH);
    }else if((level->getVal())>newState){
        digitalWrite(15, HIGH);
        digitalWrite(14, HIGH);
        digitalWrite(32, LOW);
      }
      else{return;}
    delay(3350*abs((level->getVal())-newState));
      digitalWrite(14, LOW);
      digitalWrite(32, LOW);
      digitalWrite(15, LOW);
    level->setVal(newState);
  }
  boolean manualStateUpdate(int x){
    active->setVal(x!=0);
    setState(x);
    Serial.println(level->getNewVal());
    update();
  }
  int getState(){
    return level->getVal();  
  }
  int getThresholdTemp(){
    //return temp->getVal();
  }
};

AirConditioner* AC;
void setup() {
  Serial.begin(115200);
  dht.begin();
  homeSpan.begin(Category::AirConditioners,"Air Conditioner");homeSpan.enableOTA();
  Blynk.begin(auth, ssid, pass);
  while (!Blynk.connected()) {
    delay(500);
    Serial.print(".");
  }
  pinMode(14, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(15, OUTPUT);
  digitalWrite(15, LOW);
  new SpanAccessory();                  
    new Service::AccessoryInformation();            // HAP requires every Accessory to implement an AccessoryInformation Service, with 6 *required* Characteristics
      new Characteristic::Name("Temp Sensor");                   // create all the required Characteristics with values set based on above arguments
      new Characteristic::Manufacturer("HomeSpan");
      new Characteristic::SerialNumber("123-ABD");    
      new Characteristic::Model("Sensor");
      new Characteristic::FirmwareRevision("0.1");
      new Characteristic::Identify();
    new Service::HAPProtocolInformation();          // HAP requires every Accessory (except those in a bridge) to implement a Protcol Information Service  
    new Characteristic::Version("1.1.0");           // Set the Version Characteristic to "1.1.0," which is required by HAP
    TS=new DEV_TempSensor();
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
  AC=new AirConditioner();
}

void loop() {
  Blynk.run();
  homeSpan.poll();
}

BLYNK_WRITE(V1){
  Serial.println("blynk hit");
  AC->manualStateUpdate(param.asInt());
}
