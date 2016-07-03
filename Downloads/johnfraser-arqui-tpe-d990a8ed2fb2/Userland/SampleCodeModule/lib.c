#include "include/lib.h"
#include "include/libasm.h"

/***************
**  Printing  **
***************/

void putString(char* str){
        int length = strlen(str);
        int i = 0;

        while(i<length){
          putChar(str[i++]);
        }
}

void putChar(char c){
        runSyscall(WRITE,0x0,&c,1);
}

void printf(char* fmt, ...){

        va_list args;

        va_start(args, fmt);

        int i;
        int lenght = strlen(fmt);

        for(i=0; i<lenght; i++) {

          if(fmt[i] == '%'){

              char next = fmt[i+1];

              if(next == 0){
                return;
              } else if(next == 'd'){
                int n = va_arg(args, int);
                char buffer[numberLength(n)];
                itoa(n, buffer);
                putString(buffer);
                i++;
              } else if(next == 's'){
                char* s = va_arg(args, char*);
                putString(s);
                i++;
              } else if(next == 'c'){
                char c = va_arg(args, int);
                putChar(c);
                i++;
              } else if(next == 'n'){
                int n = va_arg(args, int);
                char buffer[numberLength(n)];
                itoa_d(n, buffer);
                putString(buffer);
                i++;
              }

          } else {
            putChar(fmt[i]);
          }

        }

        va_end(args);
}

/**************
**  Reading  **
**************/

char getChar(){
        char c;
        do {
          runSyscall(READ, 0x0, &c, 0x0);
        } while(c==0);
        return c;
}

void read(char* buffer, char delimiter){
        int i = 0;
        char c;

        do {
            c = getChar();
            if(c == '\b'){
              if(i>0){
                  buffer[i--] = ' ';
              }
            } else{
                buffer[i++] = c;
            }
        } while (c != delimiter);

        buffer[i-1] = 0;
}


void clearScreen(){
        runSyscall(CLEAR_SCREEN,0x0,0x0,0x0);
}

/**************
**  Helpers  **
**************/

void split(char* buffer, char* command, char* arguments){

  int i = 0;
  int j = 0;
  char c;

  do {
    c = buffer[i];
    command[i++] = c;

    if(c == ' ' || i == (MAX_COMMAND_LENGTH-1))
      break;

  } while( c != 0);

  command[i-1] = 0;

  if(c!=' '){
    arguments[j] = 0;
    return;
  }

  do {
    c = buffer[i++];
    arguments[j++] = c;

  } while(c != 0);

  arguments[j-1] = 0;
}

int numberLength (int i) {

  int counter = 1;

  while(i!=0){
      i=i/10;
      counter++;
  }

  return counter;
}

int strlen(char* str){

        int i = 0;

        while(str[i++] != '\0') ;

        return (i-1);
}

void reverse(char s[])
{
        int i, j;
        char c;

        for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
                c = s[i];
                s[i] = s[j];
                s[j] = c;
        }
}


void itoa(int n, char* s)
{
        int i, sign;

        if ((sign = n) < 0)
                n = -n;

        i = 0;

        do {
              s[i++] = n % 10 + '0';
        } while ((n /= 10) > 0);

        if (sign < 0)
            s[i++] = '-';

        s[i] = '\0';

        reverse(s);
}

int atoi(char *str)
{
    if (*str == '\0')
       return 0;
 
    int res = 0;    // Initialize result
    int sign = 1;   // Initialize sign as positive
    int i = 0;      // Initialize index of first digit
 
     if (str[0] == '-')
    {
        sign = -1;
        i++;        // Also update index of first digit
    }
 
    for (; str[i] != '\0'; ++i)
    {
        if ( str[i] <= '0' || str[i] >= '9')  // If string contain character it will terminate
            return 0; 
            
        res = res*10 + str[i] - '0';
    }
 
    return sign*res;
}

int isNumber (unsigned char n)
{
  int t = ('0' <= n && n <= '9');
  return t;
}


void itoa_d(int n, char* s)
{
     int i, sign;

     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     if (i<2){
			s[i++] = '0';
     }
     s[i] = '\0';
     reverse(s);
}

bool strEquals(char* str1, char* str2){

        if(strlen(str1) != strlen(str2)) {
                return FALSE;
        }else{
                for(int i=0; i<strlen(str1); i++) {
                        if(str1[i] != str2[i]) {
                                return FALSE;
                        }
                }
        }

        return TRUE;
}