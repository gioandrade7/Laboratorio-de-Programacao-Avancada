#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdint.h>
#include <netinet/in.h>

typedef struct ethernet_hdr_t{
    uint8_t daddr[6];
    uint8_t saddr[6];
    uint16_t protocol;
} ethernet_hdr_t;

typedef struct ip_hdr_t{
    uint8_t hdr_len:4, // Tamanho do Cabeçalho,
            version:4; // Versão do IP
    uint8_t tos;// Tipo de serviço
    uint16_t tot_len;// Tamanho total do IP
    uint16_t id;// Id do pacote
    uint16_t frag_off; // Fragmentado?
    uint8_t ttl;// Tempo de vida
    uint8_t protocol; // Protocolo da camada superior (TCP!)
    uint16_t check;// Checksum
    uint8_t saddr[4];// Endereço IP de origem
    uint8_t daddr[4];// Endereço IP de destino
} ip_hdr_t;

typedef struct tcp_hdr_t{
    uint16_t sport;// Porta TCP de origem
    uint16_t dport;// Porta TCP de destino
    uint32_t seq;// Sequência
    uint32_t ack_seq;// Acknowledgement
    uint8_t reservado:4, // Não usado
            hdr_len:4;// Tamanho do cabecalho
    uint8_t flags;// Flags do TCP
    uint16_t window;// Tamanho da janela
    uint16_t check;// Checksum
    uint16_t urg_ptr;// Urgente
} tcp_hdr_t;

int main(int argc, char *argv[]){

    FILE *file = fopen(argv[1], "rb");
    if(file == NULL){printf("ERRO\n");}

    struct ethernet_hdr_t *ethernet = malloc(sizeof(ip_hdr_t));
    struct ip_hdr_t *ip = malloc(sizeof(ip_hdr_t));
    struct tcp_hdr_t *tcp = malloc(sizeof(tcp_hdr_t));

    while(!feof(file)){
        
        printf("Lendo Ethernet ..\n");
        fread(ethernet, sizeof(struct ethernet_hdr_t), 1, file);
        printf("    --> MAC de Origem: ");
        for (int i = 0; i < 5; i++){
            printf("%.2x:", ethernet->saddr[i]);
        }
        printf("%.2x\n", ethernet->saddr[5]);

        printf("    --> MAC de Destino: ");
        for (int i = 0; i < 5; i++){
            printf("%.2x:", ethernet->daddr[i]);
        }
        printf("%.2x\n", ethernet->daddr[5]);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        printf("Lendo IP ..\n");
        fread(ip, sizeof(struct ip_hdr_t), 1, file);
        printf("    --> Versão do IP: %d\n", ip->version);
        printf("    --> Tamanho do cabeçalho: %d bytes\n", ip->hdr_len*4);
        printf("    --> Tamanho do pacote: %d bytes\n", ntohs(ip->tot_len));
        printf("    --> Endereço IP de Origem: ");
        for (int i = 0; i < 3; i++){
            printf("%d.", ip->saddr[i]);
        }
        printf("%d\n", ip->saddr[3]); //ultimo nunero diferente do exmplo ?
        printf("    --> Endereço IP de Destino: ");
        for (int i = 0; i < 3; i++){
            printf("%d.", ip->daddr[i]);
        }
        printf("%d\n", ip->daddr[3]);  //ultimo nunero diferente do exmplo ?
        fseek(file, ip->hdr_len*4 - sizeof(ip_hdr_t), SEEK_CUR);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        printf("Lendo TCP ..\n");
        fread(tcp, sizeof(struct tcp_hdr_t), 1, file);
        printf("    --> Porta de Origem: %d\n", ntohs(tcp->sport)); 
        printf("    --> Porta de Destino: %d\n", ntohs(tcp->dport)); 
        printf("    --> Tamanho do cabeçalho: %d bytes\n", tcp->hdr_len*4);
        fseek(file, tcp->hdr_len*4 - sizeof(tcp_hdr_t), SEEK_CUR);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        printf("Lendo HTTP ..\n");
        int tam_dados = ntohs(ip->tot_len) - (ip->hdr_len*4) - (tcp->hdr_len*4);
        printf("    --> Tamanho dos dados: %d bytes\n", tam_dados);
        printf("    --> Dados:\n");
        do {
           char c = fgetc(file);
            if( feof(file) ) {
                break ;
            }
            printf("%c", c);
        }    while(1);


    }
    fclose(file);
}
