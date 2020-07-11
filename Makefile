
CC=gcc
CLFAGS=-I.

player: loadmid.o parse2.o notes.o play.o keyboard.o
	$(CC) -o player parse2.o loadmid.o notes.o play.o keyboard.o -lm

piano: notes.o play.o piano.o keyboard.o
	$(CC) -o piano piano.o notes.o play.o keyboard.o -lm
