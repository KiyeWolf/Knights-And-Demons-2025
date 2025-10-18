#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



#define OPCIONES_VALIDAS '5'
#define CARGAR_PARTIDA '1'
#define INICIAR_NUEVA_PARTIDA '2'
#define MIN_CANTIDAD_LETRAS_NOMBRE 3
#define CREDITOS '3'
#define TABLA_DE_PUNTAJES '4'
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
char mostrarMenuPrincipalConMensaje(char* msj, char* opciones);
void limpiarBuffer();

#endif // MENU_H_INCLUDED
