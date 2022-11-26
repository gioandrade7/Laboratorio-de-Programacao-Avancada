#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
int tam;

typedef struct pessoa_t{

    char nome[50];
    long long int cpf;
    int idade;

} pessoa_t;

typedef struct lista_pessoas_t{

    pessoa_t * pessoa;
    struct lista_pessoas_t * prox_pessoa;

} lista_pessoas_t;


typedef lista_pessoas_t **tabela_hash_t;

bool lista_pessoas_adicionar(pessoa_t *pessoa, lista_pessoas_t **lista){
    lista_pessoas_t *pessoa_nova = (lista_pessoas_t*) malloc(sizeof(lista_pessoas_t));

    if(pessoa_nova == NULL){return false;}
    pessoa_nova->pessoa = pessoa;
    pessoa_nova->prox_pessoa = *lista;
    *lista = pessoa_nova;
    return true;
}

void lista_pessoas_listar(lista_pessoas_t *lista){
    while(lista != NULL){
        printf("%s\t", lista->pessoa->nome);
        printf("%lld\t", lista->pessoa->cpf);
        printf("%d\n", lista->pessoa->idade);
        lista = lista->prox_pessoa;
    }
}

tabela_hash_t tabela_hash_pessoas_criar(){
    tabela_hash_t vet = (tabela_hash_t) malloc(tam * sizeof(lista_pessoas_t*));
    for(int i = 0; i < tam; i++){
        vet[i] = NULL;
    }
    return vet;
}

int tabela_hash_pessoas_funcao(pessoa_t *pessoa){
    int pos = pessoa->cpf%tam;
    return pos;
}

bool tabela_hash_pessoas_adicionar(pessoa_t *pessoa, tabela_hash_t tabela_hash){

    int pos = tabela_hash_pessoas_funcao(pessoa);
    return lista_pessoas_adicionar(pessoa, &tabela_hash[pos]);
}

void tabela_hash_pessoas_listar(tabela_hash_t tabela_hash){
    for(int i = 0; i<tam; i++){
        printf("POSICAO %d TABELA HASH:\n", i);
        lista_pessoas_listar(tabela_hash[i]);
        printf("\n");
    }

}

void main(int argc, char *argv[]){
    sscanf(argv[1], "%d", &tam);
    FILE *arq;

    char nome[50];
    long long int cpf;
    int idade;

    tabela_hash_t tabela = tabela_hash_pessoas_criar();    

    arq = fopen(argv[2], "r");

    if (arq == NULL)  // Se houve erro na abertura
  {
     printf("Problemas na abertura do arquivo\n");
     return;
  }

  

    while(!feof(arq)){
        fscanf(arq, "%50[^\t]\t%lld\t%d\n", nome, &cpf, &idade);

        pessoa_t *ev = (pessoa_t*) malloc(sizeof(pessoa_t));
        
        strcpy(ev->nome, nome);
        ev->cpf = cpf;
        ev->idade = idade;

        tabela_hash_pessoas_adicionar(ev, tabela);

        
    }

    tabela_hash_pessoas_listar(tabela);

    fclose(arq);
}