#include <stdint.h>
#include "include/lib.h"
#include "include/defines.h"
#include "include/sound.h"

void initCommandList();
void executeCommand(char* buffer);
int commandSelector(char* command);
void shellLine();

int echo(char* args);
int clear(char* args);
int help(char* args);
int hello(char* args);
int version(char* args);
int beep(char* args); 

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

int terminal() {

	char buffer[200];
	initCommandList();
	clearScreen();
	while(1) {

			shellLine();
			read(buffer,'\n');
			executeCommand(buffer);
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
	runSyscall(BEEP,0x0,0x0,0x0);
	return 0;
}