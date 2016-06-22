#ifndef PARSING_H
#define PARSING_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*add some god damn comments you hippy*/
char ** splitString(char*);
char * cleanMessage(char*);
void helpMessage();
int setMsg();
char * getSourceName(char * input);

#endif //PARSING_H
