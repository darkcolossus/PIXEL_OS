#include <stdint.h>
#define VBE_DISPI_INDEX_ID 0
#define VBE_DISPI_INDEX_XRES 1
#define VBE_DISPI_INDEX_YRES 2
#define VBE_DISPI_INDEX_BPP 3
#define VBE_DISPI_INDEX_ENABLE 4
#define VBE_DISPI_INDEX_BANK 5
#define VBE_DISPI_INDEX_VIRT_WIDTH 6
#define VBE_DISPI_INDEX_VIRT_HEIGHT 7
#define VBE_DISPI_INDEX_X_OFFSET 8
#define VBE_DISPI_INDEX_Y_OFFSET 9

#define VBE_DISPI_IOPORT_INDEX 0x01CE
#define VBE_DISPI_IOPORT_DATA 0x01CF

//Clearing Display Memory LFB parameter
#define VBE_DISPI_NOCLEARMEM 0x80
//#define VBE_DISPI_ENABLED 0x81

#define VBE_DISPI_BPP_4 0x04
#define VBE_DISPI_BPP_8 0x08
#define VBE_DISPI_BPP_15 0x0F
#define VBE_DISPI_BPP_16 0x10
#define VBE_DISPI_BPP_24 0x18
#define VBE_DISPI_BPP_32 0x20

#define VBE_DISPI_ID0 0xB0C0
#define VBE_DISPI_ID1 0xB0C1
#define VBE_DISPI_ID2 0xB0C2
#define VBE_DISPI_ID3 0xB0C3
#define VBE_DISPI_ID4 0xB0C4
#define VBE_DISPI_ID5 0xB0C5
 
#define VBE_DISPI_DISABLED 0x00
#define VBE_DISPI_ENABLED 0x01

#define VBE_DISPI_LFB_ENABLED 0x40


#define VIDEO_ADDRESS_START 0xFD000000
#define RESOLUTION 2359296
#define VIDEO_RESOLUTION 786432
#define END_OF_SCREEN ((char*)0xFD000000) + 2359296
#define WIDTH 1024
#define HEIGHT 768
#define BPP_32_FACTOR 4
#define DATA_MODULE_ADDRESS 0x500000


void outpw(uint16_t port, uint16_t value);
uint16_t inpw(uint16_t port);
void _video();
void draw_rectangle(int x, int y, int xSize, int ySize, char R, char G, char B);
void put_pixel(int x, int y, char R, char G, char B);
void loadScreen();
void draw_solid_circle(int x, int y, int radix, char R, char G, char B);
void draw_hollow_circle(int x, int y, int radix, char R, char G, char B);
int sqr(int num);


void BgaWriteRegister(unsigned short IndexValue, unsigned short DataValue)
{
    outpw(VBE_DISPI_IOPORT_INDEX, IndexValue);
    outpw(VBE_DISPI_IOPORT_DATA, DataValue);
}
 
unsigned short BgaReadRegister(unsigned short IndexValue)
{
    outpw(VBE_DISPI_IOPORT_INDEX, IndexValue);
    return inpw(VBE_DISPI_IOPORT_DATA);
}
 
int BgaIsAvailable(void)
{
    return (BgaReadRegister(VBE_DISPI_INDEX_ID) == VBE_DISPI_ID4);
}
 
void BgaSetVideoMode(unsigned int Width, unsigned int Height, 
	unsigned int BitDepth, int UseLinearFrameBuffer, int ClearVideoMemory)
{
    BgaWriteRegister(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);
    BgaWriteRegister(VBE_DISPI_INDEX_XRES, Width);
    BgaWriteRegister(VBE_DISPI_INDEX_YRES, Height);
    BgaWriteRegister(VBE_DISPI_INDEX_BPP, BitDepth);
    BgaWriteRegister(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_ENABLED |
        (UseLinearFrameBuffer ? VBE_DISPI_LFB_ENABLED : 0) |
        (ClearVideoMemory ? 0 : VBE_DISPI_NOCLEARMEM));
}
 
void BgaSetBank(unsigned short BankNumber)
{
    BgaWriteRegister(VBE_DISPI_INDEX_BANK, BankNumber);
}

void _video(){
	_beep();
	BgaSetVideoMode(1024, 768, VBE_DISPI_BPP_32,  1,  1);
}

void draw_rectangle(int x, int y, int xSize, int ySize, char R, char G, char B){
        

        

        for(int i=(int) (xSize / 2)*(-1);i<(int)(xSize/2);i++){
                for(int j=(int) (ySize / 2)*(-1);j<(int)(ySize/2);j++){
                        put_pixel(x+i,y+j,R,G,B);                   
                }
        }

}
void put_pixel(int x, int y, char R, char G, char B){
        uint8_t * screen = (uint8_t * ) VIDEO_ADDRESS_START;
        int offset = (y * WIDTH + x) *BPP_32_FACTOR;


        screen+=offset;
        *screen = B;
        screen++;
        *screen=G;
        screen++;
        *screen=R;
        /*screen[3*(x+1024*y)]=B;
        screen[3*(x+1024*y)+1]=G;
        screen[3*(x+1024*y)+2]=R;*/
}

void loadScreen(){

 
}


void draw_solid_circle(int x, int y, int radix, char R, char G, char B){
        int dist_x;
        for(int height = radix; height >= 0; height--){
                dist_x = sqr(radix * radix - height * height);
                for(int cur_x = -dist_x; cur_x <= dist_x; cur_x++){
                        put_pixel(cur_x + x,y+ height, B,G,R);
                        put_pixel(cur_x+x,y -height, B,G,R);
                }
        }
}

void draw_hollow_circle(int x, int y, int radix, char R, char G, char B){
        int dist_x;
        for(int height = radix; height >= 0; height--){
                dist_x = sqr(radix * radix - height * height);
                put_pixel(dist_x+x,y+ height, R, G, B);
                put_pixel(dist_x+x,y -height, R, G, B);
        }
}


//radix * radix = dist_x * dist_x + dist_y * dist_y
//dist_x = sqr( radix * radix - dist_y * dist_y )

int sqr(int num){
        int i;
        for(i = 0; i * i < num; i++){
        }
        return i;
}
void pixel_wrap(int x, int c, int color){
   
    char B = (char) (color%1000);
    color/=1000;
    char G = (char) (color%1000);
    color/=1000;
    char R = (char)color;


    put_pixel(x,c,B,G,R);

}