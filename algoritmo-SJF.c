#include <stdio.h>
#include <stdlib.h>

typedef enum { false = 0, true = !false } bool;

//Variaveis globais para serem setadas no SJF e usadas na manipula��o do arquivo
int esperaTotal = 0, vidaTotal = 0, qtProcessos, tempoFinal;

struct dadosProcesso {
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
struct dadosProcesso *iniciaProcessos (int id, int criacao, int duracao, int prioridade);
//Escalonamento SJF
void sjf (struct dadosProcesso *proc);
//Listando os processos
void listaprocessos (struct dadosProcesso *processos);
//Gera arquivo
void geraArquivo(struct dadosProcesso *processos);

main(int argc, char *argv[]){
    FILE *arq; //Criando a variavel ponteiro para o arquivo
    int linhaProcesso[3]; //Linha lida do arquivo
    struct dadosProcesso *processos, *processoAuxiliar; //Processos lidos do arquivo //Processo auxiliar para nao perder a referencia
	int idProcesso = 1; //Variavel para identificar cada processo que entra na fila
	
    //Abrindo o arquivo em modo "somente leitura"
    arq = fopen("processos.txt", "r");

    //Se houve erro na abertura
    if (arq == NULL){
        printf("Problemas na abertura do arquivo\n");
        return;
    }
	
    //Le o primeiro processo
    fscanf(arq, "%d %d %d", &linhaProcesso[0], &linhaProcesso[1], &linhaProcesso[2]);
	//Cria o objeto do primeiro processo  
    processos = iniciaProcessos(idProcesso++, linhaProcesso[0], linhaProcesso[1], linhaProcesso[2]);
	//Atribui o primeiro processo no auxiliar para nao perder a referencia
    processoAuxiliar = processos;

    //Enquanto nao for o fim do arquivo, o looping sera executado e sera lido os demais processos
    while(fscanf(arq, "%d %d %d", &linhaProcesso[0], &linhaProcesso[1], &linhaProcesso[2]) != EOF){
    	//Cria o objeto do processo
        processoAuxiliar->prox = iniciaProcessos(idProcesso++, linhaProcesso[0], linhaProcesso[1], linhaProcesso[2]);
		//Atribui o elemento criado como auxiliar para continuar a lista de processos
		processoAuxiliar = processoAuxiliar->prox;
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
struct dadosProcesso *iniciaProcessos (int id, int criacao, int duracao, int prioridade) {
    struct dadosProcesso *processo;

    //Alocando espaco na memoria para a struct	
    processo = (struct dadosProcesso*)malloc(sizeof(struct dadosProcesso)); 
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
void sjf (struct dadosProcesso *processos) {
    int tempoAtual = 0, menorTempo = INT_MAX; //Setando o menor tempo com o maior numero possivel para qualquer numero ser menor que ele
    struct dadosProcesso *processoAtual, *melhorProcesso = NULL;

	processoAtual = processos;
    while (processoAtual != NULL) {
        //Encontra o proximo processo a ser executado
        while (processoAtual != NULL) {
			//Procura o processo com menor tempo, que ja existe e que nao foi executado
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
			//Executa o processo e seta os dados de vida e espera e coloca como processado
	        melhorProcesso->espera = tempoAtual - melhorProcesso->criacao;
	        esperaTotal += melhorProcesso->espera;
	        tempoAtual += melhorProcesso->duracao;
	        melhorProcesso->vida = tempoAtual - melhorProcesso->criacao;
	        vidaTotal += melhorProcesso->vida;
	        melhorProcesso->processado = true;
	        qtProcessos++;
	        
	        //Reinicia as variaveis para executar a busca novamente
	        menorTempo = INT_MAX;
	        processoAtual = processos;
	        melhorProcesso = NULL;
		}
    }
    
    return;
}

//Listando os processos
void listaprocessos (struct dadosProcesso *processos) {
    printf("Lista de Processos\n");
    printf("\n");
    while (processos != NULL) {
        printf("P%d:\tCriacao: %d\tDuracao: %d\tPrioridade: %d\tEspera: %d\tVida: %d\n", processos->numero, processos->criacao, processos->duracao, processos->prioridade, processos->espera, processos->vida);
        processos = processos->prox;
    }
    printf("\n\n");
}

//Gera arquivo
void geraArquivo(struct dadosProcesso *processos){
	int i;
	float vidaMedia, esperaMedia;
	struct dadosProcesso *processosAuxiliar;
	
	FILE *arq_saida; //Criando a variavel ponteiro para o arquivo
	//Abrindo o arquivo em modo "somente escrita"
	arq_saida = fopen("saidaProcessos.txt", "w");
	
	//Testando a abertura do arquivo
	if (arq_saida == NULL)
	{
		printf("Erro ao tentar abrir o arquivo!");
		exit(1);
	}
	
	//Impresso no arquivo dos dados da materia e alunos
	fputs("\t\tTrabalho - Sistemas Operacionais\n\n", arq_saida);
	fputs("Alunos:\tCesar Augusto Santos Ferreira\n", arq_saida);
	fputs("\tOtavio Augusto Marques Koike\n\n", arq_saida);
	
	vidaMedia = (double)vidaTotal / (double)qtProcessos;
	esperaMedia = (double)esperaTotal / (double)qtProcessos;
	
	//Impresso no arquivo os dados solicitados na atividade
	fprintf(arq_saida, "Tempo medio de vida dos processos: %.2f\n", vidaMedia);
	fprintf(arq_saida, "Tempo medio de espera dos processos: %.2f\n", esperaMedia);
	fprintf(arq_saida, "Numero de trocas de contexto: %d\n\n", qtProcessos -1);
	
	//Impresso no arquivo o diagrama de tempo
	fputs("Diagrama de tempo de execucao:\n\n", arq_saida);
	fputs("\tTempo", arq_saida);
	
	for(i = 1; i <= qtProcessos; i++){
		//Impresso no arquivo os processos
		fprintf(arq_saida, "\tP%d", i);
	}
	fputs("\n", arq_saida);
	
	for(i = 0; i < tempoFinal; i++){
		//Impresso no arquivo os tempos
		fprintf(arq_saida, "\t%02d-%02d", i, i+1);
		
		processosAuxiliar = processos;
		while(processosAuxiliar != NULL){
			if(processosAuxiliar->duracao == 0){
				//Impresso no arquivo "nada" se o processo ja finalizou
				fputs("\t", arq_saida);
			}
			else if(processosAuxiliar->criacao <= i){
				if((processosAuxiliar->criacao + processosAuxiliar->espera) <= i && processosAuxiliar->duracao > 0){
					//Impresso no arquivo "##" se o processo esta em execucao
					fputs("\t##", arq_saida);
					processosAuxiliar->duracao--;
				}else {
					//Impresso no arquivo "--" se o processo existe mas esta em espera
					fputs("\t--", arq_saida);
				}
			}else {
				//Impresso no arquivo "nada" se o processo ainda nao existe
				fputs("\t", arq_saida);
			}
			processosAuxiliar = processosAuxiliar->prox;
		}
		fputs("\n", arq_saida);
	}
	
	//Fechando o arquivo
	fclose(arq_saida);
	
}
