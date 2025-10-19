#include "creditos.h"

/*Se encarga de imprimir por pantalla  un archivo txt ubicado en la ruta predefinida del encabezado
El archivo es de creditos, y se imprimirá por linea, luego una pausa esperando un imput y luego la proxima linea
hasta finalizar el archivo.*/
void mostrarCreditos()
{
    //system("chcp 65001<nul");
    FILE* archivoDeTxt = abrirArchivoTxt(RUTA_CREDITOS);
    leerStringDeArchivo(archivoDeTxt);
    fclose(archivoDeTxt);
}

/*Solo abre un archivo en formato txt, para leer*/
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

/*Tan solo imprime por pantalla un string
Está creado únicamente para mejorar la legibilidad del código*/
void mostrarStringPorPantalla(char* str)
{
    printf("%s\n",str);
}

/*Se encarga de realizar la ardua tarea, de leer por linea un archivo txt y mostrarlo, luego esperar una entrada por teclado, hasta que el archivo termine*/
void leerStringDeArchivo(FILE* arch)
{
    char linea[TAM_LINEA];
    while(fgets(linea, TAM_LINEA,arch)!=NULL)
    {
        mostrarStringPorPantalla(linea);
        getchar();
        fflush(stdin);
    }
}
