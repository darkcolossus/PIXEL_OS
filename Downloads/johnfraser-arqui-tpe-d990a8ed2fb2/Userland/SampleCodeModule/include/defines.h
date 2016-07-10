#ifndef __PIXEL_OS__DEFINES_U__
#define __PIXEL_OS__DEFINES_U__

typedef unsigned short word; //16 bits
typedef unsigned long ddword;

typedef unsigned char bool;
#define TRUE 1
#define FALSE 0

#define TOTAL_COMMANDS 13

#define MAX_COMMAND_LENGTH 50
#define MAX_ARGUMENTS_LENGTH 150

typedef enum SYSTEM_CALLS {
        WRITE = 0x4,
        READ = 0x3,
        CLEAR_SCREEN = 0x2,
        SET_SOUND = 0x5,
        SET_TIMER_HANDLER = 0x6,
        BEEP = 0x7,
        CREATE_PROCESS = 0x8,
        DELETE_PROCESS = 0x9,
        LIST_PROCESSES = 0xA,
        TERMINAL = 0xB
} SYSCALLS;

typedef int (*action)(char*);

typedef struct CommandDesc {
        char* key;
        action handler;
        char* use;
        bool hidden;
} CommandDescriptor;

#endif
