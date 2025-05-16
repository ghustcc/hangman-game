#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ranking.h"//Ta dentro o struct memoria e as funções

char buffer[MAX_N_CHAR];//Char global para ler os arquivos

void ranqueamento(char *nome, int pontuacao_player)
{
    int tamanho = contaLinhasRanking();
    int iNome= -1;     // iN é o lugar onde esteja a última pontuação do jogador.Se for -1 é pq esse nome é inédito
    Memoria memoria[tamanho + 1];

    rankingEmVetor(nome, &pontuacao_player,&iNome,memoria);
    
    organizaRanking(nome,pontuacao_player,iNome,&tamanho,memoria);

    escreveArquivo(tamanho,memoria);

    //Para debug do progrma utillize isso + main do rankning
    //   printf("=============Highscores===============");
    //  for (int i = 0; i < tamanho; i++)
    //      printf("\n%d %s %s", i + 1, memoria[i].nome,memoria[i].pontuacao);
}

void rankingEmVetor(char *nome, int *pSomaDasPontuacoes, int *piNome, Memoria memoria[])
{
    FILE *ranking;
    ranking = fopen("ranking.txt", "r");

    int i = 0;
    char *recebeBuffer; // O meio que eu encontrei para colocar os valores na memoria sem sobrescrever o outro

    while (fgets(buffer, MAX_N_CHAR, ranking))
    {
        recebeBuffer = strtok(buffer, "|");
        strncpy(memoria[i].nome, recebeBuffer, MAX_N_CHAR - 1); // Copia do recebe buffer e passa para memoria
        memoria[i].nome[MAX_N_CHAR-1]= '\0';//Acaba a nome na matriz

        recebeBuffer = strtok(NULL, "\n");                   
        strncpy(memoria[i].pontuacao,recebeBuffer,MAX_N_CHAR-1);  // Copia do recebe buffer e passa para memoria; 
        memoria[i].pontuacao[MAX_N_CHAR-1] = '\0';  // Para terminar a string  porntuacção;
                                                               
        memoria[i].pontos=atoi(memoria[i].pontuacao);
        
        if (strcmp(nome, memoria[i].nome) == 0) // Verificar se o há outras jogadas deste jogador e somar a pontuação dele
        {
            *pSomaDasPontuacoes += memoria[i].pontos;
            *piNome = i;
        }
        i++;
    }
    fclose(ranking);
}

void organizaRanking(char *nome,int pontuacao_player, int iNome, int *tamanho, Memoria memoria[]){
    int i = 0;
    int tam = *tamanho;//Para fazer mudar ou não o tamanho dela para escrever no arquivo
    Memoria aux1,aux2;
    
    //Não queremos sobrepassar a pontuação superiores a nossa e quando acabar ou seja pela pontuação anterior ser menor que a atual ou o indice
    while (pontuacao_player < memoria[i].pontos && i < *tamanho)
        i++; 

    aux1 = memoria[i];
    strncpy(memoria[i].nome, nome, MAX_N_CHAR - 1);
    sprintf(memoria[i].pontuacao, "%d", pontuacao_player);
    
    while (i < *tamanho)
    {
        i++;
        aux2 = memoria[i];
        memoria[i] = aux1;
        aux1 = aux2;
        
    }
    
    if (iNome == -1)
    {
         *tamanho = tam + 1;
         
    }else{ 

        iNome++;
        memoria[iNome] = memoria[iNome+1];
        for (int j = iNome; j <= *tamanho; j++)
            memoria[j] = memoria[j+1];
    }

}


void escreveArquivo(int tamanho, Memoria memoria[])
{
    FILE *ranking;
    ranking = fopen("ranking.txt", "w");

    for (int i = 0; i < tamanho; i++)
    {
        fputs(("%s",memoria[i].nome), ranking);
        fputs("|", ranking);
        fputs(("%s",memoria[i].pontuacao), ranking);
        fputs("\n", ranking);
    }

    fclose(ranking);
}

void leitorArquivo()
{
    FILE *ranking;
    ranking = fopen("ranking.txt", "r");

    printf("==========Ranking dos jogadores==========\n");
    int i = 0;
    while (fgets(buffer, MAX_N_CHAR, ranking))
    {
        i++;
        printf("%d %s", i, buffer);
    }

    fclose(ranking);
}

int contaLinhasRanking()
{
    FILE *arquivo;
    arquivo = fopen("ranking.txt", "r");

    int qLinhas = 0;

    // O fgets é o que faz o buffer receber cada linha do arquivo,Enqunto a linha não possuir o fim de texto o contador aumentará
    while (fgets(buffer, MAX_N_CHAR, arquivo))
    {
        qLinhas++;
    }

    fclose(arquivo);
    return qLinhas;
}
