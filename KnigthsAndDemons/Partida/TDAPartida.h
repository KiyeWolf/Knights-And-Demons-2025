#ifndef TDAPARTIDA_H_INCLUDED
#define TDAPARTIDA_H_INCLUDED

/*Bibliotecas*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <conio.h> /// para kbhit() y getch()
#include <windows.h> /// para Sleep()
#include "../Juego/TDAJuego.h"
#include "../Tablero/TDATablero.h"
/*Defines*/
#define FACIL 1
#define MEDIO 2
#define DIFICIL 3



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
#define FINAL_BUENO "\nParece que no pudiste manejar bien tu recurso mas preciado, becario...\n"\
                    " aunque siempre vas a poder volver a intentarlo, al fin y al cabo esos demonios\n"\
                    " no se van a matar solos y yo... digamos que tengo para rato aquí, je\n"

// Todos los tamanios son en realidad el orden de la matriz
#define TAM_TABLERO_1 4 // 8x8
#define TAM_TABLERO_2 10 // 10x10
#define TAM_TABLERO_3 12 // 12x12

#define ARCHIVO_CORRUPTO 1
#define TODO_OK 0

/*Archivos Externos*/
#define RUTA_PARTIDAS_GUARDADAS "./Juego/Saved/"


/* FUNCIONES */
// Principales
bool iniciarPartida(Admin* admin);
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



int jugar(Admin* manager);
void barraDeCarga();

#endif // TDAPARTIDA_H_INCLUDED
