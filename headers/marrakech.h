#ifndef MARRAKECH_H_INCLUDED
#define MARRAKECH_H_INCLUDED
#define TAM 7

typedef struct espaco Espaco;
typedef Espaco* Tabuleiro;
typedef struct assam Assam;
typedef struct jogador Jogador;
typedef Jogador* listaJogadores;

Espaco* criarEspaco();
Tabuleiro* criar();
Assam* inicializarAssam(Tabuleiro*);
void imprimirTabuleiro(Tabuleiro*, Assam*);
Jogador* criarJogador();
listaJogadores* inicializarJogadores();
void girarAssamHorario(Assam**);
void girarAssamAntiHorario(Assam**);
void imprimirJogadorAtual(Jogador*, Assam*);
void avancarJogador(Jogador**);
void fazerJogada(Tabuleiro*, Assam**, Jogador**);
void moverAssam(Assam**, int);
void inserirNaPilha(char[], Espaco*, Jogador*);
int nodeNorthIsExist(Assam **);
int nodeSouthIsExist(Assam **);
int nodeEastIsExist(Assam **);
int nodeWestIsExist(Assam **);
void colocarTapete(Tabuleiro*, Assam**, Jogador**);
void pagarJogador(Tabuleiro*, Assam*, Jogador**);
int fimDeJogo(listaJogadores*);
void verificaVitoria(Tabuleiro*, listaJogadores*, Assam*);
void corRGB(char[], int *, int *, int *);

#endif // MARRAKECH_H_INCLUDED