/***************************************************************************
*!
    @author  LooUQ Incorporated, G.Terrell

    @brief   Fork of the Adafruit FeatherOLED library, provides support for 
             the iotQi client library 

  Copyright (c) 2016, LooUQ Incorporated (loouq.com).  All rights reserved.

***************************************************************************/

/**************************************************************************/
/*!
    @file     Adafruit_FeatherOLED_WiFi.h
    @author   ktown

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2016, Adafruit Industries (adafruit.com)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/

#ifndef _IOTQI_FEATHEROLED_WIFI_H_
#define _IOTQI_FEATHEROLED_WIFI_H_

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "iotQi_FeatherOLED.h"

class iotQi_FeatherOLED_WiFi : public iotQi_FeatherOLED
{
  protected:
    bool    _connected;
    bool    _connectedVisible;
    int     _rssi;
    bool    _rssiVisible;
    char    _ssid[128];
    bool    _ssidVisible;
    int32_t _ipAddress;
    bool    _ipAddressVisible;

    void renderRSSI       ( void );
    void renderSSID       ( void );
    void renderConnected  ( void );
    void renderIPAddress  ( void );

  public:

    // Constructor
    iotQi_FeatherOLED_WiFi ( int reset = -1 ) : iotQi_FeatherOLED(reset)
    {
      _connected          = false;
      _connectedVisible   = true;
      _rssi               = -99;
      _rssiVisible        = false;
      strcpy(_ssid, "");
      _ssidVisible        = false;
      _ipAddress          = 0;
      _ipAddressVisible   = true;
    }

    void setConnected         ( bool conn )      { _connected = conn; }
    void setConnectedVisible  ( bool enable )    { _connectedVisible = enable; }
    void setRSSI              ( int rssi )       { _rssi = rssi; }
    void setRSSIVisible       ( bool enable )    { _rssiVisible = enable; }
    void setSSID              ( char* ssid )     { strcpy(_ssid, ssid); }
    void setSSIDVisible       ( bool enable )    { _ssidVisible = enable; }
    void setIPAddress         ( uint32_t addr )  { _ipAddress = addr; }
    void setIPAddressVisible  ( bool enable )    { _ipAddressVisible = enable; }
    void refreshIcons         ( void );

    // iotQi extensions 
    void clearLine      ( int lineNum );
    void writeLine      ( int lineNum, char const *text );
    void writeLineAt    ( int lineNum, int charPos, char const *text );
};

#endif /* _IOTQI_FEATHEROLED_WIFI_H_ */
