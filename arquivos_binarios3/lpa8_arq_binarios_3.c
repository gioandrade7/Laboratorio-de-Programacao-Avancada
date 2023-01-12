#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>

int main(int agrc, char *argv[]){

    FILE *mpg_file = fopen(argv[1], "rb");

    unsigned char *b = malloc(3);
    unsigned char m[3] = {0x00, 0x00, 0x01};

    while(!feof(mpg_file)){
        fread(b, 3, 1, mpg_file);

        if(memcmp(m, b, 3) != 0){
            fseek(mpg_file, -2, SEEK_CUR);
        }
        else{
            unsigned char *t = malloc(1);
            fread(t, 1, 1, mpg_file);
            printf("\n--> Código: %.2x -- ", *t);

            if(*t == 0xb3){
                unsigned char byte1 = fgetc(mpg_file);
                unsigned char byte2 = fgetc(mpg_file);
                unsigned char byte3 = fgetc(mpg_file);
                unsigned char byte4 = fgetc(mpg_file);
                unsigned int largura = byte1 * 16 + (byte2 >> 4);
                unsigned int altura = (byte2 & 0x0F) * 256 + byte3;
                unsigned int frame_rate = byte4 & 0x0f;
                //{23.976, 24.000, 25.000, 29.970, 30.000, 50.000, 59.940, 60.000};
                if(frame_rate == 1){
                    printf("Sequence Header -- Width = %d, Height = %d -- Frame rate = 23.976", largura, altura);
                }
                else if(frame_rate == 2){
                    printf("Sequence Header -- Width = %d, Height = %d -- Frame rate = 24.000", largura, altura);
                }
                else if(frame_rate == 3){
                    printf("Sequence Header -- Width = %d, Height = %d -- Frame rate = 25.000", largura, altura);
                }
                else if(frame_rate == 4){
                    printf("Sequence Header -- Width = %d, Height = %d -- Frame rate = 29.970", largura, altura);
                }
                else if(frame_rate == 5){
                    printf("Sequence Header -- Width = %d, Height = %d -- Frame rate = 30.000", largura, altura);
                }
                else if(frame_rate == 6){
                    printf("Sequence Header -- Width = %d, Height = %d -- Frame rate = 50.000", largura, altura);
                }
                else if(frame_rate == 7){
                    printf("Sequence Header -- Width = %d, Height = %d -- Frame rate = 59.940", largura, altura);
                }
                else if(frame_rate == 8){
                    printf("Sequence Header -- Width = %d, Height = %d -- Frame rate = 60.000", largura, altura);
                }
                    
            }
            else if(*t == 0xb8){
                printf("Group of Pictures");
            }
            else if(*t == 0x00){

                unsigned char byte1 = fgetc(mpg_file);
                unsigned char byte2 = fgetc(mpg_file);
                unsigned int tipo = (byte2 >> 3) & 0x07;
                if(tipo == 1){
                    printf("Picture --  Tipo = I");
                }
                else if(tipo == 2){
                    printf("Picture --  Tipo = P");
                }
                else if(tipo == 3){
                    printf("Picture --  Tipo = B");
                }
            }
            else if (*t == 0x01){
                printf("Slice");
            } 
            else {
                printf("Tipo de stream não implementado");
            }
        }
    }
    free(b);
    fclose(mpg_file);
}