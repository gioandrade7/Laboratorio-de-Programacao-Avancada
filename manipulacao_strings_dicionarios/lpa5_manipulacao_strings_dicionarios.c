#include<stdio.h>
#include <string.h>
#include<stdlib.h>
#include<math.h>

void forcaBruta(int n, char *alfabeto){
	int size = 0;
	int x = 0;
	char ch;
	for(int i = 0; alfabeto[i] != '\0'; i++){
		size++;
	}

	for(int j = 1; j <= n; j++){ //tamanhos da senha
		x = pow(size, j); // número de possibilidades
		char pass[j];
		for(int z = 0; z < x; z++){ //possibilidades de acordo com o tamanho das senhas
			int u = z;
			for(int y = 0;  y < j; y++){
				pass[y] = alfabeto[u%size];
				u = u/size;
			}
			printf("%s", pass);
			printf("\n");
		}
	}
}



int main(int argc, char *argv[]){

	int n = atoi(argv[1]);
	forcaBruta(n, argv[2]);


}