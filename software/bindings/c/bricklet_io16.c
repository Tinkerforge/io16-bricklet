/*************************************************************
 * This file was automatically generated on 2011-08-23.      *
 *                                                           *
 * If you have a bugfix for this file and want to commit it, *
 * please fix the bug in the generator. You can find a link  *
 * to the generator git on tinkerforge.com                   *
 *************************************************************/

#include "bricklet_io16.h"

#include <string.h>

#define TYPE_SET_PORT 1
#define TYPE_GET_PORT 2
#define TYPE_SET_PORT_CONFIGURATION 3
#define TYPE_GET_PORT_CONFIGURATION 4
#define TYPE_SET_DEBOUNCE_PERIOD 5
#define TYPE_GET_DEBOUNCE_PERIOD 6
#define TYPE_SET_PORT_INTERRUPT 7
#define TYPE_GET_PORT_INTERRUPT 8
#define TYPE_INTERRUPT 9

typedef void (*interrupt_func_t)(char, uint8_t, uint8_t);

#ifdef _MSC_VER
	#pragma pack(push)
	#pragma pack(1)

	#define PACKED
#else
	#define PACKED __attribute__((packed))
#endif

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	char port;
	uint8_t value_mask;
} PACKED SetPort;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	char port;
} PACKED GetPort;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	uint8_t value_mask;
} PACKED GetPortReturn;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	char port;
	uint8_t port_mask;
	char direction;
	bool value;
} PACKED SetPortConfiguration;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	char port;
} PACKED GetPortConfiguration;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	uint8_t direction_mask;
	uint8_t value_mask;
} PACKED GetPortConfigurationReturn;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	uint32_t debounce;
} PACKED SetDebouncePeriod;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
} PACKED GetDebouncePeriod;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	uint32_t debounce;
} PACKED GetDebouncePeriodReturn;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	char port;
	uint8_t interrupt_mask;
} PACKED SetPortInterrupt;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	char port;
} PACKED GetPortInterrupt;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	uint8_t interrupt_mask;
} PACKED GetPortInterruptReturn;

typedef struct {
	uint8_t stack_id;
	uint8_t type;
	uint16_t length;
	char port;
	uint8_t interrupt_mask;
	uint8_t value_mask;
} PACKED InterruptCallback;

#ifdef _MSC_VER
	#pragma pack(pop)
#endif

int io16_set_port(IO16 *io16, char port, uint8_t value_mask) {
	if(io16->ipcon == NULL) {
		return E_NOT_ADDED;
	}

	ipcon_sem_wait_write(io16);

	SetPort sp;
	sp.stack_id = io16->stack_id;
	sp.type = TYPE_SET_PORT;
	sp.length = sizeof(SetPort);
	sp.port = port;
	sp.value_mask = value_mask;

	ipcon_device_write(io16, (char *)&sp, sizeof(SetPort));

	ipcon_sem_post_write(io16);

	return E_OK;
}

int io16_get_port(IO16 *io16, char port, uint8_t *ret_value_mask) {
	if(io16->ipcon == NULL) {
		return E_NOT_ADDED;
	}

	ipcon_sem_wait_write(io16);

	io16->answer.type = TYPE_GET_PORT;
	io16->answer.length = sizeof(GetPortReturn);
	GetPort gp;
	gp.stack_id = io16->stack_id;
	gp.type = TYPE_GET_PORT;
	gp.length = sizeof(GetPort);
	gp.port = port;

	ipcon_device_write(io16, (char *)&gp, sizeof(GetPort));

	if(ipcon_answer_sem_wait_timeout(io16) != 0) {
		ipcon_sem_post_write(io16);
		return E_TIMEOUT;
	}

	GetPortReturn *gpr = (GetPortReturn *)io16->answer.buffer;
	*ret_value_mask = gpr->value_mask;

	ipcon_sem_post_write(io16);

	return E_OK;
}

int io16_set_port_configuration(IO16 *io16, char port, uint8_t port_mask, char direction, bool value) {
	if(io16->ipcon == NULL) {
		return E_NOT_ADDED;
	}

	ipcon_sem_wait_write(io16);

	SetPortConfiguration spc;
	spc.stack_id = io16->stack_id;
	spc.type = TYPE_SET_PORT_CONFIGURATION;
	spc.length = sizeof(SetPortConfiguration);
	spc.port = port;
	spc.port_mask = port_mask;
	spc.direction = direction;
	spc.value = value;

	ipcon_device_write(io16, (char *)&spc, sizeof(SetPortConfiguration));

	ipcon_sem_post_write(io16);

	return E_OK;
}

int io16_get_port_configuration(IO16 *io16, char port, uint8_t *ret_direction_mask, uint8_t *ret_value_mask) {
	if(io16->ipcon == NULL) {
		return E_NOT_ADDED;
	}

	ipcon_sem_wait_write(io16);

	io16->answer.type = TYPE_GET_PORT_CONFIGURATION;
	io16->answer.length = sizeof(GetPortConfigurationReturn);
	GetPortConfiguration gpc;
	gpc.stack_id = io16->stack_id;
	gpc.type = TYPE_GET_PORT_CONFIGURATION;
	gpc.length = sizeof(GetPortConfiguration);
	gpc.port = port;

	ipcon_device_write(io16, (char *)&gpc, sizeof(GetPortConfiguration));

	if(ipcon_answer_sem_wait_timeout(io16) != 0) {
		ipcon_sem_post_write(io16);
		return E_TIMEOUT;
	}

	GetPortConfigurationReturn *gpcr = (GetPortConfigurationReturn *)io16->answer.buffer;
	*ret_direction_mask = gpcr->direction_mask;
	*ret_value_mask = gpcr->value_mask;

	ipcon_sem_post_write(io16);

	return E_OK;
}

int io16_set_debounce_period(IO16 *io16, uint32_t debounce) {
	if(io16->ipcon == NULL) {
		return E_NOT_ADDED;
	}

	ipcon_sem_wait_write(io16);

	SetDebouncePeriod sdp;
	sdp.stack_id = io16->stack_id;
	sdp.type = TYPE_SET_DEBOUNCE_PERIOD;
	sdp.length = sizeof(SetDebouncePeriod);
	sdp.debounce = debounce;

	ipcon_device_write(io16, (char *)&sdp, sizeof(SetDebouncePeriod));

	ipcon_sem_post_write(io16);

	return E_OK;
}

int io16_get_debounce_period(IO16 *io16, uint32_t *ret_debounce) {
	if(io16->ipcon == NULL) {
		return E_NOT_ADDED;
	}

	ipcon_sem_wait_write(io16);

	io16->answer.type = TYPE_GET_DEBOUNCE_PERIOD;
	io16->answer.length = sizeof(GetDebouncePeriodReturn);
	GetDebouncePeriod gdp;
	gdp.stack_id = io16->stack_id;
	gdp.type = TYPE_GET_DEBOUNCE_PERIOD;
	gdp.length = sizeof(GetDebouncePeriod);

	ipcon_device_write(io16, (char *)&gdp, sizeof(GetDebouncePeriod));

	if(ipcon_answer_sem_wait_timeout(io16) != 0) {
		ipcon_sem_post_write(io16);
		return E_TIMEOUT;
	}

	GetDebouncePeriodReturn *gdpr = (GetDebouncePeriodReturn *)io16->answer.buffer;
	*ret_debounce = gdpr->debounce;

	ipcon_sem_post_write(io16);

	return E_OK;
}

int io16_set_port_interrupt(IO16 *io16, char port, uint8_t interrupt_mask) {
	if(io16->ipcon == NULL) {
		return E_NOT_ADDED;
	}

	ipcon_sem_wait_write(io16);

	SetPortInterrupt spi;
	spi.stack_id = io16->stack_id;
	spi.type = TYPE_SET_PORT_INTERRUPT;
	spi.length = sizeof(SetPortInterrupt);
	spi.port = port;
	spi.interrupt_mask = interrupt_mask;

	ipcon_device_write(io16, (char *)&spi, sizeof(SetPortInterrupt));

	ipcon_sem_post_write(io16);

	return E_OK;
}

int io16_get_port_interrupt(IO16 *io16, char port, uint8_t *ret_interrupt_mask) {
	if(io16->ipcon == NULL) {
		return E_NOT_ADDED;
	}

	ipcon_sem_wait_write(io16);

	io16->answer.type = TYPE_GET_PORT_INTERRUPT;
	io16->answer.length = sizeof(GetPortInterruptReturn);
	GetPortInterrupt gpi;
	gpi.stack_id = io16->stack_id;
	gpi.type = TYPE_GET_PORT_INTERRUPT;
	gpi.length = sizeof(GetPortInterrupt);
	gpi.port = port;

	ipcon_device_write(io16, (char *)&gpi, sizeof(GetPortInterrupt));

	if(ipcon_answer_sem_wait_timeout(io16) != 0) {
		ipcon_sem_post_write(io16);
		return E_TIMEOUT;
	}

	GetPortInterruptReturn *gpir = (GetPortInterruptReturn *)io16->answer.buffer;
	*ret_interrupt_mask = gpir->interrupt_mask;

	ipcon_sem_post_write(io16);

	return E_OK;
}

int io16_callback_interrupt(IO16 *io16, const unsigned char *buffer) {
	InterruptCallback *ic = (InterruptCallback *)buffer;
	((interrupt_func_t)io16->callbacks[ic->type])(ic->port, ic->interrupt_mask, ic->value_mask);
	return sizeof(InterruptCallback);
}

void io16_register_callback(IO16 *io16, uint8_t cb, void *func) {
    io16->callbacks[cb] = func;
}

void io16_create(IO16 *io16, const char *uid) {
	ipcon_device_create(io16, uid);

	io16->device_callbacks[TYPE_INTERRUPT] = io16_callback_interrupt;
}
