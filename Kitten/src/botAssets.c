#include "botAssets.h"
#include "parsing.h"

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

	tempBot = malloc(sizeof(Bot));

	if (tempBot != NULL)
	{
		tempBot->inChannel = 0;
		tempBot->inServer = 0;
		tempBot->quitMsg = strdup("QUIT :Tah Tah for now \r\n");
		tempBot->passResp = strdup("PASS DoobyDoo\r\n");

		tempBot->joinMsg = malloc(sizeof(char) * (strlen("JOIN \r\n") + strlen(argv[3])));
		sprintf(tempBot->joinMsg, "JOIN %s\r\n", argv[3]);

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
		}
	}
	return tempBot;
}

/*Error codes:
  0 = Bot State unchanged
  1 = Bot State changed
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
