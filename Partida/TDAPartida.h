#ifndef TDAPARTIDA_H_INCLUDED
#define TDAPARTIDA_H_INCLUDED

/*Bibliotecas*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <conio.h> /// para kbhit() y getch()
#include <unistd.h> /// para sleep() (no me funciono con windows.h)

/*Defines*/
#define FACIL 1
#define MEDIO 2
#define DIFICIL 3

#define PIKAS_INICIALES_FACIL 30
#define PIKAS_INICIALES_MEDIO 15
#define PIKAS_INICIALES_DIFICIL 7

#define PREMIO_PIKAS 4

#define TIEMPO_INICIALES_FACIL 300
#define TIEMPO_INICIALES_MEDIO 200
#define TIEMPO_INICIALES_DIFICIL 150
#define PENALIZACION_DE_TIEMPO_POR_NIVEL 2

// Operaciones de obtenerTiempo()
#define T_INICIAL 1
#define T_TRANSCURRIDO 2
#define T_REINICIO 3
//

#define TAM_NOMBRE 4
#define TAM_PARTIDAS 20

// Todos los tamanios son en realidad el orden de la matriz
#define TAM_TABLERO_1 8 // 8x8
#define TAM_TABLERO_2 10 // 10x10
#define TAM_TABLERO_3 12 // 12x12

#define ARCHIVO_CORRUPTO 1
#define TODO_OK 0

/*Archivos Externos*/
#include "../Tablero/TDATablero.c"

typedef struct Partidas
{
    int tiempo;
    int dificultadDePartida;
    bool estadoCompletado;
}Partidas;

typedef struct Usuario
{
    char nombre[TAM_NOMBRE];
    int dificultadSeleccionada;
    int TotalestadoUno;
    int TotalestadoDos;
    float tiempoDeJuego;
    int nivelActual;
    int pikasRestantes;
}Player;

/// PARA MANEJAR EL MOVIMIENTO EN EL TABLERO (CON LO DE AGUS NO ES NECESARIO)
/*
typedef struct
{
    int fila;
    int columna;
    bool usoPika; /// Si no convence, quizas podria cambiarse por char teclaIngresada; (para desps comparar si es "P")
}Cursor;
*/
///

typedef struct
{
    Partidas niveles[TAM_PARTIDAS];
    Player jugador;
    // Cursor cursor;
    tJugada cursor;
    /* data */
}Admin;

/* FUNCIONES */
// Principales
bool iniciarPartida(Admin*);
void ciclarPartida(Admin*);
int postNivel(Admin*, int);

// Secundarias
int guardarPartida(Admin*);
void pedirJugada(Admin*, char**, size_t);
// bool ejecutarJugada(Admin*, char**, size_t);
void mostrarNivelPikasActual(int, int);

// Auxiliares
int tiempo(const Admin*, int);
size_t obtenerTamTablero(const Admin*);
// void invertirCasilla(char* casilla);
// int obtenerBandoGanador(char**, size_t);

#endif // TDAPARTIDA_H_INCLUDED
