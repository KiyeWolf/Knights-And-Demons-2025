#ifndef TDAJUEGO_H_INCLUDED
#define TDAJUEGO_H_INCLUDED
#include <stdbool.h>
#include <string.h>
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

//este pedazo de acá modifique
#include "../Tablero/crearTablero.c"
#include "../Tablero/inicializarTablero.c"
#include "../Tablero/mostrarTablero.c"
#define ORDEN_TEMPORAL 8
//


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

void inicializarJugador(Player* jugador, char* nombre, const  int* dificultad);
int iniciarJuegoNuevo(Admin* manager, int* d, char*  nomNuevo);
int jugar(Admin* manager);
void inicializarPartidas(Partidas* partidas,const  int* dificultad);
void mostrarManager(Admin* manager);
void mostrarNiveles(Partidas* niveles);
void mostrarJugador(Player* jugador);
int guardarPartida(Admin* manager);
int cargarPartida(Admin* manager,  char* nomGuardado);


#endif // TDAJUEGO_H_INCLUDED
