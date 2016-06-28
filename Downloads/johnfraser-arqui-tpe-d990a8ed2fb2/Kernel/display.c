#include "display.h"

Color backgroundColor = DEFAULT_BG_COLOR;
Color defaultTextColor = DEFAULT_TEXT_COLOR;

char *video = (char*) 0xB8000;

int screenWidth = SCREEN_WIDTH;
int screenHegiht = SCREEN_HEIGHT;

CursorStyle cursorStyle = CURSOR_BLOCK;

int cursorI = 0;
int cursorJ = 0;

int backspaceLimitI = 0;
int backspaceLimitJ = 0;


// Updates cursor on screen

void kupdateCursorOnScreen(unsigned int position){
        // LOW
        kout(0x3D4, 0x0F);
        kout(0x3D5, (unsigned char)(position&0x000000FF));
        // HIGH
        kout(0x3D4, 0x0E);
        kout(0x3D5, (unsigned char)((position>>8)&0x000000FF));
}

// Trigger ticking

// Initializes screen with other than default paramteters - PUBLIC
void kInitializeDisplay() {
        kclearScreen();
        ksetCursorStyle(CURSOR_BLOCK);
        kupdateCursorOnScreen(0);
}

// Get the character at position
int kgetCharAtPosition(int x, int y){
        return video[2*(y * screenWidth + x)];
}

// Get the color at position
int kgetColorAtPosition(int x, int y){
        return video[2*(y * screenWidth + x) + 1];
}

// Returns the default system color
int kgetDefaultColor(){

        char bgColor = (backgroundColor & 0xF) << 4;
        char textColor = defaultTextColor & 0xF;

        return bgColor | textColor;
}

// Sets the character c at the desired position in the display with the color chosen

void ksetDisplayWithColor(int x, int y, char c, int color){
        video[2*(y * screenWidth + x)] = c;
        video[2*(y * screenWidth + x) + 1] = color;
}

// Sets the character c at the desired position in the display with the default color
void ksetDisplay(int x, int y, char c){
        ksetDisplayWithColor(x,y,c,kgetDefaultColor());
}

// Invert colors

int kinvertColor(int color){
        int textColor = color & 0x0F;
        int backgroundColor = color >> 4;

        int invertedTextColor = (0xF - textColor);
        int invertedBackgroundColor = (0xF - backgroundColor);

        backgroundColor = (invertedBackgroundColor & 0xF) << 4;
        textColor = (invertedTextColor & 0xF);

        return backgroundColor | textColor;
}

// Move cursor
void kmoveCursor(int x, int y){
        cursorI = x;
        cursorJ = y;

        unsigned int position = cursorJ * screenWidth + cursorI;

        kupdateCursorOnScreen(position);
}

// Clears the row at the desired position
void kclearRow(int j){
        int i;
        for(i=0; i<screenWidth; i++) {
                ksetDisplay(i, j, ' ');
        }
}

// Empties the screen - PUBLIC
void kclearScreen(void){
        int i;
        for(i=0; i<screenHegiht; i++) {
                kclearRow(i);
        }
        kmoveCursor(0, 0);
        backspaceLimitI = 0;
        backspaceLimitJ = 0;
}

void kcopyRowToRow(int originalRow, int destinationRow){
        int i;
        for(i=0; i<screenWidth; i++) {
                ksetDisplayWithColor(i, destinationRow, kgetCharAtPosition(i, originalRow), kgetColorAtPosition(i, originalRow));
        }
}

// Scroll screen n lines to the bottom - PUBLIC
void kscrollDown(int n){
        if(n<1)
                return;

        int i;

        for(i = n; i<screenHegiht; i++) {
                kcopyRowToRow(i, i-n);
        }

        for(i = screenHegiht - n; i<screenHegiht; i++) {
                kclearRow(i);
        }

        kmoveCursor(0, cursorJ-n);
}

// Puts a new line - PUBLIC
void kputNewLine(){
        kmoveCursor(0, cursorJ+1);
}

// Move cursor to next position - PUBLIC
void kforwardCursor(){

        // Ultimo caracter de la línea
        if(cursorI == (screenWidth-1)) {

                kputNewLine();

                // Ultimo caracter de la pantalla visible
                if(cursorJ == (screenHegiht-1)) {
                        kscrollDown(1);
                }

        } else {
                kmoveCursor(cursorI+1, cursorJ);

                if(cursorJ == (screenHegiht-1)) {
                        kscrollDown(1);
                }
        }
}

// Move cursor to next position - PUBLIC
void kbackspaceCursor(){

        if(cursorI<=backspaceLimitI && cursorJ<=backspaceLimitJ)
                return;

        // Primer caracter de la linea
        if(cursorI == 0) {

                // No es la primera linea
                if(cursorJ > 0) {
                        kmoveCursor(screenWidth-1, cursorJ-1);

                        while ((cursorI > 0) && (kgetCharAtPosition(cursorI-1,cursorJ) == ' ')) {
                                kmoveCursor(cursorI-1,cursorJ);
                        }

                        ksetDisplay(cursorI, cursorJ, ' ');
                }

        } else {
                kmoveCursor(cursorI-1, cursorJ);
                ksetDisplay(cursorI, cursorJ, ' ');
        }
}

// Puts a character at the current cursor position and moves the cursor to the next position - PUBLIC
void kputChar(char c){
        if (c == '\n') {
                kputNewLine();
                kforwardCursor();
                kbackspaceCursor();
                ksetBackspaceLimit();
        } else if(c == '\t') {
                ksetDisplay(cursorI, cursorJ, ' ');
                kforwardCursor();
                ksetDisplay(cursorI, cursorJ, ' ');
                kforwardCursor();
                ksetDisplay(cursorI, cursorJ, ' ');
                kforwardCursor();
        } else if(c == '\b') {
                kbackspaceCursor();
        } else{
                ksetDisplay(cursorI, cursorJ, c);
                kforwardCursor();
        }
}

// Puts a string of characters on screen - PUBLIC
void kputString(char* str){

        int i = 0;
        int length = kstrlen(str);

        for(i = 0; i<length; i++) {
                kputChar(str[i]);
        }
}

// Gets the screen dimensions in number of lines
int kgetScreenHeight(){
        return screenHegiht;
}

int kgetScreenWidth(){
        return screenWidth;
}

// Configures global colors - PUBLIC
void ksetBackgroundColor(Color color){
        backgroundColor = color;
}

void ksetDefaultTextColor(Color color){
        defaultTextColor = color;
}

void ksetCursorStyle(CursorStyle style){
        kout(0x3D4, 0x0A);
        kout(0x3D5, (unsigned char)(style&0xFF));
}

void ksetBackspaceLimit(){
        backspaceLimitI = cursorI;
        backspaceLimitJ = cursorJ;
}
