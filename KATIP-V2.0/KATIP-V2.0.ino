/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Social networks:            http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on NodeMCU.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right NodeMCU module
  in the Tools -> Board menu!

  For advanced settings please follow ESP examples :
   - ESP8266_Standalone_Manual_IP.ino
   - ESP8266_Standalone_SmartConfig.ino
   - ESP8266_Standalone_SSL.ino

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "token.h"

#define Relay_ON    1
#define Relay_OFF   0

#define Switch_ON   0
#define Switch_OFF  1

/* ---------- IO Define ----------------*/
const int LED1_PIN = 5;
const int LED2_PIN = 4;
//const int LED3_PIN = 0;

const int RelayA_PIN = 2;
const int RelayC_PIN = 14;

const int Switch_PIN = 10;
/*---------------------------------------*/

int RelayStatus = 0;

int lastSwitchState    = 0;
int currentSwitchState = 0;

void turnON()
{
  digitalWrite(RelayA_PIN, HIGH);
  digitalWrite(RelayC_PIN, LOW);
  delay(50);
  digitalWrite(RelayA_PIN, LOW);
  digitalWrite(RelayC_PIN, LOW);
  
  //digitalWrite(LED3_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  Blynk.virtualWrite(V0, HIGH);
}

void turnOFF()
{
  digitalWrite(RelayA_PIN, LOW);
  digitalWrite(RelayC_PIN, HIGH);
  delay(50);
  digitalWrite(RelayA_PIN, LOW);
  digitalWrite(RelayC_PIN, LOW);
  
  //digitalWrite(LED3_PIN, HIGH);
  digitalWrite(LED2_PIN, HIGH);
  Blynk.virtualWrite(V0, LOW);
}

void togglRelay()
{
  if(RelayStatus == Relay_ON)
  {
    turnOFF();
    Blynk.virtualWrite(V0, LOW);
    Serial.println("Relay: OFF");
    RelayStatus = Relay_OFF;
  }
  else
  {
    turnON();
    Blynk.virtualWrite(V0, HIGH);
    Serial.println("Relay: ON");
    RelayStatus = Relay_ON;
  }
  Serial.println("");
}

// Button event (Attach to V0) from Blynk App
BLYNK_WRITE(V0)
{
  int pinValue = param.asInt();
  if(pinValue == 1)
  {
     turnON();
     Serial.println("[APP Event] Set Relay: ON");
     RelayStatus = Relay_ON;
  }
  else
  {
     turnOFF(); 
     Serial.println("[APP Event] Set Relay: OFF");
     RelayStatus = Relay_OFF;
        
  }
}

BlynkTimer timer;
void myTimerEvent()
{
    currentSwitchState = digitalRead(Switch_PIN);
    if (currentSwitchState != lastSwitchState)
    {
      togglRelay();
      if (currentSwitchState == Switch_ON)
      {
         Serial.println("[Manual Event] : Switch is ON");
      }
      else
      {
         Serial.println("[Manual Event] : Switch is OFF");
      }
      lastSwitchState = currentSwitchState;
    }
}

void setup()
{
  Serial.println("!!! Board Reset !!!");
  pinMode(RelayA_PIN, OUTPUT);
  pinMode(RelayC_PIN, OUTPUT);

  
  // Debug console
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(100L, myTimerEvent);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  //pinMode(LED3_PIN, OUTPUT);
  
  //digitalWrite(LED1_PIN, HIGH);
  //digitalWrite(LED2_PIN, HIGH);
  //digitalWrite(LED3_PIN, HIGH);

  pinMode(Switch_PIN, INPUT);
  currentSwitchState = digitalRead(Switch_PIN);
  lastSwitchState = currentSwitchState;

  Serial.println();
  if(lastSwitchState == Switch_ON)
  {
    Serial.println("Init: Switch is at ON");
    RelayStatus = Relay_ON;
    turnON();
  }
  else
  {
    Serial.println("Init: Switch is at OFF");
    RelayStatus = Relay_OFF;
    turnOFF();
  }
}


unsigned int count = 0;
unsigned char normalState = 0;
void loop()
{
  Blynk.run();
  timer.run();

  count++;
  if(count >= 30000)
  {
     count = 0;
     if(normalState == 1)
     {
        normalState = 0;
        digitalWrite(LED1_PIN, LOW);
     }
     else
     {
        normalState = 1;
        digitalWrite(LED1_PIN, HIGH);
     } 
  }
}

