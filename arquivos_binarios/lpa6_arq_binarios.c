#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct zip_file_hdr{
	int signature;
	short version;
	short flags;
	short compression;
	short mod_time;
	short mod_date;
	int crc;
	int compressed_size;
	int uncompressed_size;
	short name_length;
	short extra_field_length;
} __attribute__ ((packed));

int main(int argc, char *argv[]){

	FILE *zip_file = fopen(argv[1], "rb");
	struct zip_file_hdr *file_hdr = malloc(sizeof(struct zip_file_hdr));
	int n = 1;

	while(!feof(zip_file)){
		fread(file_hdr, sizeof(struct zip_file_hdr), 1, zip_file);
		if(file_hdr->signature == 0x04034b50){
			char *name = malloc(sizeof(char)*(file_hdr->name_length+1));
			fread(name, sizeof(char)*(file_hdr->name_length), 1, zip_file);
			name[file_hdr->name_length] = '\n';
			printf("Arquivo %d ..\n", n);
			printf(" --> Nome do Arquivo: %s", name);
			printf(" --> Tamanho Compactado: %d\n", file_hdr->compressed_size);
			printf(" --> Tamanho decompactado: %d\n", file_hdr->uncompressed_size);
			printf("\n");
			free(name);
			fseek(zip_file, file_hdr->extra_field_length+file_hdr->compressed_size, SEEK_CUR);
			n++;
		}
		else{
			break;
		}
	}
	free(file_hdr);
	fclose(zip_file);
}