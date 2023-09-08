#!/bin/bash

clang main.c bignum.c \
-Wall -Werror -Wpedantic -Wextra \
-std=c99 -c -Og -g3 \
-fsanitize=memory -fPIE -fno-omit-frame-pointer

clang -lm -fsanitize=memory -fPIE main.o bignum.o -o app.exe
