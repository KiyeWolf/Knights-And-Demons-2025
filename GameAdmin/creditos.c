#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
//Desarrollar una función que muestre los créditos del juego mostrarCredito()
//1. Desarrollar una función que abra un archivo de Texto txt, mediante su nombre.
//2. Desarrollar una función para que muestre la primer linea del archivo por pantalla.
//3. Ciclar la función, hasta que lea todas las líneas de del archivo de txt.
//PRE-CONDICIONES:
//1.Activar los acentos para mostrarlos por pantalla.
//2. Debe estar incluida <stdio.h>
//3. Debe estar incluida <stdlib.h>
//4. Debe estar incluida <ctype.h>
//POST-CONDICIONES:
//1. (depende de como se haga el menu) retornar un TRUE.
#define TAM_LINEA 2001
FILE* abrirArchivoTxt(char* arch);
void mostrarStringPorPantalla(char* str);
void leerStringDeArchivo(FILE* arch);
void mostrarCreditos();

void mostrarCreditos()
{
    //system("chcp 65001<nul");
    FILE* archivoDeTxt = abrirArchivoTxt("textoDeLosCreditos.txt");
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
