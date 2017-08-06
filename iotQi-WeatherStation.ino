// Copyright (c) LooUQ Incorporated. All rights reserved.
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Use Arduino IDE 1.6.8 or later.

// library iotQi includes
#include <iotQi.h>
#include <iotQiWifiNetwork.h>
// end: library iotQi includes

// iotQi project (ino) includes
#include "settings.h"
#include "userModel.h"
// end: project includes

static iotQiWiFiNetwork iotqiNetwork;

/* setup() ------------------------------------------------------------------------------------------------------------ */

void setup() {
    // Setup the serial communication
    delay(3000);
    Serial.begin(115200);
    pinMode(13, OUTPUT);

    // Network Connection Requirements
    iotqiNetwork.SetPassword(WIFI_PW);
    iotqiNetwork.SetSSID(WIFI_SSID);

    // now initialize iotQi and your device data model
    iotqiClient.Init(IOTQI_CONNECTION, iotqiNetwork);
    iotqiClient.Begin();
    iotqiClient.SetCallbacks(UserModel_GetCommands, UserModel_CommandMsgCallback);
    initUserModel();

    // Example Alerts
    iotqiClient.SendAlert("greeting", "Hello World!"); // Just a Subject
    iotqiClient.SendAlert("windAlert", "Wind Velocity High!", WindAlert); // Subject and a body

    initBme(UNITS_TYPE_METRIC);
}


/* loop() ------------------------------------------------------------------------------------------------------------- */

/* Multiple telemetry "loops" can be defined by creating additional prev<TelemetryType> \ TELEMETRY_INTERVAL variable\value pairs */
int prevBmeTelemetry = 0;

void loop() 
{
    /* send pending messages & check for commands */
    iotqiClient.DoWork();


    //  if ( currMillis - prevUserDoWork >= ( TELEMETRY_INTERVAL*1000 ) )        // TELEMETRY_INTERVAL: interval at which to send telemetry sample to iotQi cloud (defined in settings.h)
    //  {
    //      prevUserDoWork = currMillis;
    //      doUserWork = true;
    //  }
    //  if ( doUserWork )
    //  {
    //       ->User model implements telemetry samples
    //      UserModel_DoWork();
    //  }

    //                         your descriptive telemetry name, this is returned in your data
    //                         |                     userModel telemetry template to use
    //                         |                     |          the variable above that stores millis() from previous invoke
    //                         |                     |          |                 For periodic (timed) samples, the inter-sample period in milliseconds
    //                         |/                    |/         |/                |/
    iotqiClient.SendTelemetry("bmeSensor-telemetry", BmeSensor, prevBmeTelemetry, TELEMETRY_INTERVAL);
}


