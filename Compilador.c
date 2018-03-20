
#include"Compilador.h"

#define SINTAX_OK 0
#define SINTAX_ERR 1
#define TRUE 1
#define FALSE 0

enum states { start = 0, number = 1, operator = 2, open = 3, close = 4, finish = 5 };
struct branch {
	unsigned char new_state : 2;
	unsigned char set_error : 1;
};
struct branch the_table[5][5] = {
	/*                 number         operator         open         close         finish         */
	/* start */   { { number,0  }, { operator,1 }, { open,0   }, { close,1  }, { finish,1 } },
	/* number */  { { number,0  }, { operator,0 }, { open,1   }, { close,0  }, { finish,0 } },
	/* operator */{ { number,0  }, { operator,1 }, { open,0   }, { close,1  }, { finish,1 } },
	/* open */    { { number,0  }, { operator,1 }, { open,0   }, { close,1  }, { finish,1 } },
	/* close  */  { { number,1  }, { operator,0 }, { open,1   }, { close,0  }, { finish,0 } }
};

// char* simbol = (char*)malloc(sizeof(char));
char sim[] = { "0123456789+-*/()" };

char* step(enum states *state, char* input, int input_type)
{
	char* status = NULL;
	char temp[500] = { 0 };
	struct branch *b = &the_table[*state][input_type];
	*state = (enum states)(b->new_state);
	if (b->set_error)
	{
		sprintf(temp," -> Erro de Sintaxe: %c posicao invalida\n", input[0]);
	}
	else {

	}
	if (*state == finish) {
		printf("----------------------------------------------------------------------\n");
		printf("-------------------------- LOG EXECUCAO: -----------------------------\n");
		printf("----------------------------------------------------------------------\n");
		if (status){
			printf(status);
			printf("----------------------------------------------------------------------\n");
			printf("--------- ANALISE ENCONTROU ERRO(S) DE SINTAXE------------------------\n");
		}
		else {
			printf("----------------------------------------------------------------------\n");
			printf("--------- ANALISE DE SINTAXE EXECUTADA COM SUCESSO -------------------\n");
		}

	}
	return NULL;
}
int sintax(char* in, enum states *state) {
	int ret = 0;
	int i, j, k = 0;
	int isOperador = FALSE;
	int isSimbol = FALSE;
	int isValid = FALSE;
	int s_in = strlen(in);
	int s_simb = strlen(sim);

	char c;
	char temp[32] = {NULL};

	for (i = 0; i < s_in; i++) {
		for (j = 0; j < 15; j++) {
			char a = in[i];
			char b = sim[j];
			if (!j) state = 1;
			// Localiza simbolo
			if (in[i] == sim[j]) {
				isValid = TRUE;
				// Simbolo eh number 
				if (j < 10) {
					isOperador = FALSE;
					temp[k] = *(in + i);
					k++;
					if (i == (s_in - 1)) {
						temp[k + 1] = '\0';
						step(&state, &temp, number);
						printf(" Simbolo %s valido sucess\n", temp);
					}
				}
				// Simbolo eh operador
				else if (j < 14) {
					if (k) {
						temp [k + 1] = '\0';
						step(&state, &in[i], number);
						printf(" Simbolo %s valido sucess\n", temp);
						k = 0;						
					}
					// Primeiro e ultimo nao pode ser operador
					if (i == 0) {
						printf(" begin operador %c error\n", in[i]);
					}
					else if (i == s_in - 1) {
						printf(" end operador %c error\n", in[i]);
					}
					else {
						// Dois operadores seguidos
						if (isOperador) {
							printf(" 2x operador error\n");
						}
						else {
							isOperador = TRUE;
							step(&state, &in[i], operator);
							printf(" Operador %c valido sucess\n", in[i]);
						}
					}
				}
				else {
					// simbolo eh open
					if (k == 14) {
						step(state, in, open);
					}
					// simbolo eh close
					else {
						step(state, in, close);
					}
				}
				break;
			}
		}
		if (!isValid) {
			printf(" invalid simbol %c error\n", in[i]);
			isOperador = FALSE;
		}
		isValid = FALSE;
	}
	return ret;
	step(state, in, finish);
}
