/*
 * file.h
 *
 *  Created on: 29 mar. 2020
 *      Author: Ludo
 */

#ifndef FILE_H
#define FILE_H

#include "stdio.h"

/*** FILE macros ***/

#define FILE_LINE_MAX_LENGTH	200

/*** FILE functions ***/

void FILE_Open(FILE** file, const char* file_path);
void FILE_GetLastLine(FILE* file, char* last_line);
void FILE_Close(FILE* file);

#endif /* FILE_H */
