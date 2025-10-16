#include "creditos.h"
void mostrarCreditos()
{
    //system("chcp 65001<nul");
    FILE* archivoDeTxt = abrirArchivoTxt(RUTA_CREDITOS);
    leerStringDeArchivo(archivoDeTxt);
    fclose(archivoDeTxt);
}
FILE* abrirArchivoTxt(char* arch)
{
    FILE* archivo = fopen(arch,"rt");
    if(archivo==NULL)
    {
        return NULL;
    }
    else
    {
        return archivo;
    }
}
void mostrarStringPorPantalla(char* str)
{
    printf("%s\n",str);
}
void leerStringDeArchivo(FILE* arch)
{
    char linea[TAM_LINEA];
    while(fscanf(arch,"%s", linea)==1)
    {
        mostrarStringPorPantalla(linea);
        getchar();
        fflush(stdin);
    }
}
