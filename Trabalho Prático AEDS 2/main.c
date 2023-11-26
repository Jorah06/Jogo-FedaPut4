//  JOGO FEDAPUT*  //

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define OURO   1
#define ESPADA 2
#define COPAS  3
#define PAUS   4
#define  MAX   8

typedef struct cartas
{
    int naipe;
    int valor;
    int power;
    int id;
}cartas;

typedef struct elemento
{
    cartas br;
    struct elemento* anterior;
}elemento;

typedef struct baralho  // PILHA DINAMICA
{
    elemento* topo;
}baralho;

typedef struct mao // LISTA EST�TICA
{
    int fim;
    int quantidadeC;
    cartas cards[5];
}mao;

typedef struct jogador
{
    char nome[50];
    mao vetor;
    int vidas;
    int fazer;
    int fez;
}jogador;

typedef  struct mesa // FILA "CIRCULAR"
{
    int comeco;
    int fim;
    int tamanho;
    int vez;
    jogador players[MAX];
    int somatorioQF;
    int direcaoC;
} mesa;

typedef struct monte // JOGADORES JOGAM SUAS CARTAS AQUI
{
    int quemFez;
    cartas cartasMT;
}monte;

void iniciarCartas(cartas*c);
baralho* construirBaralho();
void liberarBaralho(baralho*b);
void bubbleSort(cartas*c);
void empilharBaralho(baralho*b, cartas *c,int posicao);
void desempilharBaralho(baralho*b);
mesa* criaMesa();
void liberaMesa(mesa*m);
void iniciarJogadoes(jogador*j);
void adicionarJog(mesa*m, jogador*j,int num);
void puxarCarta(baralho*b,mesa*m,mao*h,int rodada);
void destribuirC(baralho*b,mesa*m,mao*h,int rodada,int escolhaJ);
mao* criarMao();
void jogarCarta(mesa*m,monte *mt,int posicao,int mn);
void imprimirCarta(mesa*m,int posicao);
void quantasFazer(mesa*m,int rodada,int escolhaJ);
int eliminarJogador(mesa*m,int escolhaJ,FILE*arq);
void resetarFF(mesa*m,int escolhaJ);
void verGanhador(monte*mt,mesa*m,int escolhaJ,FILE*arq);
void verVidasP(mesa*m,int escolhaJ);
void limparMonte(monte*mt);
void imprimirArquivo(FILE*arq,mesa*m,int posicao);

int main()
{
    int i,k,x,y,cartasMAO,escolha,escolhaJ,rodada,posicao,lugarMT,eliminados,novoJogo,cursoP,comecarJ;
    FILE* arq;
    arq = fopen("notasJogo","w");
    if(arq != NULL){
        fprintf(arq,"ARQUIVO CRIADO COM SUCESSO\n");
    }
    cartas* c;
    baralho* b;
    mesa* m;
    jogador* j;
    mao* h;
    h = criarMao();
    if(h!=NULL){
        fprintf(arq,"MAO CRIADO COM SUCESSO\n");
    }
    else{
        return 0;
    }
    monte* mt;
    mt = (monte*) malloc (8*sizeof(monte));
    if(mt!=NULL){
        fprintf(arq,"MONTE CRIADO COM SUCESSO\n");
    }
    else{
        return 0;
    }
    m = criaMesa();
    j = (jogador*) malloc (8*sizeof(jogador));
    if(j!=NULL){
        fprintf(arq,"JOGADORES CRIADA COM SUCESSO\n");
    }
    else{
        return 0;
    }
    iniciarJogadoes(j);
    fprintf(arq,"JOGADORES INICIADOS COM SUCESSO\n");
    while(1)
    {
        printf("1. Iniciar jogo\n");
        printf("0. Sair\n");
        scanf ("%d",&escolha);
        switch(escolha)
        {
            case 1:
                fprintf(arq,"JOGO INICIADO COM SUCESSO\n");
                printf("Defina o formato do jogo\n");
                printf("2 jogadores\n");
                printf("3 jogadores\n");
                printf("4 jogadores\n");
                printf("5 jogadores\n");
                printf("6 jogadores\n");
                printf("7 jogadores\n");
                printf("8 jogadores\n");
                scanf ("%d",&escolhaJ);
                fprintf(arq,"FORMATO DE JOGO DEFINIDO: %d JOGADORES\n",escolhaJ);
                if (escolhaJ >=2 && escolhaJ<=8)
                {
                    for (i=0;i<escolhaJ;i++)
                    {
                        adicionarJog(m,j,i);
                        fprintf(arq,"jogador %d: %s\n",i+1,m->players[i].nome);
                    }
                    system("cls");
                    rodada = 5;
                    cursoP = 1;
                    comecarJ= 0;
                    while(escolhaJ > 1)
                    {
                        fprintf(arq,"NOVA RODADA (de %d cartas)\n",rodada);
                        if(rodada == 6){
                            rodada = 4;
                            cursoP = -1;
                        }
                        else if(rodada == 0){
                            rodada = 5;
                            cursoP = 1;
                        }
                        c = (cartas*) malloc (40*sizeof(cartas));
                        iniciarCartas(c);
                        bubbleSort(c);
                        b = construirBaralho();
                        for (x=0;x<40;x++){
                            empilharBaralho(b,c,x); // coloca as cartas no baralho
                        }
                        for(y=0;y<escolhaJ;y++){
                            puxarCarta(b,m,h,rodada); // puxando as cartas para a mao do jogador
                            m->comeco++;
                        }
                        m->comeco = 0;
                        resetarFF(m,escolhaJ);
                        m->vez =0;
                        quantasFazer(m,rodada,escolhaJ);
                        lugarMT = 0;
                        cartasMAO = rodada;
                        printf("CARTASMAO: %d\n",cartasMAO);

                        while(cartasMAO > 0)
                        {
                            for(k=0;k < escolhaJ;k++)
                            {
                                if(m->vez == m->fim){
                                    m->vez = m->comeco;
                                }
                                printf("VEZ DO JOGADOR: %s",m->players[m->vez].nome);
                                fprintf(arq,"\n\nVEZ DO JOGADOR: %s",m->players[m->vez].nome);
                                printf("        VIDAS: %d\n",m->players[m->vez].vidas);
                                fprintf(arq,"       VIDAS: %d\n",m->players[m->vez].vidas);
                                printf("QUANTOS TURNOS DEVEM SER VENCIDOS: %d\n",m->players[m->vez].fazer);
                                fprintf(arq,"QUANTOS TURNOS DEVEM SER VENCIDOS: %d\n",m->players[m->vez].fazer);
                                printf("QUANTOS TURNOS JA FORAM VENCIDOS: %d\n",m->players[m->vez].fez);
                                fprintf(arq,"QUANTOS TURNOS JA FORAM VENCIDOS: %d\n",m->players[m->vez].fez);
                                printf("MAO:\n");
                                for (i=0;i < m->players[m->vez].vetor.quantidadeC;i++)
                                {
                                    printf ("  %d.  ",i+1);
                                    imprimirCarta(m,i);
                                }
                                retornar:
                                printf("ESCOLHA UMA CARTA PARA JOGAR:");
                                scanf ("%d",&posicao);
                                if(posicao > m->players[m->vez].vetor.quantidadeC){
                                    printf("\nESSE NUMERO SUPERA A QUANTIDADE DE CARTAS EM SUA MAO\n");
                                    goto retornar;
                                }
                                printf ("\n%s JOGOU: ",m->players[m->vez].nome);
                                fprintf(arq,"%s JOGOU: ",m->players[m->vez].nome);
                                imprimirCarta(m,posicao-1);
                                imprimirArquivo(arq,m,posicao);
                                jogarCarta(m,mt,posicao-1,lugarMT);
                                lugarMT++;
                                printf("\n");
                            }
                            cartasMAO--;
                            verGanhador(mt,m,escolhaJ,arq);
                            limparMonte(mt);
                            lugarMT = 0;
                            system("pause");
                            system("cls");
                        }
                        verVidasP(m,escolhaJ);
                        eliminados = eliminarJogador(m,escolhaJ,arq);
                        escolhaJ = escolhaJ - eliminados;
                        printf("\n%d JOGADORES RESTANTES\n",escolhaJ);
                        fprintf(arq,"\n%d JOGADORES RESTANTES\n",escolhaJ);
                        free(c);
                        liberarBaralho(b);
                        rodada = rodada + cursoP;
                    }
                    printf("\n");
                    printf("-----------------------------------------\n");
                    printf("  PARABENS (%s) VOCE VENCEU\n",m->players[m->comeco].nome);
                    printf("-----------------------------------------\n");
                    fprintf(arq,"\n");
                    fprintf(arq,"-----------------------------------------\n");
                    fprintf(arq,"  PARABENS (%s) VOCE VENCEU\n",m->players[m->comeco].nome);
                    fprintf(arq,"-----------------------------------------\n");
                    fclose(arq);
                    voltA:
                    printf ("gostaria de jogar de novo?\n");
                    printf ("(1) SIM\n");
                    printf ("(2) NAo\n");
                    scanf("%d",&novoJogo);
                    switch(novoJogo)
                    {
                        case 1:
                            break;
                        case 2:
                            exit(1);
                            break;
                        default:
                            printf ("OPCAO INVALIDA\n\n");
                            goto voltA;
                            break;
                    }
                }
                else{
                    printf("FORMATO DE JOGO INVALIDO\n");
                }
                break;

            case 0:
                exit(1);
                break;

            default:
                printf ("OPCAO INVALIDA\n\n");
                break;
        }
    }
    return 0;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------//
void iniciarCartas(cartas*c) // CRIA AS CARTAS DO JOGO
{
    int i;
    srand(time(NULL));
    for(i=0;i<40;i++)
    {
        if(i>=0 && i<=9){         // CARTAS DE OURO
            c[i].id = rand();
            c[i].naipe = 1;
            c[i].valor = i+1;
            c[0].power = 1 +  OURO;
            c[1].power = 5 +  OURO;
            c[2].power = 9 +  OURO;
            c[3].power = 41 + OURO; // MANILHA
            c[4].power = 17 + OURO;
            c[5].power = 21 + OURO;
            c[6].power = 25 + OURO;
            c[7].power = 29 + OURO;
            c[8].power = 33 + OURO;
            c[9].power = 37 + OURO;
        }
        else if (i>=10 && i<=19){ // CARTAS DE ESPADA
            c[i].id = rand();
            c[i].naipe = 2;
            c[i].valor = i+1;
            c[10].power = 1 +  ESPADA;
            c[11].power = 5 +  ESPADA;
            c[12].power = 9 +  ESPADA;
            c[13].power = 13 + ESPADA;
            c[14].power = 17 + ESPADA;
            c[15].power = 21 + ESPADA;
            c[16].power = 25 + ESPADA;
            c[17].power = 41 + ESPADA; // MANILHA
            c[18].power = 33 + ESPADA;
            c[19].power = 37 + ESPADA;
        }
        else if (i>=20 && i<=29){ // CARTAS DE COPAS
            c[i].id = rand();
            c[i].naipe = 3;
            c[i].valor = i+1;
            c[20].power = 1 +  COPAS;
            c[21].power = 5 +  COPAS;
            c[22].power = 9 +  COPAS;
            c[23].power = 41 + COPAS; // MANILHA
            c[24].power = 17 + COPAS;
            c[25].power = 21 + COPAS;
            c[26].power = 25 + COPAS;
            c[27].power = 29 + COPAS;
            c[28].power = 33 + COPAS;
            c[29].power = 37 + COPAS;
        }
        else if (i>=30 && i<=39){ // CARTAS DE PAUS
            c[i].id = rand();
            c[i].naipe = 4;
            c[i].valor = i+1;
            c[30].power = 41 + PAUS; // MANILHA
            c[31].power = 5 +  PAUS;
            c[32].power = 9 +  PAUS;
            c[33].power = 13 + PAUS;
            c[34].power = 17 + PAUS;
            c[35].power = 21 + PAUS;
            c[36].power = 25 + PAUS;
            c[37].power = 29 + PAUS;
            c[38].power = 33 + PAUS;
            c[39].power = 37 + PAUS;
        }
    }
}

void bubbleSort(cartas*c) // EMBARALHO AS CARTAS DE ACORDO COM O ID
{
    int i,j;
    for (i=0;i<40;i++){
        for (j=0;j<39;j++){
            if (c[j].id > c[j+1].id)
            {
                int aux = c[j].id;
                c[j].id = c[j+1].id;
                c[j+1].id = aux;

                int aux2 = c[j].naipe;
                c[j].naipe = c[j+1].naipe;
                c[j+1].naipe = aux2;

                int aux3 = c[j].power;
                c[j].power =c[j+1].power;
                c[j+1].power = aux3;

                int aux4 = c[j].valor;
                c[j].valor = c[j+1].valor;
                c[j+1].valor = aux4;
            }
        }
    }
}

baralho* construirBaralho() // CRIA A ESTRUTURA DO BARALHO (PILHA)
{
    baralho *b;
    b =(baralho*) malloc (sizeof(baralho));
    b -> topo = NULL;
    return b;
}

void liberarBaralho(baralho*b) // LIBERA O ESPA�O OCUPADO PELO BARALHO
{
    while (b->topo != NULL)
    {
        elemento* aux;
        aux = b->topo;
        b->topo = b->topo->anterior;
        free(aux);
    }
    free(b);
}

void empilharBaralho(baralho*b, cartas *c,int posicao) // PASSA AS CARTAS DE UM VETOR DINAMICO PARA O BARALHO (PILHA)
{
    elemento *novo;
    novo = (elemento*) malloc (sizeof(elemento));
    novo->br.id = c[posicao].id;
    novo->br.naipe = c[posicao].naipe;
    novo->br.valor = c[posicao].valor;
    novo->br.power = c[posicao].power;
    novo -> anterior = b->topo;
    b->topo = novo;
}

void desempilharBaralho(baralho* b) // RETIRO A CARTA DO TOPO DO BARALHO
{
    elemento* aux;
    aux = b->topo;
    b->topo = b->topo->anterior;
    free(aux);
}

mesa* criaMesa()  // CRIA A ESTRUTURA DA MESA ONDE ESTAR�O OS JOGADORES
{
    mesa*m;
    m = (mesa*)malloc(sizeof(mesa));
    m->  comeco = 0;
    m -> fim = 0 ;
    m -> tamanho = 0 ;
    m -> vez = 0;
    m -> somatorioQF = 0;
    m -> direcaoC = 0;
    return m;
}

void liberaMesa(mesa* m){ // LIBERA A ESTRUTURA DA MESA ()
    free(m);
}

void adicionarJog(mesa*m,jogador*j,int num) // ADICIONA O JOGADOR A MESA
{
    m->players[m->fim].fazer = j[num].fazer;
    m->players[m->fim].fez = j[num].fez;
    m->players[m->fim].vidas = j[num].vidas;
    printf("Digite o nome do jogador\n");
    scanf ("%s",&m->players[m->fim].nome[0]);
    m->fim += 1;
    m->tamanho += 1;
}

void iniciarJogadoes(jogador*j) // CRIA A ESTRUTURA DOS JOGADORES
{
    int i;
    for (i=0;i<8;i++)
    {
        j[i].fazer = 0;
        j[i].fez = 0;
        j[i].vidas = 5;
    }
}

void puxarCarta(baralho*b,mesa*m,mao*h,int rodada) // ADICIONA CARTAS A M�O DO JOGADOR
{
    int aux;
    m->players[m->comeco].vetor.quantidadeC = 0;
    aux = h->fim;
    for(int i=0;i<rodada;i++)
    {
        m->players[m->comeco].vetor.cards[aux].id = b->topo->br.id;
        m->players[m->comeco].vetor.cards[aux].naipe = b->topo->br.naipe;
        m->players[m->comeco].vetor.cards[aux].power = b->topo->br.power;
        m->players[m->comeco].vetor.cards[aux].valor = b->topo->br.valor;
        aux++;
        m->players[m->comeco].vetor.quantidadeC++;
        desempilharBaralho(b);
    }
}

mao* criarMao() // CRIA A ESTRUTURA DA MAO DO JOGADOR
{
    mao *h;
    h =(mao*) malloc (sizeof(mao));
    h -> quantidadeC = 0;
    h -> fim = 0;
    return h;
}

void destribuirC(baralho*b,mesa*m,mao*h,int rodada,int escolhaJ) // ENTREGA AS CARTAS PARA TODOS NA MESA
{
    for(int i=0;i<escolhaJ;i++)
    {

        puxarCarta(b,m,h,rodada);          // TA QUEBRADA
        m->comeco++;                       // obs: conversar com o Ed dps
    }
    m->comeco = 0;
}

void jogarCarta(mesa*m,monte *mt,int posicao,int mn) // JOGA UMA CARTA PRESENTE NA MAO DO JOGADOR NO MONTE
{
    if (m->vez == m->fim){
        m->vez = m->comeco;
    }
    int i;
    mt[mn].cartasMT.id = m->players[m->vez].vetor.cards[posicao].id;
    mt[mn].cartasMT.naipe = m->players[m->vez].vetor.cards[posicao].naipe;
    mt[mn].cartasMT.power = m->players[m->vez].vetor.cards[posicao].power;
    mt[mn].cartasMT.valor = m->players[m->vez].vetor.cards[posicao].valor;
    mt[mn].quemFez = m->vez;
    for (i = posicao+1; i < m->players[m->vez].vetor.quantidadeC; i++){
        m->players[m->vez].vetor.cards[i-1].id = m->players[m->vez].vetor.cards[i].id;
        m->players[m->vez].vetor.cards[i-1].naipe = m->players[m->vez].vetor.cards[i].naipe;
        m->players[m->vez].vetor.cards[i-1].power = m->players[m->vez].vetor.cards[i].power;
        m->players[m->vez].vetor.cards[i-1].valor = m->players[m->vez].vetor.cards[i].valor;
    }
    m->players[m->vez].vetor.cards[m->players[m->vez].vetor.quantidadeC-1].id = 0;
    m->players[m->vez].vetor.cards[m->players[m->vez].vetor.quantidadeC-1].naipe = 0;
    m->players[m->vez].vetor.cards[m->players[m->vez].vetor.quantidadeC-1].power = 0;
    m->players[m->vez].vetor.cards[m->players[m->vez].vetor.quantidadeC-1].valor = 0;
    m->players[m->vez].vetor.quantidadeC--;
    m->vez++;
}

void quantasFazer(mesa*m,int rodada,int escolhaJ) // PERGUNTA AO JOGADOR QUANTAS VEZES ELE IR� VENCER EM DETERMINADA JODADA
{
    int auxilio;
    auxilio = m->vez;
    int i,k;
    int ultimo = 0;
    for(i=0;i<escolhaJ;i++)
    {
        if (m->vez == m->fim){
            m->vez = m->comeco;
        }
        ponto:
        printf("-----------RODADA DE %d CARTAS-----------\n",rodada);
        printf("QUANTOS TURNOS SERAO VENCIDOS PELO JOGADOR: %s \n",m->players[m->vez].nome);
        printf("MAO: \n");
         for (k=0;k < m->players[m->vez].vetor.quantidadeC;k++)
        {
            printf ("%d. ",k+1);
            imprimirCarta(m,k);
        }
        scanf("%d",&m->players[m->vez].fazer);
        if(m->players[m->vez].fazer > m->players[m->vez].vetor.quantidadeC)
        {
            system("cls");
            printf ("NAO E POSSIVEL FAZER MAIS VEZES DO QUE O NUMERO DE CARTAS\n\n");
            goto ponto;
        }
        system("cls");
        ultimo = m->vez;
        m->somatorioQF = m->somatorioQF + m->players[m->vez].fazer;
        m->vez++;
    }
    if(m->somatorioQF == rodada)
    {
        m->somatorioQF = m->somatorioQF - m->players[ultimo].fazer;
        printf("O ULTIMO JOGADOR DEVE ESSCOLHER UM NUMERO DIFERENTE (%s)\n",m->players[ultimo].nome);
        scanf("%d",&m->players[ultimo].fazer);
        m->somatorioQF = m->somatorioQF + m->players[ultimo].fazer;
    }
    m->vez = auxilio;
    system("cls");
}

void verGanhador(monte*mt,mesa*m,int escolhaJ,FILE*arq) // MOSTRA QUEM GANHOU EM CADA TURNO
{
    int ganhador=0;
    int posicao;
    int i;
    for(i=0;i<escolhaJ;i++)
    {
        if(mt[i].cartasMT.power > ganhador){
            ganhador = mt[i].cartasMT.power;
            posicao = mt[i].quemFez;

        }
    }
    printf("JOGADOR %s GANHOU ESTE TURNO\n\n",m->players[posicao].nome);
    fprintf(arq,"\nJOGADOR %s GANHOU ESTE TURNO\n\n",m->players[posicao].nome);
    m->players[posicao].fez++;
    m->vez = posicao;
}

void verVidasP(mesa*m,int escolhaJ) // MOSTRA QUANTAS VIDAS FORAM PERDIDAS EM CADA RODADA
{
    int i;
    int perdeuVIDA;
    for(i=0;i<escolhaJ;i++)
    {
        if(m->players[i].fazer == m->players[i].fez){
            perdeuVIDA = 0;
        }
        else if(m->players[i].fazer > m->players[i].fez){
            perdeuVIDA = m->players[i].fazer - m->players[i].fez;
        }
        else if(m->players[i].fazer < m->players[i].fez){
            perdeuVIDA = m->players[i].fez - m->players[i].fazer;
        }
        m->players[i].vidas = m->players[i].vidas - perdeuVIDA;
        if(perdeuVIDA == 0){
            printf("O JOGADOR %s NAO PERDEU NENHUMA VIDA\n",m->players[i].nome);
        }
        else{
            printf("O JOGADOR %s PERDEU %d NESTA RODADA\n",m->players[i].nome,perdeuVIDA);
        }
    }
}

void resetarFF(mesa*m,int escolhaJ) // RESETAQUANTA VOCE IA FAZER E QUANTAS VOCE FEZ
{
    int i;
    for(i=0;i<escolhaJ;i++)
    {
        m->players[i].fazer = 0;
        m->players[i].fez = 0;
    }
}

void limparMonte(monte*mt) // LIMPA O MONTE EM CADA
{
    int i;
    for(i=0;i<8;i++)
    {
        mt[i].cartasMT.id = 0;
        mt[i].cartasMT.naipe = 0;
        mt[i].cartasMT.power = 0;
        mt[i].cartasMT.valor = 0;
    }
}

int eliminarJogador(mesa*m,int escolhaJ,FILE*arq) // RETIRA O JOGADOR QUANDO SUAS VIDAS ACABAM
{
    int i;
    int j;
    int k;
    int eliminados = 0;
    for(i=0;i<escolhaJ;i++)
    {
        if(m->players[i].vidas <= 0)
        {
            printf ("O JOGADOR %s FOI ELIMINADO\n",m->players[i].nome);
            fprintf (arq,"O JOGADOR %s FOI ELIMINADO\n",m->players[i].nome);
            m->tamanho--;
            for(j=i+1;j<escolhaJ;j++)
            {
                m->players[j-1].vidas = m->players[j].vidas;
                for(k=0;k<50;k++)
                {
                    m->players[j-1].nome[k] = m->players[j].nome[k];
                }
            }
            eliminados++;
        }
    }
    if(eliminados==0){
        printf ("NINGUEM FOI ELIMINADO NESTA RODADA\n");
        fprintf (arq,"NINGUEM FOI ELIMINADO NESTA RODADA\n");
    }
    return eliminados;
}

void imprimirCarta(mesa*m,int posicao) // MOSTRA QUAIS AS CARTAS QUE ESTAO NA M�O DO JOGADOR
{
    switch(m->players[m->vez].vetor.cards[posicao].valor)
    {
        case 1:
            printf ("4 de OURO\n");
            break;
        case 2:
            printf ("5 de OURO\n");
            break;
        case 3:
            printf ("6 de OURO\n");
            break;
        case 4:
            printf ("7 de OURO\n");
            break;
        case 5:
            printf ("Rainha de OURO\n");
            break;
        case 6:
            printf ("Valete de OURO\n");
            break;
        case 7:
            printf ("Rei de OURO\n");
            break;
        case 8:
            printf ("AS de OURO\n");
            break;
        case 9:
            printf ("2 de OURO\n");
            break;
        case 10:
            printf ("3 de OURO\n");
            break;
        case 11:
            printf ("4 de ESPADA\n");
            break;
        case 12:
            printf ("5 de ESPADA\n");
            break;
        case 13:
            printf ("6 de ESPADA\n");
            break;
        case 14:
            printf ("7 de ESPADA\n");
            break;
        case 15:
            printf ("Rainha de ESPADA\n");
            break;
        case 16:
            printf ("Valete de ESPADA\n");
            break;
        case 17:
            printf ("Rei de ESPADA\n");
            break;
        case 18:
            printf ("ESPADILHA\n");
            break;
        case 19:
            printf ("2 de ESPADA\n");
            break;
        case 20:
            printf ("3 de ESPADA\n");
            break;
        case 21:
            printf ("4 de COPAS\n");
            break;
        case 22:
            printf ("5 de COPAS\n");
            break;
        case 23:
            printf ("6 de COPAS\n");
            break;
        case 24:
            printf ("7 de COPAS\n");
            break;
        case 25:
            printf ("Rainha de COPAS\n");
            break;
        case 26:
            printf ("Valete de COPAS\n");
            break;
        case 27:
            printf ("Rei de COPAS\n");
            break;
        case 28:
            printf ("AS de COPAS\n");
            break;
        case 29:
            printf ("2 de COPAS\n");
            break;
        case 30:
            printf ("3 de COPAS\n");
            break;
        case 31:
            printf ("ZAP\n");
            break;
        case 32:
            printf ("5 de PAUS\n");
            break;
        case 33:
            printf ("6 de PAUS\n");
            break;
        case 34:
            printf ("7 de PAUS\n");
            break;
        case 35:
            printf ("Rainha de PAUS\n");
            break;
        case 36:
            printf ("Valete de PAUS\n");
            break;
        case 37:
            printf ("Rei de PAUS\n");
            break;
        case 38:
            printf ("AS de PAUS\n");
            break;
        case 39:
            printf ("2 de PAUS\n");
            break;
        case 40:
            printf ("3 de PAUS\n");
            break;
    }
}

void imprimirArquivo(FILE*arq,mesa*m,int posicao) // PRINTA A CARTA NO ARQUIVO
{
    switch(m->players[m->vez].vetor.cards[posicao].valor)
    {
        case 1:
            fprintf (arq,"4 de OURO\n");
            break;
        case 2:
            fprintf (arq,"5 de OURO\n");
            break;
        case 3:
            fprintf (arq,"6 de OURO\n");
            break;
        case 4:
            fprintf (arq,"7 de OURO\n");
            break;
        case 5:
            fprintf (arq,"Rainha de OURO\n");
            break;
        case 6:
            fprintf (arq,"Valete de OURO\n");
            break;
        case 7:
            fprintf (arq,"Rei de OURO\n");
            break;
        case 8:
            fprintf (arq,"AS de OURO\n");
            break;
        case 9:
            fprintf (arq,"2 de OURO\n");
            break;
        case 10:
            fprintf (arq,"3 de OURO\n");
            break;
        case 11:
            fprintf (arq,"4 de ESPADA\n");
            break;
        case 12:
            fprintf (arq,"5 de ESPADA\n");
            break;
        case 13:
            fprintf (arq,"6 de ESPADA\n");
            break;
        case 14:
            fprintf (arq,"7 de ESPADA\n");
            break;
        case 15:
            fprintf (arq,"Rainha de ESPADA\n");
            break;
        case 16:
            fprintf (arq,"Valete de ESPADA\n");
            break;
        case 17:
            fprintf (arq,"Rei de ESPADA\n");
            break;
        case 18:
            fprintf (arq,"ESPADILHA\n");
            break;
        case 19:
            fprintf (arq,"2 de ESPADA\n");
            break;
        case 20:
            fprintf (arq,"3 de ESPADA\n");
            break;
        case 21:
            fprintf (arq,"4 de COPAS\n");
            break;
        case 22:
            fprintf (arq,"5 de COPAS\n");
            break;
        case 23:
            fprintf (arq,"6 de COPAS\n");
            break;
        case 24:
            fprintf (arq,"7 de COPAS\n");
            break;
        case 25:
            fprintf (arq,"Rainha de COPAS\n");
            break;
        case 26:
            fprintf (arq,"Valete de COPAS\n");
            break;
        case 27:
            fprintf (arq,"Rei de COPAS\n");
            break;
        case 28:
            fprintf (arq,"AS de COPAS\n");
            break;
        case 29:
            fprintf (arq,"2 de COPAS\n");
            break;
        case 30:
            fprintf (arq,"3 de COPAS\n");
            break;
        case 31:
            fprintf (arq,"ZAP\n");
            break;
        case 32:
            fprintf (arq,"5 de PAUS\n");
            break;
        case 33:
            fprintf (arq,"6 de PAUS\n");
            break;
        case 34:
            fprintf (arq,"7 de PAUS\n");
            break;
        case 35:
            fprintf (arq,"Rainha de PAUS\n");
            break;
        case 36:
            fprintf (arq,"Valete de PAUS\n");
            break;
        case 37:
            fprintf (arq,"Rei de PAUS\n");
            break;
        case 38:
            fprintf (arq,"AS de PAUS\n");
            break;
        case 39:
            fprintf (arq,"2 de PAUS\n");
            break;
        case 40:
            fprintf (arq,"3 de PAUS\n");
            break;
    }
}
