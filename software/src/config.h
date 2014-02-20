/* io16-bricklet
 * Copyright (C) 2012-2014 Matthias Bolte <matthias@tinkerforge.com>
 * Copyright (C) 2010-2012 Olaf Lüke <olaf@tinkerforge.com>
 *
 * config.h: IO-16 Bricklet specific configuration
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib/drivers/board/sam3s/SAM3S.h"

#include "io.h"

#define BRICKLET_FIRMWARE_VERSION_MAJOR 2
#define BRICKLET_FIRMWARE_VERSION_MINOR 0
#define BRICKLET_FIRMWARE_VERSION_REVISION 5

#define BRICKLET_HARDWARE_VERSION_MAJOR 1
#define BRICKLET_HARDWARE_VERSION_MINOR 1
#define BRICKLET_HARDWARE_VERSION_REVISION 0

#define BRICKLET_DEVICE_IDENTIFIER 28

#define INVOCATION_IN_BRICKLET_CODE

#define PIN_INT_A      (BS->pin1_ad)
#define PIN_INT_B      (BS->pin2_da)
#define PIN_RESET      (BS->pin3_pwm)

#define NUM_PINS_PER_PORT 8
#define NUM_EDGE_COUNT 2

#define NUM_PORTS 2
#define PORT_A 0
#define PORT_B 1

typedef struct {
	uint8_t current_gpinten[NUM_PORTS];
	uint8_t current_gpio[NUM_PORTS];
	uint8_t current_iodir[NUM_PORTS];
	uint8_t current_gppu[NUM_PORTS];
	uint8_t current_olat[NUM_PORTS];
	uint8_t monoflop_callback_mask[NUM_PORTS];

	uint8_t edge_type[NUM_EDGE_COUNT];
	uint8_t edge_debounce[NUM_EDGE_COUNT];
	uint8_t edge_debounce_counter[NUM_EDGE_COUNT];
	uint8_t edge_last_state[NUM_EDGE_COUNT];
	uint32_t edge_count[NUM_EDGE_COUNT];

	uint32_t debounce_period;
	uint32_t counter[NUM_PORTS];

	uint32_t time[NUM_PINS_PER_PORT][NUM_PORTS];
	uint32_t time_remaining[NUM_PINS_PER_PORT][NUM_PORTS];

	bool interrupt_callback[NUM_PORTS];
	bool interrupt_edge;
} BrickContext;

#endif
