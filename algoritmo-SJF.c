#include <stdio.h>
#include <stdlib.h>

main(int argc, char *argv[]){
	// criando a vari�vel ponteiro para o arquivo
	FILE *arq;
	char processo[5];
	
	//abrindo o arquivo em modo "somente leitura"
	arq = fopen("processos.txt", "r");
	
	if (arq == NULL){ // Se houve erro na abertura
	    printf("Problemas na abertura do arquivo\n");
	    return;
	}
	
	//enquanto n�o for fim de arquivo o looping ser� executado e ser� impresso o texto
	while(fgets(processo, 5, arq) != NULL){
		printf("%s", processo);	
	}
	
	// fechando arquivo
	fclose(arq);
	
	return 0;
	
}
