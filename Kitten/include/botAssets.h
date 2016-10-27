#ifndef BOTASSETS
#define BOTASSETS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linkedList.h"

/*typedef struct responsesAndMessagesForBot botMsg;
typedef struct listRoot root;

struct listRoot{
	int size;

	botMsg * head;
	botMsg * tail;

};

struct botMsg{
	int interval;
	char * listenFor[10];
	char * response[10];
	char * repeat[10];
	char * target;
	
	botMsg * next;
	botMsg * prev;
};*/

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
	root * list;
	
} Bot;

Bot * createBot(int, char**);
int checkBotState(Bot*, char*);

#endif
