/*

ESP69GW

ESP8266 to RFM69 Gateway
Monteino gateway sample code
Based on sample code by Felix Rusu - http://LowPowerLab.com/contact

Copyright (C) 2016 by Xose Pérez <xose dot perez at gmail dot com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <RFM69Manager.h>
#include <SPI.h>
#include <SPIFlash.h>
//#include <WirelessHEX69.h>

// -----------------------------------------------------------------------------
// Configutation
// -----------------------------------------------------------------------------

#define NODEID              1
#define GATEWAYID           1
#define NETWORKID           164
#define FREQUENCY           RF69_868MHZ
#define ENCRYPTKEY          "fibonacci0123456"
#define IS_RFM69HW          0

#define SERIAL_BAUD         115200

#ifdef MONTEINO
    #define LED_PIN             9       // Moteinos have LEDs on D9
    #define FLASH_SS            8       // and FLASH SS on D8
    #define FLASH_ID            0xEF30  // EF30 for 4mbit  Windbond chip (W25X40CL)
#endif

#ifdef JEELINK
    #define LED_PIN             9       // Jeenodes have LEDs on D9
    #define FLASH_SS            8       // and FLASH SS on D8
    #define FLASH_ID            0x2020  // 2020 for ??
#endif

// -----------------------------------------------------------------------------
// Globals
// -----------------------------------------------------------------------------

SPIFlash flash(FLASH_SS, FLASH_ID);
RFM69Manager radio;

unsigned long transmitInterval = 2000;

// -----------------------------------------------------------------------------
// Utils
// -----------------------------------------------------------------------------

void blink(byte times, byte mseconds) {
    pinMode(LED_PIN, OUTPUT);
    for (byte i=0; i<times; i++) {
        digitalWrite(LED_PIN, LOW);
        delay(mseconds);
        digitalWrite(LED_PIN, HIGH);
        delay(mseconds);
    }
}

// -----------------------------------------------------------------------------
// Flash
// -----------------------------------------------------------------------------

void flashSetup() {

    if (flash.initialize()) {
        Serial.print("SPI Flash Init OK. Unique MAC = [");
        flash.readUniqueId();
        for (byte i=0;i<8;i++) {
            Serial.print(flash.UNIQUEID[i], HEX);
            if (i!=8) Serial.print(':');
        }
        Serial.println(']');
        blink(1, 50);

    } else {
        Serial.println("SPI Flash MEM not found (is chip soldered?)...");
        Serial.println(flash.readDeviceId());
        blink(5, 50);
    }

}

// -----------------------------------------------------------------------------
// RFM69
// -----------------------------------------------------------------------------

void radioSetup() {
    delay(10);
    radio.initialize(FREQUENCY, NODEID, NETWORKID, ENCRYPTKEY);
}

void radioLoop() {
    radio.loop();
}

// -----------------------------------------------------------------------------
// Common methods
// -----------------------------------------------------------------------------

void setup() {
    Serial.begin(SERIAL_BAUD);
    flashSetup();
    radioSetup();
}

void loop() {
    radioLoop();
    delay(1);
}
