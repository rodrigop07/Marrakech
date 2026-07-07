#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "../headers/marrakech.h"

int main()
{   
    setlocale(LC_ALL, "pt_BR.UTF-8");
    system("chcp 65001 > nul");
    Tabuleiro *tabuleiro = criar();
    Assam *assam = inicializarAssam(tabuleiro);
    listaJogadores* jogadores = inicializarJogadores();
    Jogador *jogadorAtual = *jogadores;
    int jogoEncerrado = 0;
    while(!jogoEncerrado){
        imprimirTabuleiro(tabuleiro, assam);
        imprimirJogadorAtual(jogadorAtual, assam);
        fazerJogada(tabuleiro, &assam, &jogadorAtual);
        jogoEncerrado = fimDeJogo(jogadores);
    }
    printf("\njogo encerrado!\n");
    verificaVitoria(tabuleiro, jogadores, assam);
    
    return 0;
}