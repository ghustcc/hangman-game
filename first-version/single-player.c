#include <stdio.h> 
#include <string.h> 
#include <locale.h> 
#include <time.h> 
#include <stdlib.h>
#include <ctype.h> // Para isalpha() e strchr()
#include <windows.h> 
//----------------------------------------------------
#include "palavras.h"
#include "interface.h"
#include "single-player.h"
#include "ranking.h"
//----------------------------------------------------

// Váriavies globais
#define TAM_NOME 101
#define MAX_ERROS 6
#define TAM_PALAVRA 31
#define TAM_DICA 100
#define ESPERAR 3000 // Tempo de espera em segundos (para Sleep)

void singlePlayerJogo() 
{
    setlocale(LC_ALL, "Portuguese_Brazil");

    // Definição de variáveis
    //----------------------------------------------------
    char *palavra; // Variável para armazenar a palavra escolhida
    char *dica; // Variável para armazenar a dica da palavra escolhida
    char *dificuldade; // Variável para armazenar a dificuldade da palavra escolhida
    int pontuacao_player = 0; // Variável para armazenar a pontuação do jogador
    char letra[TAM_PALAVRA]; // Variável para armazenar a letra digitada pelo usuário
    char nome_player[TAM_NOME];
    char jogador1[TAM_NOME], jogador2[TAM_NOME];
	char letras_testadas[27];  // 27 posições, cada uma pode armazenar 1 letra + '\0'
	int erros = 0; // Contador de erros
	int acertos = 0; // Contador de acertos
	int i, pos; // Operar sobre o vetor de letras testadas
    char partes_do_corpo[6] = {'0', '|', '/', '\\', '/', '\\'}; // Partes do corpo para poder alterar a imagem do boneco
    char errou_palavra = ' ';
    
    temArquivo(); // Verfica se existe o arquivo.txt
    palavraAleatoria(&palavra, &dica); // Inicia qual será a palavra e a dica
	int tamanho = strlen(palavra);
    //----------------------------------------------------
    
    // Instancias importantes
    //----------------------------------------------------
    // Inicializa letras_testadas com strings vazias e adicona o caractere nulo (operador nulo) em cada elemento.
    for(i = 0; i < 27; i++){
        letras_testadas[i] = '\0';
    }

	char descoberta[tamanho + 1]; // Vetor para armazenar e mostrar as letras descobertas (corretas)
    
    // Inicializa o vetor descoberta com '_' e adiciona o caractere nulo no final
	for(i = 0; i < tamanho; i++){
        descoberta[i]= '_';
	}
	descoberta[tamanho] = '\0';
    //----------------------------------------------------

    //----------------------------------------------------
    dificuldadeDaPalavra(tamanho, &dificuldade); // Define a dificuldade da palavra
    //----------------------------------------------------

    // Início do jogo, obtençao do nome do jogador ou retornar ao menu principal
    //----------------------------------------------------
    if (telaSingleplayer(nome_player, TAM_NOME) == 1) {
        return; // Se o usuário quiser voltar, sai da função SinglePlayerJogo
    }

    // Loop do jogo
    //----------------------------------------------------
	while(erros < MAX_ERROS && acertos < tamanho)
    {
        telaSingleplayerJogo(letra, dica, letras_testadas, descoberta, acertos, erros, MAX_ERROS, tamanho, partes_do_corpo, TAM_PALAVRA, dificuldade);
        
        if (strlen(letra) == 1)
        {
            if(!isalpha(letra[0])) {
                inputErroLogicaUsuario("A entrada deve ser uma letra. Caractere invalido! ", partes_do_corpo);
            }
            else if(strchr(palavra, letra[0]) != 0) { // Compara cacharacteres da palavra com a letra digitada
                if (letraTestada(letra, letras_testadas) != 0) {
                    inputErroLogicaUsuario("Essa letra ja foi testada. Tente uma diferente! ", partes_do_corpo);
                } else {
                    for (i = 0; i <= tamanho; i++) {
                        if (palavra[i] == letra[0] && descoberta[i] == '_') {
                            descoberta[i] = letra[0]; 
                            acertos++;
                        }
                    } 
                } 
                // Verifica se a letra já foi testada (se está em letras_testadas); se não, adiciona na próxima posição livre de letras_testadas
                pos = -1;
                for (i = 0; i < 26; i++) {
                    if (letras_testadas[i] == letra[0]) {
                        pos = i;
                        break;
                    } 
                    if (letras_testadas[i] == '\0') {
                        pos = i;
                        break;
                    }
                }
                if (pos != -1 && letras_testadas[pos] == '\0') {
                    letras_testadas[pos] = letra[0];
                }
            } else {
                if (letraTestada(letra, letras_testadas) != 0) {
                    inputErroLogicaUsuario("Essa letra ja foi testada. Tente uma diferente! ", partes_do_corpo);
                } else {
                    inputErroLogicaUsuario("Errou! Essa letra nao esta na palavra! ", partes_do_corpo);
                    erros++;
                    for (i = MAX_ERROS - 1; i >= MAX_ERROS - erros; i--) 
                    {
                        partes_do_corpo[i] = errou_palavra; // Atualiza a parte do corpo do boneco
                    }
                }
            }
        } else if (strlen(letra) == tamanho) {
            if (strcmp(letra, palavra) == 0) {
                acertos = tamanho; // Define o número de acertos como o tamanho da palavra
            } else {
                inputErroLogicaUsuario("Errou! Essa palavra nao eh a correta! ", partes_do_corpo);
                erros++;
                for (i = MAX_ERROS - 1; i >= MAX_ERROS - erros; i--) 
                {
                    partes_do_corpo[i] = errou_palavra; // Atualiza a parte do corpo do boneco
                }
            }
        } else if (strlen(letra) != 1 || strlen(letra) != tamanho) {
            inputErroLogicaUsuario("Tente uma letra ou a palavra inteira! ", partes_do_corpo);
        }
	}
	if (acertos == tamanho) {
        // Pontuação: tamanho da palavra * (tantativas - quantidade de erros) * 100 - (quantidade de letras repetidas na palavra * 100) 
        pontuacao_player = tamanho * (MAX_ERROS - erros) * 100 - (quantidadeLetrasRepetidasPalavra(tamanho, palavra) * 100); // Cálculo da pontuação
		telaResultadoJogo(partes_do_corpo, palavra, acertos, erros, letras_testadas, "    CONGRATULATIONS    ", "Parabens! Voce acertou a palavra e ganhou o jogo!",nome_player, pontuacao_player);
        ranqueamento(nome_player,pontuacao_player);

	} else {
        pontuacao_player = 0;
		telaResultadoJogo(partes_do_corpo, palavra, acertos, erros, letras_testadas, "      GAME OVER      ", "Sadness! Voce perdeu o jogo! ", nome_player, pontuacao_player);
	}
}

int letraTestada(char *letra, char *letras_testadas) {
    for (int i = 0; i < strlen(letras_testadas); i++) 
    {
        if (toupper(letras_testadas[i]) == letra[0]) {
            return 1; // A letra já foi testada
        }
    }
    return 0;
}

void dificuldadeDaPalavra(int tamanho, char **dificuldade) {
    if (tamanho <= 4) {
        *dificuldade = "Facil";
    } else if (tamanho <= 7) {
        *dificuldade = "Medio";
    } else {
        *dificuldade = "Dificil";
    }
}

int quantidadeLetrasRepetidasPalavra(int tamanho, char *palavra) {
    int i, j, cont, rep = 0;
    for (i = 0; i < tamanho; i++) {
        cont = 0;
        for (j = 0 + 1; j < tamanho; j++) {
            if (palavra[i] == palavra[j]) {
                cont++;
            }
        }
        if (cont > 1) {
            rep++;
        }
    }
    if (rep == 0) 
        return 1;
    else
        return rep;
}
