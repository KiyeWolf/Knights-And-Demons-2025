#include "scores.h"

/*Recibe un archivo y lee el archivo por linea, mostrando la linea y luego un salto de linea*/
void leerLineasDeArchivoTxt(FILE* arch)
{
    char linea[TAM_LINEA_SCORES];
    while(fgets(linea, TAM_LINEA_SCORES,arch)!=NULL)
    {
       printf("%s\n",linea);
    }
}

/*Buscar un archivo txt en la ruta definida en el encabezado y luego leer el archivo txt, mostrandolo en
pantalla sin hacer pausas y cierra los archivos correspondientes.*/
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

/*Solo es una función que imprime un string recibido por parámetro*/
void mostrarStringPorPantallaSinSalto(char* str)
{
    printf("%s",str);
}

/*Se encarga de recibir multiples datos de la estructura de Jugador y abrir un archivo txt en la ruta
predefinida en el encabezado, luego al final del archivo coloca todos los datos del jugador.
Con un formato específico*/
int colocarJugadorEnTablaDePuntajes(char* nomJugador, int k,int d,int pikasRestantes, int nivelesCompletados)
{
    FILE* arch = fopen(RUTA_SCORES, "r+t");
    if(!arch)
    {
        return ARCHIVO_CORRUPTO;
    }

    fseek(arch,0*(long)TAM_LINEA_SCORES,SEEK_END);
    fprintf(arch, "\n| ..%s.. | .....%2d  KNIGHTS ....... | .....%2d  DEMONS ....... | ......%2d....... | ..... %d......... |", nomJugador,k,d,pikasRestantes,nivelesCompletados);
    fclose(arch);
    return TODO_OK;
}
