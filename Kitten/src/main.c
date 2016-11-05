#include "mainBot.h"

//I can't remember why this is here
void error(char * errorMessage)
{
    perror(errorMessage);
    exit(1);
}

int main(int argc, char * argv[])
{
    int sockFeed = SOCK_DGRAM;
	int ln = 0;
	int count = 0;
	int i = 0;
	int j = 0;
	int stateChange = 0;
	struct addrinfo *result;
    char bufferIn[255];
	FILE * logFile;
	char ** stringList = NULL;
	char *pongResp = NULL;
	char * name;
	char * timestamp;
	Bot * settings;

	for (i = 0; i < argc; i++)
	{
		if (strncmp(argv[i], "--help", 6) == 0) helpMessage();
	}
	i = 0;

	if (argc < 4)
	{
		fprintf(stderr, "Not enough arguments\n");
		helpMessage();
	}

	settings = createBot(argc, argv);

	if (settings == NULL)
	{
		fprintf(stderr, "Error creating Bot\n");
	}

	if (!(logFile = fopen("assets/log.txt", "w")))
	{
		perror("Error opening file:");
		exit(1);
	}

    if ((getaddrinfo(argv[1], argv[2], NULL, &result)) != 0)
	{
		perror("Error finding server");
		exit(1);
	}

    if ((sockFeed = socket(result->ai_family, result->ai_socktype, result->ai_protocol)) < 0)
    {
        fprintf(stderr, "Unable to create Socket.\n");
        exit(0);
    }

    if (connect(sockFeed, result->ai_addr, result->ai_addrlen) < 0)
    {
        perror("Connection Failed");
        exit(0);
    }    

	
	/*Sending user and nick to irc. Standard log in stuff.*/
	write(sockFeed, settings->passResp, strlen(settings->passResp));
	write(sockFeed, settings->userResp, strlen(settings->userResp));
	write(sockFeed, settings->nickResp, strlen(settings->nickResp));

	sleep(1);

	/*Join server, wait a moment, continue.*/
	/*write(sockFeed, settings->joinMsg, strlen(settings->joinMsg));
	sleep(1);
	settings->inChannel = 1;*/
	

	/*This needs to be in it's own function
	  Reads a line from the socket into a buffer before checking the input for
 	  stuff, such as PING*/
	while (/*count < 10 &&*/ (ln = read(sockFeed, bufferIn, 254)))
	{
		stringList = splitString(bufferIn);
		if (stringList[0] != NULL)
		{
			for (j = 0; stringList[j] != NULL; j++)
			{
				stateChange = 0;
				stateChange = checkBotState(settings, stringList[j]);
				name = getSourceName(stringList[j]);
				timestamp = getTimestamp(stringList[j]);
				pongResp = serverPingCheck(stringList[j]);

				if (stateChange == 1 && settings->inServer == 1 && settings->inChannel == 0)
				{
					write(sockFeed, settings->joinMsg, strlen(settings->joinMsg));
				}

				//Ping response to server.
				if (pongResp != NULL)
				{
					write(sockFeed, pongResp, strlen(pongResp));
					puts(pongResp);
					free(pongResp);
					pongResp = NULL;
				}

				//Ping response to user.
				pongResp = createPingResp(name, timestamp);
				if (pongResp != NULL)
				{
					write(sockFeed, pongResp, strlen(pongResp));
					puts(pongResp);
					free(pongResp);
					pongResp = NULL;
				}

				checkAndRespond(stringList[j], sockFeed, settings);

				/*Free string list for next round*/
				if (stringList[j] != NULL)
				{
					free(stringList[j]);
					stringList[j] = NULL;
				}
				if (name != NULL)
				{
					free(name);
					name = NULL;
				}
			}
			/*Write buffer to a log file, this will be optional at some point*/
			fputs(bufferIn, logFile);
			fputs("\n", logFile);
			fflush(logFile);
		}
		if (stringList != NULL)
		{
			free(stringList);
			stringList = NULL;
		}
		count++;
		/*Voids out the buffer so reading doesn't become buggy for subsequent reads*/
		memset(bufferIn, '\0', 255);
	}
	/*Sends a message to the server that we are disconnecting*/
	write(sockFeed, settings->quitMsg, strlen(settings->quitMsg));

	/*Closing socket and log file*/
	fclose(logFile);
	close(sockFeed);

	/*Freeing memory from string list*/
	if (stringList != NULL)
	{
		i = 0;
		while(stringList[i] != NULL)
		{
			free(stringList[i]);
			i++;
		}
		free(stringList);
	}

	/*Freeing memory allocated for the bot here*/
	if (settings != NULL)
	{
		if(settings->nickResp != NULL) free(settings->nickResp);
		if(settings->userResp != NULL) free(settings->userResp);
		if(settings->joinMsg != NULL) free(settings->joinMsg);
		if(settings->quitMsg != NULL) free(settings->quitMsg);
		if(settings->passResp != NULL) free(settings->passResp);
		free(settings);
	}

    return 0;
}
