#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "creditos.c"
#include "TDAJuego.c"
#include "scores.c"


#define OPCIONES_VALIDAS 53
#define CARGAR_PARTIDA 49
#define INICIAR_NUEVA_PARTIDA 50
#define CREDITOS 51
#define TABLA_DE_PUNTAJES 52
#define MENSAJE_DEL_MENU_PRINCIPAL \
"------------------------------------------------\n"\
"\tBienvenido a Knights && Demons (2025)\n"\
"------------------------------------------------\n"\
"\t1. CARGAR PARTIDA \n"\
"\t2. NUEVO JUEGO\n"\
"\t3. CREDITOS\n"\
"\t4. TABLA DE PUNTAJES\n"\
"\t5. SALIR"

int solicitarDificultad();
void pausaYLimpiadoDePantalla();
char mostarMenuPrincipalConMensaje(char* msj, char* opciones);


#endif // MENU_H_INCLUDED
