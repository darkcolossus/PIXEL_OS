#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);

int kstrlen(char* str);
char* kstrcat(char *dest, const char *src);
void kitoa(int n, char s[]);
void kitoa_d(int n, char s[]);
void kreverse(char s[]);
void kprintHex(uint64_t value);
void kprintDec(uint64_t value);
void kprintBase(uint64_t value, uint32_t base);
static uint32_t kuintToBase(uint64_t value, char * buffer, uint32_t base);


void kWrite(char* str, int length);
void kRead(char* buffer);

extern void kout(unsigned short port, unsigned char data);
extern unsigned char kin(unsigned short port);

#endif
