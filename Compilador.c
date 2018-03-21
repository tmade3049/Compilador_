
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "Compilador.h"
#include "stack.h"

typedef char* string;
typedef enum { FALSE, TRUE } bool;

char errors[16][500] = { 0 };
double   stk[MaxStackSize];
double c_stk[MaxStackSize];
int    stkcount, c_stkcount;
int indice = 0;

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

char* step(enum states *state, char* input, int input_type){
	char status = 0;
	char num[32] = { 0 };
	
	struct branch *b = &the_table[*state][input_type];
	*state = (enum states)(b->new_state);
	if (b->set_error) {
		sprintf(&errors[indice][0]," -> Erro de Sintaxe: %c posicao invalida\n", input[0]);
		indice++;
		status = 1;
	}
	if (b->new_state == open) {
		Push(1, c_stk, &c_stkcount);
	}
	else if (b->new_state == close) {
		Pop(c_stk, &c_stkcount);
		
	}
	if (b->new_state == finish) {
		if (c_stkcount != 0) {
			sprintf(errors[indice], " -> Erro de Sintaxe: %c posicao invalida\n", input[0]);
			indice++;
			status = 1;
		}
		printf("----------------------------------------------------------------------\n");
		printf("-------------------------- LOG EXECUCAO: -----------------------------\n");
		printf("----------------------------------------------------------------------\n");
		if (status){
			int i = 0;
			//printf(status);
			printf("----------------------------------------------------------------------\n");
			printf("--------- ANALISE ENCONTROU ERRO(S) DE SINTAXE------------------------\n");
			printf("----------------------------------------------------------------------\n");
			for (;  errors[i][0] > (char)NULL; i++);
			for (int j = 0 ; j < i ; j++)			
				printf("%s", errors[j]);
			printf("\n");
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
	int oper_type = -1;
	int i, joy = 0, k = 0;
	int s_in = strlen(in);
	char temp[32] = { NULL };

	for (i = 0; i < s_in; i++) {
		if (!isdigit(in[i]) || !isOperator(in[i])) {
			printf("Simbolo %c invalido", in[i]);
			joy = 1;
		}
	}
	
	if (joy) return joy;

	for (i = 0; i < s_in; i++) {
		{
			if (isdigit(in[i])) {
				temp[k] = *(in + i);
				k++;
				if (i == (s_in - 1)) {
					temp[k + 1] = '\0';
					step(state, &temp, number);
				}
			}
			else {
				if (oper_type = isOperator(in[i]))
				{
					if (k)
					{
						temp[k + 1] = '\0';
						step(state, &in[i], number);
						k = 0;
					}
					step(state, &in[i], oper_type);
				}
			}
		}					
	}
	if(!joy)
		step(state, &in[i], finish);
	return joy;
}

int isOperator(char in) {
	char     sim[] = { "+-*/()" };
	int      pos = 0;
	int      i = 0;

	for (i; i < 6; i++) {
		if (in == sim[i]) {
			switch (i)
			{
			case 5:
				pos = close;
				break;
			case 4:
				pos = open;
				break;
			default:
				pos = operator;
				break;
			}
			break;
		}
	}
	return pos;
}
