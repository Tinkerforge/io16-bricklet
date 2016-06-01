/* io16-bricklet
 * Copyright (C) 2012-2014 Matthias Bolte <matthias@tinkerforge.com>
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

		case FID_GET_EDGE_COUNT: {
			get_edge_count(com, (GetEdgeCount*)data);
			break;
		}

		case FID_SET_EDGE_COUNT_CONFIG: {
			set_edge_count_config(com, (SetEdgeCountConfig*)data);
			break;
		}

		case FID_GET_EDGE_COUNT_CONFIG: {
			get_edge_count_config(com, (GetEdgeCountConfig*)data);
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

	BC->debounce_period = 100;

	// Enable interrupt for edge count
	io_write(I2C_INTERNAL_ADDRESS_GPINTEN_A, MASK_EDGE_COUNT);
	BC->interrupt_edge = false;

	for(uint8_t i = 0; i < NUM_PORTS; i++) {
		BC->counter[i] = 0;

		io_write(I2C_INTERNAL_ADDRESS_IOCON_A + i, IOCON_ODR);

		// Default is input pull up
		io_write(I2C_INTERNAL_ADDRESS_GPPU_A+i, 0xFF);
		io_write(I2C_INTERNAL_ADDRESS_IODIR_A+i, 0xFF);

		BC->current_gpinten[i] = 0;
		BC->current_olat[i] = 0;
		BC->current_gpio[i] = 0xFF;
		BC->current_gppu[i] = 0xFF;
		BC->current_iodir[i] = 0xFF;

		for(uint8_t j = 0; j < NUM_PINS_PER_PORT; j++) {
			BC->time[j][i] = 0;
			BC->time_remaining[j][i] = 0;
		}

		BC->monoflop_callback_mask[i] = 0;

		BC->edge_count[i] = 0;
		BC->edge_type[i] = EDGE_TYPE_RISING;
		BC->edge_debounce[i] = 100;
		BC->edge_debounce_counter[i] = 0;
		BC->edge_last_state[i] = 1;

		BC->interrupt_callback[i] = false;
	}
}

void destructor(void) {
}

void send_interrupt_callback(uint8_t port_num) {
	uint8_t new_gpio = io_read(I2C_INTERNAL_ADDRESS_GPIO_A + port_num);
	const uint8_t mask = (new_gpio ^ BC->current_gpio[port_num]) & BC->current_gpinten[port_num];

	if(mask != 0) {
		InterruptSignal is;
		BA->com_make_default_header(&is, BS->uid, sizeof(InterruptSignal), FID_INTERRUPT);

		is.port           = port_num + 'a';
		is.interrupt_mask = mask;
		is.value_mask     = new_gpio;

		BC->current_gpio[port_num] = new_gpio;

		BA->send_blocking_with_timeout(&is,
		                               sizeof(InterruptSignal),
		                               *BA->com_current);
		BC->counter[port_num] = BC->debounce_period;
	}
}

void update_monoflop_time(const uint8_t port_num) {
	uint8_t monoflop_done_mask = 0;

	for(uint8_t i = 0; i < NUM_PINS_PER_PORT; i++) {
		if(BC->time_remaining[i][port_num] != 0) {
			BC->time_remaining[i][port_num]--;

			if(BC->time_remaining[i][port_num] == 0) {
				monoflop_done_mask |= (1 << i);
			}
		}
	}

	if(monoflop_done_mask == 0) {
		return;
	}

	for(uint8_t i = 0; i < NUM_PINS_PER_PORT; i++) {
		if(monoflop_done_mask & (1 << i)) {
			if(BC->current_olat[port_num] & (1 << i)) {
				BC->current_olat[port_num] &= ~(1 << i);
			} else {
				BC->current_olat[port_num] |= (1 << i);
			}
		}
	}

	io_write(I2C_INTERNAL_ADDRESS_OLAT_A + port_num, BC->current_olat[port_num]);

	BC->monoflop_callback_mask[port_num] |= monoflop_done_mask;
}

void send_monoflop_callback(const uint8_t port_num) {
	MonoflopDone md;
	BA->com_make_default_header(&md, BS->uid, sizeof(MonoflopDone), FID_MONOFLOP_DONE);

	md.port           = port_num + 'a';
	md.selection_mask = 0;
	md.value_mask     = 0;

	uint8_t gpio = io_read(I2C_INTERNAL_ADDRESS_GPIO_A + port_num);

	for(uint8_t i = 0; i < NUM_PINS_PER_PORT; i++) {
		if (BC->monoflop_callback_mask[port_num] & (1 << i)) {
			md.selection_mask |= (1 << i);

			if(gpio & (1 << i)) {
				md.value_mask |= (1 << i);
			}
		}
	}

	BA->send_blocking_with_timeout(&md,
	                               sizeof(MonoflopDone),
	                               *BA->com_current);

	BC->monoflop_callback_mask[port_num] = 0;
}

void update_edge_counter(void) {
	uint16_t gpio_a = 0xFFFF;

	for(uint8_t i = 0; i < NUM_EDGE_COUNT; i++) {
		if(BC->edge_debounce_counter[i] != 0) {
			continue;
		}

		if((BC->current_iodir[0] & (1 << i)) == 0) {
			continue;
		}

		if(gpio_a == 0xFFFF) {
			gpio_a = io_read(I2C_INTERNAL_ADDRESS_GPIO_A);
		}

		uint8_t state = (gpio_a & (1 << i)) ? 1 : 0;

		if(state == BC->edge_last_state[i]) {
			continue;
		}

		BC->edge_last_state[i] = state;
		BC->edge_debounce_counter[i] = BC->edge_debounce[i];

		if((BC->edge_type[i] == EDGE_TYPE_BOTH) ||
		   (state && (BC->edge_type[i] == EDGE_TYPE_RISING)) ||
		   (!state && (BC->edge_type[i] == EDGE_TYPE_FALLING))) {
				BC->edge_count[i]++;
		}
	}
}

void tick(const uint8_t tick_type) {
	if(tick_type & TICK_TASK_TYPE_CALCULATION) {
		for(uint8_t i = 0; i < NUM_PORTS; i++) {
			if(BC->counter[i] != 0) {
				BC->counter[i]--;
			}

			update_monoflop_time(i);

			// edge counter
			if(BC->edge_debounce_counter[i] != 0) {
				BC->edge_debounce_counter[i]--;
			}
		}

		uint8_t tmp_a = (PIN_INT_A.pio->PIO_PDSR & PIN_INT_A.mask) == 0;

		if(!BC->interrupt_edge) {
			BC->interrupt_edge = tmp_a;
		}

		if(!BC->interrupt_callback[0]) {
			BC->interrupt_callback[0] = tmp_a;
		}
		if(!BC->interrupt_callback[1]) {
			BC->interrupt_callback[1] = (PIN_INT_B.pio->PIO_PDSR & PIN_INT_B.mask) == 0;
		}

		if(BC->interrupt_edge) {
			update_edge_counter();
		}
	}

	if(tick_type & TICK_TASK_TYPE_MESSAGE) {
		for(uint8_t i = 0; i < NUM_PORTS; i++) {
			if(BC->counter[i] == 0 && BC->interrupt_callback[i]) {
				BC->interrupt_callback[i] = false;
				send_interrupt_callback(i);
			}

			if(BC->monoflop_callback_mask[i]) {
				send_monoflop_callback(i);
			}
		}
	}
}

uint8_t get_i2c_address(void) {
	if(BS->address == I2C_EEPROM_ADDRESS_HIGH) {
		return I2C_ADDRESS_HIGH;
	} else {
		return I2C_ADDRESS_LOW;
	}
}

void io_write(const uint8_t internal_address, const uint8_t value) {
	const uint8_t address = get_i2c_address();
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
	const uint8_t address = get_i2c_address();
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

uint8_t port_to_num(const char c) {
	return c < 'C' ? c - 'A' : c - 'a';
}

void get_port(const ComType com, const GetPort *data) {
	const uint8_t port_num = port_to_num(data->port);

	if(port_num > 1) {
		BA->com_return_error(data, sizeof(GetPortReturn), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	GetPortReturn gpr;

	gpr.header        = data->header;
	gpr.header.length = sizeof(GetPortReturn);;
	gpr.value_mask    = io_read(I2C_INTERNAL_ADDRESS_GPIO_A + port_num);

	BA->send_blocking_with_timeout(&gpr, sizeof(GetPortReturn), com);
}

void set_port(const ComType com, const SetPort *data) {
	// set_port sets output latches, values are measured on port if port
	// is configured as output
	const uint8_t port_num = port_to_num(data->port);

	if(port_num > 1) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	for(uint8_t i = 0; i < NUM_PINS_PER_PORT; i++) {
		 BC->time_remaining[i][port_num] = 0;
	}

	BC->current_olat[port_num] = data->value_mask;
	io_write(I2C_INTERNAL_ADDRESS_OLAT_A + port_num, BC->current_olat[port_num]);

	BA->com_return_setter(com, data);
}

void set_port_configuration(const ComType com, const SetPortConfiguration *data) {
	const uint8_t port_num = port_to_num(data->port);

	if(port_num > 1) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	if(data->direction == 'i' || data->direction == 'I') {
		BC->current_iodir[port_num] |= data->selection_mask;
		io_write(I2C_INTERNAL_ADDRESS_IODIR_A + port_num, BC->current_iodir[port_num]);

		if(data->value) {
			BC->current_gppu[port_num] |= data->selection_mask;
		} else {
			BC->current_gppu[port_num] &= ~(data->selection_mask);
		}
		io_write(I2C_INTERNAL_ADDRESS_GPPU_A + port_num, BC->current_gppu[port_num]);
	} else if(data->direction == 'o' || data->direction == 'O') {
		if(data->value) {
			BC->current_olat[port_num] |= data->selection_mask;
		} else {
			BC->current_olat[port_num] &= ~(data->selection_mask);
		}
		io_write(I2C_INTERNAL_ADDRESS_OLAT_A + port_num, BC->current_olat[port_num]);

		BC->current_iodir[port_num] &= ~(data->selection_mask);
		io_write(I2C_INTERNAL_ADDRESS_IODIR_A + port_num, BC->current_iodir[port_num]);
	} else {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	for(uint8_t i = 0; i < NUM_PINS_PER_PORT; i++) {
		if(data->selection_mask & (1 << i)) {
			BC->time_remaining[i][port_num] = 0;
		}
	}

	BA->com_return_setter(com, data);
}

void get_port_configuration(const ComType com, const GetPortConfiguration *data) {
	const uint8_t port_num = port_to_num(data->port);

	if(port_num > 1) {
		BA->com_return_error(data, sizeof(GetPortConfigurationReturn), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	GetPortConfigurationReturn gpcr;

	gpcr.header         = data->header;
	gpcr.header.length  = sizeof(GetPortConfigurationReturn);

	gpcr.direction_mask = BC->current_iodir[port_num];
	gpcr.value_mask     = (  BC->current_iodir[port_num]  & BC->current_gppu[port_num]) |
	                      ((~BC->current_iodir[port_num]) & BC->current_olat[port_num]);

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
	const uint8_t port_num = port_to_num(data->port);

	uint8_t mask = data->interrupt_mask;

	if(port_num > 1) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	BC->current_gpinten[port_num] = data->interrupt_mask;

	if(port_num == 0) {
		mask |= MASK_EDGE_COUNT;
	}
	io_write(I2C_INTERNAL_ADDRESS_GPINTEN_A + port_num, mask);
	BA->com_return_setter(com, data);
}

void get_port_interrupt(const ComType com, const GetPortInterrupt *data) {
	const uint8_t port_num = port_to_num(data->port);

	if(port_num > 1) {
		BA->com_return_error(data, sizeof(GetPortInterruptReturn), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	GetPortInterruptReturn gpir;

	gpir.header         = data->header;
	gpir.header.length  = sizeof(GetPortInterruptReturn);
	gpir.interrupt_mask = BC->current_gpinten[port_num];

	BA->send_blocking_with_timeout(&gpir, sizeof(GetPortInterruptReturn), com);
}

void set_port_monoflop(const ComType com, const SetPortMonoflop *data) {
	const uint8_t port_num = port_to_num(data->port);

	if(port_num > 1) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	for(uint8_t i = 0; i < NUM_PINS_PER_PORT; i++) {
		if((data->selection_mask & (1 << i)) && (BC->current_iodir[port_num] & (1 << i)) == 0) {
			if(data->value_mask & (1 << i)) {
				BC->current_olat[port_num] |= (1 << i);
			} else {
				BC->current_olat[port_num] &= ~(1 << i);
			}

			BC->time[i][port_num] = data->time;
			BC->time_remaining[i][port_num] = data->time;
		}
	}

	io_write(I2C_INTERNAL_ADDRESS_OLAT_A + port_num, BC->current_olat[port_num]);
	BA->com_return_setter(com, data);
}

void get_port_monoflop(const ComType com, const GetPortMonoflop *data) {
	const uint8_t port_num = port_to_num(data->port);

	if(port_num > 1 || data->pin >= NUM_PINS_PER_PORT) {
		BA->com_return_error(data, sizeof(GetPortMonoflopReturn), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	GetPortMonoflopReturn gpmr;

	gpmr.header         = data->header;
	gpmr.header.length  = sizeof(GetPortMonoflopReturn);
	gpmr.value          = (io_read(I2C_INTERNAL_ADDRESS_GPIO_A + port_num) & (1 << data->pin)) ? 1 : 0;
	gpmr.time           = BC->time[data->pin][port_num];
	gpmr.time_remaining = BC->time_remaining[data->pin][port_num];

	BA->send_blocking_with_timeout(&gpmr, sizeof(GetPortMonoflopReturn), com);
}

void set_selected_values(const ComType com, const SetSelectedValues *data) {
	const uint8_t port_num = port_to_num(data->port);

	if(port_num > 1) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	for(uint8_t i = 0; i < NUM_PINS_PER_PORT; i++) {
		if(data->selection_mask & (1 << i)) {
			BC->time_remaining[i][port_num] = 0;
			if(data->value_mask & (1 << i)) {
				BC->current_olat[port_num] |= 1 << i;
			} else {
				BC->current_olat[port_num] &= ~(1 << i);
			}
		}
	}

	io_write(I2C_INTERNAL_ADDRESS_OLAT_A + port_num, BC->current_olat[port_num]);

	BA->com_return_setter(com, data);
}

void get_edge_count(const ComType com, const GetEdgeCount *data) {
	if(data->pin > 1) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
	}

	GetEdgeCountReturn gecr;

	gecr.header        = data->header;
	gecr.header.length = sizeof(GetEdgeCountReturn);
	gecr.count         = BC->edge_count[data->pin];

	BA->send_blocking_with_timeout(&gecr,
	                               sizeof(GetEdgeCountReturn),
	                               com);

	if(data->reset_counter) {
		BC->edge_count[data->pin] = 0;
	}
}

void set_edge_count_config(const ComType com, const SetEdgeCountConfig *data) {
	if(data->pin > 1) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
	}

	BC->edge_type[data->pin] = data->edge_type;
	BC->edge_debounce[data->pin] = data->debounce;
	BC->edge_count[data->pin] = 0;

	BA->com_return_setter(com, data);
}

void get_edge_count_config(const ComType com, const GetEdgeCountConfig *data) {
	if(data->pin > 1) {
		BA->com_return_error(data, sizeof(GetEdgeCountConfigReturn), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
	}

	GetEdgeCountConfigReturn geccr;

	geccr.header        = data->header;
	geccr.header.length = sizeof(GetEdgeCountConfigReturn);
	geccr.edge_type = BC->edge_type[data->pin];
	geccr.debounce  = BC->edge_debounce[data->pin];

	BA->send_blocking_with_timeout(&geccr,
	                               sizeof(GetEdgeCountConfigReturn),
	                               com);
}
