#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>

struct png_chunk_hdr{
	int lenght;
	char type[4];
} __attribute__ ((packed));

struct chunk_ihdr_hdr{
	int width;
	int height;
	char bit_depth;
	char colour_type;
	char comp_method;
	char filter_method;
	char interlace_method;
} __attribute__ ((packed));

int main(int argc, char *argv[]){

	int n = 1;

	struct png_chunk_hdr *png_chunk = malloc(sizeof(struct png_chunk_hdr));
	struct chunk_ihdr_hdr *chunk_hdr = malloc(sizeof(struct chunk_ihdr_hdr));

	FILE *png_file = fopen(argv[1], "rb");

	fseek(png_file, 8, SEEK_CUR);

	while(!feof(png_file)){
		fread(png_chunk, sizeof(struct png_chunk_hdr), 1, png_file);
		printf("Lendo chunk %d:\n", n);
		printf("	--> Tamanho: %d\n", ntohl(png_chunk->lenght));
		printf("	--> Tipo:    %.4s\n", png_chunk->type);

		if(strncmp(png_chunk->type, "IHDR", 4) == 0){
			fread(chunk_hdr, sizeof(struct chunk_ihdr_hdr), 1, png_file);
			printf("		--> Largura: %d\n", ntohl(chunk_hdr->width));
			printf("		--> Altura: %d\n", ntohl(chunk_hdr->height));
			fseek(png_file, 4, SEEK_CUR);
		}
		else if(strncmp(png_chunk->type, "IEND", 4) == 0){
			break;
		}
		else{
			fseek(png_file, ntohl(png_chunk->lenght) + 4, SEEK_CUR);
		}
		n++;

	}

	free(png_chunk);
	free(chunk_hdr);
	fclose(png_file);
}


