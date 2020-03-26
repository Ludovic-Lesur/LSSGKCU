/*
 * lsmcu.c
 *
 *  Created on: 25 mar. 2020
 *      Author: Ludo
 */

#include "lsmcu.h"

#include "C:/Users/Ludovic/Documents/Eclipse/LSMCU/inc/applicative/lssgkcu.h"
#include "serial.h"
#include "stdio.h"
#include "zvm.h"

/*** LSMCU local macros ***/

#define LSMCU_SERIAL_BAUDRATE	9600
#define LSMCU_LOG

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
#ifdef LSMCU_LOG
	printf("LSMCU *** TX command = 0x%x.\n", tx_command);
	fflush(stdout);
#endif
}

/* MAIN TASK OF LSMCU MANAGER.
 * @param:	None.
 * @return:	None.
 */
void LSMCU_Task(void) {
	// Read serial port.
	unsigned char rx_command = LSMCU_OUT_NOP;
	unsigned char rx_success = SERIAL_Read(&lsmcu_handle, &rx_command);
	if ((rx_success != 0) && (rx_command != LSMCU_OUT_NOP)) {
#ifdef LSMCU_LOG
		printf("LSMCU *** RX command = 0x%x.\n", rx_command);
		fflush(stdout);
#endif
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
}


