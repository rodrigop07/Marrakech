# Marrakech (Terminal Edition)

Bem-vindo ao projeto do jogo de tabuleiro **Marrakech**, implementado em linguagem C para ser jogado diretamente no terminal.

Este projeto foi desenvolvido como parte da disciplina de **Análise e Estruturas de Dados** (AED).

* **Curso:** Ciência da Computação
* **Universidade:** Universidade Federal do Cariri (UFCA)

---

## Sobre o Jogo

[Marrakech](https://ludopedia.com.br/jogo/marrakech) é um jogo estratégico onde os jogadores assumem o papel de vendedores de tapetes em um mercado competitivo. O objetivo é terminar a partida sendo o jogador mais rico (somando os dirhams/moedas e a quantidade de metades de tapete visíveis no tabuleiro). 

Durante a partida, os participantes movimentam o inspetor do mercado (Assam), pagam taxas se pararem nos tapetes dos adversários e tentam cobrir a maior área possível com seus próprios tapetes, utilizando as estruturas de dados adequadas para gerenciar a sobreposição (pilhas de tapetes).

## Estrutura do Projeto

O código-fonte do projeto está organizado para facilitar a manutenção e leitura:

```text
Marrakech/
├── headers/
│   └── marrakech.h      # Declaração das structs, listas, pilhas e assinaturas de funções
├── src/
│   ├── main.c           # Ponto de entrada (loop principal do jogo)
│   └── marrakech.c      # Implementação das regras, lógicas de movimentação e estruturas
└── README.md            # Documentação do projeto
```

## Funcionalidades e Estruturas de Dados
Para o funcionamento adequado das regras de Marrakech, o projeto faz uso de:
- **Alocação Dinâmica:** Criação do tabuleiro, jogadores e empilhamento dos tapetes dinamicamente.
- **Listas Encadeadas:** Usadas para gerenciar a ordem dos jogadores, facilitando a passagem de turnos de forma circular.
- **Matriz de Ponteiros / Grafos:** Para estruturar os "Espaços" do tabuleiro e permitir navegação Norte-Sul-Leste-Oeste do Assam.
- **Pilhas:** Em cada espaço do tabuleiro, há uma pilha de tapetes, permitindo que apenas a cor do topo (o último tapete colocado) seja visível e considerada para pagamentos.
- **Cores ANSI:** Uso do terminal com suporte a _escape codes_ para apresentar um tabuleiro colorido e amigável.

## Como Compilar e Executar

Para rodar o jogo, é necessário ter um compilador C instalado (como o `gcc` ou `MinGW` no Windows).

1. **Clone o repositório:**
   ```bash
   git clone https://github.com/rodrigop07/Marrakech.git
   cd Marrakech
   ```

2. **Compile o código-fonte:**
   ```bash
   gcc src/main.c src/marrakech.c -o marrakech.exe
   ```

3. **Execute o jogo:**
   * No **Linux/Mac**:
     ```bash
     ./marrakech.exe
     ```
   * No **Windows**:
     ```cmd
     marrakech.exe
     ```

## Como Jogar
O jogo é interativo e todo via terminal. Basta seguir os _prompts_ na tela:
1. Informe o **número de jogadores** (2 a 4).
2. Na sua vez, escolha se deseja **girar o Assam** (e para qual lado).
3. O dado será rolado e o Assam vai **andar**.
4. Caso o Assam pare sobre o tapete de outro jogador, o pagamento (transferência de moedas) será processado automaticamente usando a busca em profundidade/largura para contar os tapetes adjacentes.
5. Digite as coordenadas para **colocar seu tapete**. O formato requisitado sempre será informado na tela.

Divirta-se dominando o mercado de tapetes! 
