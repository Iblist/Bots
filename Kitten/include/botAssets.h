#ifndef BOTASSETS
#define BOTASSETS

#include <stdlib.h>
#include <stdio.h>

/*Contains important bot stuff*/
typedef struct SettingsForBot{
	char * nickResp;
	char * userResp;
	char * joinMsg;
	char * quitMsg;
	char * passResp;
	char * pongResp;
	int recordFlag;
	int inServer;
	int inChannel;
	
} Bot;

Bot * createBot(int, char**);
int checkBotState(Bot*, char*);

#endif
