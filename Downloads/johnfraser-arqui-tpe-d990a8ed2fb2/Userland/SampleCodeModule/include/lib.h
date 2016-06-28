#ifndef __PIXEL_OS__LIB__
#define __PIXEL_OS__LIB__

#include "libasm.h"
#include "defines.h"
#include <stdarg.h>

// Printing
void printf(char* fmt, ...);
void putChar(char c);

// Clear screen
void clearScreen();

// Reading
void read(char* buffer, char delimiter);
char getChar();



// Helpers
int numberLength (int i);
int strlen(char* str);
void itoa(int n, char* s);
void itoa_d(int n, char* s);
bool strEquals(char* str1, char* str2);
void split(char* buffer, char* command, char* arguments);

#endif