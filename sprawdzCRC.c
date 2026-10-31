#include <stdio.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "tablica.h"

#define Init 0
#define Ceck 0

int main(int argc,char *argv[])
{
    unsigned int wielomian = 0xEDB88320;

    if(argc < 2)
    {
        printf("nie poprawna ilość argumentów\n");
        return 0;
    }

    unsigned int *wskaznikTabela;
    unsigned int tabela[256];

    if(argc == 3)
    {
        wielomian = (unsigned int)strtoul(argv[2], NULL, 16);
        
        unsigned char fileName[13];
        genCRCTable(fileName, tabela, wielomian);
        wskaznikTabela = tabela;
    }
    else
    {
        wskaznikTabela = tabelaWartosci;
    }

    FILE *openfile = fopen(argv[1], "rb");

    unsigned int regCRC = Init;
    int znak;
    while((znak = fgetc(openfile)) != EOF)
    {
        regCRC = (regCRC >> 8) ^ wskaznikTabela[(regCRC ^ (unsigned char)znak) & 0xFF];
    }

    unsigned int crcKon = regCRC ^ Init;
    
    if(crcKon == Ceck)
    {
        printf("CRC OK\n");
        printf("obliczone CRC : %08X\n", crcKon);
    }
    else
    {
        printf("plik uszkodzony\n");
        printf("obliczone CRC : %08X\n", crcKon);
    }

    fclose(openfile);


}