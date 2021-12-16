# MQTTDevice4

MQTTDevice4 enables sensors, actors and an induction hob to be connected via WLAN to [CraftBeerPi V4](https://github.com/Manuel83/craftbeerpi4).
According to the current development status in dec 2021, this fork [CraftBeerPi V4](https://github.com/avollkopf/craftbeerpi4) is highly recommended!

MQTTDevice4 is an Arduino sketch for the ESP8266 Wemos D1 mini modules. This makes it possible to establish communication between the MQTT broker mosquitto and an ESP8266 in order to control sensors and actors with CraftBeerPi V4. MQTTDevice is optimzed for version 4 of craftbeerpi.

## MQTTdevice2 or MQTTDevice4?

There are only a few differences between MQTTDevice 2 and 4. MQTTDevice2 with firmware version 2.5x and earlier were developed for CBPi3. MQTTDevice4 and firmware version 2.6x and above is optimzed for CBPi4. There will be no further development in MQTTDevice2 repository (EOL). If you are using CBPi3 and Influx-Grafana functions from MQTTDevice you have to stay with MQTTDevice2 firmware version 2.5x. All other users should update to MQTTDevice4 firmware version 2.6 or newer.

## Update notice Firmware version 2.6

When updating to MQTTDevice4 from MQTTDevice2 you must use file upload: MQTTDevice4.ino.bin

## Documentation

A detailed documentation is available on github pages: <https://innuendopi.github.io/MQTTDevice4/>
(not yet ready)

## Support

There is support in the hobby brewer forum <https://hobbybrauer.de/forum/> and in facebook CraftbeerPi User Group

## Sketch Information

Libraries: Version 2.60, 12.2021

- ESP8266 3.0.2 (LittleFS)
- Arduino IDE 1.8.15
- Visual Code 1.52.1 + modified ESP8266FS Plugin (VSCode 1.52.1 Aruindo 1.8.16)
- PubSubClient 2.8.0
- ArduinoJSON 6.18.5
- InfluxDB 3.9.0
- WiFiManager 2.0.5

Board configuration:
Flash size 4MB (FS:2MB OTA:~1019kB)
SSL support all SSL ciphers (most comp)
Exceptions Disabled
IwIP variant v2 lower mem

Debug output:
For debug output, the debug port must be set to serial. Set the debug level accordingly for special debug outputs (default none).
