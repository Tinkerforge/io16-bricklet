/* io16-bricklet
 * Copyright (C) 2010-2011 Olaf Lüke <olaf@tinkerforge.com>
 *
 * io.c: Implementation of IO-16 Bricklet messages
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

#include "io.h"

#include "brickletlib/bricklet_entry.h"
#include "bricklib/utility/mutex.h"
#include "bricklib/utility/init.h"
#include "bricklib/bricklet/bricklet_communication.h"
#include "config.h"
#include <pio/pio_it.h>
#include <pio/pio.h>

const ComMessage com_messages[] = {
	{TYPE_SET_PORT, (message_handler_func_t)set_port},
	{TYPE_GET_PORT, (message_handler_func_t)get_port},
	{TYPE_SET_PORT_CONFIGURATION, (message_handler_func_t)set_port_configuration},
	{TYPE_GET_PORT_CONFIGURATION, (message_handler_func_t)get_port_configuration},
	{TYPE_SET_DEBOUNCE_PERIOD, (message_handler_func_t)set_debounce_period},
	{TYPE_GET_DEBOUNCE_PERIOD, (message_handler_func_t)get_debounce_period},
	{TYPE_SET_PORT_INTERRUPT, (message_handler_func_t)set_port_interrupt},
	{TYPE_GET_PORT_INTERRUPT, (message_handler_func_t)get_port_interrupt},
};


void invocation(uint8_t com, uint8_t *data) {
	uint8_t id = ((StandardMessage*)data)->type - 1;
	if(id < NUM_MESSAGES) {
		BRICKLET_OFFSET(com_messages[id].reply_func)(com, data);
	}
}

void constructor(void) {
	// Disable Reset
    PIN_RESET.type = PIO_OUTPUT_1;
    BA->PIO_Configure(&PIN_RESET, 1);

	PIN_INT_A.type = PIO_INPUT;
	PIN_INT_A.attribute = PIO_PULLUP;
	BA->PIO_Configure(&PIN_INT_A, 1);


	PIN_INT_B.type = PIO_INPUT;
	PIN_INT_B.attribute = PIO_PULLUP;
	BA->PIO_Configure(&PIN_INT_B, 1);

	BC->port_a_counter = 0;
	BC->port_b_counter = 0;
	BC->debounce_period = 100;

	io_write(I2C_INTERNAL_ADDRESS_IOCON_A, IOCON_ODR);
	io_write(I2C_INTERNAL_ADDRESS_IOCON_B, IOCON_ODR);

	// Default is input pull up
	io_write(I2C_INTERNAL_ADDRESS_GPPU_A, 0xFF);
	io_write(I2C_INTERNAL_ADDRESS_GPPU_B, 0xFF);
	io_write(I2C_INTERNAL_ADDRESS_IODIR_A, 0xFF);
	io_write(I2C_INTERNAL_ADDRESS_IODIR_B, 0xFF);
}

void destructor(void) {
	PIN_RESET.type = PIO_INPUT;
	PIN_RESET.attribute = PIO_PULLUP;
	BA->PIO_Configure(&PIN_RESET, 1);
}

void tick(uint8_t tick_type) {
	if(tick_type & TICK_TASK_TYPE_CALCULATION) {
		if(BC->port_a_counter != 0) {
			BC->port_a_counter--;
		}
		if(BC->port_b_counter != 0) {
			BC->port_b_counter--;
		}
	} else if(tick_type & TICK_TASK_TYPE_MESSAGE) {
		if(BC->port_a_counter == 0) {
			if((PIN_INT_A.pio->PIO_PDSR & PIN_INT_A.mask) == 0) {
				InterruptSignal is = {
					BS->stack_id,
					TYPE_INTERRUPT,
					sizeof(InterruptSignal),
					'a',
					io_read(I2C_INTERNAL_ADDRESS_INTF_A),
					io_read(I2C_INTERNAL_ADDRESS_GPIO_A)
				};

				BA->send_blocking_with_timeout(&is,
											   sizeof(InterruptSignal),
											   *BA->com_current);
				BC->port_a_counter = BC->debounce_period;
			}
		}

		if(BC->port_b_counter == 0) {
			if((PIN_INT_B.pio->PIO_PDSR & PIN_INT_B.mask) == 0) {
				InterruptSignal is = {
					BS->stack_id,
					TYPE_INTERRUPT,
					sizeof(InterruptSignal),
					'b',
					io_read(I2C_INTERNAL_ADDRESS_INTF_B),
					io_read(I2C_INTERNAL_ADDRESS_GPIO_B)
				};

				BA->send_blocking_with_timeout(&is,
											   sizeof(InterruptSignal),
											   *BA->com_current);
				BC->port_b_counter = BC->debounce_period;
			}
		}
	}
}

void io_write(const uint8_t internal_address, uint8_t value) {
	uint8_t address;
	if(BS->address == I2C_EEPROM_ADDRESS_HIGH) {
		address = I2C_ADDRESS_HIGH;
	} else {
		address = I2C_ADDRESS_LOW;
	}

	const uint8_t port = BS->port - 'a';

	BA->mutex_take(*BA->mutex_twi_bricklet, MUTEX_BLOCKING);
	BA->bricklet_select(port);
    BA->TWID_Write(BA->twid,
                   address,
                   internal_address,
                   I2C_INTERNAL_ADDRESS_BYTES,
                   &value,
                   I2C_DATA_LENGTH,
                   NULL);
    BA->bricklet_deselect(port);

    BA->mutex_give(*BA->mutex_twi_bricklet);
}

uint8_t io_read(const uint8_t internal_address) {
	uint8_t address;
	if(BS->address == I2C_EEPROM_ADDRESS_HIGH) {
		address = I2C_ADDRESS_HIGH;
	} else {
		address = I2C_ADDRESS_LOW;
	}

	const uint8_t port = BS->port - 'a';
	uint8_t value;

	BA->mutex_take(*BA->mutex_twi_bricklet, MUTEX_BLOCKING);
	BA->bricklet_select(port);
    BA->TWID_Read(BA->twid,
                  address,
                  internal_address,
                  I2C_INTERNAL_ADDRESS_BYTES,
                  &value,
                  I2C_DATA_LENGTH,
                  NULL);
	BA->bricklet_deselect(port);

    BA->mutex_give(*BA->mutex_twi_bricklet);

	return value;
}

void get_port(uint8_t com, const GetPort *data) {
	uint8_t internal_address;
	if(data->port == 'a' || data->port == 'A') {
		internal_address = I2C_INTERNAL_ADDRESS_GPIO_A;
	} else if(data->port == 'b' || data->port == 'B') {
		internal_address = I2C_INTERNAL_ADDRESS_GPIO_B;
	} else {
		// TODO: Error?
		return;
	}

	GetPortReturn gpr;

	gpr.stack_id      = data->stack_id;
	gpr.type          = data->type;
	gpr.length        = sizeof(GetPortReturn);
	gpr.port_mask     = io_read(internal_address);

	BA->send_blocking_with_timeout(&gpr, sizeof(GetPortReturn), com);
}

void set_port(uint8_t com, const SetPort *data) {
	// set_port sets output latches, values are measured on port if port
	// is configured as output

	uint8_t internal_address;
	if(data->port == 'a' || data->port == 'A') {
		internal_address = I2C_INTERNAL_ADDRESS_OLAT_A;
	} else if(data->port == 'b' || data->port == 'B') {
		internal_address = I2C_INTERNAL_ADDRESS_OLAT_B;
	} else {
		// TODO: Error?
		return;
	}

	io_write(internal_address, data->port_mask);
}

void set_port_configuration(uint8_t com, const SetPortConfiguration *data) {
	uint8_t internal_address_iodir;
	uint8_t internal_address_gppu;
	uint8_t internal_address_olat;

	if(data->port == 'a' || data->port == 'A') {
		internal_address_iodir = I2C_INTERNAL_ADDRESS_IODIR_A;
		internal_address_gppu  = I2C_INTERNAL_ADDRESS_GPPU_A;
		internal_address_olat  = I2C_INTERNAL_ADDRESS_OLAT_A;
	} else if(data->port == 'b' || data->port == 'B') {
		internal_address_iodir = I2C_INTERNAL_ADDRESS_IODIR_B;
		internal_address_gppu  = I2C_INTERNAL_ADDRESS_GPPU_B;
		internal_address_olat  = I2C_INTERNAL_ADDRESS_OLAT_B;
	} else {
		// TODO: Error?
		return;
	}

	uint8_t iodir;


	if(data->direction == 'i' || data->direction == 'I') {
		iodir = io_read(internal_address_iodir);
		iodir |= data->port_mask;
		io_write(internal_address_iodir, iodir);

		uint8_t gppu = io_read(internal_address_gppu);
	    if(data->value) {
			gppu |= data->port_mask;
	    } else {
	    	gppu &= ~(data->port_mask);
	    }
	    io_write(internal_address_gppu, gppu);
	} else if(data->direction == 'o' || data->direction == 'O') {
		uint8_t gpio = io_read(internal_address_olat);
	    if(data->value) {
	    	gpio |= data->port_mask;
	    } else {
	    	gpio &= ~(data->port_mask);
	    }
	    io_write(internal_address_olat, gpio);

	    iodir = io_read(internal_address_iodir);
		iodir &= ~(data->port_mask);
		io_write(internal_address_iodir, iodir);
	} else {
		// TODO: Error?
	}
}

void get_port_configuration(uint8_t com, const GetPortConfiguration *data) {
	uint8_t internal_address_iodir;
	uint8_t internal_address_gppu;
	uint8_t internal_address_olat;

	if(data->port == 'a' || data->port == 'A') {
		internal_address_iodir = I2C_INTERNAL_ADDRESS_IODIR_A;
		internal_address_gppu  = I2C_INTERNAL_ADDRESS_GPPU_A;
		internal_address_olat  = I2C_INTERNAL_ADDRESS_OLAT_A;
	} else if(data->port == 'b' || data->port == 'B') {
		internal_address_iodir = I2C_INTERNAL_ADDRESS_IODIR_B;
		internal_address_gppu  = I2C_INTERNAL_ADDRESS_GPPU_B;
		internal_address_olat  = I2C_INTERNAL_ADDRESS_OLAT_B;
	} else {
		// TODO: Error?
		return;
	}

	GetPortConfigurationReturn gpcr;

	gpcr.stack_id       = data->stack_id;
	gpcr.type           = data->type;
	gpcr.length         = sizeof(GetPortConfigurationReturn);
	gpcr.direction_mask = io_read(internal_address_iodir);
	uint8_t latch_mask  = io_read(internal_address_olat);
	uint8_t pullup_mask = io_read(internal_address_gppu);

	gpcr.value_mask = (  gpcr.direction_mask  & pullup_mask) |
	                  ((~gpcr.direction_mask) & latch_mask);

	BA->send_blocking_with_timeout(&gpcr, sizeof(GetPortConfigurationReturn), com);
}

void set_debounce_period(uint8_t com, const SetDebouncePeriod *data) {
	BC->debounce_period = data->debounce;
}

void get_debounce_period(uint8_t com, const GetDebouncePeriod *data) {
	GetDebouncePeriodReturn gdpr;

	gdpr.stack_id       = data->stack_id;
	gdpr.type           = data->type;
	gdpr.length         = sizeof(GetDebouncePeriodReturn);
	gdpr.debounce       = BC->debounce_period;

	BA->send_blocking_with_timeout(&gdpr, sizeof(GetDebouncePeriodReturn), com);
}

void set_port_interrupt(uint8_t com, const SetPortInterrupt *data) {
	uint8_t internal_address_gpinten;

	if(data->port == 'a' || data->port == 'A') {
		internal_address_gpinten = I2C_INTERNAL_ADDRESS_GPINTEN_A;
	} else if(data->port == 'b' || data->port == 'B') {
		internal_address_gpinten = I2C_INTERNAL_ADDRESS_GPINTEN_B;
	} else {
		// TODO: Error?
		return;
	}

	io_write(internal_address_gpinten, data->interrupt_mask);
}

void get_port_interrupt(uint8_t com, const GetPortInterrupt *data) {
	uint8_t internal_address_gpinten;

	if(data->port == 'a' || data->port == 'A') {
		internal_address_gpinten = I2C_INTERNAL_ADDRESS_GPINTEN_A;
	} else if(data->port == 'b' || data->port == 'B') {
		internal_address_gpinten = I2C_INTERNAL_ADDRESS_GPINTEN_B;
	} else {
		// TODO: Error?
		return;
	}

	GetPortInterruptReturn gpir;

	gpir.stack_id       = data->stack_id;
	gpir.type           = data->type;
	gpir.length         = sizeof(GetPortInterruptReturn);
	gpir.interrupt_mask = io_read(internal_address_gpinten);

	BA->send_blocking_with_timeout(&gpir, sizeof(GetPortInterruptReturn), com);
}
