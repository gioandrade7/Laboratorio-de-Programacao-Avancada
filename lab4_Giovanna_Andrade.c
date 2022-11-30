#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<math.h>
#include <string.h>

typedef struct evento_t{

    float tempo;
    int alvo;
    int tipo;

} evento_t;

typedef struct lista_eventos_t{

    evento_t * evento;
    struct lista_eventos_t *prox_evento;

} lista_eventos_t;

typedef struct lista_vizinhos_t{
	int id_vizinho;
	struct lista_vizinhos_t *prox_vizinho;
}lista_vizinhos_t;

typedef struct no_t{

    int id;
    double posX;
    double posY;
    lista_vizinhos_t *lista_vizinhos;
    bool enviado;

} no_t;

typedef no_t *grafo_t;

bool lista_vizinhos_adicionar(int vizinho, lista_vizinhos_t **lista){

	lista_vizinhos_t *lista_nova = malloc(sizeof(lista_vizinhos_t));

	if(lista_nova == NULL){return false;}
	lista_nova->id_vizinho = vizinho;
	lista_nova->prox_vizinho = *lista;
	*lista = lista_nova;
	return true;
}


grafo_t grafo_criar(int tam){
	grafo_t grafo = malloc(tam * sizeof(no_t));
	return grafo;
}

void grafo_atualizar_vizinhos(int tam, double raio_comunicacao, grafo_t grafo){
	double dist = 0.0;
	for(int i = 0; i < tam; i++){
		for(int j = 0; j < tam; j++){
			if(grafo[j].id != grafo[i].id){
				dist = sqrt(pow(grafo[i].posX - grafo[j].posX, 2) + pow(grafo[i].posY - grafo[j].posY, 2));
				if(dist < raio_comunicacao){
					lista_vizinhos_adicionar(grafo[i].id, &grafo[j].lista_vizinhos);
				}

			}
		}

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

 void simulacao_iniciar(lista_eventos_t **lista, grafo_t grafo){
 	while(*lista != NULL){
 		lista_eventos_t *x = malloc(sizeof(lista_eventos_t));
 		x = *lista;
 		evento_t *ev;
 		
 		ev = x->evento;
 		*lista = x->prox_evento;

 		printf("[%3.6f] Nó %d recebeu pacote\n", ev->tempo, ev->alvo);

 		if(grafo[ev->alvo].enviado != true){
 			lista_vizinhos_t *listaViz = grafo[ev->alvo].lista_vizinhos;
 			while(listaViz != NULL){
 				evento_t *novo_ev = malloc(sizeof(evento_t));
 				novo_ev->tipo = 1;
 				novo_ev->alvo = listaViz->id_vizinho;
 				novo_ev->tempo = ev->tempo + ((listaViz->id_vizinho*0.01) + 0.1);

 				lista_eventos_adicionar_ordenado(novo_ev, lista);

 				printf("\t--> Repassando pacote para nó %d...\n",listaViz->id_vizinho);

 				listaViz = listaViz->prox_vizinho;
 			}
 			grafo[ev->alvo].enviado = true;
 		}

 	}

 }


int main(int argc, char *argv[]){

	int tam = 0;
	double raio = 0.0;
	grafo_t grafo;

	
	FILE *arq;
	arq = fopen(argv[1], "r");

	 if(arq == NULL){
        printf("Erro ao abrir arquivo!\n");
        return 0;
    }

	fscanf(arq, "%d\t%lf\n", &tam, &raio);

	grafo = grafo_criar(tam);

	int i=0;
	while (!feof(arq)) {
		fscanf(arq, "%d\t%lf\t%lf\n", &grafo[i].id, &grafo[i].posX, &grafo[i].posY);
		grafo[i].lista_vizinhos = NULL;
		i++;

	}

	grafo_atualizar_vizinhos(tam, raio, grafo);
	
	lista_eventos_t *listaEv = NULL;
	evento_t *novo_ev = malloc(sizeof(evento_t));
	novo_ev->tempo = 1.0;
	novo_ev->alvo = 0;
	novo_ev->tipo = 1;

	lista_eventos_adicionar_ordenado(novo_ev, &listaEv);

	simulacao_iniciar(&listaEv, grafo);

	fclose(arq);
}
