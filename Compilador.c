
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
char sim[] = { "123456789+-*/()" };

char* step(enum states *state, char* input, int input_type)
{
	char* status = NULL;
	char* temp = NULL;
	struct branch *b = &the_table[*state][input_type];
	*state = (enum states)(b->new_state);
	if (b->set_error)
	{
		sprintf(temp," -> Erro de Sintaxe: %c posicao invalida\n", input);
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
	return status;
}
int sintax(char* in, int* pos) {
	int ret = 0;
	int i, j, k = 0;
	int isOperador = FALSE;
	int isSimbol = FALSE;
	int isValid = FALSE;
	int s_in = strlen(in);
	int s_simb = strlen(sim);

	char c;
	char *temp = (char*)malloc(1);

	for (i = 0; i < s_in; i++) {
		for (j = 0; j < 13; j++) {
			char a = in[i];
			char b = sim[j];
			//Localiza simbolo
			if (in[i] == sim[j]) {
				isValid = TRUE;
				//se simbolo � operador
				if (j > 8) {
					if (k) {
						temp = (char*)realloc(temp, k + 1);
						*(temp + k + 1) = '\0';
						printf(" Simbolo %s valido sucess\n", temp);
						k = 0;
						temp = (char *)malloc(1);
					}
					//Primeiro e ultimo nao pode ser operador
					if (i == 0) {
						printf(" begin operador %c error\n", in[i]);
					}
					else if (i == s_in - 1) {
						printf(" end operador %c error\n", in[i]);
					}
					else {
						//Dois operadores seguidos
						if (isOperador) {
							printf(" 2x operador error\n");
						}
						else {
							isOperador = TRUE;
							printf(" Operador %c valido sucess\n", in[i]);
						}
					}
				}
				else {
					isOperador = FALSE;
					*(temp + k) = *(in + i);
					k++;
					if (i == (s_in - 1)) {
						*(temp + k + 1) = '\0';
						printf(" Simbolo %s valido sucess\n", temp);
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
}
