#!/usr/bin/sh

clang main.c --std=c99 -Wall -Werror -fsanitize=address -lraylib -o breakout
