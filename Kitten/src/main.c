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
    //sockaddr * serv_addr;
	struct addrinfo *result;
    //int portNum;
    //int clientSize;
    char bufferIn[255];
    //struct addrinfo hints;

    /*hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = 0;
    hints.ai_protocol = 0;   
    hints.ai_flags = 0; */

	

    if (argc != 3)
    {
        fprintf(stderr, "Correct use is: SERVER_NAME PORT_NUMBER\n");
        exit(1);
    }

    if ((getaddrinfo(argv[1], argv[2], NULL, &result)) != 0)
	{
		perror("Error finding server: ");
		exit(1);
	}

    if ((sockFeed = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "Unable to create Socket.\n");
        exit(0);
    }

    if (connect(sockFeed, result->ai_addr, result->ai_addrlen) < 0)
    {
        perror("Connection Failed: ");
        exit(0);
    }
    else
    {
        printf("Dude, you did it!  Yay!  Fuck teh police!\n");
    }    

	read(sockFeed, bufferIn, 254);

	printf("bufferIn: %s\n", bufferIn);

    close(sockFeed);

    return 0;
}
