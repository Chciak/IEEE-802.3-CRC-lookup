#include "tablica.h"
unsigned char * genCRCTable(unsigned char fileName[], unsigned int tabela[], unsigned int wielomian)
{
    snprintf(fileName, 13*sizeof(char), "%08X.txt", wielomian);
    FILE *plikTabeli = fopen(fileName, "w");

    fprintf(plikTabeli, "static unsigned int tabelaWartosci[256] = {\n");
    for(int indeks = 0; indeks < 256; indeks++)
    {
        unsigned int rejestr = indeks;

        for(int bit = 0; bit < 8; bit++)
        {
            if(rejestr & 1)             
                rejestr = (rejestr >> 1) ^ wielomian;
            else
                rejestr >>= 1;
        }

        fprintf(plikTabeli, "    0x%08X", rejestr);
        if(indeks < 255) 
            fprintf(plikTabeli, ",\n");
        else             
            fprintf(plikTabeli, "\n");
        tabela[indeks] = rejestr;
    }
    fprintf(plikTabeli, "};");
    fclose(plikTabeli);
    return fileName;
}