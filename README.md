# RFM69GW

This is a RFM69 gateway for an ESP8266-based custom board.

The project is currently in beta-stage with a working hardware prototype and full featured code. Both the code, libraries and eagle schematics and board layout are free and open source and can be found in this repository.

## Hardware

![Sonoff board - front view](/images/20160819_002338.jpg)

The custom PCB has the minimum required components:

* ESP12 footprint
* RFM69CW module footprint
* Two push buttons to RESET and FLASH the board
* A 2.1 5mm jack for powering with AMS1117-3V3 to power the modules
* Filtering and pushup resistors and capacitors
* A 5 pin header to program it
* An edge footprint for an SMA or RP-SMA antenna

The ESP12 and RFM69CW modules are wired according to the following schema:

| ESP12  | RFM69CW |
|--------|---------|
| GPIO4  | DIO0    |
| GPIO12 | MISO    |
| GPIO13 | MOSI    |
| GPIO14 | SCK     |
| GPIO15 | NSS     |

You can also use your own hardware, SPI pins will probably be the same, only remember to change the pin numbers for SPI_CS and interruption in the RFM69Manager.h file.

```
#define SPI_CS              SS
#define IRQ_PIN             5
#define IRQ_NUM             5
```

Note: the was an error in the ESP12 footprint for version 0.1 of the board and GPIO 4 and 5 where swapped, so even thou the schematic showed IRQ pin to be GPIO4 it was 5 instead.

## Firmware

* **WebServer for configuration** using the great [PureCSS framework][1]
* **Flashing firmware Over-The-Air** (OTA)
* Up to **3 configurable WIFI networks**
* **MQTT support**
* Arbitrary number of **mappings between nodes/keys and MQTT topics** (as long as there is free memory available)
* Optional default topic for non mapped nodes/keys.
* Configurable IP and Heartbeat topics
* Visual status of the received messages via the ESP12 LED

The project uses a series of state-of-the-art libraries:

* [RFM69_ATC][5] by Felix Rusu and Thomas Studwell
* [SPIFlash][6] by Felix Rusu
* [PubSubClient][3] by Nick O'Leary
* [ArduinoJson][4] by Benoit Blanchon
* [Embedis][7] by thingsSoC
* [PureCSS][1] by Yahoo

## Flashing

The board has a header with 3V3, GND, RX and TX pins, and two buttons for RESET and FLASH. Connect the pins to your favorite FTDI-like programmer. Remember: **only 3V3 voltage** and connect RX to your FTDI TX pin and TX to your RX pin.

To enter flash mode you have to hold the FLASH button pressed while reseting the board with the RESET button.

The project is ready to be build using [PlatformIO][2].
Please refer to their web page for instructions on how to install the builder. Once installed:

```bash
> platformio run --target upload -e wire-debug
> platformio run --target uploadfs -e wire-debug
```

Once you have flashed it you can flash it again over-the-air using the ```ota``` environment:

```bash
> platformio run --target upload -e ota-debug
> platformio run --target uploadfs -e ota-debug
```

When using OTA environment it defaults to the IP address of the device in SoftAP mode. If you want to flash it when connected to your home network best way is to supply the IP of the device:

```bash
> platformio run --target upload -e ota-debug--upload-port 192.168.1.151
> platformio run --target uploadfs -e ota-debug --upload-port 192.168.1.151
```

Library dependencies not included in the project are automatically managed via PlatformIO Library Manager.

## Usage

On normal boot (i.e. button not pressed) it will execute the firmware. It configures the radio, the SPIFFS memory access, the WIFI, the WebServer and MQTT connection.

Obviously the default values for WIFI network and MQTT will probably not match your requirements. The device will start in Soft AP creating a WIFI SSID named "RFM69_GATEWAY". Connect with phone, PC, laptop, whatever to that network, password is "fibonacci". Once connected browse to 192.168.4.1 and you will be presented a configuration page where you will be able to define up to 3 possible WIFI networks and the MQTT configuration parameters, and topic mappings.

It will then try to connect to the configured WIFI networks one after the other. If none of the 3 attempts succeed it will default to SoftAP mode again. Once connected it will try to connect the MQTT server.

You can configure several aspects, most importantly:

* up to 3 **wifi SSID and password**
* **MQTT server, port** and optionally **user and password**
* **MQTT topics for IP and heartbeat messages**
* **nodeID**, **key** and **topic** to map a given key from a given nodeID to a certain MQTT topic
* a **defaultTopic**, if defined, will be used as a default topic for any combination of nodeID+key not mapped. You can use {nodeid} and {key} as placeholders that will be replaced in execution time.

Messages from the nodes (probably Monteinos) do have to stick to one of these formats:

```
<key>:<value>
<key>:<value>:<packetID>
```

The first one being the default for Rusu's MotionNotes, for instance. "BAT:3812" could be a valid payload to notify battery status. The ```send``` method of the RFM69Manager library included will take care of the formatting. It will also add a correlative packetID that will be used in the gateway to detect duplicates or missing packages.

The RFM69Manager library inherits from RFM69_ATC, so it supports [Automatic Transmission Control][8] that improves battery life by reducing radio power dynamically.

The device will publish its IP to the given topic upon reboot and will send a heartbeat message every 60 seconds.

## Troubleshooting

After flashing the firmware via serial do a hard reset of the device (unplug & plug). There is an issue with the ESP.reset() method that may stuck the device after an OTA update. Check [https://github.com/esp8266/Arduino/issues/1017][3] for more info.

[1]: http://purecss.io/
[2]: http://www.platformio.org
[3]: https://github.com/knolleary/pubsubclient
[4]: https://github.com/bblanchon/ArduinoJson
[5]: https://github.com/LowPowerLab/RFM69
[6]: https://github.com/LowPowerLab/SPIFlash
[7]: https://github.com/thingSoC/embedis
[8]: https://lowpowerlab.com/blog/2015/11/11/rfm69_atc-automatic-transmission-control/
