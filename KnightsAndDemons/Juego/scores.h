#ifndef SCORES_H_INCLUDED
#define SCORES_H_INCLUDED
#define TAM_LINEA_SCORES 2001
#define ARCHIVO_CORRUPTO 1
#define TODO_OK 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//  Ruta relativa del archivo de puntajes
#define RUTA_SCORES "./Juego/scores.txt"

void leerLineasDeArchivoTxt(FILE* arch);
void mostrarStringPorPantallaSinSalto(char* str);
int mostrarTablaDePuntajes();
int colocarJugadorEnTablaDePuntajes(char* nomJugador, int k,int d,int pikasRestantes, int nivelesCompletados);

#endif // SCORES_H_INCLUDED
