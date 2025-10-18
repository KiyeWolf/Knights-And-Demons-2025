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
int colocarJugadorEnTablaDePuntajes(char* nomJugador, int k,int d,int pikasRestantes, int nivelesCompletados)
{
    FILE* arch = fopen(RUTA_SCORES, "r+t");
    if(!arch)
    {
        return ARCHIVO_CORRUPTO;
    }
    //buscarUltimoAparicionEnArch(arch, "| ....... | ........................ | ....................... | ............... | ............ |");
    fseek(arch,0*(long)TAM_LINEA,SEEK_END);
    fprintf(arch, "\n| ..%s.. | .....%2d  KNIGHTS ....... | .....%2d  DEMONS ....... | ......%2d....... | ..... %d......... |", nomJugador,k,d,pikasRestantes,nivelesCompletados);
    fclose(arch);
    return TODO_OK;
}
