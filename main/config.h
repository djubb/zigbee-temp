#pragma once

#include <stdint.h>
#include "driver/gpio.h"

// 1-Wire data bus for the DS18B20 sensors (GND/VCC go straight to the v1738
// terminal board, which already carries the 4.7k pull-up resistor on DATA).
//
// GPIO2 was picked because on the ESP32-C6 SuperMini it is free of any other
// role: not a strapping pin (4/5/8/9/15), not the native USB D-/D+ pair
// (12/13 - and unlike the battery-powered co2 sensor, this board is always
// USB-powered so those two stay busy), not the onboard WS2812 LED (8) and
// not the antenna-switch pins (3/14, see configure_internal_antenna() in
// main.c). Double-check this against the actual pin silkscreen on your board
// before soldering the header pins directly - change the define if needed.
#define ONE_WIRE_GPIO GPIO_NUM_2

#define MAX_DS18B20 3
// Zigbee endpoint assigned to each DS18B20, in order of ascending 1-Wire ROM
// address (stable across reboots as long as the same 3 sensors stay wired).
#define DS18B20_ENDPOINTS { 10, 11, 12 }

// How often each sensor's temperature is read and reported over Zigbee.
#define REPORT_INTERVAL_MS (60 * 1000)

// Sanity bounds for reported values - DS18B20's full rated range.
#define TEMP_MIN -40
#define TEMP_MAX 85
