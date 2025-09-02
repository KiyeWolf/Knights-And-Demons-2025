#define TAM_LINEA 2001
#define ARCHIVO_CORRUPTO 1
#define TODO_OK 0
#include <stdlib.h>
void leerLineasDeArchivoTxt(FILE* arch);
void mostrarStringPorPantallaSinSalto(char* str);
int mostrarTablaDePuntajes();
void leerLineasDeArchivoTxt(FILE* arch)
{
    char linea[TAM_LINEA];
    while(fscanf(arch,"%s", linea)==1)
    {
        mostrarStringPorPantallaSinSalto(linea);
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