#include <stdint.h>
#include "include/lib.h"
#include "include/defines.h"
#include "include/libasm.h"
#include "include/sound.h"
void initializeNotes(int*tiles);
void startGame();
int videoAct();
int sys_pixel(int x, int y, char R, char G, char B);
char getCharC();
int parseInput(int* notes, char input);
void moveTiles(int* tiles, int speed);
void draw_rectangle(int x, int y, int xSize, int ySize, char R, char G, char B);
void put_pixel(int x, int y, char R, char G, char B);
void loadScreen();
void drawCircles();
void draw_solid_circle(int x, int y, int radix, char R, char G, char B);
void clearLastTiles(int * notes);
void draw_hollow_circle(int x, int y, int radix, char R, char G, char B);
void clearSingleTile(int height, int col);
int sqr(int num);
void clearTiles(int * notes, int speed);
void updateNotes(int* tiles);
int INITIAL_SPEED= 4;
static int pid;
static int inputpid;
static int sound =0;
int gameS (){
	pid =runSyscall(PID,0x0,0x0,0x0);
	inputpid=runSyscall(MQREAD,pid,0x1,0x0);
	printf("EN GAME LOS PROCESOS DE JUEGO E INPUT SON :%d ---- %d\n",pid,inputpid);
	char input;
	showMenu();
	input=(char)runSyscall(MQREAD,pid,inputpid,0x0);
	input=0;

	/*while(1){
		input=(char)runSyscall(MQREAD,pid,inputpid,0x0);
		if(input!=0){
			
			printf("----------%c----------\n",input);
		}
	}*/
	do {
		while((input=(char)runSyscall(MQREAD,pid,inputpid,0x0))==0){

		}
		
		//runSyscall(BEEP,0x0,0x0,0x0);
		printf("Choose an option\n");
		//input = getChar();
   			
	} while(input!='\n' && input!='q' && input!='Q' );

	if (input == '\n'){

		startGame();
	}else{
		return 1;
	}
	while(1){
	if ((input=(char)runSyscall(MQREAD,pid,inputpid,0x0))=='\n')
		//runSyscall(CREATE_PROCESS,(uint64_t)gamesound,"GameInputProc",0x0);
		startGame();

	}

}

int videoAct(){
	runSyscall(VIDEO,553321,0x0,0x0);
	return 0;
}

int sys_pixel(int x, int y, char R, char G, char B){
	int color=0;
	color+=R;
	color*=1000;
	color+=G;
	color*=1000;
	color+=B;
	runSyscall(PIXEL,x,y,color);
}

char getCharC(){
  char c;
  runSyscall(READ,0x0,&c,0x0);
  return c;
}

void startGame(){
	videoAct();
	
	
	if (sound==0){
		sound=1;
		runSyscall(CREATE_PROCESS,(uint64_t)gamesound,"GameInputProc",0x0);
	
	}
	

	

  	int speedinc=0;
	int speed= INITIAL_SPEED;
	int interval;
	unsigned long cumulative=0;
	unsigned long cum=0;
	int time=0;
	unsigned long MAX_TIME= 0;
	int playing =1;
	int notes[16] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
	int rdnm = 12;
	int frameRate=120024;
//	getSequence(&notes,6);
	getSequence(notes,0);
	char input;
	drawCircles();
	while(playing){
		input = 0;

		if(cumulative<MAX_TIME){
			//runSyscall(BEEP,0x0,0x0,0x0);

			char input = (char)runSyscall(MQREAD,pid,inputpid,0x0);
			// if(aux!=0){
			// 	input = aux;
			// }
			playing = parseInput(notes,input);
			//rdnm += input;
	
			//rdnm += speed;
			rdnm = rdnm % 16;
			cumulative++;
			cum++;
		}else{

			char input = (char)runSyscall(MQREAD,pid,inputpid,0x0);
			playing = parseInput(notes,input);
			rdnm = rdnm % 16;
			cumulative++;
			cum++;


			if(availability(notes)){
				speed++;
				updateNotes(notes);
				getSequence(notes,(int)(cum%16));
				}
				if(playing){
					playing = checkEnd(notes);
				}
				clearTiles(notes,speed);
				moveTiles(notes,speed);
				drawTiles(notes);
				drawCircles();
			
		}
		

		
	}
	draw_rectangle(400, 400, 800, 800,0, 0,0);
	draw_rectangle(800, 400, 800, 800,0, 0,0);
	draw_rectangle(400, 600, 800, 800,0, 0,0);
	draw_rectangle(800, 600, 800, 800,0, 0,0);
}

void drawCircles(){
	draw_solid_circle(128, 700, 25, 255, 255,0);
    draw_solid_circle(384, 700, 25, 255, 255,0);

    draw_solid_circle(630, 700, 25, 255, 255,0);
    draw_solid_circle(886, 700, 25, 255, 255,0);
}

void clearLastTiles(int * notes){
	int x=128;
	for(int i=12;i<16;i++){
		x=128;
		if(notes[i]!=-1){
			x+= (i%4 * 256);
			draw_rectangle(x, notes[i], 100, 195,0,0,0 );
		}
	}
	drawCircles();
}

void clearTiles(int * notes, int speed){
	int x=128;
	for(int i=0;i<16;i++){
		x=128;
		if(notes[i]!=-1){
			x+= (i%4 * 256);
			draw_rectangle(x, notes[i]-88, 100, speed+2,0,0,0 );
		}
	}
}

void clearSingleTile(int height, int col){
	int x= 128 +256*col;
	draw_rectangle(x,height,100,176,0,0,0);
}

void drawTiles(int* notes){
	int x=0;
	for(int i=0;i<16;i++){
		x=128;
		if(notes[i]!=-1){
			x+= (i%4 * 256);
			draw_rectangle(x, notes[i], 100, 176,255,255,255 );
		}
	}
}



int availability(int* notes){
	
	for(int i=12;i<16;i++){
		if(notes[i] != -1){
			return 0;
		}
	}
	for(int j=0;j<4;j++){
		if(notes [j]!= -1 && notes[j] < 280){

			
		
			return 0;
		}
	}
	
	return 1  ;
}

int checkEnd(int* notes){
	
	for(int i=12;i<16;i++){
		if(notes[i]!= -1 && notes[i]-88> 725){
			for(int i=12;i<16;i++){
				clearLastTiles(notes);
				notes[i]=-1;
			}
			return 0;
		}
	}
	return 1;
}

int parseInput(int* notes, char input){
	if (notes[13]==-1){
	//	runSyscall(BEEP,0x0,0x0,0x0);
	}
	if(input == 0){
		return 1;
	}




	if(input=='c'||input=='C'){
		runSyscall(MQSEND,5,5,1);
		draw_solid_circle(128, 700, 25, 255, 126,126);
		if ((notes[12]!=-1) && ((notes[12]-88 <675) && (notes[12]+88> 675)) || ((notes[12]-88<725) && (notes[12]+88 >725)))
		{
			clearSingleTile(notes[12],0);
			//runSyscall(BEEP,0x0,0x0,0x0);
			notes[12]=-1;
			return 1;
		}
		else{
			if ((notes[8]!=-1) && ((notes[8]-88 <675) && (notes[8]+88> 675)) || ((notes[8]-88<725 )&& (notes[8]+88 >725)))
			{
				clearSingleTile(notes[8],0);
			//	runSyscall(BEEP,0x0,0x0,0x0);
				notes[8] = -1;
			//	runSyscall(BEEP,0x0,0x0,0x0);
				return 1;
			}
			else{
			return 0;
			}
		}	
		
	}else if(input=='v'||input=='V'){
		runSyscall(MQSEND,5,5,1);
		draw_solid_circle(384, 700, 25, 255, 126,126);
		if ((notes[13]!=-1) && ((notes[13]-88 <675) && (notes[13]+88> 675)) || ((notes[13]-88<725 )&&( notes[13]+88 >725)))
		{
			clearSingleTile(notes[13],1);
			//runSyscall(BEEP,0x0,0x0,0x0);
			notes[13] = -1;
			//runSyscall(BEEP,0x0,0x0,0x0);
			return 1;
		}
		else{
			if ((notes[9]!=-1) && ((notes[9]-88 <675) && (notes[9]+88> 675)) || ((notes[9]-88<725) && (notes[9]+88 >725)))
			{
				clearSingleTile(notes[9],1);
			//	runSyscall(BEEP,0x0,0x0,0x0);
				notes[9] = -1;
			//	runSyscall(BEEP,0x0,0x0,0x0);
				return 1;
			}
			else{
			return 0 ;
			}
		}
	} else if(input=='b'||input=='B'){
		runSyscall(MQSEND,5,5,1);
		draw_solid_circle(630, 700, 25, 255, 126,126);
		if ((notes[14]!=1) && ((notes[14]-88 <675) && (notes[14]+88> 675)) || ((notes[14]-88<725) && (notes[14]+88 >725)))
		{
			clearSingleTile(notes[14],2);
			//runSyscall(BEEP,0x0,0x0,0x0);
			notes[14]=-1;
			return 1;
		}
		else{
			if ((notes[10]!=-1) && ((notes[10]-88) <675 && (notes[10]+88> 675)) || ((notes[10]-88<725) && (notes[10]+88 >725)))
			{
				clearSingleTile(notes[10],2);
				//runSyscall(BEEP,0x0,0x0,0x0);
				notes[10] = -1;
				//runSyscall(BEEP,0x0,0x0,0x0);
				return 1;
			}
			else{
			return 0;
			}
		}
	}else if(input=='n'||input=='N'){
		runSyscall(MQSEND,5,5,1);
		draw_solid_circle(886, 700, 25, 255, 126,126);
		if (notes[15]!=-1 && (notes[15]-88 <675 && notes[15]+88> 675) || (notes[15]-88<725 && notes[15]+88 >725))
		{
			clearSingleTile(notes[15],3);
			//runSyscall(BEEP,0x0,0x0,0x0);
			notes[15]=-1;
			return 1;
		}
		
		else{
			if (notes[11]!=-1 && (notes[11]-88 <675 && notes[11]+88> 675) || (notes[11]-88<725 && notes[11]+88 >725))
			{
				clearSingleTile(notes[11],3);
			//	runSyscall(BEEP,0x0,0x0,0x0);
				notes[11] = -1;
			//	runSyscall(BEEP,0x0,0x0,0x0);
				return 1;
			}
			else{
			return 0;
			}
		}
	}
	return 1;
}




void moveTiles(int* tiles, int speed){


	for(int i =0;i<16;i++){
		if(tiles[i]!=-1){	
			tiles[i]=tiles[i]+speed;
			}
		}
	

	
}



void initializeNotes(int* tiles){
	
	for (int i=1;i<16;i++){
		tiles[i]=-1;
	}
}




void updateNotes(int* tiles){
	tiles[15]=tiles[11];
	tiles[14]=tiles[10];
	tiles[13]=tiles[9];
	tiles[12]=tiles[8];
	
	tiles[11]=tiles[7];
	tiles[10]=tiles[6];
	tiles[9]=tiles[5];
	tiles[8]=tiles[4];
	
	tiles[7]=tiles[3];
	tiles[6]=tiles[2];
	tiles[5]=tiles[1];
	tiles[4]=tiles[0];
	
	for (int i=0;i<4;i++){
		tiles[i]=-1;
	}


}


void getSequence(int* tiles, int time){
	switch (time%16){
		case 0:
			tiles[0]=90;
			tiles[1]=90;
			tiles[2]=90;
			tiles[3]=90;
			break;
		case 1:
			tiles[0]=90;
			tiles[1]=90;
			tiles[2]=90;
			tiles[3]=-1;
			break;
		case 2:
			tiles[0]=90;
			tiles[1]=90;
			tiles[2]=-1;
			tiles[3]=90;
			break;
		case 3:
			tiles[0]=90;
			tiles[1]=-1;
			tiles[2]=90;
			tiles[3]=90;
			break;
		case 4:
			tiles[0]=-1;
			tiles[1]=90;
			tiles[2]=90;
			tiles[3]=90;
			break;
		case 5:
			tiles[0]=-1;
			tiles[1]=-1;
			tiles[2]=90;
			tiles[3]=90;
			break;
		case 6:
			tiles[0]=-1;
			tiles[1]=90;
			tiles[2]=90;
			tiles[3]=-1;
			break;
		case 7:
			tiles[0]=90;
			tiles[1]=-1;
			tiles[2]=-1;
			tiles[3]=90;
			break;
		case 8:
			tiles[0]=90;
			tiles[1]=90;
			tiles[2]=-1;
			tiles[3]=-1;
			break;
		case 9:
			tiles[0]=-1;
			tiles[1]=90;
			tiles[2]=90;
			tiles[3]=-1;
			break;
		case 10:
			tiles[0]=-1;
			tiles[1]=-1;
			tiles[2]=-1;
			tiles[3]=90;
			break;
		case 11:
			tiles[0]=-1;
			tiles[1]=90;
			tiles[2]=-1;
			tiles[3]=90;
			break;
		case 12:
			tiles[0]=-1;
			tiles[1]=-1;
			tiles[2]=90;
			tiles[3]=-1;
			break;
		case 13:
			tiles[0]=-1;
			tiles[1]=90;
			tiles[2]=-1;
			tiles[3]=-1;
			break;
		
		case 14:
			tiles[0]=90;
			tiles[1]=-1;
			tiles[2]=-1;
			tiles[3]=-1;
			break;
		
		case 15:
			tiles[0]=-1;
			tiles[1]=-1;
			tiles[2]=-1;
			tiles[3]=-1;
			break;
			
	}

	return;

}


void showMenu(){
	clearScreen();
	printf("\n\n\n\n\n");
	printf("P I A N O     T I L E S\n");
	printf("\n");
	printf("you'll need to click on every tile before they go past the circles\n");
	printf("\n\n\n");
	printf("Press ENTER when ready to play\n");
	printf("\n\\n\n");
	printf("Use the C V B N keys to control each one of the four circles\n");
	printf("\n\n\n");

	return;
}



void draw_rectangle(int x, int y, int xSize, int ySize, char R, char G, char B){
        /*for(int i=(int) (xSize / 2)*(-1);i<(int)(xSize/2);i++){
                for(int j=(int) (ySize / 2)*(-1);j<(int)(ySize/2);j++){
                        sys_pixel(x+i,y+j,R,G,B);                   
                }
        }*/


	int color=0;
	color+=R;
	color*=1000;
	color+=G;
	color*=1000;
	color+=B;

	int pos=0;
	pos+=y;
	pos*=1000;
	pos+=x;


	int size=0;
	size+=ySize;
	size*=1000;
	size+=xSize;
	runSyscall(DRAW_RECT,pos,size,color);
}
void draw_solid_circle(int x, int y, int radix, char R, char G, char B){
        /*int dist_x;
        for(int height = radix; height >= 0; height--){
                dist_x = sqr(radix * radix - height * height);
                for(int cur_x = -dist_x; cur_x <= dist_x; cur_x++){
                        sys_pixel(cur_x + x,y+ height, R,G,B);
                        sys_pixel(cur_x+x,y -height, R,G,B);
                }
        }*/

       int color=0;
	color+=R;
	color*=1000;
	color+=G;
	color*=1000;
	color+=B;

	int pos=0;
	pos+=y;
	pos*=1000;
	pos+=x;


	
	runSyscall(DRAW_CIRC,pos,radix,color);
}

void draw_hollow_circle(int x, int y, int radix, char R, char G, char B){
        int dist_x;
        for(int height = radix; height >= 0; height--){
                dist_x = sqr(radix * radix - height * height);
                sys_pixel(dist_x+x,y+ height, R, G, B);
                sys_pixel(dist_x+x,y -height, R, G, B);
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