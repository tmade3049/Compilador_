/*
* AnalisadorLex.c
*
*  Created on: 4 de mar de 2018
*      Author: tai
*/
#include"Compilador.h"

int main() {


	char* input;
	char in[256] = { 0 };
	int ret = 0, pos;
	int i;

	do {

		printf("Input:\n");
		scanf("%s", in);
		for (i = 1; in[i] != '\0'; ++i);
		input = malloc(i+1);
		input = in;
		ret = sintax(input, &pos);
		step(i, in, ret);

	} while (strcmp(input,"q"));

	return 0;
}

