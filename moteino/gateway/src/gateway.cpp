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
// Configuration
// -----------------------------------------------------------------------------

#define NODEID              2
#define GATEWAYID           2
#define PROMISCUOUS         1
#define NETWORKID           164
#define FREQUENCY           RF69_868MHZ
#define ENCRYPTKEY          "fibonacci0123456"
#define IS_RFM69HW          0

#define SERIAL_BAUD         115200

#ifdef MOTEINO
    #define LED_PIN         9       // Moteinos have LEDs on D9
    #define FLASH_SS        8       // and FLASH SS on D8
    #define FLASH_ID        0xEF30  // EF30 for 4mbit  Windbond chip (W25X40CL)
    #define USE_FLASH       1
#endif

#ifdef JEELINK
    #define LED_PIN         9       // Jeenodes have LEDs on D9
    #define FLASH_SS        8       // and FLASH SS on D8
    #define FLASH_ID        0x2020  // 2020 for ??
    #define USE_FLASH       0
#endif

// -----------------------------------------------------------------------------
// Globals
// -----------------------------------------------------------------------------

#if USE_FLASH
    SPIFlash flash(FLASH_SS, FLASH_ID);
#endif
RFM69Manager radio;

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

#if USE_FLASH

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

#endif

// -----------------------------------------------------------------------------
// RFM69
// -----------------------------------------------------------------------------

void radioMessage(packet_t * data) {

    blink(5, 1);

    Serial.print("[MESSAGE] messageID:");
    Serial.print(data->messageID);
    Serial.print(" senderID:");
    Serial.print(data->senderID);
    Serial.print(" targetID:");
    Serial.print(data->targetID);
    Serial.print(" packetID:");
    Serial.print(data->packetID);
    Serial.print(" name:");
    Serial.print(data->name);
    Serial.print(" value:");
    Serial.print(data->value);
    Serial.print(" rssi:");
    Serial.print(data->rssi);
    Serial.println();

}

void radioSetup() {
    delay(10);
    if (!radio.initialize(FREQUENCY, NODEID, NETWORKID, ENCRYPTKEY)) {
        Serial.println("[RADIO] Error initializing radio");
        while (true);
    }
    radio.promiscuous(PROMISCUOUS);
    radio.onMessage(radioMessage);
}

void radioLoop() {
    radio.loop();
}

// -----------------------------------------------------------------------------
// Common methods
// -----------------------------------------------------------------------------

void setup() {
    Serial.begin(SERIAL_BAUD);
    Serial.println();
    Serial.println();
    #if USE_FLASH
        flashSetup();
    #endif
    radioSetup();
    Serial.println();
    Serial.println();
}

void loop() {
    radioLoop();
    delay(1);
}
