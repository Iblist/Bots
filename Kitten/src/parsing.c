#include "parsing.h"

/*Function: splitString
  Expects: char*
  Returns: char**
  This function takes a newline deliminated string and splits it into sepperate strings.
  The messages read from a socket are in the form of blobs of strings deliminated by 
  carriage returns (\r\n). This function searches through the string until it finds a
  '\r', cuts everything up until that out and assigns it to it's own strign in the list.
  The last entry in the list is NULL so the list can safely be looped through by looking
  for NULL.
*/
char ** splitString(char * inString)
{
	int i;
	int j = 1;
	int breakPoint = 0;
	char ** outString = NULL;

	for (i = 0; i < strlen(inString); i++)
	{
		if(inString[i] == '\n')
		{
			outString = realloc(outString, sizeof(char*)*j);
			outString[j-1] = strndup(&inString[breakPoint], i-breakPoint);
			breakPoint = i+1;
			j++;
		}
		outString = realloc(outString, sizeof(char*)*j);
		outString[j-1] = NULL;
	}

	return outString;
}

/*Function: helpMessage
  Expects: nothing
  Returns: nothing
  This function simply types a help message to the screen before exiting the program with a status code 0
*/
void helpMessage()
{
	puts("WIP");
	puts("usage: ./bin/run |<server> <port> <channel>| [--nick] <nickName> [--name] <realName> [--help] [--record]");
	puts("WIP");

	exit(0);
}

/*Function: cleanMessage
  Expects: char*
  Returns: char*
  This function takes a string, cleans it, and returns the cleaned string.
  The string read from the irc server aren't the best for readability.
  This function takes those hard to read strings and simplifies them to the form
  <user>"Message"
  <user> is the the person who sent the message to the channel.
  "Message" is the actual message sent by that user.
  
  NOTE TO SELF: Might be a good idea to include the channel name, however, if an ncurses gui
  is included that shouldn't be necessary.
  SECOND NOTE TO SELF: I'll implement this later, readability isn't important atm.
*/
char * cleanMessage(char * input)
{

	return NULL;
}

/*Gets the name of whoever sent some message
  Note to self: Write better comment later*/
char * getSourceName(char * input)
{
	int i;
	char * name = NULL;

	/*Trying to avoid segmentation faults*/
	if (strlen(input) > 1)
	{
		/*Searching through the provided string
		  Messages from other users are started off
   		  with something like;
 
		  nick!realName@serviceProvider COMMANDS

		  So it's a simple matter of scanning until an ! point is found
		  if a space is found first, it's a message from the server.*/
		for (i = 0; i < strlen(input); i++)
		{
			if (input[i] == '!')
			{
				name = strndup(&input[1], i-1); //This might be a little dirty, but oh well.
				break;
			}
			else if (input[i] == ' ')
			{
				name = strndup(&input[1], i-1);
				break;
			}
		}
	}

	return name;
}

/*Gets timestamp from a ping sent by user
  NOTE TO SELF write better comment.*/
char * getTimestamp(char * input)
{
	char * timestamp = NULL;
	char * ptr;

	if ((ptr = strstr(input, ":\x01PING")) != NULL)
	{
		timestamp = strndup(ptr, 100);
	}

	return timestamp;
}

char * serverPingCheck(char * input)
{
	char * pingMsg = NULL;
	char * ptr;

	if((ptr = strstr(input, "PING :"))!= NULL)
	{
		pingMsg = malloc(sizeof(char) * (strlen(ptr) + 2));
		strcpy(pingMsg, ptr);
		strcat(pingMsg, "\r\n");
		pingMsg[1] = 'O';
	}

	return pingMsg;
}

/*FINISH ME!*/
char * userPingCheck(char * input)
{

	return NULL;
}

char * createPingResp(char * name, char * timestamp)
{
	char * pingResp = NULL;

	if (name != NULL && timestamp != NULL)
	{
		pingResp = malloc(sizeof(char) * (strlen(name) + (strlen(timestamp) + strlen("NOTICE  \r\n"))));
		sprintf(pingResp, "NOTICE %s %s\r\n", name, timestamp);
	}

	return pingResp;
}

/*For future!
  Consider adding a "containString" function.
  This function would check if a given string contained a second given string.
*/
