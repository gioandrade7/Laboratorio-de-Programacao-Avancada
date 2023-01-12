#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

void criar_arquivo(char *nome, int i){
    
    sprintf(nome, "video_parte_%d.mpg", i);
    
    printf("Criando arquivo %s ..\n", nome);
    
}

int main(int agrc, char *argv[]){
    int x = 1;
    char nome_arq[30] = "";
    FILE *mpg_file = fopen(argv[1], "rb"); //Abre o arquivo de entrada
    if(mpg_file == NULL){
        printf("ERRO\n");
    }
    criar_arquivo(nome_arq, x);
    
    FILE *saida = fopen(nome_arq, "wb"); //Abre o primeiro arquivo de saída (video_parte_1.mpg)
    
    if(saida == NULL){
        printf("ERRO\n");
    }
    
    long int tam_max = atoi(argv[2])*pow(10,6);
    long long int *buffer = malloc(tam_max); /*Aloca memória para um buffer (tamanho passado no argv[2] convertido para bytes). Este buffer armazenará o
                                                            conteúdo completo de um único fluxo "Sequence" retirado do arquivo de entrada.*/                                                        
    unsigned char sequence[4] = {0x00, 0x00, 0x01, 0xb3};
    unsigned char *bytes = malloc(4);
    int tam_buffer = 0;
    long int tam_arquivo = 0; //tamanho_arquivo(saida);

    while(1){ //Enquanto verdadeiro
        fread(bytes, 4, 1, mpg_file); //Lê quatro bytes (fread)
        if(!feof(mpg_file) && memcmp(bytes, sequence, 4) != 0){ /*Se não for código de Sequence (memcmp com a string "\x00\x00\x01\xB3") nem o final do arquivo de
                                                                    entrada (feof)*/
            buffer[tam_buffer] = bytes[0]; //Salve o primeiro byte lido no buffer
            tam_buffer ++;
            fseek(mpg_file, -3, SEEK_CUR);  //Retorna três bytes
            continue;
        }
        else{
            
            if( tam_arquivo + tam_buffer > tam_max){ /*Se o tamanho do arquivo de saída atual mais o tamanho do buffer (que contém um único sequence) for maior
                                                                        que o tamanho máximo, teremos que salvar o buffer atual já em um novo arquivo*/
                fclose(saida);//Fecha o arquivo de saída atual
                x++; 
                criar_arquivo(nome_arq, x);
                saida = fopen(nome_arq, "wb"); //Abra o novo arquivo de saída
                fwrite(buffer, tam_buffer, 1, saida); //Escreva todo o buffer nele (fwrite)
                tam_arquivo = tam_buffer; //Sete o tamanho do arquivo atual para o tamanho do buffer recém-salvo
            }
            else{
                fwrite(buffer, tam_buffer, 1, saida); //Salve o buffer lido no arquivo já aberto (fwrite)
                tam_arquivo += tam_buffer; //Incremente o tamanho do arquivo atual com o tamanho do buffer
            }

            if(feof(mpg_file)){
                fclose(saida);
                break;
            }
            memcpy(buffer, bytes, 4);
            tam_buffer = 4;
        }
    }
    fclose(mpg_file);
    free(buffer);
    free(bytes);   
}