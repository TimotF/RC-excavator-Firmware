#include <Arduino.h>
#include <ESP32Servo.h>
#include "Adafruit_MCP23X17.h"
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include "secrets.h"

const char *ssid = WIFI_SSID;
const char *password = WIFI_PWD;

// defines
#define clawServoPin 5
#define auxServoPin 18
#define cabLights 32
#define auxLights 33

#define pivot0 15
#define pivot1 14
#define mainBoom0 9
#define mainBoom1 8
#define secondBoom0 0
#define secondBoom1 1
#define tiltAttach0 3
#define tiltAttach1 2
#define thumb0 11
#define thumb1 10
#define auxAttach0 12
#define auxAttach1 13

#define leftMotor0 7
#define leftMotor1 6
#define rightMotor0 4
#define rightMotor1 5

Adafruit_MCP23X17 mcp;
Servo clawServo;
Servo auxServo;
int dly = 250;
int clawServoValue = 90;
int auxServoValue = 90;
int player = 0;
int battery = 0;
int servoDelay = 0;

bool cabLightsOn = false;
bool auxLightsOn = false;
bool moveClawServoUp = false;
bool moveClawServoDown = false;
bool moveAuxServoUp = false;
bool moveAuxServoDown = false;

void setup()
{
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA
      .onStart([]()
               {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
      } else {  // U_SPIFFS
        type = "filesystem";
      }

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type); })
      .onEnd([]()
             { Serial.println("\nEnd"); })
      .onProgress([](unsigned int progress, unsigned int total)
                  { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
      .onError([](ota_error_t error)
               {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
      } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
      } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
      } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
      } else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
      } });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  ArduinoOTA.handle();
}
