#include <stdio.h>
#include <stdlib.h>

//Variavel global para identificar cada processo que entra na fila
int idProcesso = 0;

struct dados_processo {
	int numero;					//Numero do processo para melhor identificacao
	int criacao;				//Tempo em que o processo entrou na fila
	int duracao;				//Duracao do processo
	int prioridade;				//Prioridade do processo
	int espera;					//Tempo de espera para ser executado
	int vida;					//Tempo de vida do processo
	struct processos *prox;		//Proximo processo na fila
};

//Criando lista de processos
struct dados_processo *init_processos (int criacao, int duracao, int prioridade) {
	struct dados_processo *processo;
	
	//Alocando espaco na memoria	
	processo = (struct dados_processo*)malloc(sizeof(struct dados_processo)); 
	//Caso de erro ao alocar o espaco na memoria
	if (processo == NULL) {
        printf("Erro.");
        exit(1);
    };
    
	//Preenchimento do objeto    
	processo->numero = idProcesso++;
    processo->criacao = criacao;
    processo->duracao = duracao;
    processo->prioridade = prioridade;
    processo->espera = 0;
    processo->vida = 0;
    processo->prox = NULL;
    return(processo);
};

//Listando os processos
void listprocs (struct dados_processo *processos) {
  printf("\tListagem de Processos\n");
  printf("\n");
  while (processos != NULL) {
    printf("Processo: %d\tCriacao: %d\tDuracao: %d\tPrioridade: %d\n", processos->numero, processos->criacao, processos->duracao, processos->prioridade);
    processos = processos->prox;
  };
  printf("\n\n");
 };

/* Escalonamento SJF*/
void sjf (struct dados_processo *proc) {
  int tempo = 0, inicio = 0, fim, shortest;
  struct dados_processo *copia, *tmpsrc, *tmp, *beforeshortest;
  printf("\tEscalonamento SJF\n");
  printf("\n");
  /* Lista de dados_processo é replicada */
  tmpsrc = proc;
  copia = tmp = NULL;
  while (tmpsrc != NULL) {
    if (copia == NULL) {
		copia = init_processos(tmpsrc->criacao, tmpsrc->duracao, tmpsrc->prioridade);
		tmp = copia;
    } else {
		tmp->prox = init_processos(tmpsrc->criacao, tmpsrc->duracao, tmpsrc->prioridade);
		tmp = tmp->prox;
    };
    tmpsrc = tmpsrc->prox;
  };
  while (copia != NULL) {
    /* Encontra o proximo processo*/
    beforeshortest = NULL;
    shortest = copia->duracao;
    tmp = copia->prox;
    tmpsrc = copia;
    while (tmp != NULL) {
		if (tmp->duracao < shortest) {
			shortest = tmp->duracao;
			beforeshortest = tmpsrc;
		};
		tmpsrc = tmp;
		tmp = tmp->prox;
    };
    /* Executa processo e remove ráplica da lista de dados_processo */
    if (beforeshortest == NULL) {
		/* Aloca o primeiro processo caso o mesmo seja menor */
		inicio += tempo;
		tempo += copia->duracao;
		fim = tempo+inicio;
		printf("Id: %d\tduracao: %d\tEspera: %d\tRetorno: %d\n", copia->numero, tempo, inicio, fim);
		tmpsrc = copia;
		copia = copia->prox;
		free(tmpsrc);
    } else {
		/* Aloca o primeiro processo caso não haja 
		ocorrencia de outro menor 
		*/
		tmp = beforeshortest->prox;
		inicio += tempo;
		tempo += tmp->duracao;
		fim = tempo+inicio;
		printf("Id: %d\tduracao: %d\tEspera: %d\tRetorno: %d\n", tmp->numero, tempo, inicio, fim);
		beforeshortest->prox = tmp->prox;
		free(tmp);
    }
  }
	return;
}

main(int argc, char *argv[]){
	FILE *arq; //Criando a vari�vel ponteiro para o arquivo
	char linha_processo[10]; //Linha lida do arquivo
	struct dados_processo *processos, *processo_auxiliar; //Processos lidos do arquivo //Processo auxiliar para nao perder a referencia
	
	//Abrindo o arquivo em modo "somente leitura"
	arq = fopen("processos.txt", "r");
	
	// Se houve erro na abertura
	if (arq == NULL){
	    printf("Problemas na abertura do arquivo\n");
	    return;
	}
	
	//Le o primeiro processo
	fgets(linha_processo, 10, arq);
	processos = init_processos((int)(linha_processo[0] - '0'), (int)(linha_processo[2] - '0'), (int)(linha_processo[4] - '0'));
	processo_auxiliar = processos;
	
	//Enquanto n�o for fim de arquivo o looping ser� executado e ser� lido os demais processos
	while(fgets(linha_processo, 10, arq) != NULL){
		processo_auxiliar->prox = init_processos((int)(linha_processo[0] - '0'), (int)(linha_processo[2] - '0'), (int)(linha_processo[4] - '0'));
		processo_auxiliar = processo_auxiliar->prox;
	}
	
	//Listando os processos	
	listprocs(processos);
	
	// Fechando arquivo
	fclose(arq);
	
	sjf(processos);

	return 0;
}
