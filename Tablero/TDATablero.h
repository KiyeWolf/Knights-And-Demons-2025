#ifndef TDATABLERO_H_INCLUDED
#define TDATABLERO_H_INCLUDED

//Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

//Defines
#define CORRECTO 1
#define INCORRECTO 0
#define ADY 4 //cantidad de casillas adyacentes

//Estructura de Jugada
typedef struct Jugada{
    size_t posCursorX;
    size_t posCursorY;
    bool actua;
    bool seUsoPika;
}tJugada;

//Funciones del tablero
int verificarCoordenadasCursor(const int, tJugada *);
void mostrarTablero(const int, char **, tJugada *);
void liberarMemoriaTablero(char **, const int);
char **crearTablero(const int);
void inicializarTablero(const int, char **, tJugada *);
int verificarVictoria(const int, char **);
char intercambiarEstadoCasilla(char *);
int ejecutarJugada(const int, char **, tJugada *);

#endif // TDATABLERO_H_INCLUDED
