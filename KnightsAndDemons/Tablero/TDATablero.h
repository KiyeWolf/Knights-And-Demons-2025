#ifndef TDATABLERO_H_INCLUDED
#define TDATABLERO_H_INCLUDED

//Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

//Externa
#include "../Juego/TDAJuego.h"

//Defines
#define CORRECTO 1
#define INCORRECTO 0
#define ADY 4 //cantidad de casillas adyacentes
#define GANO_KNIGHTS 1
#define GANO_DEMONS 2

//Colores del tablero
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

//Funciones del tablero
void mostrarTablero(const int, char **, tJugada *);
void liberarMemoriaTablero(char **, const int);
char **crearTablero(const int);
void inicializarTablero(const int, char **, tJugada *);
int verificarVictoria(const int, char **);
char intercambiarEstadoCasilla(char *);
//int ejecutarJugada(const int, char **, tJugada *);

#endif // TDATABLERO_H_INCLUDED
