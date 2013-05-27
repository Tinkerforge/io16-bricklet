/* io16-bricklet
 * Copyright (C) 2012-2013 Matthias Bolte <matthias@tinkerforge.com>
 * Copyright (C) 2010-2013 Olaf Lüke <olaf@tinkerforge.com>
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
#include "bricklib/drivers/pio/pio.h"
#include "config.h"

void invocation(const ComType com, const uint8_t *data) {
	switch(((MessageHeader*)data)->fid) {
		case FID_SET_PORT: {
			set_port(com, (SetPort*)data);
			break;
		}

		case FID_GET_PORT: {
			get_port(com, (GetPort*)data);
			break;
		}

		case FID_SET_PORT_CONFIGURATION: {
			set_port_configuration(com, (SetPortConfiguration*)data);
			break;
		}

		case FID_GET_PORT_CONFIGURATION: {
			get_port_configuration(com, (GetPortConfiguration*)data);
			break;
		}

		case FID_SET_DEBOUNCE_PERIOD: {
			set_debounce_period(com, (SetDebouncePeriod*)data);
			break;
		}

		case FID_GET_DEBOUNCE_PERIOD: {
			get_debounce_period(com, (GetDebouncePeriod*)data);
			break;
		}

		case FID_SET_PORT_INTERRUPT: {
			set_port_interrupt(com, (SetPortInterrupt*)data);
			break;
		}

		case FID_GET_PORT_INTERRUPT: {
			get_port_interrupt(com, (GetPortInterrupt*)data);
			break;
		}

		case FID_SET_PORT_MONOFLOP: {
			set_port_monoflop(com, (SetPortMonoflop*)data);
			break;
		}

		case FID_GET_PORT_MONOFLOP: {
			get_port_monoflop(com, (GetPortMonoflop*)data);
			break;
		}

		case FID_SET_SELECTED_VALUES: {
			set_selected_values(com, (SetSelectedValues*)data);
			break;
		}

		default: {
			BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_NOT_SUPPORTED, com);
			break;
		}
	}
}

void constructor(void) {
	_Static_assert(sizeof(BrickContext) <= BRICKLET_CONTEXT_MAX_SIZE, "BrickContext too big");

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

	for(uint8_t i = 0; i < NUM_PINS_PER_PORT; i++) {
		BC->port_a_time[i] = 0;
		BC->port_a_time_remaining[i] = 0;
		BC->port_b_time[i] = 0;
		BC->port_b_time_remaining[i] = 0;
	}

	BC->port_a_monoflop_callback_mask = 0;
	BC->port_b_monoflop_callback_mask = 0;
}

void destructor(void) {
	PIN_RESET.type = PIO_INPUT;
	PIN_RESET.attribute = PIO_PULLUP;
	BA->PIO_Configure(&PIN_RESET, 1);
}

void send_interrupt_callback(const char port,
                             const uint8_t internal_address_inft,
                             const uint8_t internal_address_gpio,
                             uint8_t *last_intf,
                             uint8_t *last_gpio,
                             uint32_t *port_counter) {
	uint8_t new_intf = io_read(internal_address_inft);
	uint8_t new_gpio = io_read(internal_address_gpio);
	if(new_intf != *last_intf ||
	   (new_intf & new_gpio) != (*last_intf & *last_gpio)) {
		InterruptSignal is;
		BA->com_make_default_header(&is, BS->uid, sizeof(InterruptSignal), FID_INTERRUPT);

		is.port           = port;
		is.interrupt_mask = new_intf;
		is.value_mask     = new_gpio;

		*last_intf = new_intf;
		*last_gpio = new_gpio;

		BA->send_blocking_with_timeout(&is,
		                               sizeof(InterruptSignal),
		                               *BA->com_current);
		*port_counter = BC->debounce_period;
	}
}

void update_monoflop_time(const uint8_t internal_address_olat,
                          uint8_t *monoflop_callback_mask,
                          uint32_t **time_remaining) {
	uint8_t monoflop_done_mask = 0;

	for(uint8_t i = 0; i < NUM_PINS_PER_PORT; i++) {
		if((*time_remaining)[i] != 0) {
			(*time_remaining)[i]--;

			if((*time_remaining)[i] == 0) {
				monoflop_done_mask |= (1 << i);
			}
		}
	}

	if (monoflop_done_mask == 0) {
		return;
	}

	uint8_t gpio = io_read(internal_address_olat);

	for(uint8_t i = 0; i < NUM_PINS_PER_PORT; i++) {
		if(monoflop_done_mask & (1 << i)) {
			if(gpio & (1 << i)) {
				gpio &= ~(1 << i);
			} else {
				gpio |= (1 << i);
			}
		}
	}

	io_write(internal_address_olat, gpio);

	*monoflop_callback_mask |= monoflop_done_mask;
}

void send_monoflop_callback(const char port,
                            const uint8_t internal_address_gpio,
                            uint8_t *monoflop_callback_mask) {
	MonoflopDone md;
	BA->com_make_default_header(&md, BS->uid, sizeof(MonoflopDone), FID_MONOFLOP_DONE);

	md.port           = port;
	md.selection_mask = 0;
	md.value_mask     = 0;

	uint8_t gpio = io_read(internal_address_gpio);

	for(uint8_t i = 0; i < NUM_PINS_PER_PORT; i++) {
		if (*monoflop_callback_mask & (1 << i)) {
			md.selection_mask |= (1 << i);

			if(gpio & (1 << i)) {
				md.value_mask |= (1 << i);
			}
		}
	}

	BA->send_blocking_with_timeout(&md,
	                               sizeof(MonoflopDone),
	                               *BA->com_current);

	*monoflop_callback_mask = 0;
}

void tick(const uint8_t tick_type) {
	if(tick_type & TICK_TASK_TYPE_CALCULATION) {
		if(BC->port_a_counter != 0) {
			BC->port_a_counter--;
		}
		if(BC->port_b_counter != 0) {
			BC->port_b_counter--;
		}

		uint32_t *a = BC->port_a_time_remaining;
		uint32_t *b = BC->port_b_time_remaining;

		update_monoflop_time(I2C_INTERNAL_ADDRESS_OLAT_A,
		                     &BC->port_a_monoflop_callback_mask,
		                     &a);
		update_monoflop_time(I2C_INTERNAL_ADDRESS_OLAT_B,
		                     &BC->port_b_monoflop_callback_mask,
		                     &b);
	}

	if(tick_type & TICK_TASK_TYPE_MESSAGE) {
		if(BC->port_a_counter == 0 &&
		   (PIN_INT_A.pio->PIO_PDSR & PIN_INT_A.mask) == 0) {
			send_interrupt_callback('a',
			                        I2C_INTERNAL_ADDRESS_INTF_A,
			                        I2C_INTERNAL_ADDRESS_GPIO_A,
			                        &BC->port_a_last_intf,
			                        &BC->port_a_last_gpio,
			                        &BC->port_a_counter);
		}
		if(BC->port_b_counter == 0 &&
		   (PIN_INT_B.pio->PIO_PDSR & PIN_INT_B.mask) == 0) {
			send_interrupt_callback('b',
			                        I2C_INTERNAL_ADDRESS_INTF_B,
			                        I2C_INTERNAL_ADDRESS_GPIO_B,
			                        &BC->port_b_last_intf,
			                        &BC->port_b_last_gpio,
			                        &BC->port_b_counter);
		}

		if(BC->port_a_monoflop_callback_mask) {
			send_monoflop_callback('a',
			                       I2C_INTERNAL_ADDRESS_GPIO_A,
			                       &BC->port_a_monoflop_callback_mask);
		}
		if(BC->port_b_monoflop_callback_mask) {
			send_monoflop_callback('b',
			                       I2C_INTERNAL_ADDRESS_GPIO_B,
			                       &BC->port_b_monoflop_callback_mask);
		}
	}
}

void io_write(const uint8_t internal_address, const uint8_t value) {
	uint8_t address;
	if(BS->address == I2C_EEPROM_ADDRESS_HIGH) {
		address = I2C_ADDRESS_HIGH;
	} else {
		address = I2C_ADDRESS_LOW;
	}

	const uint8_t port = BS->port - 'a';
	uint8_t write_value = value;

	BA->mutex_take(*BA->mutex_twi_bricklet, MUTEX_BLOCKING);
	BA->bricklet_select(port);
    BA->TWID_Write(BA->twid,
                   address,
                   internal_address,
                   I2C_INTERNAL_ADDRESS_BYTES,
                   &write_value,
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

void get_port(const ComType com, const GetPort *data) {
	uint8_t internal_address;
	if(data->port == 'a' || data->port == 'A') {
		internal_address = I2C_INTERNAL_ADDRESS_GPIO_A;
	} else if(data->port == 'b' || data->port == 'B') {
		internal_address = I2C_INTERNAL_ADDRESS_GPIO_B;
	} else {
		BA->com_return_error(data, sizeof(GetPortReturn), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	GetPortReturn gpr;

	gpr.header        = data->header;
	gpr.header.length = sizeof(GetPortReturn);;
	gpr.value_mask    = io_read(internal_address);

	BA->send_blocking_with_timeout(&gpr, sizeof(GetPortReturn), com);
}

void set_port(const ComType com, const SetPort *data) {
	// set_port sets output latches, values are measured on port if port
	// is configured as output
	uint8_t internal_address;
	uint32_t *time_remaining;

	if(data->port == 'a' || data->port == 'A') {
		internal_address = I2C_INTERNAL_ADDRESS_OLAT_A;
		time_remaining = BC->port_a_time_remaining;
	} else if(data->port == 'b' || data->port == 'B') {
		internal_address = I2C_INTERNAL_ADDRESS_OLAT_B;
		time_remaining = BC->port_b_time_remaining;
	} else {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	for(uint8_t i = 0; i < NUM_PINS_PER_PORT; i++) {
		time_remaining[i] = 0;
	}

	io_write(internal_address, data->value_mask);

	BA->com_return_setter(com, data);
}

void set_port_configuration(const ComType com, const SetPortConfiguration *data) {
	uint8_t internal_address_iodir;
	uint8_t internal_address_gppu;
	uint8_t internal_address_olat;
	uint32_t *time_remaining;

	if(data->port == 'a' || data->port == 'A') {
		internal_address_iodir = I2C_INTERNAL_ADDRESS_IODIR_A;
		internal_address_gppu  = I2C_INTERNAL_ADDRESS_GPPU_A;
		internal_address_olat  = I2C_INTERNAL_ADDRESS_OLAT_A;
		time_remaining = BC->port_a_time_remaining;
	} else if(data->port == 'b' || data->port == 'B') {
		internal_address_iodir = I2C_INTERNAL_ADDRESS_IODIR_B;
		internal_address_gppu  = I2C_INTERNAL_ADDRESS_GPPU_B;
		internal_address_olat  = I2C_INTERNAL_ADDRESS_OLAT_B;
		time_remaining = BC->port_b_time_remaining;
	} else {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	uint8_t iodir;

	if(data->direction == 'i' || data->direction == 'I') {
		iodir = io_read(internal_address_iodir);
		iodir |= data->selection_mask;
		io_write(internal_address_iodir, iodir);

		uint8_t gppu = io_read(internal_address_gppu);
		if(data->value) {
			gppu |= data->selection_mask;
		} else {
			gppu &= ~(data->selection_mask);
		}
		io_write(internal_address_gppu, gppu);
	} else if(data->direction == 'o' || data->direction == 'O') {
		uint8_t gpio = io_read(internal_address_olat);
		if(data->value) {
			gpio |= data->selection_mask;
		} else {
			gpio &= ~(data->selection_mask);
		}
		io_write(internal_address_olat, gpio);

		iodir = io_read(internal_address_iodir);
		iodir &= ~(data->selection_mask);
		io_write(internal_address_iodir, iodir);
	} else {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	for(uint8_t i = 0; i < NUM_PINS_PER_PORT; i++) {
		if (data->selection_mask & (1 << i)) {
			time_remaining[i] = 0;
		}
	}

	BA->com_return_setter(com, data);
}

void get_port_configuration(const ComType com, const GetPortConfiguration *data) {
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
		BA->com_return_error(data, sizeof(GetPortConfigurationReturn), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	GetPortConfigurationReturn gpcr;

	gpcr.header         = data->header;
	gpcr.header.length  = sizeof(GetPortConfigurationReturn);
	gpcr.direction_mask = io_read(internal_address_iodir);
	uint8_t latch_mask  = io_read(internal_address_olat);
	uint8_t pullup_mask = io_read(internal_address_gppu);

	gpcr.value_mask = (  gpcr.direction_mask  & pullup_mask) |
	                  ((~gpcr.direction_mask) & latch_mask);

	BA->send_blocking_with_timeout(&gpcr, sizeof(GetPortConfigurationReturn), com);
}

void set_debounce_period(const ComType com, const SetDebouncePeriod *data) {
	BC->debounce_period = data->debounce;
	BA->com_return_setter(com, data);
}

void get_debounce_period(const ComType com, const GetDebouncePeriod *data) {
	GetDebouncePeriodReturn gdpr;

	gdpr.header         = data->header;
	gdpr.header.length  = sizeof(GetDebouncePeriodReturn);
	gdpr.debounce       = BC->debounce_period;

	BA->send_blocking_with_timeout(&gdpr, sizeof(GetDebouncePeriodReturn), com);
}

void set_port_interrupt(const ComType com, const SetPortInterrupt *data) {
	uint8_t internal_address_gpinten;

	if(data->port == 'a' || data->port == 'A') {
		internal_address_gpinten = I2C_INTERNAL_ADDRESS_GPINTEN_A;
	} else if(data->port == 'b' || data->port == 'B') {
		internal_address_gpinten = I2C_INTERNAL_ADDRESS_GPINTEN_B;
	} else {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	io_write(internal_address_gpinten, data->interrupt_mask);
	BA->com_return_setter(com, data);
}

void get_port_interrupt(const ComType com, const GetPortInterrupt *data) {
	uint8_t internal_address_gpinten;

	if(data->port == 'a' || data->port == 'A') {
		internal_address_gpinten = I2C_INTERNAL_ADDRESS_GPINTEN_A;
	} else if(data->port == 'b' || data->port == 'B') {
		internal_address_gpinten = I2C_INTERNAL_ADDRESS_GPINTEN_B;
	} else {
		BA->com_return_error(data, sizeof(GetPortInterruptReturn), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	GetPortInterruptReturn gpir;

	gpir.header         = data->header;
	gpir.header.length  = sizeof(GetPortInterruptReturn);
	gpir.interrupt_mask = io_read(internal_address_gpinten);

	BA->send_blocking_with_timeout(&gpir, sizeof(GetPortInterruptReturn), com);
}

void set_port_monoflop(const ComType com, const SetPortMonoflop *data) {
	uint8_t internal_address_iodir;
	uint8_t internal_address_olat;
	uint32_t *time;
	uint32_t *time_remaining;

	if(data->port == 'a' || data->port == 'A') {
		internal_address_iodir = I2C_INTERNAL_ADDRESS_IODIR_A;
		internal_address_olat = I2C_INTERNAL_ADDRESS_OLAT_A;
		time = BC->port_a_time;
		time_remaining = BC->port_a_time_remaining;
	} else if(data->port == 'b' || data->port == 'B') {
		internal_address_iodir = I2C_INTERNAL_ADDRESS_IODIR_B;
		internal_address_olat = I2C_INTERNAL_ADDRESS_OLAT_B;
		time = BC->port_b_time;
		time_remaining = BC->port_b_time_remaining;
	} else {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	uint8_t direction_mask = io_read(internal_address_iodir);
	uint8_t gpio = io_read(internal_address_olat);

	for(uint8_t i = 0; i < NUM_PINS_PER_PORT; i++) {
		if((data->selection_mask & (1 << i)) && (direction_mask & (1 << i)) == 0) {
			if(data->value_mask & (1 << i)) {
				gpio |= (1 << i);
			} else {
				gpio &= ~(1 << i);
			}

			time[i] = data->time;
			time_remaining[i] = data->time;
		}
	}

	io_write(internal_address_olat, gpio);
	BA->com_return_setter(com, data);
}

void get_port_monoflop(const ComType com, const GetPortMonoflop *data) {
	uint8_t internal_address;
	uint32_t *time;
	uint32_t *time_remaining;

	if(data->port == 'a' || data->port == 'A') {
		internal_address = I2C_INTERNAL_ADDRESS_GPIO_A;
		time = BC->port_a_time;
		time_remaining = BC->port_a_time_remaining;
	} else if(data->port == 'b' || data->port == 'B') {
		internal_address = I2C_INTERNAL_ADDRESS_GPIO_B;
		time = BC->port_b_time;
		time_remaining = BC->port_b_time_remaining;
	} else {
		BA->com_return_error(data, sizeof(GetPortMonoflopReturn), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	if(data->pin >= NUM_PINS_PER_PORT) {
		BA->com_return_error(data, sizeof(GetPortMonoflopReturn), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	GetPortMonoflopReturn gpmr;

	gpmr.header         = data->header;
	gpmr.header.length  = sizeof(GetPortMonoflopReturn);
	gpmr.value          = (io_read(internal_address) & (1 << data->pin)) ? 1 : 0;
	gpmr.time           = time[data->pin];
	gpmr.time_remaining = time_remaining[data->pin];

	BA->send_blocking_with_timeout(&gpmr, sizeof(GetPortMonoflopReturn), com);
}

void set_selected_values(const ComType com, const SetSelectedValues *data) {
	uint8_t internal_address;
	uint32_t *time_remaining;

	if(data->port == 'a' || data->port == 'A') {
		internal_address = I2C_INTERNAL_ADDRESS_OLAT_A;
		time_remaining = BC->port_a_time_remaining;
	} else if(data->port == 'b' || data->port == 'B') {
		internal_address = I2C_INTERNAL_ADDRESS_OLAT_B;
		time_remaining = BC->port_b_time_remaining;
	} else {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	uint8_t gpio = io_read(internal_address);
	for(uint8_t i = 0; i < NUM_PINS_PER_PORT; i++) {
		if(data->selection_mask & (1 << i)) {
			time_remaining[i] = 0;
			if(data->value_mask & (1 << i)) {
				gpio |= 1 << i;
			} else {
				gpio &= ~(1 << i);
			}
		}
	}

	io_write(internal_address, gpio);

	BA->com_return_setter(com, data);
}
