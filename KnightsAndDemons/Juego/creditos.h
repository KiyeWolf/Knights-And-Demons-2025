#ifndef CREDITOS_H_INCLUDED
#define CREDITOS_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
//Ruta del archivo de creditos desde  la cbp
#define RUTA_CREDITOS "./Juego/textoDeLosCreditos.txt"

#define TAM_LINEA 2001

FILE* abrirArchivoTxt(char* arch);
void mostrarStringPorPantalla(char* str);
void leerStringDeArchivo(FILE* arch);
void mostrarCreditos();



#endif // CREDITOS_H_INCLUDED
