/*
 * main.c
 *
 *  Created on: 25 mar. 2020
 *      Author: Ludo
 */

#include "keyboard.h"
#include "lsmcu.h"
#include "serial.h"
#include "sound.h"
#include "stdio.h"
#include "time.h"
#include "zvm.h"

/* MAIN FUNCTION.
 * @param:	None.
 * @return:	None.
 */
int main (void) {
	// Start print.
	printf("**********************************************************************\n");
	printf("*** Locomotive Simulator Sound Game and KVB Control Unit (LSSGKCU) ***\n");
	printf("**********************************************************************\n\n");
	fflush(stdout);
	// Init time.
	TIME_Init();
	// Init sounds.
	SOUND_FmodSystemInit();
	ZVM_Init();
	// Init serial link.
	LSMCU_Init("COM3");
	// Main loop.
	while (1) {
		LSMCU_Task();
		ZVM_Task();
	}
	printf("End of program");

	return 0;
}
