#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct evento_t{

    float tempo;
    int alvo;
    int tipo;

} evento_t;

typedef struct list_eventos_t{

    evento_t * evento;
    struct list_eventos_t * prox_evento;

} lista_eventos_t;

bool lista_eventos_adicionar_inicio(evento_t *evento, lista_eventos_t **lista){
  lista_eventos_t *item_novo = malloc(sizeof(lista_eventos_t)); //aloca um novo nó retornando o endereço 

  if(item_novo == NULL){return false;}
  item_novo->evento = evento;
  item_novo->prox_evento = *lista;
  *lista = item_novo;
  return true;
}

void lista_eventos_listar(lista_eventos_t *lista){

    lista_eventos_t * atual = lista;
    while(atual!= NULL){
        printf("%f\t", atual->evento->tempo);
        printf("%d\t", atual->evento->alvo);
        printf("%d\n", atual->evento->tipo);
        atual = atual->prox_evento;
    }
}

bool lista_eventos_adicionar_fim(evento_t *evento, lista_eventos_t **lista){
  lista_eventos_t *item_novo = malloc(sizeof(lista_eventos_t));
  lista_eventos_t *atual = *lista;
  if(item_novo == NULL){return false;}
  item_novo->evento = evento;
  item_novo->prox_evento = NULL;

  if(*lista == NULL){
    *lista = item_novo;
    return true;
  }
  else{
    while(atual->prox_evento != NULL){
      atual = atual->prox_evento;
    }
    atual->prox_evento =item_novo;
    return true;
  }
}

bool lista_eventos_adicionar_ordenado(evento_t *evento, lista_eventos_t **lista){
  lista_eventos_t *item_novo = malloc(sizeof(lista_eventos_t));
  lista_eventos_t *atual = *lista;
  if(item_novo == NULL){return false;}
  item_novo->evento = evento;
  item_novo->prox_evento = NULL;

  if(*lista == NULL){
    *lista = item_novo;
    return true;
  }

  if(item_novo->evento->tempo < atual->evento->tempo){
   item_novo->prox_evento = *lista;
   *lista = item_novo;
  }
  else{
    while(atual->prox_evento != NULL && atual->prox_evento->evento->tempo < item_novo->evento->tempo){
      atual = atual->prox_evento;
    }
    item_novo->prox_evento = atual->prox_evento;
    atual->prox_evento= item_novo;
    return true;
  }
}


void main(int argc, char *argv[]){
  
    FILE *arq;
    float tempo;
    int alvo, tipo;

    lista_eventos_t *lista = NULL;
    

    arq = fopen(argv[1], "r");

    if (arq == NULL)  // Se houve erro na abertura
  {
     printf("Problemas na abertura do arquivo\n");
     return;
  }
  /*//QUESTÃO 1
  while(!feof(arq)){
    fscanf(arq, "%f\t%d\t%d\n", &tempo, &alvo, &tipo);

    evento_t *ev = malloc(sizeof(evento_t));
    
    ev->tempo = tempo;
    ev->alvo = alvo;
    ev->tipo = tipo;

    lista_eventos_adicionar_inicio(ev, &lista);
  }
  lista_eventos_listar(lista);*/
 
 /*//QUESTÃO2
 while(!feof(arq)){
    fscanf(arq, "%f\t%d\t%d\n", &tempo, &alvo, &tipo);

    evento_t *ev = malloc(sizeof(evento_t));
    
    ev->tempo = tempo;
    ev->alvo = alvo;
    ev->tipo = tipo;

    lista_eventos_adicionar_fim(ev, &lista);
  }
  lista_eventos_listar(lista);*/

  /*//QUESTÃO3
 while(!feof(arq)){
    fscanf(arq, "%f\t%d\t%d\n", &tempo, &alvo, &tipo);

    evento_t *ev = malloc(sizeof(evento_t));
    
    ev->tempo = tempo;
    ev->alvo = alvo;
    ev->tipo = tipo;

    lista_eventos_adicionar_ordenado(ev, &lista);
  }
  lista_eventos_listar(lista);*/

  fclose(arq);
  
  



  

}


