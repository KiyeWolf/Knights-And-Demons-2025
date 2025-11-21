#ifndef VISUAL_H_INCLUDED
#define VISUAL_H_INCLUDED
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "../../menu.h"
#include "../../Juego/TDAJuego.h"
#include "../../Juego/scores.h"
#include "../../SDL3-Archivos/Audio/Audio.h"

#define WIDTH 800
#define HIGHT 600
#define HEIGHT 600
#define RUTA_ARCHIVO_FUENTES_DE_TEXTO_1 "./SDL3-Archivos/Visual/freefont-20120503/FreeMono.ttf"
#define RUTA_ARCHIVO_FUENTES_DE_TEXTO_2 "./SDL3-Archivos/Visual/freefont-20120503/FreeMono.ttf"
#define MENSAJE_MENU_PRINCIPAL "Knights && Demons 2025"
#define MENSAJE2 "Valiente Guerrero..."


void mostrarPantallaBienvenida(SDL_Renderer* renderer,TTF_Font* font);
void mostrarMenuPrincipal(SDL_Renderer* renderer, TTF_Font* font, char* opcion, tSonido* sonidoBotonMenu);
void mostrarPantallaNombre(SDL_Renderer* renderer, TTF_Font* font, char* nombrePlayer, SDL_Window* window);
void mostrarPantallaDificultad(SDL_Renderer* renderer,TTF_Font* font,int* dificultad);
void mostrarPantallaHistoriaInicial(SDL_Renderer* renderer,TTF_Font* font, size_t* retorno);


void mostrarMensajeEnVentanaYBorrarDespuesDeTecla(SDL_Renderer* renderer, TTF_Font* font, char* mensaje);

//void mostrarPantallaSolicitudDeNombreParaCargarPartida(SDL_Renderer* renderer, TTF_Font* font);
//void mostrarMensajeDePartidaNoEncontrada(SDL_Renderer* renderer, TTF_Font* font);

void mostrarCreditosEnPantalla(SDL_Renderer* renderer, TTF_Font* font, size_t* retorno);

void mostrarTablaDePuntajesDeArchivo(SDL_Renderer* renderer,TTF_Font* font);

#endif // VISUAL_H_INCLUDED
