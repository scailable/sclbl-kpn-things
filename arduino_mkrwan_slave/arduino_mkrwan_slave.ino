/*             
 *
 * (c) 2021 Scailable / KPN
 *
 * https://github.com/scailable/sclbl-kpn-things
 * https://scailable.net
 * https://www.kpn.com/zakelijk/internet-of-things/kpn-things.htm
 *
 * License: MIT License
 *
 * In order to retrieve your DeviceUID, do the following in your Arduino IDE:
 *
 * - from Library manager (Tools->Manage Libraries...), search for 'ThingsML' and install
 * - flash program and run one time to get deveui
 * - register in kpn things, gets appeui and appkey
 * - fill out appeui and appkey in the source code
 * - flash final program and run
 *
 */

//////////////////////////////////////
// To DEBUG or not to DEBUG
#define DEBUG true

//////////////////////////////////////
// set APPEUI and APPKEY here
// see for instance https://docs.kpnthings.com/portal/getting-started/connect-a-device/marvin#register-your-marvin-to-kpn-things 
// for a guideline on how to obtain them
#define APPEUI ""
#define APPKEY ""

//////////////////////////////////////
// MKRWAN.h provides APIs to communicate with LoRa and LoRaWAN networks
// [https://github.com/arduino-libraries/MKRWAN/blob/master/src/MKRWAN.h]
#include <MKRWAN.h>
LoRaModem modem;

//////////////////////////////////////
// Wire.h is needed for I2C communication between devices
// [https://www.arduino.cc/en/reference/wire]
#include <Wire.h>

//////////////////////////////////////
// The KPN-IOT Thingsml library 
// [https://github.com/kpn-iot/thingsml-c-library]
#include <thingsml.h>
SenMLPack device;
SenMLStringRecord emotion("emotion", SENML_UNIT_NONE, NULL);

//////////////////////////////////////
// Emotions recognized by Scailable.net optimized emotion model running on Raspberry Pi or ESP32
const char *classes[8] = {"angry", "disgust", "fear", "happy", "sad", "surprise", "neutral", "none"};

void setup() {

  //////////////////////////////////////
  // serial to monitor
  Serial.begin(9600);
  while (!Serial);
  
  
  //////////////////////////////////////
  // start lora modem
  
  if (!modem.begin(EU868)) {
    Serial.println("Failed to start module");
    while (1) {}
  };

  //////////////////////////////////////
  // get lora modem info
  Serial.print("Your module version is: ");
  Serial.println(modem.version());
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());

  //////////////////////////////////////
  // join over gateway
  modem.setPort(1);
  int connected = modem.joinOTAA(APPEUI, APPKEY);
  if (!connected) {
    Serial.println("Something went wrong; are you indoor? Move near a window and retry");
    while (1) {}
  }

  //////////////////////////////////////
  // Set poll interval to 60 secs.
  modem.minPollInterval(60);
  Serial.println("Connected!");

  //////////////////////////////////////
  // start serial for output
  Serial.begin(9600);      

  //////////////////////////////////////
  // add emotion record definition for KPN things
  device.add(emotion);

  //////////////////////////////////////
  // I2C communication
  Wire.begin(0x08); 
  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent);

  Serial.println("running");
  
}

//////////////////////////////////////
// triggered when message received over I2C
void receiveEvent(int howMany)
{
  
  while(1 < Wire.available())       // loop through all but the last
  {
    int x = Wire.read();            // receive byte as a character
      Serial.println(x);            // print the character
  }
  int x = Wire.read();              // receive byte as an integer
  Serial.println(x);                // print the character
  Serial.println(classes[x]);         

  
  Serial.println("Sending message...");
  emotion.set(classes[x]);

  modem.beginPacket();
  device.toCbor(modem);
  int err = modem.endPacket(false);
  if (err > 0) {
    Serial.println("Message sent correctly!");
  } 
 
  
}

void loop() {
  delay(3000); 
}
