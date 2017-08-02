// Copyright (c) LooUQ Incorporated. All rights reserved.
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Use Arduino IDE 1.6.8 or later.
#include "globals.h"
#include <time.h>
#include <sys/time.h>

// board specific defintions and includes
#ifdef ARDUINO_SAMD_FEATHER_M0
  #define VBAT_ENABLED              1
  #define VBAT_PIN                  A7

  #define WINC_CS   8
  #define WINC_IRQ  7
  #define WINC_RST  4
  #define WINC_EN   2

  #include <WiFi101.h>
  #include <WiFiSSLClient.h>
  #include <WiFiUdp.h>
  WiFiSSLClient sslClient;
  #include "NTPClient.h"

#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <WiFiClientSecure.h>
  #include <WiFiUdp.h>
  WiFiClientSecure sslClient;               // for ESP8266

#elif defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) 
  #include <WiFi101.h>
  #include <WiFiSSLClient.h>
  #include <WiFiUdp.h>
  WiFiSSLClient sslClient;
  #include <NTPClient.h>
#endif //wifi101

// library includes to convert (" " to < >)
#include <iotQi.h>
#include <iotQiModel.h>
#include <iotQiNetwork.h>

// end: library includes to convert


// iotQi project (ino) includes
#include "settings.h"
#include "userModel.h"
#include "bme280.h"
// end: project includes

static iotQiWiFiNetwork iotqiNetwork;

/* setup() ------------------------------------------------------------------------------------------------------------ */

void setup() {
    initSerial();
    initWifi();
    initTime();
    initBme(UNITS_TYPE_METRIC);
    
    // now initialize iotQi and your device data model
    initIotqi(IOTQI_CONNECTION, UserModel_GetCommands, UserModel_CommandMsgCallback);
    initUserModel();

    iotqiNetwork.begin(WiFi);
}


/* loop() ------------------------------------------------------------------------------------------------------------- */

/* Multiple telemetry "loops" can be defined by creating additional prevUserDoWork and TELEMETRY_INTERVAL var\val pairs */


static unsigned long prevUserDoWork = 0;

void loop() 
{
	// Sample Interval Mechanics
    bool doUserWork = false;
    unsigned long currMillis = millis();

    if ( currMillis - prevUserDoWork >= ( TELEMETRY_INTERVAL*1000 ) )        // TELEMETRY_INTERVAL: interval at which to send telemetry sample to iotQi cloud (defined in settings.h)
    {
        prevUserDoWork = currMillis;
        doUserWork = true;
    }
    if ( doUserWork )
    {
        Serial.println("loop(): calling userModel_DoWork() \r\n");
		// User model implements telemetry samples
        UserModel_DoWork();
    }
    /* send & check for commands */
    iotQiDoWork();
}


/* INIT functions ===================================================================================================== */

#pragma region DeviceInitialization

void initSerial() {
  //Initialize serial and wait for port to open:
  #ifdef ESP8266
    Serial.begin(115200);
    Serial.setDebugOutput(true);
  #elif defined(ARDUINO_SAMD_FEATHER_M0)
    delay(10000);
    Serial.begin(115200);
  #else
    // For SAMD boards (e.g. MKR1000, Arduino Zero)
    Serial.begin(9600);
  #endif
}


void initWifi() {
  #ifdef ARDUINO_SAMD_FEATHER_M0
    Serial.println(F("WINC1500 on FeatherM0 detected."));
    Serial.println(F("Setting pins for WiFi101 library (WINC1500 on FeatherM0)"));

    //Configure pins for Adafruit ATWINC1500 Feather
    WiFi.setPins(WINC_CS, WINC_IRQ, WINC_RST, WINC_EN);

    // for the Adafruit WINC1500 we need to enable the chip
    pinMode(WINC_EN, OUTPUT);
    digitalWrite(WINC_EN, HIGH);
    Serial.println(F("Enabled WINC1500 interface for FeatherM0"));
  #endif

  // check for the presence of the shield :
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(WIFI_SSID);

  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(WIFI_SSID, WIFI_PW);
  int8_t rssi = WiFi.RSSI();
  Serial.print("RSSI=");  
  Serial.println(rssi);

  #define CONNECTION_TIMEOUT 10000

  int connStart = millis();
  while (WiFi.status() != WL_CONNECTED && ((millis() - connStart) < CONNECTION_TIMEOUT)) {
    delay(500);
    Serial.print(".");
  }

  IPAddress ipAddress = IPAddress();

  if (WiFi.status() == WL_CONNECTED) {
    ipAddress = WiFi.localIP();
    Serial.print("Connected to WiFi at: ");
    Serial.println(ipAddress);
  }
  else {
    Serial.println("\r\nWiFi connect failed, No network available!");
    Serial.println("Device halted.");
    while(true);
  }
}


void initTime() {
  #ifdef ARDUINO_SAMD_FEATHER_M0
    //  Adafruit_WINC1500UDP     _udp;
    WiFiUDP     _udp;
  #elif defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000)
    WiFiUDP     _udp;
  #endif
  
  unsigned long epochTime = 0;
    
  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  
    Serial.println("Fetching NTP time...");
    while (true) {
      epochTime = time(NULL);
      if (epochTime == 0) {
        Serial.println("Fetching NTP epoch time failed! Waiting 2 seconds to retry.");
        delay(2000);
      }
      else {
        Serial.print("Fetched NTP epoch time is: ");
        Serial.println(epochTime);
        break;
      }
    }
  #else
    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP, "time.nist.gov");          // substitute a different time server such as pool.ntp.org, your choice
    Serial.println("Fetching NTP time...");
  
    timeClient.begin();
    while (!timeClient.update()) {
      Serial.println("Fetching NTP epoch time failed! Waiting 5 seconds to retry.");
      delay(5000);
    }
    timeClient.end();
  
    epochTime = timeClient.getEpochTime();
    Serial.print("Fetched NTP epoch time is: ");
    Serial.println(epochTime);

    //iotHubClient.setEpochTime(epochTime);
  
    struct timeval tv;
    tv.tv_sec = epochTime;
    tv.tv_usec = 0;
    settimeofday(&tv, NULL);
  #endif // !ESP8266
}

#pragma endregion

