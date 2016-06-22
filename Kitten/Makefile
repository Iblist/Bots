CC = clang

all:
	$(CC) -Wall -pedantic src/main.c src/parsing.c src/botAssets.c -Iinclude -o bin/run

errorCheck:
	$(CC) -Wall -pedantic -g src/main.c src/parsing.c src/botAssets.c -Iinclude -o bin/run
