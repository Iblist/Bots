#include "parsing.h"
#include "linkedList.h"

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

/*Function: getSourceName
  Expects: char*
  Returns: char*
  
  This function takes a string as input, and finds the name of
  whatever sent it. This could be the name of either the server
  or the name of another user. The function will look through
  a provided string, and on the first instance of either a '!'
  or ' ', exit the loop. Messages from the server will have 
  a space after the name, messages from users will have an '!'
  sepperating their nick name and real name.  
*/
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

/*Function: getTimeStamp
  Expects: char*
  Returns: char*

  This function looks through a given string for a timestamp,
  copies the timestamp and returns it. A timestamp will have the
  form ':\x01PING TIMESTAMP\x01'. The function looks for a 
  ':\x01PING', and copies the remainder of the string.
  It should be noted, the timestamp can be pretty much anything,
  so extra lengths should be taken to avoid buffer overflows.
*/
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

/*Function: serverPingCheck
  Expects: char*
  Returns: char* on success,
           NULL otherwise.

  This function checks a given string for a ping from a server.
  If a ping from the server is found, the function returns an
  appropriate PONG response to send back to the server. Otherwise
  it returns NULL.

*/
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

/*FINISH ME!
  Note to self:
  Can't remember what this function is for,
  leave out for now.*/
/*char * userPingCheck(char * input)
{

	return NULL;
}*/

/*Function: createPingResp
  Expects: char* and char*
  Returns: char* on success
           NULL otherwise.

  This function creates a ping response message if a ping from
  another user was recieved. If neither argument is NULL, a ping
  response is created, otherwise NULL is returned.
*/
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

/*This function is causing things to break, not sure why.*/
botMsg * parseIntoNode(botMsg * newMsg, char * input)
{
	int listenForF = 0;
	int listenForC = 0;
	int responseF = 0;
	int responseC = 0;
	int repeatF = 0;
	int repeatC = 0;
	int targetF = 0;
	int targetC = 0;
	//int keepLooping = 1;
	char * token = NULL;
	//char * toCopy;
	//char * fullInput;

	//fullInput = strdup(input);

	token = strtok(input, " ");
	//if (strncmp(token, "#", 1) == 0) token = NULL;
	while(token != NULL)
	{
		setFlags(&listenForF, &responseF, &repeatF, &targetF, token);
		if (listenForF == 1)
		{
			//token = strotk(NULL, "\"");
			token = strtok(NULL, "\"");
			if(newMsg->listenFor[listenForC] == NULL)
			{
				//toCopy = getQuotedString(token);
				newMsg->listenFor[listenForC] = malloc(sizeof(char)* 100/*strnlen(token, 64)*/);
				//token[3] = '\0';
				strncpy(newMsg->listenFor[listenForC], token, 50);
				listenForC++;
			}
			
		}
		else if (responseF == 1)
		{
			if(newMsg->response[responseC] == NULL)
			{
				token = strtok(NULL, "\"");
				//toCopy = getQuotedString(token);
				newMsg->response[responseC] = malloc(sizeof(char)*100 /*strnlen(token, 64)*/);
				strncpy(newMsg->response[responseC], token, 50);

				responseC++;
			}
		}
		else if (repeatF == 1)
		{

			if(newMsg->repeat[repeatC] == NULL)
			{
				token = strtok(NULL, "\"");
				//toCopy = getQuotedString(token);
				newMsg->repeat[repeatC] = malloc(sizeof(char)*strnlen(token, 64));
				strncpy(newMsg->repeat[repeatC], token, 50);
				repeatC++;
			}
		}
		else if (targetF == 1)
		{
			if(newMsg->repeat[targetC] == NULL)
			{
				token = strtok(NULL, "\"");
				//toCopy = getQuotedString(token);
				newMsg->repeat[targetC] = malloc(sizeof(char)*strnlen(token, 64));
				strncpy(newMsg->repeat[targetC], token, 50);
				targetC++;
			}
		}
		token = strtok(NULL, " ");
	}
	//free(fullInput);
	return newMsg;
}

/*Add comments later*/
void setFlags(int * flag1, int * flag2, int * flag3, int * flag4, char * token)
{
		if(strncmp(token, "LISTENFOR", 9) == 0)
		{
			*flag1 = 1;
			*flag2 = 0;
			*flag3 = 0;
			*flag4 = 0;
		}
		else if(strncmp(token, "RESPOND", 7) == 0)
		{
			*flag1 = 0;
			*flag2 = 1;
			*flag3 = 0;
			*flag4 = 0;
		}
		else if (strncmp(token, "REPEAT", 6) == 0)
		{
			*flag1 = 0;
			*flag2 = 0;
			*flag3 = 1;
			*flag4 = 0;
		}
		else if (strncmp(token, "TARGET", 6) == 0)
		{
			*flag1 = 0;
			*flag2 = 0;
			*flag3 = 0;
			*flag4 = 1;
		}
		else
		{
			*flag1 = 0;
			*flag2 = 0;
			*flag3 = 0;
			*flag4 = 0;
		}
		/*else if (strncmp(token, "#", 1) == 0)
		{
		
		}*/

}

/*A comment goes here*/
char * getQuotedString(char * input)
{
	int i;
	int counter = 0;
	char * start = NULL;
	char * end = NULL;
	char * quotedString = NULL;

	if (input != NULL && strnlen(input, 255) > 0)
	{
		for(i = 0; i < strnlen(input, 255); i++)
		{
			if (start == NULL && input[i] == '\"')
			{
				counter = 0;
				start = &input[i];
			}
			else if (end == NULL && input[i] == '\"')
			{
				end = &input[i];
			}
			else if (end != NULL && start != NULL)
			{
				quotedString = malloc(sizeof(char)*(counter)); //Error here
				strncpy(quotedString, start, counter);
				i = strnlen(input, 255);
			}
			counter++;
		}
	}
	return quotedString;
}

