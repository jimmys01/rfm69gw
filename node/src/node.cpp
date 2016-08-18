/*

ESP69GW

ESP8266 to RFM69 Gateway
Monteino node sample code
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
#include <WirelessHEX69.h>

// -----------------------------------------------------------------------------
// Configutation
// -----------------------------------------------------------------------------

#define NODEID              10
#define GATEWAYID           1
#define NETWORKID           164
#define FREQUENCY           RF69_868MHZ
#define ENCRYPTKEY          "fibonacci0123456"
#define IS_RFM69HW          0
//#define ENABLE_ATC

#define SERIAL_BAUD         115200
#define LED_PIN             9       // Moteinos have LEDs on D9
#define FLASH_SS            8       // and FLASH SS on D8
#define FLASH_ID            0xEF30  //EF30 for 4mbit  Windbond chip (W25X40CL)

// -----------------------------------------------------------------------------
// Globals
// -----------------------------------------------------------------------------

RFM69Manager radio;
SPIFlash flash(FLASH_SS, FLASH_ID);

unsigned long transmitInterval = 2000;

// -----------------------------------------------------------------------------
// Utils
// -----------------------------------------------------------------------------

void blink(unsigned int time) {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
    delay(time);
    digitalWrite(LED_PIN, LOW);
}

void radioSend() {

    static unsigned long lastPeriod = 0;
	unsigned long currPeriod = millis() / transmitInterval;
	if (currPeriod != lastPeriod) {
	    lastPeriod = currPeriod;
        radio.send((char *) "BAT", (char *) "2310", (uint8_t) 2);
        blink(50);
	}

}

// -----------------------------------------------------------------------------
// RFM69
// -----------------------------------------------------------------------------

void radioSetup() {
    delay(10);
    radio.initialize(FREQUENCY, NODEID, NETWORKID, GATEWAYID, ENCRYPTKEY, IS_RFM69HW);
}

void radioLoop() {

    // wireless programming token check
    // DO NOT REMOVE, or this node will not be wirelessly programmable any more!
    //CheckForWirelessHEX(radio, flash, true);

    radio.loop();

    radioSend();

}

// -----------------------------------------------------------------------------
// Common methods
// -----------------------------------------------------------------------------

void setup() {
    Serial.begin(SERIAL_BAUD);
    flash.initialize();
    radioSetup();
}

void loop() {
    radioLoop();
    delay(1);
}
