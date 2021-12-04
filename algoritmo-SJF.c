#include <stdio.h>
#include <stdlib.h>

typedef enum { false = 0, true = !false } bool;

//Variaveis globais para serem setadas no SJF e usadas na manipulação do arquivo
int esperaTotal = 0, vidaTotal = 0, qtProcessos, tempoFinal;

struct dados_processo {
    int numero;             //Numero do processo para melhor identificacao
    int criacao;            //Tempo em que o processo entrou na fila
    int duracao;            //Duracao do processo
    int prioridade;         //Prioridade do processo
    int espera;             //Tempo de espera para ser executado
    int vida;               //Tempo de vida do processo
    bool processado;		//Identificacao se o processo ja foi ou nao processado
    struct processos *prox; //Proximo processo na fila
};

//Criando lista de processos
struct dados_processo *inicia_processos (int id, int criacao, int duracao, int prioridade);
//Escalonamento SJF
void sjf (struct dados_processo *proc);
//Listando os processos
void listaprocessos (struct dados_processo *processos);
//Gera arquivo
void geraArquivo(struct dados_processo *processos);

main(int argc, char *argv[]){
    FILE *arq; //Criando a variavel ponteiro para o arquivo
    char linha_processo[10]; //Linha lida do arquivo
    struct dados_processo *processos, *processo_auxiliar; //Processos lidos do arquivo //Processo auxiliar para nao perder a referencia
	int idProcesso = 1; //Variavel para identificar cada processo que entra na fila
	
    //Abrindo o arquivo em modo "somente leitura"
    arq = fopen("processos.txt", "r");

    //Se houve erro na abertura
    if (arq == NULL){
        printf("Problemas na abertura do arquivo\n");
        return;
    }
	
    //Le o primeiro processo
    fgets(linha_processo, 10, arq);
	//Cria o objeto do primeiro processo    
    processos = inicia_processos(idProcesso++, (int)(linha_processo[0] - '0'), (int)(linha_processo[2] - '0'), (int)(linha_processo[4] - '0'));
	//Atribui o primeiro processo no auxiliar para nao perder a referencia
    processo_auxiliar = processos;

    //Enquanto nao for o fim do arquivo, o looping sera executado e sera lido os demais processos
    while(fgets(linha_processo, 10, arq) != NULL){
    	//Cria o objeto do processo
        processo_auxiliar->prox = inicia_processos(idProcesso++, (int)(linha_processo[0] - '0'), (int)(linha_processo[2] - '0'), (int)(linha_processo[4] - '0'));
		//Atribui o elemento criado como auxiliar para continuar a lista de processos
		processo_auxiliar = processo_auxiliar->prox;
    }
    
    //Fechando arquivo
    fclose(arq);
    
    //Listando os processos	
	listaprocessos(processos);
    
	//Escalonamento SJF    
    sjf(processos);
    
    //Listando os processos	
    listaprocessos(processos);
    
    //Gerar arquivo
    geraArquivo(processos);
	printf("Para mais detalhes abra o arquivo: saidaProcessos.");
    return 0;
}

//Criando lista de processos
struct dados_processo *inicia_processos (int id, int criacao, int duracao, int prioridade) {
    struct dados_processo *processo;

    //Alocando espaco na memoria para a struct	
    processo = (struct dados_processo*)malloc(sizeof(struct dados_processo)); 
    //Caso de erro ao alocar o espaco na memoria
    if (processo == NULL) {
        printf("Erro.");
        exit(1);
    }

    //Preenchimento do objeto    
    processo->numero = id++;
    processo->criacao = criacao;
    processo->duracao = duracao;
    processo->prioridade = prioridade;
    processo->espera = 0;
    processo->vida = 0;
    processo->processado = false;
    processo->prox = NULL;
    return(processo);
}

//Escalonamento SJF
void sjf (struct dados_processo *processos) {
    int tempoAtual = 0, menorTempo = 1000;
    struct dados_processo *processoAtual, *melhorProcesso = NULL;

	processoAtual = processos;
    while (processoAtual != NULL) {
        //Encontra o proximo processo a ser executado
        while (processoAtual != NULL) {
        	
            if (processoAtual->duracao < menorTempo && processoAtual->criacao <= tempoAtual && processoAtual->processado == false) {
                melhorProcesso = processoAtual;
				menorTempo = processoAtual->duracao;
            }
            processoAtual = processoAtual->prox;
        }
        
		//Se todos ja tiverem sido processado, finaliza a funcao
        if(melhorProcesso == NULL){
        	tempoFinal = tempoAtual;
        	return;
		}else {
			//Executa o processo e seta como processado
	        melhorProcesso->espera = tempoAtual - melhorProcesso->criacao;
	        esperaTotal += melhorProcesso->espera;
	        tempoAtual += melhorProcesso->duracao;
	        melhorProcesso->vida = tempoAtual - melhorProcesso->criacao;
	        vidaTotal += melhorProcesso->vida;
	        melhorProcesso->processado = true;
	        qtProcessos++;
	        
	        //Reinicia as variaveis para executar novamente
	        menorTempo = 1000;
	        processoAtual = processos;
	        melhorProcesso = NULL;	
		}
    }
    
    return;
}

//Listando os processos
void listaprocessos (struct dados_processo *processos) {
    printf("Lista de Processos\n");
    printf("\n");
    while (processos != NULL) {
        printf("P%d:\tCriacao: %d\tDuracao: %d\tPrioridade: %d\tEspera: %d\tVida: %d\n", processos->numero, processos->criacao, processos->duracao, processos->prioridade, processos->espera, processos->vida);
        processos = processos->prox;
    }
    printf("\n\n");
}

//Gera arquivo
void geraArquivo(struct dados_processo *processos){
	int i;
	float vidaMedia, esperaMedia;
	struct dados_processo *processosAuxiliar;
	
	FILE *arq_saida;
	arq_saida = fopen("saidaProcessos.txt", "w");
	
	//Testando a abertura do arquivo
	if (arq_saida == NULL)
	{
		printf("Erro ao tentar abrir o arquivo!");
		exit(1);
	}
	
	fputs("\t\tTrabalho - Sistemas Operacionais\n\n", arq_saida);
	fputs("Alunos:\tCesar Augusto Santos Ferreira\n", arq_saida);
	fputs("\tOtavio Augusto Marques Koike\n\n", arq_saida);
	
	vidaMedia = (double)vidaTotal / (double)qtProcessos;
	esperaMedia = (double)esperaTotal / (double)qtProcessos;
	
	fprintf(arq_saida, "Tempo medio de vida dos processos: %.2f\n", vidaMedia);
	fprintf(arq_saida, "Tempo medio de espera dos processos: %.2f\n", esperaMedia);
	fprintf(arq_saida, "Numero de trocas de contexto: %d\n\n", qtProcessos -1);
	
	fputs("Diagrama de tempo de execucao:\n\n", arq_saida);
	fputs("\tTempo", arq_saida);
	
	for(i = 1; i <= qtProcessos; i++){
		fprintf(arq_saida, "\tP%d", i);
	}
	fputs("\n", arq_saida);
	
	for(i = 0; i < tempoFinal; i++){
		fprintf(arq_saida, "\t%02d-%02d", i, i+1);
		
		processosAuxiliar = processos;
		while(processosAuxiliar != NULL){
			if(processosAuxiliar->duracao == 0){
				fputs("\t", arq_saida);
			}
			else if(processosAuxiliar->criacao <= i){
				if((processosAuxiliar->criacao + processosAuxiliar->espera) <= i && processosAuxiliar->duracao > 0){
					fputs("\t##", arq_saida);
					processosAuxiliar->duracao--;
				}else {
					fputs("\t--", arq_saida);
				}
			}else {
				fputs("\t", arq_saida);
			}
			processosAuxiliar = processosAuxiliar->prox;
		}
		fputs("\n", arq_saida);
	}
	
}
