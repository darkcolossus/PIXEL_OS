#include <stdint.h>
#include <stddef.h>
#include "include/lib.h"
#include "include/defines.h"
#include "include/sound.h"

void initCommandList();
void executeCommand(char* buffer);
int commandSelector(char* command);
void shellLine();


int pidtester();

int echo(char* args);
int clear(char* args);
int help(char* args);
int hello(char* args);
int version(char* args);
int beep(char* args);
int createProcess();
int deleteProcess(char * args);
int listProcesses();
int ipc();
int game();
int testter2();

extern void _beep();


CommandDescriptor commands[TOTAL_COMMANDS];

#define SHELL_STR_LENGTH 15

/**********************
**  Text Management  **
**********************/

#define SHELL_TEXT_MAX 40 - SHELL_STR_LENGTH	// check this value with (display length - SHELL_STRING length)

/***********
**  Main  **
***********/
static int hola = 1;
int terminal() {

	char buffer[200];
	initCommandList();
	clearScreen();
	while(1) {
		if(hola){
			shellLine();
			read(buffer,'\n');
			executeCommand(buffer);
		}
	}
	return 0;
}

void initCommandList(){
	commands[0].key = "echo";
	commands[0].handler = &echo;
	commands[0].use = "Repeats text following command.";
	commands[0].hidden = FALSE;

	commands[1].key = "clear";
	commands[1].handler = &clear;
	commands[1].use = "Cleans the terminal screen.";
	commands[1].hidden = FALSE;

	commands[2].key = "help";
	commands[2].handler = &help;
	commands[2].use = "Displays list of commands.";
	commands[2].hidden = FALSE;

	commands[3].key = "hello";
	commands[3].handler = &hello;
	commands[3].use = "Easter Egg";
	commands[3].hidden = FALSE;

  	commands[4].key = "version";
  	commands[4].handler = &version;
  	commands[4].use = "System version.";
  	commands[4].hidden = FALSE;

	commands[5].key = "beep";
	commands[5].handler = &beep;
	commands[5].use = "beep sound.";
	commands[5].hidden = FALSE;

	commands[6].key = "play_song";
	commands[6].handler = &startSong;
	commands[6].use = "Executes a beautiful song. Select by adding a number (for example, \"play_song 2\")";
	commands[6].hidden = FALSE;

	commands[7].key = "stop_song";
	commands[7].handler = &stopSong;
	commands[7].use = "Stops a song before it finishes on its own.";
	commands[7].hidden = FALSE;

	commands[8].key = "create_process";
	commands[8].handler = &createProcess;
	commands[8].use = "Create a process.";
	commands[8].hidden = FALSE;

	commands[9].key = "delete_process";
	commands[9].handler = &deleteProcess;
	commands[9].use = "Delete a process, specified by process id.";
	commands[9].hidden = FALSE;

	commands[10].key = "ps";
	commands[10].handler = &listProcesses;
	commands[10].use = "Show the list of processes.";
	commands[10].hidden = FALSE;

	commands[11].key = "ipc";
	commands[11].handler = &ipc;
	commands[11].use = "Show the ipc structure.";
	commands[11].hidden = FALSE;

	commands[12].key = "game";
	commands[12].handler = &game;
	commands[12].use = "Play this funny game.";
	commands[12].hidden = FALSE;


}

void shellLine(){
  printf("user@pixel_os:> ");
}

void executeCommand(char* buffer){

	char command[MAX_COMMAND_LENGTH];
	char arguments[MAX_ARGUMENTS_LENGTH];

	if(strlen(buffer) == 0){
		return;
	}

	split(buffer, command, arguments);

	int commandId = commandSelector(command);

	if(commandId == -1){
		printf("%s is not a valid command.\n", command);
	} else {
		commands[commandId].handler(arguments);
	}
}

int commandSelector(char* command){

	for(int i=0; i<TOTAL_COMMANDS;i++){
		if(strEquals(command, commands[i].key)){
			return i;
		}
	}
	return -1;
}

/***************
**  Commands  **
***************/

int echo(char* args){
	if(strlen(args) == 0)
		printf("\n");
	else
		printf("%s\n",args);
	return 0;
}

int clear(char* args){
	clearScreen();
	return 1;
}

int hello(char* args){
  printf("__________________________________________________\n");
  printf("< hello user, I'm a cow and you're using PIXEL_OS>\n");
  printf("--------------------------------------------------\n");
  printf("        \\   ^__^\n");
  printf("         \\  (oo)\\_______\n");
  printf("            (__)\\       )\\/\\\n");
  printf("                  ||----w |\n");
  printf("                  ||     ||\n");

  return 0;
}

int version(char* args){
	printf("PIXEL_OS v.0.2\n");
	return 0;
}

int help(char* args){
	for(int i=0; i<TOTAL_COMMANDS;i++){
		if(commands[i].hidden == FALSE){
			printf("- %s: %s\n",commands[i].key, commands[i].use);
		}
	}
	return 0;
}

int beep(char* args){
	char R=177;
	char G=222;
	char B=111;
	int color=0;
	color+=R;
	color*=1000;
	color+=G;
	color*=1000;
	color+=B;
	runSyscall(DRAW_RECT,0,0,color);
	

	//runSyscall(BEEP,0x0,0x0,0x0);
	return 0;
}

int createProcess(){
	int a= 	runSyscall(CREATE_PROCESS,0x0,0x0,0x0);
	printf("El proceso se ha creado con exito con PID %d \n",a);

	return 0;
}

int deleteProcess(char* args){
	runSyscall(DELETE_PROCESS,0x0,0x0,atoi(args));
	printf("El proceso ha sido eliminado con exito\n");

	return 0;
}

int listProcesses(){
	int a=0;
	runSyscall(LIST_PROCESSES,0x0,0x0,0x0);
	

	//printf("Listando procesos...\n");
	return 0;
}

int ipc(){
	printf("La estructura IPC implementada es: MessageQueue\n");
	printf("Las estructuras utilizadas son:\n");
	printf("===========================\n");
	printf("|| message{              ||\n");
	printf("||    void * msg;        ||\n");
	printf("||    uint64_t msgSize;  ||\n");
	printf("||    uint64_t senderId; ||\n");
	printf("||    message * next;    ||\n");
	printf("||  }                    ||\n");
	printf("===========================\n");
	printf("============================\n");
	printf("|| msgQueueNode           ||\n");
	printf("|| {                      ||\n");
	printf("||    uint64_t id;        ||\n");
	printf("||    message * msgs;     ||\n");
	printf("||    message * lastmsg;  ||\n");
	printf("||    msgQueueNode * next;||\n");
	printf("||  }                     ||\n");
	printf("============================\n");
	printf("=============================\n");
	printf("|| messageQueue            ||\n");
	printf("|| {                       ||\n");
	printf("||    msgQueueNode * first;||\n");
	printf("||    uint64_t size;       ||\n");
	printf("||  }                      ||\n");
	printf("=============================\n");
	return 0;
}

int game(){
	printf("Listo para jugar?\n");
	
	//runSyscall(CREATE_PROCESS,(uint64_t) gameS, "Piano Tiles", 0x0);
	//runSyscall(CREATE_PROCESS,(uint64_t) gamesound, "Sound", 0x0);
	hola=0;
	
	int gameprocess= runSyscall(CREATE_PROCESS,(uint64_t)gameS,"PianoTiles",0x0);
	int inputprocess = runSyscall(CREATE_PROCESS,(uint64_t)game_input,"GameInputProc",0x0);
	//runSyscall(CREATE_PROCESS,(uint64_t)gamesound,"GameInputProc",0x0);
	printf("SE CREO GAME E INPUT CON PIDS : %d y %d  \n", gameprocess,inputprocess);

	runSyscall(MQSEND,gameprocess,0x1,inputprocess);
	runSyscall(MQSEND,inputprocess,0x1,gameprocess);
	/*int proca =	runSyscall(CREATE_PROCESS,(uint64_t) pidtester, "tester", 0x0);
	int procb = runSyscall(CREATE_PROCESS,(int64_t)testter2, "testter2",0x0);
	runSyscall(MQSEND, proca,0x1,procb);
	runSyscall(MQSEND, procb,0x1,proca);
		runSyscall(MQSEND, proca,0x1,procb);
	runSyscall(MQSEND, procb,0x1,proca);
		runSyscall(MQSEND, proca,0x1,procb);
	runSyscall(MQSEND, procb,0x1,proca);
		runSyscall(MQSEND, proca,0x1,procb);
	runSyscall(MQSEND, procb,0x1,proca);
		runSyscall(MQSEND, proca,0x1,procb);
	runSyscall(MQSEND, procb,0x1,proca);
		runSyscall(MQSEND, proca,0x1,procb);
	runSyscall(MQSEND, procb,0x1,proca);
	*/
	//runSyscall(CREATE_PROCESS,(uint64_t) ipc, 0x0, 0x0);
	//runSyscall(CREATE_PROCESS,(uint64_t) ipc, 0x0, 0x0);
	//gameS();
	return 0;
}


int pidtester()
{	
	int pid = runSyscall(PID,0x0,0x0,0x0);
	int a=1;
	while (a){
		int c;
		c=(int)runSyscall(MQREAD,pid,0x1,0x0);
		if(c!=0){
			printf("Soy el proceso %d y me mandaron el pid del proceso %d \n",pid,c);
		}else{
			a=0;
		}
	}
	return 0;
}

int testter2()
{
	
	int pid = runSyscall(PID,0x0,0x0,0x0);
	
	while (1){
		int c;
		c=(int)runSyscall(MQREAD,pid,0x1,0x0);
		if(c!=0){
			//printf("Soy el proceso %d y me mandaron el pid del proceso %d \n",pid,c);
		}else{
			
		}
	}
	return 0;
}

