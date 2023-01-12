#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<math.h>
#include <string.h>

typedef struct lista_vizinhos_t{
	int id_vizinho;
	struct lista_vizinhos_t *prox_vizinho;
}lista_vizinhos_t;

typedef struct no_t{

    int id;
    double posX;
    double posY;
    lista_vizinhos_t *lista_vizinhos;

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

void lista_vizinhos_imprimir(lista_vizinhos_t *lista){
	while(lista != NULL){
		printf("%d ", lista->id_vizinho);
		lista = lista->prox_vizinho;
	}
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

void grafo_imprimir(int tam, grafo_t grafo){
	for(int i = 0; i<tam; i++){
		printf("NÓ %d: ", i);
		lista_vizinhos_imprimir(grafo[i].lista_vizinhos);
		printf("\n");

	}
}

int main(int argc, char *argv[]){

	int tam, posX, posY, id = 0;
	double raio = 0.0;
	int cont = 0;
	grafo_t grafo;

	
	FILE *arq;
	arq = fopen(argv[1], "r");

	fscanf(arq, "%d\t%lf\n", &tam, &raio);

	grafo = grafo_criar(tam);

	int i=0;
	while (!feof(arq)) {
		fscanf(arq, "%d\t%lf\t%lf\n", &grafo[i].id, &grafo[i].posX, &grafo[i].posY);
		grafo[i].lista_vizinhos = NULL;
		i++;

	}

	grafo_atualizar_vizinhos(tam, raio, grafo);
	grafo_imprimir(tam, grafo);

	fclose(arq);
}



