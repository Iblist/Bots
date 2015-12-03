#include "mainBot.h"

void error(char * errorMessage)
{
    perror(errorMessage);
    exit(1);
}

int main(int argc, char * argv[])
{
    int sockFeed;
    int server;
    //int portNum;
    //int clientSize;
    char bufferIn[255];
    struct addrinfo hints;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = 0;
    hints.ai_protocol = 0;   
    hints.ai_flags = 0; 

    if (argc != 3)
    {
        printf("Correct use is: SERVER_NAME PORT_NUMBER\n");
        exit(1);
    }

    

    if ((sockFeed = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Unable to create Socket.\n");
        exit(0);
    }

    if (connect(sockFeed, server, sizeof(server)) < 0)
    {
        printf("Connection Failed.\n");
        exit(0);
    }
    else
    {
        printf("Dude, you did it!  Yay!  Fuck teh police!\n");
    }    

    memset(&bufferIn, 0, sizeof(bufferIn));

    return 0;
}
