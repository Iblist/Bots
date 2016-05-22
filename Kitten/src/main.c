#include "mainBot.h"

/*****************
It should be noted!
argv[1] should be irc.hackthissite.org
argv[2] should be 7000
******************/

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
	struct addrinfo *result;
    char bufferIn[255];
	char *userResp = "USER Kitten 0 0 :Kitten\n";
	char *nickResp = "NICK LongStringOfChars\n";
	char *joinMsg = "JOIN ##security\r\n";
	char *quitMsg = "QUIT :Tah tah for now\r\n";
	FILE * logFile;
	//char *message = "PRIVMSG #channel :Haiya, I'm kitten bot, pleased to meet ya! Nyaa~!\r\n";
	//char *pongMessage = "PRIVMSG #channel :*Happy ponging!* Nyaa!\r\n";
	char ** stringList = NULL;
	char *pongResp = NULL;

    if (argc != 3)
    {
        fprintf(stderr, "Correct use is: SERVER_NAME PORT_NUMBER\n");
        exit(1);
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

    if ((sockFeed = socket(AF_INET, SOCK_STREAM, 0)) < 0)
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
	write(sockFeed, userResp, strlen(userResp));
	write(sockFeed, nickResp, strlen(nickResp));

	sleep(1);

	/*Join server, wait a moment, continue.*/
	write(sockFeed, joinMsg, strlen(joinMsg));
	sleep(1);
	//write(sockFeed, message, strlen(message));

	while (/*count < 10 &&*/ (ln = read(sockFeed, bufferIn, 254)))
	{
		//fputs(bufferIn, stdout);
		//fputs("\n", stdout);
		//fputs("~~~~~~~~~~~~~~~~~~~\n", stdout);
		stringList = splitString(bufferIn);
		if (stringList[0] != NULL)
		{
			for(i = 0; i < strlen(stringList[0]); i++)
			{
				if (strncmp(&stringList[0][i], "PING", 4)==0)
				{
					stringList[0][i+1] = '0';
					pongResp = malloc(strlen(&stringList[0][i])+2);
					strcpy(pongResp, &stringList[0][i]);
					strcat(pongResp, "\r\n");
					write(sockFeed, pongResp, strlen(pongResp));
					puts("Ponged");
					//sleep(1);
					/*write(sockFeed, pongMessage, strlen(pongMessage));Removed for now*/
				}
			}
			fputs(stringList[0], logFile);
			fputs("\n", logFile);
			fflush(logFile);
		}
		//puts("~~~~~~~~~~~~~~~~~~~");
		count++;
	}
	write(sockFeed, quitMsg, strlen(quitMsg));

	fclose(logFile);
	close(sockFeed);
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

    return 0;
}

char ** splitString(char * inString)
{
	int i;
	int j = 1;
	int breakPoint = 0;
	char ** outString = NULL;

	for (i = 0; i < strlen(inString); i++)
	{
		if(inString[i] == '\r')
		{
			outString = realloc(outString, sizeof(char*)*j);
			outString[j-1] = strndup(&inString[breakPoint], i-breakPoint);
			j++;
		}
		outString = realloc(outString, sizeof(char*)*j);
		outString[j-1] = NULL;
	}

	return outString;
}
