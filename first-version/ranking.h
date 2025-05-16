#pragma once

#define MAX_N_CHAR 255

typedef struct 
{
    char nome[MAX_N_CHAR];
    char pontuacao[MAX_N_CHAR];
    int pontos;
}Memoria;

int contaLinhasRanking();
void ranqueamento(char *nome , int pontuacao_player);
void escreveArquivo(int tamanho , Memoria memoria[]);
void rankingEmVetor(char *nome, int *pSomaDasPontuacoes, int *piNome, Memoria memoria[]);
void  organizaRanking(char *nome,int pontuacao_player, int iNome, int *tamanho, Memoria memoria[]);
void leitorArquivo();
