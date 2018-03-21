#pragma once
#include"stack.h"
#define MaxStackSize 100
void Push(double element, double* stk, int* stkcount);
double Pop(double* stk, int* stkcount);
void ApplyOperator(char op, double* stk, int* stkcount);
