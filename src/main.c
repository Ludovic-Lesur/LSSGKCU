/*
 * main.c
 *
 *  Created on: 25 mar. 2020
 *      Author: Ludo
 */

#include "comp.h"
#include "keyboard.h"
#include "kvb.h"
#include "log.h"
#include "lsmcu.h"
#include "sound.h"
#include "stdio.h"
#include "time.h"
#include "zba.h"
#include "zdj.h"
#include "zpt.h"
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
	// Init log.
	LOG_Init();
	// Init sounds.
	SOUND_FmodSystemInit();
	COMP_Init();
	KVB_Init();
	ZBA_Init();
	ZDJ_Init();
	ZPT_Init();
	ZVM_Init();
	// Init serial link.
	LSMCU_Init("COM5");
	// Main loop.
	while (1) {
		COMP_Task();
		LOG_Task();
		LSMCU_Task();
		KVB_Task();
		ZVM_Task();
	}
	return 0;
}
