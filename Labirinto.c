/*
  Exercicio sobre labirinto - EDA
  Fernando Ribeiro Aguilar - 14/0139281
  Primeiramente faremos utilizando uma matriz Labirinto[N][N].
  Posiçoes livres(valor 0), paredes (valor 32767).
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define MAX 1000 /*Capacidade de armazenamento da pilha e da fila*/
#define N 33 /*Tamanho da matriz que representa o labirinto*/
#define LIVRE 0 /*Marca de posicao livre no labirinto*/
#define PAREDE 32767  /*Marca de posicao com parede no Labirinto*/

#define RED   "\x1B[31m"
#define RESET "\x1B[0m"
#define GRN   "\x1B[32m"

typedef int titem;/*Tipo dos itens armazenados na pilha e na fila*/

typedef struct fila{
  int inicio; /*primeira posicao cheia*/
  int fim;/*ultima posicao cheia*/
  titem elementosX[MAX];
  titem elementosY[MAX];
} Fila;

/*Declaracao das funcoes*/
void fila_cria(Fila* f);
int fila_vazia(Fila* f);
void insereFila(titem x, titem y, Fila *F);
int* removeFila(Fila *F);
void cria(int L[N][N]);
void exibe(int L[N][N]);
void anota(int L[N][N]);
void extrai(int L[N][N]);

/*Funcao main*/
int main(){
  int L[N][N];
  char r;
  srand(time(NULL));
  
do{
    system("clear");
    cria(L);
    
    anota(L);
    extrai(L);
    printf("Deseja continuar? (s/n)\n");
    scanf("%c%*c", &r);
  }
  while(toupper(r) != 'N');

  return 0;
}/*fim-main*/

/*inicializa a fila com o primeiro = ultimo elemento*/
void fila_cria(Fila* f){
  f->fim = 0;
  f->inicio = 0;
}

int fila_vazia(Fila* f){
  return (f->inicio >= f->fim);
}

/*funcao para inserir um elemento do tipo titem na fila*/
void insereFila(titem x, titem y, Fila *F) {
  if( F->fim == MAX ){
    printf("Overflow\n"); /* O fim chegou a ultima posiçao do vetor*/
    exit(1);
  }
  else{
    F->elementosX[F->fim] = x;
    F->elementosY[F->fim] = y;
    (F->fim)++;
  }
}

/*remove um elemento da fila*/
int* removeFila(Fila *F) {
  int* aux = (int*) malloc(sizeof(int)*2);
  
  if(F->inicio == F->fim){
    printf("Vazia\n");
    exit(1);
  }

  aux[0] = F->elementosX[F->inicio];
  aux[1] = F->elementosY[F->inicio];
  (F->inicio)++;
  /* teste para check se o novo inicio passou do fim*/
  if (F->inicio == (F->fim + 1) % MAX)
     printf("O novo inicio passou do fim\n");
  return aux;
}

/*cria a matriz do labirinto*/
void cria(int L[N][N]){
  int i, j;
  for(i=0; i < N; i++){
    L[i][0] = PAREDE;
    L[i][N-1] = PAREDE;
  }
  for(j=0; j < N; j++){
    L[0][j] = (PAREDE-2);
    L[N-1][j] = (PAREDE-2);
  }
  for(i=1; i<N-1; i++){
    for(j=1; j<N-1; j++){
      if (rand()%3 == 0)
        L[i][j] = (PAREDE-3);
      else
        L[i][j] = LIVRE;
    }
  } L[1][0] = LIVRE;
    L[1][1] = LIVRE;
    L[N-2][N-1] = LIVRE;
    L[N-2][N-2] = LIVRE;
} /*fim-cria*/

/*exibe o labirinto criado*/
void exibe(int L[N][N]){
  int i, j;
  for(i = 0; i< N; i++){
    for(j = 0; j<N; j++){
      
      //if(i == 1 && j == 0) putchar('I');
      if(i == N-2 && j == N-1) putchar('F');

      switch(L[i][j]){
        case LIVRE: 
          putchar(' ');
          break;
        case PAREDE:
          putchar('|');
          break;
        case -1:
          printf(RED "." RESET);
          break;
        case (PAREDE-2):
          putchar('-');
          break;
        case (PAREDE-3): 
          printf(GRN "#" RESET);
          break;
        default: printf(" ");
      }
      putchar(' ');
    }
    printf("\n");
  }
}

/*Funcao anota():
  anota na matriz l um numero minimo de passos necessários para
  atingir cada uma das posicoes do labirinto a partir da entrada
*/
void anota(int L[N][N]){
  int i,j, x, y;
  int c;
  Fila* F = (Fila*) malloc(sizeof(Fila));
  int* aux;
 
  fila_cria(F);
  i = j = 1;
 
  L[1][1] = 1;
  insereFila(1, 1, F);
  
  while(!fila_vazia(F)){
    aux = removeFila(F);
    i = aux[0];
    j = aux[1]; 
    c = (L[i][j] + 1);
    for(x=-1; x<=1; x++)
      for(y=-1; y<=1; y++){
        if(L[i + x][j + y] == 0 && (!x || !y)){
          L[i + x][j + y] = c;
          insereFila(i + x, j + y , F);
        }
      }
  }
}

/* Funcao extrai():
  Responsavel por apresentar o menor caminho a partir da funcao anotada utilizando a funcao anota() .
*/

void extrai(int L[N][N]){
  int x, y, i, j;
  x = y = N - 2;

  exibe(L);
  while (L[N-2][N-2] == 0){
    printf("Não Existe Saida para o labirinto.\n");
    printf("Gerando Novo Labirinto: \n");
    sleep(3);
    cria(L);
    exibe(L);
    anota(L);
  }
  while(x != 1 && y != 1){
    usleep(400000); exibe(L); 
    for( i= -1; i<= 1; i++)
      for(j= -1; j<=1; j++){
        if((!i || !j) && L[x+i][y+j] == L[x][y]-1){
          L[x][y] = -1;
          x += i;
          y += j;
        } 
      }
  }
  L[x][y] = -1;
  L[1][1] = -1;
  exibe(L);
}
