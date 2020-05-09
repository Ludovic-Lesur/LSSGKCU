/*
 * log.c
 *
 *  Created on: 29 mar. 2020
 *      Author: Ludo
 */

#include "log.h"

#include "file.h"
#include "keyboard.h"
#include "lsmcu.h"
#include "openrails.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"

/*** LOG local macros ***/

#define LOG_PERIOD_MS			500
#define LOG_SPEED_MAX_LENGTH	10
#define LOG_SPEED_ERROR			0xFF
//#define LOG_DEBUG

/*** LOG local structures ***/

typedef struct {
	FILE* log_file;
	unsigned char log_enable;
	unsigned long log_next_time;
} LOG_Context;

/*** LOG local global variables ***/

static LOG_Context log_ctx;

/*** LOG local functions ***/

unsigned int LOG_GetSpeed() {
	// Local strings.
	char log_line[FILE_LINE_MAX_LENGTH];
	char speed_string[LOG_SPEED_MAX_LENGTH];
	// Get last line.
	FILE_GetLastLine(log_ctx.log_file, log_line);
#ifdef LOG_DEBUG
	printf("LOG *** Line = %s\n", log_line);
#endif

	unsigned int speed = LOG_SPEED_ERROR;
	// Search "km/h" in the last line.
	char* log_line_kmh = strstr(log_line, "km/h");
	if (log_line_kmh != ((void*) 0)) {
		unsigned int index_kmh = (log_line_kmh - log_line);
#ifdef LOG_DEBUG
		printf("LOG *** Index km/h = %d\n", index_kmh);
#endif
		// Search previous space.
		unsigned int char_idx = index_kmh;
		unsigned int space_idx = 0;
		for (; char_idx>0; char_idx--) {
			if (log_line[char_idx] == ' ') {
				space_idx = char_idx;
				break;
			}
		}
#ifdef LOG_DEBUG
		printf("LOG *** Previous space index = %d\n", space_idx);
#endif
		// Extract speed.
		unsigned int speed_idx = 0;
		for (char_idx=(space_idx+1); char_idx<index_kmh ; char_idx++) {
			// Stop reading if a coma or "km/h" is found.
			if ((log_line[char_idx] == ',') || (log_line[char_idx] == 'k')) {
				break;
			}
			else {
				speed_string[speed_idx] = log_line[char_idx];
				speed_idx++;
			}
		}
		// Fill remaining characters with '\0'.
		for (; speed_idx < LOG_SPEED_MAX_LENGTH; speed_idx++) {
			speed_string[speed_idx] = '\0';
		}
#ifdef LOG_DEBUG
		printf("LOG *** Speed string = <%s>\n", speed_string);
#endif
		// Parse speed.
		speed = atoi(speed_string);
#ifdef LOG_DEBUG
		printf("LOG *** Speed = %d\n", speed);
#endif
	}
	else {
#ifdef LOG_DEBUG
		printf("LOG *** String 'km/h' was not found.\n");
#endif
	}
	return speed;
}

/*** LOG functions ***/

/* INIT GAME LOG UNIT.
 * @param:	None.
 * @return:	None.
 */
void LOG_Init(void) {
	// Open OpenRails log file.
	FILE_Open(&log_ctx.log_file, "C:/Users/Ludovic/Desktop/OpenRailsDump.csv");
	log_ctx.log_next_time = 0;
	log_ctx.log_enable = 0;
}

/* START GAME LOG.
 * @param:	None.
 * @eturn:	None.
 */
void LOG_Enable(void) {
	log_ctx.log_enable = 1;
}

/* STOP GAME LOG.
 * @param:	None.
 * @eturn:	None.
 */
void LOG_Disable(void) {
	log_ctx.log_enable = 0;
}

/* MAIN TASK OF GAME LOG UNIT.
 * @param:	None.
 * @return:	None.
 */
void LOG_Task(void) {
	// Check enable bit and period.
	if ((log_ctx.log_enable != 0) && (TIME_GetMs() > log_ctx.log_next_time)) {
		// Update next time.
		log_ctx.log_next_time = TIME_GetMs() + LOG_PERIOD_MS;
		// Activate log.
		KEYBOARD_Send(OPENRAILS_LOG, OPENRAILS_PRESS_DURATION_MS_DEFAULT);
		// Get speed.
		unsigned char speed_kmh = LOG_GetSpeed();
		if (speed_kmh != LOG_SPEED_ERROR) {
#ifdef LOG_DEBUG
			printf("LOG *** Speed = %dkm/h\n", speed_kmh);
#endif
			// Transmit to dashboard.
			LSMCU_Send(speed_kmh);
		}
#ifdef LOG_DEBUG
		else {
			printf("LOG *** Speed error\n");
		}
		fflush(stdout);
#endif
	}
}
