cmake_minimum_required(VERSION 3.10)
project(midinoise C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(midinoise
        keyboard.c
        loadmid.c
        loadmid.h
        notes.c
        notes.h
        parse2.c
        piano.c
        play.c
        play.h
        play2.c
        popen.c
        popen.h xclient.c xkeypress.c)
