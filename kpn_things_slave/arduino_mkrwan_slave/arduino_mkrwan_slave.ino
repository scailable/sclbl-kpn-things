#include <MKRWAN.h>
LoRaModem modem;

#define APPEUI "0059AC0000010A74"
#define APPKEY "ed6853f6b287c6de22480dfba9d61f71"
#define DEBUG true

// I2C
#include <Wire.h>

// Thingsml
#include <thingsml.h>
SenMLPack device;
SenMLStringRecord emotion("emotion", SENML_UNIT_NONE, NULL);

// Classes
const char *classes[7] = {"angry", "disgust", "fear", "happy", "sad", "surprise", "neutral"};

void setup() {

  //////////////////////////////////////
  // serial to monitor
  Serial.begin(9600);
  while (!Serial);
  
  /*
  //////////////////////////////////////
  // start lora modem
  if (!modem.begin(EU868)) {
    Serial.println("Failed to start module");
    while (1) {}
  };
  // get lora modem info
  Serial.print("Your module version is: ");
  Serial.println(modem.version());
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());
  // join over gateway
  modem.setPort(1);
  int connected = modem.joinOTAA(APPEUI, APPKEY);
  if (!connected) {
    Serial.println("Something went wrong; are you indoor? Move near a window and retry");
    while (1) {}
  }
  // Set poll interval to 60 secs.
  modem.minPollInterval(60);
  Serial.println("Connected!");

  //////////////////////////////////////
  // add emotion record definition for KPN things
  device.add(emotion);
  */

  //////////////////////////////////////
  Serial.begin(9600);           // start serial for output
  // set I2C
  Wire.begin(0x08); 
  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent);

  Serial.print("running");
  
}

void receiveEvent(int howMany)
{
  while(1 < Wire.available()) // loop through all but the last
  {
    int x = Wire.read(); // receive byte as a character
      Serial.println(x);             // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);             // print the character
  Serial.println(classes[x]);         

  /*
  Serial.println("Sending message...");
  const char str[] = classes[x];
  emotion.set(str);

  modem.beginPacket();
  device.toCbor(modem);
  int err = modem.endPacket(false);
  if (err > 0) {
    Serial.println("Message sent correctly!");
  } 
  */
  
}
void loop() {
  delay(3000); 
}
