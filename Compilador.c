
#include<string.h>
#include <ctype.h>
#include"Compilador.h"

#define MaxStackSize 100

typedef char* string;
typedef enum { FALSE, TRUE } bool;

char     sim[]             = { "+-*/()" };
double   stk[MaxStackSize];
double c_stk[MaxStackSize];
int    stkcount, c_stkcount;

enum states { start = 0, number = 1, operator = 2, open = 3, close = 4, finish = 5 };
struct branch {	
	unsigned char new_state : 3;
	unsigned char set_error : 2;
	unsigned char you_again : 1;
};
struct branch the_table[6][6] = {
	/*                   start                   number               operator          open               close             finish         */
	/* start */   { { start, 0, 1 }, { number, 0, 0 }, { operator, 1, 0 }, { open, 0, 0   }, { close, 1, 0  }, { finish, 1, 0} },
	/* number */  { { start, 1, 0 }, { number, 0, 1 }, { operator, 0, 0 }, { open, 1, 0   }, { close, 0, 0  }, { finish, 0, 0 } },
	/* operator */{ { start, 1, 0 }, { number, 0, 0 }, { operator, 1, 1 }, { open, 0, 0   }, { close, 1, 0  }, { finish, 1, 0 } },
	/* open */    { { start, 1, 0 }, { number, 0, 0 }, { operator, 1, 0 }, { open, 0, 1   }, { close, 1, 0  }, { finish, 1, 0 } },
	/* close  */  { { start, 1, 0 }, { number, 1, 0 }, { operator, 0, 0 }, { open, 1, 0   }, { close, 0, 1  }, { finish, 0, 0 } },
	/* finish */  { { start, 1, 0 }, { number, 1, 0 }, { operator, 1, 0 }, { open, 1, 0   }, { close, 1, 0  }, { finish, 1, 1 } }
};

void Push(double element, double* stk, int* stkcount)
{
	if (stkcount == MaxStackSize){
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

void ApplyOperator(char op)
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

char* step(enum states *state, char* input, int input_type){
	char status = 0;
	char temp[500] = { 0 };
	char num[32] = { 0 };
	struct branch *b = &the_table[*state][input_type];
	*state = (enum states)(b->new_state);
	if (b->set_error) {
		sprintf(temp," -> Erro de Sintaxe: %c posicao invalida\n", input[0]);
		status = 1;
	}
	if (b->new_state == open) {
		Pop(c_stk, &c_stkcount);
	}
	else if (b->new_state == close) {
		Push(1, c_stk, &c_stkcount);
	}
	if (b->new_state == finish) {
		if (c_stkcount != 0) {
			sprintf(temp, " -> Erro de Sintaxe: %c posicao invalida\n", input[0]);
			status = 1;
		}
		printf("----------------------------------------------------------------------\n");
		printf("-------------------------- LOG EXECUCAO: -----------------------------\n");
		printf("----------------------------------------------------------------------\n");
		if (status){
			//printf(status);
			printf("----------------------------------------------------------------------\n");
			printf("--------- ANALISE ENCONTROU ERRO(S) DE SINTAXE------------------------\n");
			printf("----------------------------------------------------------------------\n");
			printf(temp);
		}
		else {
			printf("----------------------------------------------------------------------\n");
			printf("--------- ANALISE DE SINTAXE EXECUTADA COM SUCESSO -------------------\n");
			printf("----------------------------------------------------------------------\n");
		}

	}
	return &num;
}
int sintax(char* in, enum states *state) {
	int openned = 0;
	int i, j, k = 0;
	int s_in = strlen(in);
	int s_simb = strlen(sim);
	char temp[32] = {NULL};

	for (i = 0; i < s_in; i++) {
		if (isdigit(in[i])) {
			temp[k] = *(in + i);
			k++;
			if (i == (s_in - 1)) {
				temp[k + 1] = '\0';
				step(state, &temp, number);
			}
		}
		else {
			for (j = 0; j < 6; j++) {
				char a = { in[i] };
				char b = { sim[j] };
				if (a == b) {
					if (j < 5) {
						if (k) {
							temp[k + 1] = '\0';
							step(state, &in[i], number);
							k = 0;
						}
						step(state, &in[i], operator);
					}
					else {
						// OPEN
						if (j == 5) {
							step(state, &in[i], open);
							openned++;
						}
						// CLOSE
						else {
							step(state, &in[i], close);
							openned--;
						}
					}
					break;
				}
			}
		}
	}	
	step(state, &in[i], finish);
	return NULL;
}
