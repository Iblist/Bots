#ifndef MAINBOT_H
#define MAINBOT_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define HOSTNAME "irc.hackthissite.org"
#define BOTNAME "Kitten" /*...Yes*/

char ** splitString(char*);

#endif //MAINBOT_H
