#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "tablica.h"

#define Init 0

void prinTabela(unsigned int tabela[], int ile, int wypisz)
{
    int licznik = 0;
    for(int i = 0; i<ile; i++)
    {
        printf(" 0x%08X,", tabela[i]);

        licznik +=1;
        if(licznik >= wypisz)
        {
            printf("\n");
            licznik = 0;
        }
    }
}

int liczDoZnaku(const char *napis, char znak)
{
    int licznik = 0;
    while(*napis != '\0' && *napis != znak)
    {
        licznik+=1;
        napis += 1;
    }
    return licznik;
}

int main(int argc,char *argv[])
{

    unsigned int wielomian = 0xEDB88320;

    printf("opcje uruchomienia : \n"
           "podaj nazwę pliku, aby dopisać crc\n"
           "dodatkowo podaj 32b wielomian generujący CRC aby z niego skorzystać\n");

    if(argc < 2)
    {
        printf("nie poprawna ilość argumentów\n");
    }

    FILE *openfile = fopen(argv[1], "rb");

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
    
    printf("wartości tabeli : \n");
    prinTabela(wskaznikTabela, 256, 8);

    unsigned int regCRC = Init;
    int znak;
    while((znak = fgetc(openfile)) != EOF)
    {
        regCRC = (regCRC >> 8) ^ wskaznikTabela[(regCRC ^ (unsigned char)znak) & 0xFF];
    }

    unsigned int crcKon = regCRC ^ Init;
    printf("wielomain : %08X\n", wielomian);
    printf("obliczoneCRC: %08X\n", crcKon);

    fclose(openfile);


    unsigned char nazwa[20];
    int ile = liczDoZnaku(argv[1], '.');
    strncpy(nazwa, argv[1], ile);
    nazwa[ile] = '\0';
    strcat(nazwa, ".crc\0");

    printf("zapisuję do pliku: %s\n", nazwa);

    FILE *writeToFile = fopen(nazwa, "wb");
    FILE *readFromFile = fopen(argv[1], "rb");

    while((znak = fgetc(readFromFile)) != EOF)
    {
        fputc(znak, writeToFile);     
    }

    unsigned char crcBytes[5];
    crcBytes[4] = '\0';
    crcBytes[0] = (crcKon)       & 0xFF;
    crcBytes[1] = (crcKon >> 8)  & 0xFF;
    crcBytes[2] = (crcKon >> 16) & 0xFF;
    crcBytes[3] = (crcKon >> 24) & 0xFF;

    printf("bity crc : %02X %02X %02X %02X\n",
    crcBytes[0], crcBytes[1], crcBytes[2], crcBytes[3]);
    fwrite(crcBytes, sizeof(unsigned char), 4, writeToFile);

    fclose(writeToFile);
    fclose(readFromFile);
    return 0;
}