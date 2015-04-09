#include "mainBot.h"

int main()
{
    int socketDesc;
    char *hostname = HOSTNAME;
    char ipAddr[100];
    struct hostent *hostCreds;
    struct in_addr **addrList;
    struct sockaddr_in server;
    int i;

    if ((hostCreds = gethostbyname(hostname)) == NULL)
    {
        printf("Unable to resolve hostname\n");
        exit(0);
    }

    addrList = (struct in_addr **) hostCreds->h_addr_list;

    for(i = 0; addrList[i] != NULL; i++)
    {
        strcpy(ipAddr, inet_ntoa(*addrList[i]));
    }

    printf("Host resolved to: %s\n", ipAddr);

    socketDesc = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDesc == -1)
    {
        printf("You done fucked up bro");
        exit(0);
    }
    server.sin_addr.s_addr = inet_addr(ipAddr);
    server.sin_family = AF_INET;
    server.sin_port = htons(6667);

    if(connect(socketDesc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        puts("Connect error");
        exit(0);
    }
    else
    {
        puts("Connected");
    }
    close(socketDesc);
    return 0;
}
