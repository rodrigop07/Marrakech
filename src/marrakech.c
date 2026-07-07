#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>
#include "../headers/marrakech.h"

typedef struct tapete{
    char cor[10];
    int numTap;
    struct tapete *metade;
}Tapete;

typedef struct noPilha{
    Tapete tapete;
    struct noPilha* prox;
}NoPilha;

typedef struct{
    int tam;
    NoPilha* topo;
}Pilha;

typedef struct espaco{
    Pilha* tapetes;
    struct espaco* norte;
    struct espaco* sul;
    struct espaco* leste;
    struct espaco* oeste;
}Espaco;

typedef struct assam{
    int linha;
    int coluna;
    char orientacao[6];
    Espaco* posicao;
}Assam;

typedef struct jogador{
    int dinheiro;
    int tapetes;
    int jogando;
    char cor[10];
    int ptTotais;
    struct jogador *prox;
}Jogador;

Pilha* criarPilhaTapetes(){
    Pilha* novaPilha = (Pilha*)malloc(sizeof(Pilha));
    novaPilha->tam = 0;
    novaPilha->topo = NULL;

    return novaPilha;
}

Espaco* criarEspaco(){
    Espaco* novoEspaco = (Espaco*)malloc(sizeof(Espaco));
    novoEspaco->norte = NULL;
    novoEspaco->sul = NULL;
    novoEspaco->leste = NULL;
    novoEspaco->oeste = NULL;
    novoEspaco->tapetes = criarPilhaTapetes();

    return novoEspaco;
}

Assam* inicializarAssam(Tabuleiro *tabuleiro){
    Assam *novoAssam = (Assam*)malloc(sizeof(Assam));

    novoAssam->linha = (TAM + 1)/2;
    novoAssam->coluna = (TAM + 1)/2;

    srand(time(NULL));
    int or = rand() % 4;
    if(or == 0)
        strcpy(novoAssam->orientacao, "Norte");
    else if(or == 1)
        strcpy(novoAssam->orientacao, "Sul");
    else if(or == 2)
        strcpy(novoAssam->orientacao, "Leste");
    else if(or == 3)
        strcpy(novoAssam->orientacao, "Oeste");

    Espaco *atual = *tabuleiro;
    for(int i = 1; i < (TAM + 1)/2; i++){
        atual = atual->leste;
        atual = atual->sul;
    }
    novoAssam->posicao = atual;

    return novoAssam;
}

Tabuleiro* criar(){
    Tabuleiro* tabuleiro = (Tabuleiro*)malloc(sizeof(Tabuleiro));
    Espaco* anterior = NULL;
    Espaco* linhaAnterior = NULL;
    Espaco* inicioLinhaAnterior = NULL;
    Espaco* inicioLinhaAtual = NULL;

    for(int i = 0; i < TAM; i++){
        for(int j = 0; j < TAM; j++){
            Espaco* novoEspaco = criarEspaco();
            if(i == 0 && j == 0){
                *tabuleiro = novoEspaco;
            }

            if(j > 0){
                anterior->leste = novoEspaco;
                novoEspaco->oeste = anterior;
            }
            
            if(i > 0){
                novoEspaco->norte = linhaAnterior;
                linhaAnterior->sul = novoEspaco;
                linhaAnterior = linhaAnterior->leste;
            }

            novoEspaco->tapetes = criarPilhaTapetes();

            if(j == 0)
                inicioLinhaAtual = novoEspaco;
            anterior = novoEspaco;
        }
        inicioLinhaAnterior = inicioLinhaAtual;
        linhaAnterior = inicioLinhaAnterior;
    }

    Espaco *atual = *tabuleiro;
    while(atual->leste != NULL){
        if(atual->norte == NULL){
            atual->norte = atual->leste;
            atual->leste->norte = atual;
            atual = atual->leste;
        }else
            atual = atual->leste;
    }
    if(atual->norte == NULL){
        atual->norte = atual;
        atual->leste = atual;
    }else{
        atual->leste = atual->sul;
        atual->sul->leste = atual;
    }

    atual = atual->sul;
    while(atual->sul != NULL){
        if(atual->leste == NULL){
            atual->leste = atual->sul;
            atual->sul->leste = atual;
            atual = atual->sul;
        }else
            atual = atual->sul;
    }
    if(atual->leste == NULL){
        atual->leste = atual;
        atual->sul = atual;
    }else{
        atual->sul = atual->oeste;
        atual->oeste->sul = atual;
    }

    atual = atual->oeste;
    while(atual->oeste != NULL){
        if(atual->sul == NULL){
            atual->sul = atual->oeste;
            atual->oeste->sul = atual;
            atual = atual->oeste;
        }else
            atual = atual->oeste;
    }
    if(atual->sul == NULL){
        atual->sul = atual;
        atual->oeste = atual;
    }else{
        atual->oeste = atual->norte;
        atual->norte->oeste = atual;
    }

    atual = atual->norte;
    while(atual->norte != atual->leste){
        if(atual->oeste == NULL){
            atual->oeste = atual->norte;
            atual->norte->oeste = atual;
            atual = atual->norte;
        }else{
            atual = atual->norte;
        }
    }

    return tabuleiro;
}

Jogador* criarJogador(){
    Jogador *novoJogador = (Jogador*)malloc(sizeof(Jogador));
    strcpy(novoJogador->cor, "\0");
    novoJogador->dinheiro = 30;
    novoJogador->jogando = 1;
    novoJogador->ptTotais = 0;
    novoJogador->prox = NULL;
    novoJogador->tapetes = 12;

    return novoJogador;
}

listaJogadores* inicializarJogadores(){
    int qtd;
    printf(" Quantos jogadores jogarão? (2-4): ");
    scanf("%d", &qtd);
    while(qtd < 2 || qtd > 4){
        system("cls");
        fflush(stdin);
        printf(" Escolha uma opção válida: ");
        scanf("%d", &qtd);
    }

    listaJogadores *novosJogadores = (listaJogadores*)malloc(sizeof(listaJogadores));

    Jogador *jogador1 = criarJogador();
    Jogador *jogador2 = criarJogador();

    if(qtd == 2){
        strcpy(jogador1->cor, "Vermelho");
        strcpy(jogador2->cor, "Azul");
    
        jogador1->prox = jogador2;
        jogador2->prox = jogador1;  
    }else if(qtd == 3){
        Jogador *jogador3 = criarJogador();

        strcpy(jogador1->cor, "Vermelho");
        strcpy(jogador2->cor, "Azul");
        strcpy(jogador3->cor, "Amarelo");

        jogador1->prox = jogador2;
        jogador2->prox = jogador3;
        jogador3->prox = jogador1;
    }else if(qtd == 4){
        Jogador *jogador3 = criarJogador();
        Jogador *jogador4 = criarJogador();
    
        strcpy(jogador1->cor, "Vermelho");
        strcpy(jogador2->cor, "Azul");
        strcpy(jogador3->cor, "Amarelo");
        strcpy(jogador4->cor, "Verde");
        
        jogador1->prox = jogador2;
        jogador2->prox = jogador3;
        jogador3->prox = jogador4;
        jogador4->prox = jogador1;
    }

    *novosJogadores = jogador1;

    return novosJogadores;
}

void girarAssamHorario(Assam **assam){
    if(strcmp((*assam)->orientacao, "Norte") == 0){
        strcpy((*assam)->orientacao, "Leste");
    }else if(strcmp((*assam)->orientacao, "Leste") == 0){
        strcpy((*assam)->orientacao, "Sul");
    }else if(strcmp((*assam)->orientacao, "Sul") == 0){
        strcpy((*assam)->orientacao, "Oeste");
    }else if(strcmp((*assam)->orientacao, "Oeste") == 0){
        strcpy((*assam)->orientacao, "Norte");
    }
}

void girarAssamAntiHorario(Assam **assam){
    if(strcmp((*assam)->orientacao, "Norte") == 0){
        strcpy((*assam)->orientacao, "Oeste");
    }else if(strcmp((*assam)->orientacao, "Oeste") == 0){
        strcpy((*assam)->orientacao, "Sul");
    }else if(strcmp((*assam)->orientacao, "Sul") == 0){
        strcpy((*assam)->orientacao, "Leste");
    }else if(strcmp((*assam)->orientacao, "Leste") == 0){
        strcpy((*assam)->orientacao, "Norte");
    }
}

void imprimirTabuleiro(Tabuleiro* tabuleiro, Assam* assam){
    int r, g, b;
    Espaco *inicioLinha = *tabuleiro;
    Espaco *atual = inicioLinha;

    printf("\n       ");
    for(int i = 0; i < TAM; i++)
        printf("%c    ", i + 65);
    printf("\n   ");
    for(int i = 0; i < TAM; i++){
        if(atual->norte == atual->oeste)
            printf("    \u2190");
        else if(atual->norte == atual->leste)
            printf("    \u2192");
        atual = atual->leste;
    }
    printf("\n     ");

    for(int i = 0; i < TAM; i++)
        printf("+----");
    printf("+");
    for(int i = 0; i < TAM; i++){
        printf("\n");
        if(i < 9)
            printf(" %d ", i + 1);
        else
            printf("%d ", i + 1);
        atual = inicioLinha;
        inicioLinha = inicioLinha->sul;
        if(atual->oeste == atual->norte)
            printf("\u2191 ");
        else if(atual->oeste == atual->sul)
            printf("\u2193 ");
        for(int j = 0; j < TAM; j++){
            printf("|");
            if(assam->posicao == atual){
                if(atual->tapetes->topo != NULL)
                    corRGB(atual->tapetes->topo->tapete.cor, &r, &g, &b);
                else
                    corRGB("", &r, &g, &b);
                if(strcmp(assam->orientacao, "Norte") == 0){
                    printf("\x1b[38;2;%d;%d;%dm \u2191  \x1b[0m", r, g, b);
                }else if(strcmp(assam->orientacao, "Leste") == 0){
                    printf("\x1b[38;2;%d;%d;%dm \u2192  \x1b[0m", r, g, b);
                }else if(strcmp(assam->orientacao, "Sul") == 0){
                    printf("\x1b[38;2;%d;%d;%dm \u2193  \x1b[0m", r, g, b);
                }else if(strcmp(assam->orientacao, "Oeste") == 0){
                    printf("\x1b[38;2;%d;%d;%dm \u2190  \x1b[0m", r, g, b);
                }
            }else if(atual->tapetes->tam != 0){
                corRGB(atual->tapetes->topo->tapete.cor, &r, &g, &b);
                if(atual->tapetes->topo->tapete.numTap < 10)
                    printf("  \x1b[38;2;%d;%d;%dm%d \x1b[0m", r, g, b, atual->tapetes->topo->tapete.numTap);
                else
                printf(" \x1b[38;2;%d;%d;%dm%d \x1b[0m", r, g, b, atual->tapetes->topo->tapete.numTap);
            }else
                printf("    ");
            atual = atual->leste;
        }
        atual = atual->leste;
        printf("|");
        if(atual->leste == atual->norte)
            printf(" \u2191");
        else if(atual->leste == atual->sul)
            printf(" \u2193");
        printf("\n     ");
        for(int i = 0; i < TAM; i++)
            printf("+----");
        printf("+");
    }
    printf("\n   ");
    atual = inicioLinha;
    for(int i = 0; i < TAM; i++){
        if(atual->sul == atual->leste)
            printf("    \u2192");
        else if(atual->sul == atual->oeste)
            printf("    \u2190");
        atual = atual->leste;
    }
}

void imprimirJogadorAtual(Jogador *jogadorAtual, Assam *assam){
    int r, g, b;
    corRGB(jogadorAtual->cor, &r, &g, &b);

    printf("\n\n    ╔═══════════════════════════════════╗");
    printf("\n    ║");
    for(int i = 0; i < (35 - 15 - strlen(jogadorAtual->cor))/2; i++)
        printf(" ");
    printf("\x1b[38;2;%d;%d;%dmVez do jogador %s\x1b[0m", r, g, b, jogadorAtual->cor);
    for(int i = 0; i < (36 - 15 - strlen(jogadorAtual->cor))/2; i++)
        printf(" ");
    printf("║");
    printf("\n    ╠═══════════════════════════════════╣");
    printf("\n    ║ \x1b[38;2;%d;%d;%dmMoedas restantes\x1b[0m: %-16d║", r, g, b, jogadorAtual->dinheiro);
    printf("\n    ║ \x1b[38;2;%d;%d;%dmTapetes restantes\x1b[0m: %-15d║", r, g, b, jogadorAtual->tapetes);
    printf("\n    ║ \x1b[38;2;%d;%d;%dmOrientação do Assam\x1b[0m: %-13s║", r, g, b, assam->orientacao);
    printf("\n    ╚═══════════════════════════════════╝");
}

void avancarJogador(Jogador **jogadorAtual){
    do{
        *jogadorAtual = (*jogadorAtual)->prox;
    }while((*jogadorAtual)->jogando == 0);
}

void moverAssam(Assam **assam, int resultado){
    for(int i = 0; i < resultado; i++){
        if((*assam)->linha == 1 && strcmp((*assam)->orientacao, "Norte") == 0){
            if((*assam)->posicao != (*assam)->posicao->norte){
                if((*assam)->posicao->norte == (*assam)->posicao->oeste){
                    (*assam)->posicao = (*assam)->posicao->norte;
                    strcpy((*assam)->orientacao, "Sul");
                    (*assam)->coluna--;
                }else if((*assam)->posicao->norte == (*assam)->posicao->leste){
                    (*assam)->posicao = (*assam)->posicao->norte;
                    strcpy((*assam)->orientacao, "Sul");
                    (*assam)->coluna++;
                }
            }else{
                if((*assam)->posicao->norte == (*assam)->posicao->oeste){
                    strcpy((*assam)->orientacao, "Leste");
                }else if((*assam)->posicao->norte == (*assam)->posicao->leste){
                    strcpy((*assam)->orientacao, "Oeste");
                }
            }
        }else if((*assam)->coluna == TAM && strcmp((*assam)->orientacao, "Leste") == 0){
            if((*assam)->posicao != (*assam)->posicao->leste){
                if((*assam)->posicao->leste == (*assam)->posicao->norte){
                    (*assam)->posicao = (*assam)->posicao->leste;
                    strcpy((*assam)->orientacao, "Oeste");
                    (*assam)->linha--;
                }else if((*assam)->posicao->leste == (*assam)->posicao->sul){
                    (*assam)->posicao = (*assam)->posicao->leste;
                    strcpy((*assam)->orientacao, "Oeste");
                    (*assam)->linha++;
                }
            }else{
                if((*assam)->posicao->leste == (*assam)->posicao->norte){
                    strcpy((*assam)->orientacao, "Sul");
                }else if((*assam)->posicao->leste == (*assam)->posicao->sul){
                    strcpy((*assam)->orientacao, "Norte");
                }
            }
        }else if((*assam)->linha == TAM && strcmp((*assam)->orientacao, "Sul") == 0){
            if((*assam)->posicao != (*assam)->posicao->sul){
                if((*assam)->posicao->sul == (*assam)->posicao->oeste){
                    (*assam)->posicao = (*assam)->posicao->sul;
                    strcpy((*assam)->orientacao, "Norte");
                    (*assam)->coluna--;
                }else if((*assam)->posicao->sul == (*assam)->posicao->leste){
                    (*assam)->posicao = (*assam)->posicao->sul;
                    strcpy((*assam)->orientacao, "Norte");
                    (*assam)->coluna++;
                }
            }else{
                if((*assam)->posicao->sul == (*assam)->posicao->oeste){
                    strcpy((*assam)->orientacao, "Leste");
                }else if((*assam)->posicao->sul == (*assam)->posicao->leste){
                    strcpy((*assam)->orientacao, "Oeste");
                }
            }
        }else if((*assam)->coluna == 1 && strcmp((*assam)->orientacao, "Oeste") == 0){
            if((*assam)->posicao != (*assam)->posicao->oeste){
                if((*assam)->posicao->oeste == (*assam)->posicao->norte){
                    (*assam)->posicao = (*assam)->posicao->oeste;
                    strcpy((*assam)->orientacao, "Leste");
                    (*assam)->linha--;
                }else if((*assam)->posicao->oeste == (*assam)->posicao->sul){
                    (*assam)->posicao = (*assam)->posicao->oeste;
                    strcpy((*assam)->orientacao, "Leste");
                    (*assam)->linha++;
                }
            }else{
                if((*assam)->posicao->oeste == (*assam)->posicao->norte){
                    strcpy((*assam)->orientacao, "Sul");
                }else if((*assam)->posicao->oeste == (*assam)->posicao->sul){
                    strcpy((*assam)->orientacao, "Norte");
                }
            }
        }else{
            if(strcmp((*assam)->orientacao, "Norte") == 0){
                (*assam)->posicao = (*assam)->posicao->norte;
                (*assam)->linha--;
            }else if(strcmp((*assam)->orientacao, "Leste") == 0){
                (*assam)->posicao = (*assam)->posicao->leste;
                (*assam)->coluna++;
            }else if(strcmp((*assam)->orientacao, "Sul") == 0){
                (*assam)->posicao = (*assam)->posicao->sul;
                (*assam)->linha++;
            }else if(strcmp((*assam)->orientacao, "Oeste") == 0){
                (*assam)->posicao = (*assam)->posicao->oeste;
                (*assam)->coluna--;
            }
        }
    }
}

void fazerJogada(Tabuleiro *tabuleiro, Assam **assam, Jogador **jogadorAtual){
    int girarAssam, direcao, r, g, b;
    corRGB((*jogadorAtual)->cor, &r, &g, &b);
    printf("\n\n    >>>>>>>>>\x1b[38;2;%d;%d;%dmRotacionar Assam?\x1b[0m<<<<<<<<<\n", r, g, b);
    printf("    [0] \x1b[38;2;%d;%d;%dmNão\x1b[0m\n    [1] \x1b[38;2;%d;%d;%dmSim\n    Opção\x1b[0m: ", r, g, b, r, g, b);
    scanf("%d", &girarAssam);
    while(getchar() != '\n');

    if(girarAssam != 0 && girarAssam != 1){
        printf("\n    Digite uma opção válida: ");
        do{
            fflush(stdin);
            scanf("  %d", &girarAssam);
            while(getchar() != '\n');
        }while(girarAssam != 0 && girarAssam != 1);
    }

    if(girarAssam == 1){
        printf("\n    >>>>>>>>>\x1b[38;2;%d;%d;%dmEm qual sentido?\x1b[0m<<<<<<<<<", r, g, b);
        printf("\n    [0] \x1b[38;2;%d;%d;%dmAnti horário\x1b[0m\n    [1] \x1b[38;2;%d;%d;%dmHorário\x1b[0m", r, g, b, r, g, b);
        printf("\n    \x1b[38;2;%d;%d;%dmOpção\x1b[0m: ", r, g, b);
        scanf("%d", &direcao);
        while(getchar() != '\n');

        if(girarAssam != 0 && girarAssam != 1){
            printf("\n    Digite uma opção válida: ");
            do{
                fflush(stdin);
                scanf("  %d", &direcao);
                while(getchar() != '\n');
            }while(girarAssam != 0 && girarAssam != 1);
        }

        if(direcao == 0)
            girarAssamAntiHorario(assam);
        if(direcao == 1)
            girarAssamHorario(assam);
    }

    system("cls");
    imprimirTabuleiro(tabuleiro, *assam);
    imprimirJogadorAtual(*jogadorAtual, *assam);
    printf("\n\n    Pressione enter para rolar o dado");
    getchar();
    printf("\n    Rolando dado...");
    Sleep(800);

    int resultado = rand() % 6;
    if(resultado == 0)
        resultado = 1;
    else if(resultado == 1 || resultado == 2)
        resultado = 2;
    else if(resultado == 3 || resultado == 4)
        resultado = 3;
    else
        resultado = 4;

    printf("\n    Resultado: %d\n", resultado);
    Sleep(600);
    for(int i = 0; i < resultado; i++){
        system("cls");
        printf("\n    Rolando dado...");
        printf("\n    Resultado: %d\n", resultado);
        moverAssam(assam, 1);
        imprimirTabuleiro(tabuleiro, *assam);
        Sleep(600);
    }
    
    imprimirJogadorAtual(*jogadorAtual, *assam);
    pagarJogador(tabuleiro, *assam, jogadorAtual);

    colocarTapete(tabuleiro, assam, jogadorAtual);
    system("cls");
    avancarJogador(jogadorAtual);
}

void inserirNaPilha(char cor[], Espaco *espaco, Jogador *jogador){
    if(espaco == NULL || cor == NULL){
        return;
    }
    
    NoPilha *novo = (NoPilha*)malloc(sizeof(NoPilha));
    if(novo == NULL){
        return;
    }

    strcpy(novo->tapete.cor, cor);
    novo->tapete.numTap = 12 - jogador->tapetes + 1;

    if(espaco->tapetes->topo == NULL){
        espaco->tapetes->topo = novo;
        novo->prox = NULL;
    }else{
        novo->prox = espaco->tapetes->topo;
        espaco->tapetes->topo = novo;
    }

    espaco->tapetes->tam++;
}

//verificam se o nó está dentro do limite do tabuleiro
int nodeNorthIsExist(Assam **assam){
    if((*assam) == NULL) return -1;
    if(((*assam)->linha) - 1 <= TAM && ((*assam)->linha) - 1 >= 1){
        return 1;
    }
}

int nodeSouthIsExist(Assam **assam){
    if((*assam) == NULL) return -1;
    if(((*assam)->linha) + 1 <= TAM && ((*assam)->linha) + 1 >= 1){
        return 1;
    }
}
//oeste
int nodeWestIsExist(Assam **assam){
    if((*assam) == NULL) return -1;
    if(((*assam)->coluna) - 1 <= TAM && ((*assam)->coluna) - 1 >= 1){
        return 1;
    }
}
//lest
int nodeEastIsExist(Assam **assam){
    if((*assam) == NULL) return -1;
    if(((*assam)->coluna) + 1 <= TAM && ((*assam)->coluna) + 1 >= 1){
        return 1;
    }
}

void colocarTapete(Tabuleiro *tabuleiro, Assam **assam, Jogador **jogadorAtual) {
    int l1, l2, r, g, b;
    char c1, c2, entrada[6];
    Espaco *auxL = *tabuleiro;
    Espaco *auxC, *tap1, *tap2;
    corRGB((*jogadorAtual)->cor, &r, &g, &b);
    while(1) {
        printf("\n    Escolha da \x1b[38;2;%d;%d;%dmprimeira coordenada\x1b[0m:\n    (\x1b[38;2;%d;%d;%dmLinha\x1b[0m, \x1b[38;2;%d;%d;%dmColuna\x1b[0m): ", r, g, b, r, g, b, r, g, b);
        fgets(entrada, sizeof(entrada), stdin);
        if(sscanf(entrada, "%d%c", &l1, &c1) == 2){
            c1 = toupper(c1);
        }else if(sscanf(entrada, "%c%d", &c1, &l1) == 2){
            c1 = toupper(c1);
        }else{
            printf("\n  Formato inválido\n");
            continue;
        }
        if((l1 < 1 || l1 > TAM) || (c1-64 < 1 || c1-64 > TAM)) {
            printf("  Coordenadas informadas invalidas. Tente novamente.\n");
            continue;
        }

        while(1) {
            printf("\n    Escolha da \x1b[38;2;%d;%d;%dmsegunda coordenada\x1b[0m:\n  (\x1b[38;2;%d;%d;%dmLinha\x1b[0m, \x1b[38;2;%d;%d;%dmColuna\x1b[0m): ", r, g, b, r, g, b, r, g, b);
            fgets(entrada, sizeof(entrada), stdin);
            if(sscanf(entrada, "%d%c", &l2, &c2) == 2){
                c2 = toupper(c2);
            }else if(sscanf(entrada, "%c%d", &c2, &l2) == 2){
                c2 = toupper(c2);
            }else{
                printf("\n    Formato inválido\n");
                continue;
            }
            if((l2 < 1 || l2 > TAM) || (c2-64 < 1 || c2-64 > TAM)) {
                printf("    Coordenadas informadas invalidas. Tente novamente.\n");
                continue;
            }

            break;           
        }

        auxL = *tabuleiro;
        for(int i=1; i<=TAM; i++) {
            auxC = auxL;
            if(l1 == i) {
                for(int j=1; j<=TAM; j++) {
                    if(j == c1-64){
                        tap1 = auxC;
                    }
                    auxC = auxC->leste;
                }    
            }
            auxL = auxL->sul;
        }
    
        auxL = *tabuleiro;
        for(int i=1; i<=TAM; i++) {
            auxC = auxL;
            if(l2 == i) {
                for(int j=1; j<=TAM; j++) {
                    if(j == c2-64){
                        tap2 = auxC;
                    }
                    auxC = auxC->leste;
                }    
            }
            auxL = auxL->sul;
        }
        if((*assam)->posicao->norte != tap1 && (*assam)->posicao->leste != tap1 && (*assam)->posicao->sul != tap1 && (*assam)->posicao->oeste != tap1) {
            if((*assam)->posicao->norte != tap2 && (*assam)->posicao->leste != tap2 && (*assam)->posicao->sul != tap2 && (*assam)->posicao->oeste != tap2) {
                printf("\n    Pelo menos uma das informacoes passadas deve ser adjacente ao Asaam. Tente novamente:");
                continue;
            }
        }
        if(tap1->norte != tap2 && tap1->leste != tap2 && tap1->sul != tap2 && tap1->oeste != tap2) {
            printf("\n    O tapete deve ser colocado em duas posicoes adjacentes. Tente novamente:");
            continue;
        }
        if(tap1->tapetes->topo != NULL) {
            if(tap1->tapetes->topo->tapete.metade == &(tap2->tapetes->topo->tapete) && tap2->tapetes->topo->tapete.metade == &(tap1->tapetes->topo->tapete)) {
                printf("\n    O tapete nao pode cobrir totalmete outro tapete. Tente novamente:");
                continue;
            } 
        }
        if(tap1 == (*assam)->posicao || tap2 == (*assam)->posicao) {
            printf("\n    O tapete nao pode ser colocado de baixo do Assam. Tente novamente:");
            continue;
        }
    
        inserirNaPilha((*jogadorAtual)->cor, tap1, *jogadorAtual);
        inserirNaPilha((*jogadorAtual)->cor, tap2, *jogadorAtual);
        tap1->tapetes->topo->tapete.metade = &(tap2->tapetes->topo->tapete);
        tap2->tapetes->topo->tapete.metade = &(tap1->tapetes->topo->tapete);

        break;
    }
    
    (*jogadorAtual)->tapetes--;
}

void areaTapete(Espaco *espacoAtual, Pilha *tapAdj){
    NoPilha *novo = (NoPilha*)malloc(sizeof(NoPilha));
    strcpy(novo->tapete.cor, espacoAtual->tapetes->topo->tapete.cor);
    novo->tapete.metade = espacoAtual->tapetes->topo->tapete.metade;
    novo->tapete.numTap = espacoAtual->tapetes->topo->tapete.numTap;
    if(tapAdj->topo == NULL){
        tapAdj->topo = novo;
        novo->prox = NULL;
    }else{
        novo->prox = tapAdj->topo;
        tapAdj->topo = novo;
    }
    tapAdj->tam++;
    NoPilha *aux, *atual;
    if(espacoAtual->norte->tapetes->topo != NULL){
        if(strcmp(espacoAtual->tapetes->topo->tapete.cor, espacoAtual->norte->tapetes->topo->tapete.cor) == 0){
            aux = espacoAtual->norte->tapetes->topo;
            atual = tapAdj->topo;
            while(atual != NULL){
                if(atual->tapete.metade != aux->tapete.metade)
                    atual = atual->prox;
                else
                    break;
            }
            if(atual == NULL)
                areaTapete(espacoAtual->norte, tapAdj);
        }
    }
    if(espacoAtual->leste->tapetes->topo != NULL){
        if(strcmp(espacoAtual->tapetes->topo->tapete.cor, espacoAtual->leste->tapetes->topo->tapete.cor) == 0){
            aux = espacoAtual->leste->tapetes->topo;
            atual = tapAdj->topo;
            while(atual != NULL){
                if(atual->tapete.metade != aux->tapete.metade)
                    atual = atual->prox;
                else
                    break;
            }
            if(atual == NULL){
                areaTapete(espacoAtual->leste, tapAdj);
            }
        }
    }
    if(espacoAtual->sul->tapetes->topo != NULL){
        if(strcmp(espacoAtual->tapetes->topo->tapete.cor, espacoAtual->sul->tapetes->topo->tapete.cor) == 0){
            aux = espacoAtual->sul->tapetes->topo;
            atual = tapAdj->topo;
            while(atual != NULL){
                if(atual->tapete.metade != aux->tapete.metade)
                    atual = atual->prox;
                else
                    break;
            }
            if(atual == NULL)
                areaTapete(espacoAtual->sul, tapAdj);
        }
    }
    if(espacoAtual->oeste->tapetes->topo != NULL){
        if(strcmp(espacoAtual->tapetes->topo->tapete.cor, espacoAtual->oeste->tapetes->topo->tapete.cor) == 0){
            aux = espacoAtual->oeste->tapetes->topo;
            atual = tapAdj->topo;
            while(atual != NULL){
                if(atual->tapete.metade != aux->tapete.metade)
                    atual = atual->prox;
                else
                    break;
            }
            if(atual == NULL)
                areaTapete(espacoAtual->oeste, tapAdj);
        }
    }
}

void pagarJogador(Tabuleiro *tabuleiro, Assam *assam, Jogador **jogadorAtual){
    if(assam->posicao->tapetes->tam == 0 || strcmp((*jogadorAtual)->cor, assam->posicao->tapetes->topo->tapete.cor) == 0)
        return;

    Jogador *donoTapete = *jogadorAtual;
    while(strcmp(donoTapete->cor, assam->posicao->tapetes->topo->tapete.cor) != 0)
        donoTapete = donoTapete->prox;
        
    if(donoTapete->jogando == 0)
        return;
        
    int r, g, b;
    corRGB((*jogadorAtual)->cor, &r, &g, &b);
    printf("\n\n    O \x1b[38;2;%d;%d;%dmJogador %s\x1b[0m pisou em cima do tapete do ", r, g, b, (*jogadorAtual)->cor);
    corRGB(assam->posicao->tapetes->topo->tapete.cor, &r, &g, &b);
    printf("\x1b[38;2;%d;%d;%dmJogador %s\x1b[0m... hora de pagar o preço", r, g, b, donoTapete->cor);

    Pilha *tapAdj = criarPilhaTapetes();
    areaTapete(assam->posicao, tapAdj);
    int moedas = tapAdj->tam;
    NoPilha *aux, *atual = tapAdj->topo;
    tapAdj->topo = NULL;
    free(tapAdj);
    while(atual != NULL){
        aux = atual;
        atual = atual->prox;
        free(aux);
    }

    printf("\n    Calculando quantas moedas devem ser pagas");
    for(int i = 0; i < 3; i++){
        Sleep(500);
        printf(".");
    }
    printf(" Pronto");
    corRGB((*jogadorAtual)->cor, &r, &g, &b);
    printf("\n    Transferindo %d moedas do \x1b[38;2;%d;%d;%dmJogador %s\x1b[0m para o ", moedas, r, g, b, (*jogadorAtual)->cor);
    corRGB(donoTapete->cor, &r, &g, &b);
    printf("\x1b[38;2;%d;%d;%dmJogador %s\x1b[0m", r, g, b, donoTapete->cor);
    for(int i = 0; i < 3; i++){
        Sleep(500);
        printf(".");
    }

    if((*jogadorAtual)->dinheiro >= moedas){
        (*jogadorAtual)->dinheiro -= moedas;
        donoTapete->dinheiro += moedas;
    }else{
        donoTapete->dinheiro += (*jogadorAtual)->dinheiro;
        (*jogadorAtual)->dinheiro = 0;
    }
    
    system("cls");
    imprimirTabuleiro(tabuleiro, assam);
    imprimirJogadorAtual(*jogadorAtual, assam);
    corRGB((*jogadorAtual)->cor, &r, &g, &b);
    printf("\n\n    O \x1b[38;2;%d;%d;%dmJogador %s\x1b[0m pisou em cima do tapete do ", r, g, b, (*jogadorAtual)->cor);
    corRGB(assam->posicao->tapetes->topo->tapete.cor, &r, &g, &b);
    printf("\x1b[38;2;%d;%d;%dmJogador %s\x1b[0m... hora de pagar o preço", r, g, b, donoTapete->cor);
    corRGB((*jogadorAtual)->cor, &r, &g, &b);
    printf("\n    Transferindo %d moedas do \x1b[38;2;%d;%d;%dmJogador %s\x1b[0m para o ", moedas, r, g, b, (*jogadorAtual)->cor);
    corRGB(donoTapete->cor, &r, &g, &b);
    printf("\x1b[38;2;%d;%d;%dmJogador %s\x1b[0m... Pronto", r, g, b, donoTapete->cor);
}

int fimDeJogo(listaJogadores *jogadores) {
    Jogador *aux = *jogadores;

    do{
        if(aux->tapetes > 0 && aux->dinheiro > 0)
            return 0;
        aux = aux->prox;   
    }while(aux != *jogadores);
    
    return 1;
}

void verificaVitoria(Tabuleiro *tabuleiro, listaJogadores *jogadores, Assam *assam){
    int r, g, b;
    int tpsVermelho = 0;
    int tpsAzul = 0;
    int tpsAmarelo = 0;
    int tpsVerde = 0;

    imprimirTabuleiro(tabuleiro, assam);

    Jogador *aux = *jogadores;
    do{
        aux->ptTotais += aux->dinheiro;
        aux = aux->prox;
    }while(aux != *jogadores);

    Espaco *inicioLinha = *tabuleiro;
    Espaco *atual;
    for(int i = 0; i < TAM; i++){
        atual = inicioLinha;
        inicioLinha = inicioLinha->sul;
        for(int j = 0; j < TAM; j++){
            if(atual->tapetes->tam != 0){
                if(strcmp(atual->tapetes->topo->tapete.cor, "Vermelho") == 0)
                    tpsVermelho++;
                else if(strcmp(atual->tapetes->topo->tapete.cor, "Azul") == 0)
                    tpsAzul++;
                else if(strcmp(atual->tapetes->topo->tapete.cor, "Amarelo") == 0)
                    tpsAmarelo++;
                else if(strcmp(atual->tapetes->topo->tapete.cor, "Verde") == 0)
                    tpsVerde++;
            }

            atual = atual->leste;
        }
    }

    do{
        if(strcmp(aux->cor, "Vermelho") == 0)
            aux->ptTotais += tpsVermelho;
        else if(strcmp(aux->cor, "Azul") == 0)
            aux->ptTotais += tpsAzul;
        else if(strcmp(aux->cor, "Amarelo") == 0)
            aux->ptTotais += tpsAmarelo;
        else if(strcmp(aux->cor, "Verde") == 0)
            aux->ptTotais += tpsVerde;


        aux = aux->prox;
    }while(aux != *jogadores);

    Jogador *vencedor, *empate = NULL;
    int ptsVencedor = -1;

    do{
        if(aux->ptTotais > ptsVencedor){
            ptsVencedor = aux->ptTotais;
            vencedor = aux;
        }else if(aux->ptTotais == ptsVencedor){
            vencedor = NULL;
        }    

        aux = aux->prox;
    }while(aux != *jogadores);
    
    if(vencedor != NULL){
        corRGB(vencedor->cor, &r, &g, &b);
        printf("\n\n     Jogador \x1b[38;2;%d;%d;%dm%s\x1b[0m venceu a partida!", r, g, b, vencedor->cor);
        printf("\n     Dinheiro restante: \x1b[38;2;%d;%d;%dm%d\x1b[0m",  r, g, b, vencedor->dinheiro);
        printf("\n     Tapetes visíveis: \x1b[38;2;%d;%d;%dm%d\x1b[0m",  r, g, b, vencedor->ptTotais - vencedor->dinheiro);
        printf("\n     Pontos totais: \x1b[38;2;%d;%d;%dm%d\x1b[0m",  r, g, b, vencedor->ptTotais);
        return;
    }

    ptsVencedor = -1;

    do{
        if(aux->dinheiro > ptsVencedor){
            vencedor = aux;
            ptsVencedor = aux->dinheiro;
        }else if(aux->dinheiro == ptsVencedor){
            empate = aux;
        }

        aux = aux->prox;
    }while(aux != *jogadores);

    if(empate == NULL){
        corRGB(vencedor->cor, &r, &g, &b);
        printf("\n\n     Jogador \x1b[38;2;%d;%d;%dm%s\x1b[0m venceu no desempate!", r, g, b, vencedor->cor);
        printf("\n     Dinheiro restante: \x1b[38;2;%d;%d;%dm%d\x1b[0m",  r, g, b, vencedor->dinheiro);
        printf("\n     Tapetes visíveis: \x1b[38;2;%d;%d;%dm%d\x1b[0m",  r, g, b, vencedor->ptTotais - vencedor->dinheiro);
        printf("\n     Pontos totais: \x1b[38;2;%d;%d;%dm%d\x1b[0m",  r, g, b, ptsVencedor);
        return;
    }

    corRGB(vencedor->cor, &r, &g, &b);
    printf("\n\n     Empate! Jogador \x1b[38;2;%d;%d;%dm%s\x1b[0m empatou com o ", r, g, b, vencedor->cor);
    corRGB(aux->cor, &r, &g, &b);
    corRGB(empate->cor, &r, &g, &b);
    printf("Jogador \x1b[38;2;%d;%d;%dm%s\x1b[0m", r, g, b, empate->cor);
    corRGB(vencedor->cor, &r, &g, &b);
    printf("\n     Dinheiro restante do Jogador \x1b[38;2;%d;%d;%dm%s\x1b[0m: \x1b[38;2;%d;%d;%dm%d\x1b[0m", r, g, b, vencedor->cor,  r, g, b, vencedor->dinheiro);
    printf("\n     Dinheiro visíveis do Jogador \x1b[38;2;%d;%d;%dm%s\x1b[0m: \x1b[38;2;%d;%d;%dm%d\x1b[0m", r, g, b, vencedor->cor,  r, g, b, vencedor->ptTotais - vencedor->dinheiro);
    corRGB(empate->cor, &r, &g, &b);
    printf("\n     Dinheiro restante do Jogador \x1b[38;2;%d;%d;%dm%s\x1b[0m: \x1b[38;2;%d;%d;%dm%d\x1b[0m", r, g, b, empate->cor,  r, g, b, empate->dinheiro);
    printf("\n     Dinheiro visíveis do Jogador \x1b[38;2;%d;%d;%dm%s\x1b[0m: \x1b[38;2;%d;%d;%dm%d\x1b[0m", r, g, b, empate->cor,  r, g, b, empate->ptTotais - empate->dinheiro);
    printf("\n     Pontos totais: %d", empate->ptTotais);
}

void corRGB(char cor[], int *r, int *g, int *b){
    if(strcmp(cor, "Vermelho") == 0){
        *r = 255;
        *g = 0;
        *b = 0;
    }else if(strcmp(cor, "Azul") == 0){
        *r = 0;
        *g = 0;
        *b = 255;
    }else if(strcmp(cor, "Amarelo") == 0){
        *r = 255;
        *g = 255;
        *b = 0;
    }else if(strcmp(cor, "Verde") == 0){
        *r = 0;
        *g = 255;
        *b = 0;
    }else{
        *r = 255;
        *g = 255;
        *b = 255;
    }
}

