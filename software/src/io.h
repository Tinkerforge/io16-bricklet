/* io16-bricklet
 * Copyright (C) 2012 Matthias Bolte <matthias@tinkerforge.com>
 * Copyright (C) 2010-2011 Olaf Lüke <olaf@tinkerforge.com>
 *
 * io.h: Implementation of IO-16 Bricklet messages
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

#ifndef IO_H
#define IO_H

#include <stdint.h>


#define I2C_EEPROM_ADDRESS_HIGH 84
#define I2C_ADDRESS_HIGH 33 //0b0100001
#define I2C_ADDRESS_LOW 32 //0b0100001
#define I2C_INTERNAL_ADDRESS_BYTES 1
#define I2C_DATA_LENGTH 1

#define I2C_INTERNAL_ADDRESS_IODIR_A   0x00
#define I2C_INTERNAL_ADDRESS_IODIR_B   0x01
#define I2C_INTERNAL_ADDRESS_GPINTEN_A 0x04
#define I2C_INTERNAL_ADDRESS_GPINTEN_B 0x05
#define I2C_INTERNAL_ADDRESS_DEFVAL_A  0x06
#define I2C_INTERNAL_ADDRESS_DEFVAL_B  0x07
#define I2C_INTERNAL_ADDRESS_INTCON_A  0x08
#define I2C_INTERNAL_ADDRESS_INTCON_B  0x09
#define I2C_INTERNAL_ADDRESS_IOCON_A   0x0A
#define I2C_INTERNAL_ADDRESS_IOCON_B   0x0B
#define I2C_INTERNAL_ADDRESS_GPPU_A    0x0C
#define I2C_INTERNAL_ADDRESS_GPPU_B    0x0D
#define I2C_INTERNAL_ADDRESS_INTF_A    0x0E
#define I2C_INTERNAL_ADDRESS_INTF_B    0x0F
#define I2C_INTERNAL_ADDRESS_INTCAP_A  0x10
#define I2C_INTERNAL_ADDRESS_INTCAP_B  0x11
#define I2C_INTERNAL_ADDRESS_GPIO_A    0x12
#define I2C_INTERNAL_ADDRESS_GPIO_B    0x13
#define I2C_INTERNAL_ADDRESS_OLAT_A    0x14
#define I2C_INTERNAL_ADDRESS_OLAT_B    0x15

#define IOCON_ODR                      (1 << 2)

#define TYPE_SET_PORT 1
#define TYPE_GET_PORT 2
#define TYPE_SET_PORT_CONFIGURATION 3
#define TYPE_GET_PORT_CONFIGURATION 4
#define TYPE_SET_DEBOUNCE_PERIOD 5
#define TYPE_GET_DEBOUNCE_PERIOD 6
#define TYPE_SET_PORT_INTERRUPT 7
#define TYPE_GET_PORT_INTERRUPT 8
#define TYPE_INTERRUPT 9
#define TYPE_SET_PORT_MONOFLOP 10
#define TYPE_GET_PORT_MONOFLOP 11
#define TYPE_MONOFLOP_DONE 12

#define NUM_MESSAGES 11

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	char port;
} __attribute__((__packed__)) GetPort;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	uint8_t value_mask;
} __attribute__((__packed__)) GetPortReturn;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	char port;
	uint8_t value_mask;
} __attribute__((__packed__)) SetPort;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	char port;
	uint8_t pin_mask;
	char direction;
	bool value;
} __attribute__((__packed__)) SetPortConfiguration;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	char port;
} __attribute__((__packed__)) GetPortConfiguration;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	uint8_t direction_mask;
	uint8_t value_mask;
} __attribute__((__packed__)) GetPortConfigurationReturn;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	uint32_t debounce;
} __attribute__((__packed__)) SetDebouncePeriod;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
} __attribute__((__packed__)) GetDebouncePeriod;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	uint32_t debounce;
} __attribute__((__packed__)) GetDebouncePeriodReturn;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	char port;
	uint8_t interrupt_mask;
} __attribute__((__packed__)) SetPortInterrupt;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	char port;
} __attribute__((__packed__)) GetPortInterrupt;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	uint8_t interrupt_mask;
} __attribute__((__packed__)) GetPortInterruptReturn;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	char port;
	uint8_t interrupt_mask;
	uint8_t value_mask;
} __attribute__((__packed__)) InterruptSignal;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	char port;
	uint8_t pin_mask;
	uint8_t value_mask;
	uint32_t time;
} __attribute__((__packed__)) SetPortMonoflop;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	char port;
	uint8_t pin;
} __attribute__((__packed__)) GetPortMonoflop;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	uint8_t value;
	uint32_t time;
	uint32_t time_remaining;
} __attribute__((__packed__)) GetPortMonoflopReturn;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	char port;
	uint8_t pin_mask;
	uint8_t value_mask;
} __attribute__((__packed__)) MonoflopDone;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
} __attribute__((__packed__)) StandardMessage;

void get_port(uint8_t com, const GetPort *data);
void set_port(uint8_t com, const SetPort *data);
void set_port_configuration(uint8_t com, const SetPortConfiguration *data);
void get_port_configuration(uint8_t com, const GetPortConfiguration *data);
void set_debounce_period(uint8_t com, const SetDebouncePeriod *data);
void get_debounce_period(uint8_t com, const GetDebouncePeriod *data);
void set_port_interrupt(uint8_t com, const SetPortInterrupt *data);
void get_port_interrupt(uint8_t com, const GetPortInterrupt *data);
void set_port_monoflop(uint8_t com, SetPortMonoflop *data);
void get_port_monoflop(uint8_t com, GetPortMonoflop *data);

void invocation(uint8_t com, uint8_t *data);
void constructor(void);
void destructor(void);
void tick(uint8_t tick_type);

void io_write(const uint8_t internal_address, uint8_t value);
uint8_t io_read(const uint8_t internal_address);

#endif
