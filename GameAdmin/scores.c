#define TAM_LINEA 2001
#define ARCHIVO_CORRUPTO 1
#define TODO_OK 0
#include <stdlib.h>
#include <string.h>

void leerLineasDeArchivoTxt(FILE* arch);
void mostrarStringPorPantallaSinSalto(char* str);
int mostrarTablaDePuntajes();
int colocarJugadorEnTablaDePuntajes(Player* jugador);
//void buscarUltimoAparicionEnArch(FILE* arch, char* formatoVacio);

void leerLineasDeArchivoTxt(FILE* arch)
{
    char linea[TAM_LINEA];
    while(fgets(linea, TAM_LINEA,arch)!=NULL)
    {
        mostrarStringPorPantalla(linea);
       // mostrarStringPorPantallaSinSalto(linea);
    }
}
int mostrarTablaDePuntajes()
{
    FILE* arch = fopen("scores.txt", "rt");
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
int colocarJugadorEnTablaDePuntajes(Player* jugador)
{
    FILE* arch = fopen("scores.txt", "r+t");
    if(!arch)
    {
        return ARCHIVO_CORRUPTO;
    }
    //buscarUltimoAparicionEnArch(arch, "| ....... | ........................ | ....................... | ............... | ............ |");
    fseek(arch,0*(long)TAM_LINEA,SEEK_END);
    fprintf(arch, "\n| ..%s.. | .....%2d  KNIGHTS ....... | .....%2d  DEMONS ....... | ......%2d....... | ... %4.2f..... |", jugador->nombre,jugador->TotalestadoUno,jugador->TotalestadoDos,jugador->pikasRestantes,jugador->tiempoDeJuego);
    fclose(arch);
    return TODO_OK;
}
/*void buscarUltimoAparicionEnArch(FILE* arch, char* formatoVacio)
{
    char linea[TAM_LINEA];
    while(fgets(linea, TAM_LINEA,arch)!=NULL)
    {
        if(strcmp(formatoVacio,linea)==0)
        {
            break;
        }
    }
}*/