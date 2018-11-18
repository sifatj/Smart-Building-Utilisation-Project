/*
 * TTN code for Arduino.
 * Use OTAA method if there are issues with ABP method.
 */

#include <TheThingsNetwork.h>
#define loraSerial Serial1
#define debugSerial Serial
#define freqPlan TTN_FP_EU868

//Activation By Personalisation (ABP) Method
const char *devAddr = ""; // Device Address
const char *nwkSKey = ""; // Network Session Key
const char *appSKey = ""; // App Session Key

//Over-the-Air Activation (OTAA) Method
//const char *appEui = ""; // App EUI
//const char *appKey = ""; // App Key

int sensorPin = 2; // Pin for PIR Sensor 
int movementDetected = 0;

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

void setup() {  
  loraSerial.begin(57600);
  debugSerial.begin(9600);
  
  pinMode(sensorPin, INPUT); // Declare sensor as input

  while(!debugSerial && millis()<10000);
  debugSerial.println("Device Owner: Smart Building");
  debugSerial.println("\n###STATUS###");
  ttn.showStatus();

  debugSerial.println("\n###JOIN PROCESS###");

  // ABP Method
  ttn.personalize(devAddr, nwkSKey, appSKey);

  //OTAA method
  //ttn.join(appEui, appKey); 
}

void loop() {
  // Run repeatedly:
  byte data[1];
  
  //Read digital PIR sensor and save value in the variable 
  movementDetected = digitalRead(sensorPin); // Get digital reading from PIR sensor
  
  if(movementDetected){
    Serial.println("Movement Detected " + String(movementDetected));
    data[0] = movementDetected;
    delay(2000);
  }
  else
  {
    Serial.println("No movement detected " + String(movementDetected)); 
    delay(2000);  
  }
  
  //Send the message to The Things Network
  ttn.sendBytes(data, sizeof(data));
}
