/*
 * fsm.c
 *
 *  Created on: Nov 19, 2023
 *      Author: Admin
 */
#include "fsm.h"

#include <inttypes.h>

enum Command_state {
		Begin,
		Body
};
static enum Command_state command_state = Begin;

