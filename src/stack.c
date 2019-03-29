#include"stack.h"
#include<stdio.h>
#include<stdlib.h>


void Push(double element, double* stk, int* stkcount)
{
	if (stkcount == MaxStackSize) {
		printf("Stack size exceeded.\n");
		exit(EXIT_FAILURE);
	}
	*(stk + *stkcount++) = element;
}

double Pop(double* stk, int* stkcount)
{
	if (stkcount == 0) {
		printf("Pop of an empty stack.\n");
		exit(EXIT_FAILURE);
	}
	return stk[--*stkcount];
}

void ApplyOperator(char op, double* stk, int* stkcount)
{
	double lhs, rhs, result;

	rhs = Pop(stk, &stkcount); // operando direito
	lhs = Pop(stk, &stkcount); // operando esquerdo
	switch (op) {
	case '+': result = lhs + rhs; break;
	case '-': result = lhs - rhs; break;
	case '*': result = lhs * rhs; break;
	case '/': result = lhs / rhs; break;
	default: printf("Illegal operator\n");
		exit(EXIT_FAILURE);
	}
	printf("%g\n", result);
	Push(result, stk, &stkcount);
}