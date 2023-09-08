#!/bin/bash

gcc main.c bignum.c \
-Wall -Werror -Wpedantic -Wextra \
-std=c99 -c -O3

gcc -lm main.o bignum.o -o app.exe
