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

/* ---------- IO Define ----------------*/
const int LED1_PIN = 5;
const int LED2_PIN = 4;
const int LED3_PIN = 0;

const int RelayA_PIN = 2;
const int RelayC_PIN = 14;

const int Switch_PIN = 10;
/*---------------------------------------*/

int LightStatus = 0;

int lastSwitchState    = 0;
int currentSwitchState = 0;

void toggleLight()
{
  if(LightStatus == 0)
  {
    Blynk.virtualWrite(V0, LOW);
    Serial.println("SET Light => OFF");
  }
  else
  {
    Blynk.virtualWrite(V0, HIGH);
    Serial.println("SET Light => ON");
  }
  Serial.println("");
}

// Button event (Attach to V0) from Blynk App
BLYNK_WRITE(V0)
{
  //int pinValue = param.asInt();
  Serial.print("[APP Event] : ");
  
  LightStatus ^= 1;
  toggleLight();
}

BlynkTimer timer;
void myTimerEvent()
{
    currentSwitchState = digitalRead(Switch_PIN);
    if (currentSwitchState != lastSwitchState)
    {
      Serial.print("[Manual Event] : ");
      Serial.print(lastSwitchState);
      Serial.print(" to ");
      Serial.println(currentSwitchState);
      
      lastSwitchState = currentSwitchState;

      LightStatus ^= 1;
      toggleLight();
    }
}

void setup()
{
  pinMode(RelayA_PIN, OUTPUT);
  pinMode(RelayC_PIN, OUTPUT);
  digitalWrite(RelayA_PIN, HIGH);
  digitalWrite(RelayC_PIN, LOW);
  
  // Debug console
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(100L, myTimerEvent);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, HIGH);
  digitalWrite(LED3_PIN, HIGH);

  pinMode(Switch_PIN, INPUT);
}

void loop()
{
  Blynk.run();
  timer.run(); 
}

