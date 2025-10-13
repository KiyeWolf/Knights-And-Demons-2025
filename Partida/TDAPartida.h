#ifndef TDAPARTIDA_H_INCLUDED
#define TDAPARTIDA_H_INCLUDED

/*Bibliotecas*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*Defines*/
#define FACIL 1
#define MEDIO 2
#define DIFICIL 3

#define PIKAS_INICIALES_FACIL 30
#define PIKAS_INICIALES_MEDIO 15
#define PIKAS_INICIALES_DIFICIL 7

#define TIEMPO_INICIALES_FACIL 300
#define TIEMPO_INICIALES_MEDIO 200
#define TIEMPO_INICIALES_DIFICIL 150
#define PENALIZACION_DE_TIEMPO_POR_NIVEL 2

#define TAM_NOMBRE 4
#define TAM_PARTIDAS 20

#define ARCHIVO_CORRUPTO 1
#define TODO_OK 0

/*Archivos Externos*/
#include "../Tablero/crearTablero.c"
#include "../Tablero/inicializarTablero.c"
#include "../Tablero/mostrarTablero.c"

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

typedef struct
{
    Partidas niveles[TAM_PARTIDAS];
    Player jugador;
    /* data */
}Admin;

/* FUNCIONES */
bool iniciarPartida();
void ciclarPartida();
int postNivel();
void mostrarNivelPikasActual();

/* PENDING */
int tiempo(int);
int pedirJugada();
int ejecutarJugada();
int guardarPartida();

#endif // TDAPARTIDA_H_INCLUDED
