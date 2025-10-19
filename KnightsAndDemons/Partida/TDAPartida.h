#ifndef TDAPARTIDA_H_INCLUDED
#define TDAPARTIDA_H_INCLUDED

/*Bibliotecas*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <ctype.h> /// para tolower()
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

//Todos los finales posibles del juego
#define MENSAJE_DERROTA "\nParece que no pudiste manejar bien tu recurso mas preciado, becario...\n"\
                    " aunque siempre vas a poder volver a intentarlo, al fin y al cabo esos demonios\n"\
                    " no se van a matar solos y yo... digamos que tengo para rato aquí, je...\n"\
                    " ¡¡Ve y defiende nuestros otros frentes!!\n"

#define FINAL_BUENO "El reino estalló en vítores. Los Demonios, cuya influencia había sido totalmente borrada de la faz del mundo conocido, \n"\
                    "se retiraron a las profundidades del Averno\n"\
                    "Bakelor está orgulloso\n"

#define FINAL_NEUTRAL "\n Becario, no lo has hecho mal, pero... ¡DEBEMOS GANAR LA GUERRA, NO ES SUFICIENTE CON TENER ALGUNOS FRENTES!\n"\
                      " Apresurate y no me hagas enojar.\n"\
                      "-Bakelor, el señor de la guerra (en una situación doméstica algo delicada)\n"

#define FINAL_MALO "\nDesde su cama de convalecencia, el Rey Bakelor observó por la ventana cómo el cielo de Garnick, una vez azul, se teñía de un perpetuo tono carmesí. \n"\
                    "Las hordas demoníacas, al percibir la victoria táctica, irrumpieron en el castillo con una confianza renovada y brutal.\n"\
                    "Has logrado completar el desafío; pero con un costo impensable.\n"\
                    "Bakelor está horrorizado... Garnick está condenado\n"\
                     "¡Qué has hecho becario!\n"

#define FINAL_BUENO_MAXIMA_DIFICULTAD "El reino estalló en vítores. Los Demonios, cuya influencia había sido totalmente borrada de la faz del mundo conocido, \n"\
                                      "se retiraron a las profundidades del Averno\n"\
                                      "Bakelor, orgulloso y alegre te dice:\n"\
                                      "¡Valiente becario! No solo lograste la victoria donde yo temía la derrota,\n"\
                                      "sino que lo hiciste en las condiciones más duras que jamás se han impuesto a un comandante.\n"\
                                      "\n"\
                                      "El Rey Bakelor tomó su espada ceremonial y, colocándola sobre tu hombro, continuó:\n"\
                                      "A partir de este día, ya no eres un becario. Eres mi mano derecha, el Comandante Dorado de la Luz, el arquitecto de la paz en Garnick. \n"\
                                      "Tu nombre será recordado no solo por la victoria, sino por la perfección con la que la lograste. \n"\
                                      "Que el sol brille eternamente sobre nuestro reino gracias a tu heroísmo.\n"

// Todos los tamanios son en realidad el orden de la matriz
#define TAM_TABLERO_1 8 // 8x8
#define TAM_TABLERO_2 10 // 10x10
#define TAM_TABLERO_3 12 // 12x12

#define ARCHIVO_CORRUPTO 1
#define TODO_OK 0

/*Archivos Externos*/
#define RUTA_PARTIDAS_GUARDADAS "./Juego/Saved/"

/* FUNCIONES */
// Principales
int iniciarPartida(Admin* admin);
int ciclarPartida(Admin*);
int postNivel(Admin*, int);

// Secundarias
int guardarPartida(Admin*);
void pedirJugada(Admin*, char**, size_t);
void mostrarNivelPikasActual(int, int);

// Auxiliares
int tiempo(const Admin*, int);
size_t obtenerTamTablero(const Admin*);

//Mixes de Guille
int jugar(Admin* manager);
void barraDeCarga();
void textoDeInicioDeNivel();
#endif // TDAPARTIDA_H_INCLUDED
