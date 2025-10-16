#include "scores.h"
void leerLineasDeArchivoTxt(FILE* arch)
{
    char linea[TAM_LINEA];
    while(fgets(linea, TAM_LINEA,arch)!=NULL)
    {
       printf("%s\n",linea);
    }
}
int mostrarTablaDePuntajes()
{
    FILE* arch = fopen(RUTA_SCORES, "rt");
    if(!arch)
    {
        return ARCHIVO_CORRUPTO;
    }
    leerLineasDeArchivoTxt(arch);
    fclose(arch);
    return TODO_OK;
}
void mostrarStringPorPantallaSinSalto(char* str)
{
    printf("%s",str);
}
int colocarJugadorEnTablaDePuntajes(char* nomJugador, int knightsTotales,int demonsTotales,int pikasRestantes, float tiempoTotal)
{
    FILE* arch = fopen(RUTA_SCORES, "r+t");
    if(!arch)
    {
        return ARCHIVO_CORRUPTO;
    }
    //buscarUltimoAparicionEnArch(arch, "| ....... | ........................ | ....................... | ............... | ............ |");
    fseek(arch,0*(long)TAM_LINEA,SEEK_END);
    fprintf(arch, "\n| ..%s.. | .....%2d  KNIGHTS ....... | .....%2d  DEMONS ....... | ......%2d....... | ... %4.2f..... |", nomJugador,knightsTotales,demonsTotales,pikasRestantes,tiempoTotal);
    fclose(arch);
    return TODO_OK;
}
