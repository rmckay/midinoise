
CC=gcc
CLFAGS=-I.

player: loadmid.o parse2.o notes.o play.o keyboard.o popen.o
	$(CC) -o player parse2.o loadmid.o notes.o play.o keyboard.o popen.o -lm

piano: notes.o play.o piano.o keyboard.o popen.o
	$(CC) -o piano piano.o notes.o play.o keyboard.o popen.o -lm

xclient: xclient.o
	$(CC) -o xclient xclient.o -lX11

xkeypress: xkeypress.o
	$(CC) -o xkeypress xkeypress.o -lX11

xpiano: xpiano.o keyboard.o notes.o play.o popen.o
	$(CC) -o xpiano xpiano.o keyboard.o notes.o play.o popen.o -lX11 -lm
