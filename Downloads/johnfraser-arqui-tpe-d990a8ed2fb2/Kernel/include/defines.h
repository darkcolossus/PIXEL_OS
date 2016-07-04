#ifndef __PIXEL_OS__DEFINES__
#define __PIXEL_OS__DEFINES__

#include <stdint.h>

typedef unsigned char bool;
typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;
typedef unsigned long ddword;
typedef int (*EntryPoint)();


#define TRUE 1
#define FALSE 0
#define SCREEN_WIDTH     80
#define SCREEN_HEIGHT 25

#define IDT_SIZE 256

#define TICK_INTERVAL   55


#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

#define KEYBOARD_BUFFER_SIZE  32

#define NOT_PRINTABLE 0x0
#define TOTAL_KEYS  59

#define KEY_RELEASED  0x80

#define RS_PRESSED          0x36
#define RS_RELEASED         0xB6
#define LS_PRESSED          0x2A
#define LS_RELEASED         0xAA
#define CAPS_LOCK           0x3A
#define SPACEBAR            0x20

#define EMPTY 0xFF

#define PORT_TIMER_PREP 0x43
#define PORT_TIMER_0_DATA 0x40
#define PORT_TIMER_2_DATA 0x42

typedef enum CURSOR_STYLE{
  CURSOR_BLOCK = 0x00,
  CURSOR_LINE = 0x0E,
  NO_CURSOR = 0x10
} CursorStyle;

typedef struct KB_STATUS {
        bool shiftEnabled;
        bool capsLockEnabled;
} kKBStatus;

typedef struct KB_BUFFER {
        word readIndex;
        word writeIndex;
        unsigned char buffer[KEYBOARD_BUFFER_SIZE];
} kKBBuffer;

typedef struct IDTR_DESC {
        word limit;
        ddword base;
} IDTR;

typedef enum COLOR {
        BLACK,
        BLUE,
        GREEN,
        CYAN,
        RED,
        MAGENTA,
        BROWN,
        LIGHT_GRAY,
        DARK_GRAY,
        LIGHT_BLUE,
        LIGHT_GREEN,
        LIGHT_CYAN,
        LIGHT_RED,
        LIGHT_MAGENTA,
        YELLOW,
        WHITE
} Color;

#define DEFAULT_TEXT_COLOR LIGHT_GRAY
#define DEFAULT_BG_COLOR BLACK

/* MEMORY*/

typedef struct {
    uint64_t  Present:1;                // 0 = Not present in memory, 1 = Present in memory
    uint64_t  ReadWrite:1;              // 0 = Read-Only, 1= Read/Write
    uint64_t  UserSupervisor:1;         // 0 = Supervisor, 1=User
    uint64_t  PageLevelWriteThrough:1;  // 0 = Write-Back caching, 1=Write-Through caching
    uint64_t  CacheDisabled:1;          // 0 = Cached, 1=Non-Cached
    uint64_t  Accessed:1;               // 0 = Not accessed, 1 = Accessed (set by CPU)
    uint64_t  Ignored:1;
    uint64_t  Reserved:1;               // Reserved
    uint64_t  MustBeZero:4;             // Must Be Zero
    uint64_t  PDPTBaseAddress:40;       // Page Table Base Address
    uint64_t  AvailableHigh:11;        // Available for use by system software
    uint64_t  Nx:1;                     // No Execute bit

}   pml4_entry;

typedef struct {
    uint64_t  Present:1;                // 0 = Not present in memory, 1 = Present in memory
    uint64_t  ReadWrite:1;              // 0 = Read-Only, 1= Read/Write
    uint64_t  UserSupervisor:1;         // 0 = Supervisor, 1=User
    uint64_t  PageLevelWriteThrough:1;  // 0 = Write-Back caching, 1=Write-Through caching
    uint64_t  CacheDisabled:1;          // 0 = Cached, 1=Non-Cached
    uint64_t  Accessed:1;               // 0 = Not accessed, 1 = Accessed (set by CPU)
    uint64_t  Dirty:1;              // Ignored
    uint64_t  MustBe1:1;
    uint64_t  Global:1;
    uint64_t  Ignored_1:3;
    uint64_t  PAT:1;
    uint64_t  Reserved:17;
    uint64_t  PageAddress:22;
    uint64_t  Ignored_2:11;             //Ignored
    uint64_t  Nx:1;                     // No Execute bit

}   pdpt_entry;

struct buddy {
	int level;
	uint8_t tree[1];
};


// typedef struct{
//     uint64_t Present:1;
//     uint64_t ReadWrite:1;
//     uint64_t UserSupervisor:1;
//     uint64_t PageLevelWriteThrough:1;
//     uint64_t CacheDisabled:1;
//     uint64_t Accessed:1;
//     uint64_t Dirty:1;
//     uint64_t MustBe1:1;
//     uint64_t Global:1;
//     uint64_t Ignored_1:3;
//     uint64_t Pat:1;
//     uint64_t Ignored_2:17;
//     uint64_t PageAddress:22;
//     uint64_t Ignored_3:11;
//     uint64_t Nx:1;
// }  page_entry;

/*
typedef struct {
    uint64_t offset:30;
    uint64_t directory_ptr:9;
    uint64_t pml4:9;
} address_struct;
*/

/*SCHEDULER  */
typedef struct {

    //These registers are used to restore context
    uint64_t gs;
    uint64_t fs;
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;

    //These registers are related with the iretq
    uint64_t rip;
    uint64_t cs;
    uint64_t eflags;
    uint64_t rsp;
    uint64_t ss;
    uint64_t base;

} stackFrame;



typedef enum{
    WAITING,
    BLOCKED,
    RUNNING,
}processStatus;

typedef struct{
    void * userStack;
	void * kernelStack;
	void * entryPoint;
    int PID;
    char * name;
    processStatus status;
}process;

typedef struct processNode processNode;

typedef struct processNode{
  process * currentProcess;
  processNode * next;
  processNode * previous;
  processNode * currentProcess;
}processNode;

typedef struct processQueue{
  processNode * first;
  processNode * last;
  int size;
}processQueue;

/*END OF SCHEDULER*/

typedef struct
{
    uint64_t    pe:1;   //Protection Enabled
    uint64_t    mp:1;   //Monitor co-procesor
    uint64_t    em:1;   //Emulation
    uint64_t    ts:1;   //Task switcher
    uint64_t    et:1;   //Extension type
    uint64_t    ne:1;   //Numeric error
    uint64_t    ig_bits_1:10;
    uint64_t    wp:1;     //Write protect
    uint64_t    ig_bits_2:1;
    uint64_t    am:1;   //Alignment mask
    uint64_t    ig_bits_3:10;
    uint64_t    nw:1;   //Not write trough
    uint64_t    cd:1;   //Cache Disabled
    uint64_t    pg:1;   //Enable pagging when sets
    uint64_t    ig_bits_4:32;
} CR0;

typedef struct
{
    uint64_t    Ignored_1:3;               // 2 bits ignored
    uint64_t    PageLevelWriteThrough:1; // indirectly determines the memory type used to access the PML4 table during linear-address translation
    uint64_t    CacheDisabled:1;         // indirectly determines the memory type used to access the PML4 table during linear-address translation
    uint64_t    Ignored_2:7;               // Bits Ignored
    uint64_t    PML4BaseAddress:40;      // Physical address of the 4-KByte aligned PML4 table used for linear-address translation
    uint64_t    MustBeZero:12;
} CR3;


/*END OF MEMORY */


typedef enum SYSTEM_CALLS {
        WRITE = 0x4,
        READ = 0x3,
        CLEAR_SCREEN = 0x2,
        SET_SOUND = 0x5,
        SET_TIMER_HANDLER = 0x6,
        BEEP = 0x7,
        CREATE_PROCESS = 0x8,
        DELETE_PROCESS = 0x9,
        LIST_PROCESSES = 0xA
} SYSCALLS;

#define RTC_PORT		0x70
#define RTC_REGISTER_PORT	0x71



#endif
