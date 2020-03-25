/*
 * time.c
 *
 *  Created on: 30 jul. 2017
 *      Author: Ludo
 */

#include "time.h"

#include "windows.h"

/*** TIME local global variables ***/

static SYSTEMTIME time_structure;
static unsigned long time_start;

/*** TIME functions ***/

/* INIT SYSTEM TIME.
 * @param:	None.
 * @return: None.
 */
void TIME_Init() {
	GetSystemTime(&time_structure);
	time_start = (time_structure.wHour * 3600000) + (time_structure.wMinute * 60000) + (time_structure.wSecond * 1000) + time_structure.wMilliseconds;
}

/* RETURN THE CURRENT PROGRAM TIME.
 * @param:	None.
 * @return:	Number of milliseconds ellapsed since the program started.
 */
unsigned long TIME_GetMs() {
	GetSystemTime(&time_structure);
	unsigned long now = (time_structure.wHour * 3600000) + (time_structure.wMinute * 60000) + (time_structure.wSecond * 1000) + time_structure.wMilliseconds;
	return (now - time_start);
}
