#include <stdio.h>
#include <stdlib.h>

struct processos {
	int numero;					// Numero do processo para melhor identificacao
	int criacao;				// Tempo em que o processo entrou na fila
	int execucao;				// Duracao do processo
	int prioridade;				// Prioridade do processo
	int espera;					// Tempo de espera para ser executado
	int vida;					// Tempo de vida do processo
	struct processos *prox;		// Proximo processo na fila
};

main(int argc, char *argv[]){
	// criando a variável ponteiro para o arquivo
	FILE *arq;
	char processo[5];
	
	//abrindo o arquivo em modo "somente leitura"
	arq = fopen("processos.txt", "r");
	
	if (arq == NULL){ // Se houve erro na abertura
	    printf("Problemas na abertura do arquivo\n");
	    return;
	}
	
	//enquanto não for fim de arquivo o looping será executado e será impresso o texto
	while(fgets(processo, 5, arq) != NULL){
		printf("%s", processo);	
	}
	
	// fechando arquivo
	fclose(arq);
	
	return 0;
	
}
