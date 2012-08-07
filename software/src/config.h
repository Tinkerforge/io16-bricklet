/* io16-bricklet
 * Copyright (C) 2012 Matthias Bolte <matthias@tinkerforge.com>
 * Copyright (C) 2010-2011 Olaf Lüke <olaf@tinkerforge.com>
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

#define BRICKLET_HARDWARE_NAME "IO-16 Bricklet 1.0"
#define BRICKLET_FIRMWARE_VERSION_MAJOR 1
#define BRICKLET_FIRMWARE_VERSION_MINOR 1
#define BRICKLET_FIRMWARE_VERSION_REVISION 3

#define INVOCATION_IN_BRICKLET_CODE

#define PIN_INT_A      (BS->pin1_ad)
#define PIN_INT_B      (BS->pin2_da)
#define PIN_RESET      (BS->pin3_pwm)

#define NUM_PINS_PER_PORT 8

typedef struct {
	uint32_t debounce_period;
	uint32_t port_a_counter;
	uint32_t port_b_counter;
	uint8_t port_a_last_intf;
	uint8_t port_b_last_intf;
	uint8_t port_a_last_gpio;
	uint8_t port_b_last_gpio;
	uint32_t port_a_time[NUM_PINS_PER_PORT];
	uint32_t port_b_time[NUM_PINS_PER_PORT];
	uint32_t port_a_time_remaining[NUM_PINS_PER_PORT];
	uint32_t port_b_time_remaining[NUM_PINS_PER_PORT];
	uint8_t port_a_monoflop_callback_mask;
	uint8_t port_b_monoflop_callback_mask;
} BrickContext;

#endif
