
CC=gcc
CLFAGS=-I.

player: parse2.o loadmid.o notes.o play.o
	$(CC) -o player parse2.o loadmid.o notes.o play.o -lm
