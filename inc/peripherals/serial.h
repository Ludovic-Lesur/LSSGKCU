/*
 * serial.h
 *
 *  Created on: 30 jul. 2017
 *      Author: Ludo
 */

#ifndef SERIAL_H
#define SERIAL_H

#include "windows.h"

/*** SERIAL functions ***/

void SERIAL_Open(HANDLE* handle, char port[], unsigned int baud_rate);
BOOL SERIAL_Write(HANDLE* handle, unsigned char tx_byte);
unsigned char SERIAL_Read(HANDLE* handle);
void SERIAL_Flush(HANDLE* handle);
void SERIAL_Close(HANDLE* handle);

#endif /* SERIAL_H */
