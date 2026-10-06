#pragma once

#include <stdint.h>
#include "driver/gpio.h"

// 1-Wire data bus for the DS18B20 sensors (GND/VCC go straight to the v1738
// terminal board, which already carries the 4.7k pull-up resistor on DATA).
//
// GPIO20 sits right next to GND/3V3 on the SuperMini header, so all three
// wires can be soldered in directly with no jumper wires needed. It's not a
// strapping pin, not the onboard WS2812 LED (8) and not the antenna-switch
// pins (3/14, see configure_internal_antenna() in main.c). It is nominally
// "UART0 RX", but that role is unused here - the console runs over the
// native USB-Serial/JTAG peripheral instead, so there's no conflict even
// though this board (unlike the battery-powered co2 sensor) stays on USB
// power all the time.
#define ONE_WIRE_GPIO GPIO_NUM_20

// Headroom for sensors added later without touching the Zigbee device's
// cluster list again (which would otherwise need idf.py erase-flash + a
// ZHA re-pair). Endpoints beyond however many DS18B20s are actually wired
// just sit at their default value and never report.
#define MAX_DS18B20 10
// Zigbee endpoint assigned to each DS18B20, in order of ascending 1-Wire ROM
// address. Stable across reboots only while the set of wired sensors stays
// the same - adding/removing one reshuffles this ordering for everyone
// whose address sorts after the change, since the bus is rescanned from
// scratch on every boot.
#define DS18B20_ENDPOINTS { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 }

// How often each sensor's temperature is read and reported over Zigbee.
#define REPORT_INTERVAL_MS (20 * 1000)

// Sanity bounds for reported values - DS18B20's full rated range.
#define TEMP_MIN -40
#define TEMP_MAX 85
