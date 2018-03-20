#pragma once
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
char* step(enum states *state, char* input, int input_type);
int sintax(char* in, enum states *state);