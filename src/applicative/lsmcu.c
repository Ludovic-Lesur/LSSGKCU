/*
 * lsmcu.c
 *
 *  Created on: 25 mar. 2020
 *      Author: Ludo
 */

#include "lsmcu.h"

#include "serial.h"
#include "stdio.h"
#include "zvm.h"

/*** LSMCU local macros ***/

#define LSMCU_SERIAL_BAUDRATE	9600

/*** LSMCU local global variables ***/

static HANDLE lsmcu_handle;

/*** LSMCU functions ***/

/* INIT LSMCU MANAGER.
 * @param port:	LSMCU port number ("COMxx").
 * @return:		None.
 */
void LSMCU_Init(char port[]) {
	// Open serial port.
	SERIAL_Open(&lsmcu_handle, port, LSMCU_SERIAL_BAUDRATE);
}

/* SEND A COMMAND TO LSMCU.
 * @param tx_command:	Command to send (see enumeration in lsmcu.h).
 * @return:				None.
 */
void LSMCU_Send(unsigned tx_command) {
	SERIAL_Write(&lsmcu_handle, tx_command);
}

/* MAIN TASK OF LSMCU MANAGER.
 * @param:	None.
 * @return:	None.
 */
void LSMCU_Task(void) {
	// Read serial port.
	unsigned char rx_command = SERIAL_Read(&lsmcu_handle);
	// Decode incoming command.
	switch (rx_command) {
	case LSMCU_OUT_ZVM_ON:
		ZVM_TurnOn();
		break;
	case LSMCU_OUT_ZVM_OFF:
		ZVM_TurnOff();
		break;
	default:
		// Unknwon command.
		break;
	}
}


