#ifndef TDAJUEGO_H_INCLUDED
#define TDAJUEGO_H_INCLUDED
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#define FACIL 1
#define MEDIO 2
#define DIFICIL 3
#define PIKAS_INICIALES_FACIL 30
#define PIKAS_INICIALES_MEDIO 15
#define PIKAS_INICIALES_DIFICIL 7
#define TIEMPO_INICIALES_FACIL 300
#define TIEMPO_INICIALES_MEDIO 200
#define TIEMPO_INICIALES_DIFICIL 150
#define NIVEL_INICIAL 0
#define PENALIZACION_DE_TIEMPO_POR_NIVEL 2
#define TAM_NOMBRE 3 //Recuerda que se le suma uno por el caracter nulo dentro del código
#define TAM_LINEA_JUEGO 2001

#define TAM_PARTIDAS 8


#define ARCHIVO_CORRUPTO 1
#define TODO_OK 0
#define RUTA_PARTIDAS_GUARDADAS_PARA_CARGAR "./Juego/Saved/"
#define RUTA_HISTORIA_INICIAL "./Juego/Saved/lore.txt"




typedef struct Jugada{
            size_t posCursorX;
            size_t posCursorY;
            bool actua;
            bool seUsoPika;


         }tJugada;




//Estructura de Jugada

    typedef struct Partidas
    {
        int tiempo;
        int dificultadDePartida;
        bool estadoCompletado;
    }Partidas;

    typedef struct Usuario
    {
        char nombre[TAM_NOMBRE+1];
        int dificultadSeleccionada;
        int TotalestadoUno;
        int TotalestadoDos;
        int nivelesCompletados;
        int nivelActual;
        int pikasRestantes;
    }Player;



    typedef struct
    {
        Partidas niveles[TAM_PARTIDAS];
        Player jugador;
        tJugada cursor;
    }Admin;




void inicializarJugador(Player* jugador, char* nombre, const  int* dificultad);
int iniciarJuegoNuevo(Admin* manager, int* d, char*  nomNuevo);

void inicializarPartidas(Partidas* partidas,const  int* dificultad);

int cargarPartida(Admin* manager,  char* nomGuardado);
int mostrarHistoriaInicial();

//AUXILIAR AL DESARROLLO
/*void mostrarManager(Admin* manager);
void mostrarNiveles(Partidas* niveles);
void mostrarJugador(Player* jugador);*/

#endif // TDAJUEGO_H_INCLUDED
