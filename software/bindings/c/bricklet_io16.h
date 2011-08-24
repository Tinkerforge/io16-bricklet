/*************************************************************
 * This file was automatically generated on 23.08.2011.      *
 *                                                           *
 * If you have a bugfix for this file and want to commit it, *
 * please fix the bug in the generator. You can find a link  *
 * to the generator git on tinkerforge.com                   *
 *************************************************************/

#ifndef BRICKLET_IO16_H
#define BRICKLET_IO16_H

#include "ip_connection.h"

typedef Device IO16;

#define IO16_CALLBACK_INTERRUPT 9

void io16_create(IO16 *io16, const char *uid);

int io16_set_port(IO16 *io16, char port, uint8_t value_mask);
int io16_get_port(IO16 *io16, char port, uint8_t *ret_value_mask);
int io16_set_port_configuration(IO16 *io16, char port, uint8_t port_mask, char direction, bool value);
int io16_get_port_configuration(IO16 *io16, char port, uint8_t *ret_direction_mask, uint8_t *ret_value_mask);
int io16_set_debounce_period(IO16 *io16, uint32_t debounce);
int io16_get_debounce_period(IO16 *io16, uint32_t *ret_debounce);
int io16_set_port_interrupt(IO16 *io16, char port, uint8_t interrupt_mask);
int io16_get_port_interrupt(IO16 *io16, char port, uint8_t *ret_interrupt_mask);
int io16_interrupt(IO16 *io16, char *ret_port, uint8_t *ret_interrupt_mask, uint8_t *ret_value_mask);

void io16_register_callback(IO16 *io16, uint8_t cb, void *func);

#endif