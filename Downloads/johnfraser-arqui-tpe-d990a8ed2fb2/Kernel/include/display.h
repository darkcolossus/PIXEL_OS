#ifndef __PIXEL_OS__DISPLAY__
#define __PIXEL_OS__DISPLAY__

#include "defines.h"
#include "lib.h"

// Optional initializer
void kInitializeDisplay();

// Erase content
void kclearScreen(void);

// Handle scrolling
void kscrollDown(int n);

// Handle text cursor
void kputNewLine();
void kforwardCursor();
void ksetBackspaceLimit();
void kbackspaceCursor();
void kcursorTick();

// Insert text
void kputChar(char c);
void kputString(char* str);

// Move cursor
void kmoveCursor(int x, int y);

// Screen height and width in number of lines
int kgetScreenHeight();
int kgetScreenWidth();

// Theming
void ksetBackgroundColor(Color color);
void ksetDefaultTextColor(Color color);
void ksetCursorStyle(CursorStyle style);

// Screensaver
void kBackupScreen();
void kRestoreScreen();

#endif
