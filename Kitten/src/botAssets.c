#include "botAssets.h"

/*Function: createBot
  Expects: int, char**
  Returns: Bot*

  A Bot is a struct I created. It holds settings for the bot during execution,
  such as the bots nickname, real name, and messages it is expected to deliver.

  This function will attempt to malloc memory for a Bot, and various strings 
  contained within. On success it will return a Bot*. On failure, it will return
  NULL
*/
Bot * createBot(int argc, char * argv[])
{
	int i;
	Bot * tempBot;
	//char * input;
	//FILE * filePtr;
	char ** responses;

	tempBot = malloc(sizeof(Bot));

	if (tempBot != NULL)
	{
		tempBot->inChannel = 0;
		tempBot->inServer = 0;
		tempBot->quitMsg = strdup("QUIT :Tah Tah for now \r\n");
		tempBot->passResp = strdup("PASS herpDerp\r\n");
		tempBot->list = NULL;

		tempBot->joinMsg = malloc(sizeof(char) * (strlen("JOIN \r\n") + strlen(argv[3])));
		sprintf(tempBot->joinMsg, "JOIN %s\r\n", argv[3]);
		tempBot->channelName = strndup(argv[3], 64);

		for (i = 4; i < argc; i++)
		{
			if (strncmp(argv[i], "--record", 8) == 0) tempBot->recordFlag = 1; //At this time, the record flag should always be set
			if (strncmp(argv[i], "--nick", 6) == 0)
			{
				if (i + 1 > argc)
				{
					fprintf(stderr, "Not enough arguments for --nick\n");
					exit(1);
				}
				else
				{
					tempBot->nickResp = malloc(sizeof(char) * (strlen("NICK \r\n") + strlen(argv[i+1])));
					sprintf(tempBot->nickResp, "NICK %s\r\n", argv[i+1]);
				}
			}
			if (strncmp(argv[i], "--name", 6) ==0)
			{
				if (i+1 > argc)
				{
					fprintf(stderr, "Not enough arguments for --name\n");
					exit(1);
				}
				else
				{
					tempBot->userResp = malloc(sizeof(char) * (strlen("USER  0 0 :\r\n") + strlen(argv[i+1])*2));
					sprintf(tempBot->userResp, "USER %s 0 0 :%s\r\n", argv[i+1], argv[i+1]);
				}
			}
			if (strncmp(argv[i], "--responses", 11) == 0)
			{
				/*if (i+1 > argc)
				{
					fprintf(stderr, "Not enough arguments for --responses\n");
					exit(1);
				}
				else
				{
					responses = readResponses();
				}*/
				responses = readResponses();
				tempBot->list = addResponses(responses);
			}
		}
	}
	return tempBot;
}

/*Function: checkBotState
  Expects: Bot* and char*
  Returns: int,
  0 = Bot State unchanged
  1 = Bot State changed

  This function checks input recieved from the server and if certain
  keywords are found, changes the state of the bot. If the function
  finds "/MOTD", the bot recieved a MOTD command from the server,
  and it can be assumed the bot has successfully authenticated with
  the server.
  If the function finds "NAMES", the bot recieved a list of all users
  in a channel, and it can be assumed the bot successfully connected to
  the channel.
*/
int checkBotState(Bot * botState, char * inString)
{
	int status = 0;
	
	if(botState->inServer == 0)
	{
		if(strstr(inString, "/MOTD") != NULL)
		{
			botState->inServer = 1;
			status = 1;
		}
	}
	
	if (botState->inChannel == 0)
	{
		if(strstr(inString, "NAMES") != NULL)
		{
			botState->inChannel = 1;
			status = 1;
		}
	}

	return status; 
}

/******************
LEFT OFF HERE
I have no idea what I'm doing.
The idea is to create nodes to put into the linked list,
but I'm getting all messed up.
Create List, Create Node, Put Node into List, Return List.
Something like that
********************/
root * addResponses(char ** responses)
{
	root * newList = NULL;
	botMsg * newMessage;
	int i = 0;
	if(responses != NULL)
	{
		newList = createList();
		while(responses[i] != NULL)
		{
			newMessage = createNode();
			newMessage = parseIntoNode(newMessage, responses[i]);
			push(newList, newMessage);
			i++;
		}
	}
	return newList;
}

void checkAndRespond(char * check, int socket, Bot * theBot)
{
	//int i = 0;
	botMsg * conductor;
	char * message = NULL;
	int length;

	if (check != NULL && theBot != NULL && theBot->list != NULL && theBot->list->head != NULL)
	{
		conductor = theBot->list->head;
		while(conductor != NULL)
		{
			if(conductor->listenFor[0] != NULL && strstr(check, conductor->listenFor[0]) != NULL)
			{
				length = strlen("PRIVMSG :\n\r") + strlen(theBot->channelName) + strlen(conductor->response[0]);
				message = malloc(sizeof(char) * length + 1);
				snprintf(message, length + 1, "PRIVMSG %s :%s\n\r", theBot->channelName, conductor->response[0]);
				write(socket, message, strlen(message));
				//fflush(socket);
			}
			conductor = conductor->next;
		}
	}	
}


