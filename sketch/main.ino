#include <SPI.h>       //SPI 
#include <MFRC522.h>   //RFID Reader
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

//RIFD Reader
#define RST_PIN  05 //Pin D1
#define SS_PIN   04 //Pin D2
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;

//DoorSwitch
#define SWITCH_PIN 16 //Pin D0

//Customize
const int maxUser = 100;
const char *masterUID = "0000000";
const char *ssid = "SSID";  //WIFI SETTINGS
const char *password = "PASSWORD";
const char *host = "API-HOST";
const int port = 443;
const char *param = "API-URI+apikey";
const char fingerprint[] PROGMEM = "SHA1Fingerprint"; //SHA1 Fingerprint of SSL Certificate
//End Customize


String allowedUID[maxUser];

// Init array that will store new NUID
byte nuidPICC[4];
String currentUID;

//State 0=boot, 1=wifi, 2=fetch,3=ready
//closed, is door closed (does not say anything about the locking state)
int state = 0;
boolean closed = true;
boolean locked = true;
//#####################################################
void setup() {
  delay(500);
  Serial.begin(9600); // start serial connection to print out debug messages and data
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

  pinMode(16, INPUT);


  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  
  connectWifi();
  
  Serial.println("boot complete");
  fetchTeri();
  disableWifi();
  printallowedUID();
  doorStatus();
}

void loop() {
  //If ready
  if(state==3 && closed == true) {
    // Look for new cards
    if ( ! rfid.PICC_IsNewCardPresent())
      return;
  
    // Verify if the NUID has been readed
    if ( ! rfid.PICC_ReadCardSerial())
      return;
  
  
  
    for (byte i = 0; i < 4; i++) {      //Read UID
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
  
    currentUID = convertHex(rfid.uid.uidByte, rfid.uid.size); //Convert UID to Hex
    Serial.print(currentUID);
    
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  
    if(checkUser()) {
      Serial.print(" is allowed");
      Serial.println("");
      openDoor();
      delay(2000);
      doorStatus();
      if(closed) {
        closeDoor();
      }
    }
    else {
      Serial.print(" is not allowed");
      Serial.println("");
    }
  }
  else if (state == 3 && closed == false) {
    while (!closed) {
      doorStatus();
      delay(400);
    }
    closeDoor();
  }
  

  

}




//###############################################
// 
void die() {

}
//check if UID in allowedUID array
boolean checkUser() {
  if (currentUID == masterUID) {
    return true;
  }
  for (byte i = 0; i < maxUser; i++) {
    if (currentUID == allowedUID[i]) {
      return true;
    }
}
return false;
}

void printallowedUID() {
    Serial.println("----------------------");
    Serial.println("allowed UID");
    for (byte i = 0; i < maxUser; i++) {
      if(allowedUID[i] != "") {
        Serial.println(allowedUID[i]);
      }
    }
    Serial.println("----------------------");
}


String convertHex(byte *buffer, byte bufferSize) {
  String currentUID;
  for (byte i = 0; i < bufferSize; i++) {
    currentUID += String(buffer[i], HEX);
  }
  return currentUID;
}


//#######################################################################
