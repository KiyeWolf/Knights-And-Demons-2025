#ifndef VISUALPARTIDA_H_INCLUDED
#define VISUALPARTIDA_H_INCLUDED
#include <SDL3/SDL_surface.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "../../Partida/TDAPartida.h"
#include "../../Tablero/TDATablero.h"
#include "VisualTablero.h"
#include "VisualJuego.h"
#include "../Audio/Audio.h"
#define RUTA_IMAGEN_CABALLERO "./SDL3-Archivos/Visual/Imagenes/caballero.bmp"
#define RUTA_FUENTE_MENSAJE_INICIO_NIVEL "./SDL3-Archivos/Visual/freefont-20120503/FreeMonoOblique.ttf"
#define RUTA_IMAGEN_DEMONIO  "./SDL3-Archivos/Visual/Imagenes/demonio.bmp"
#define RUTA_FUENTE_TIEMPO_RESTANTE  "./SDL3-Archivos/Visual/freefont-20120503/FreeMonoOblique.ttf"
#define TAMANIO_TEXTO_TIEMPO_RESTANTE 30
#define TAMANIO_TEXTO_INICIO_NIVEL 30


#define TAMANIO_CELDA 40

int jugar(Admin* manager,  SDL_Renderer* renderer, TTF_Font* font,tSonido* sonidoBotonCasilla);
int ciclarPartida(Admin* admin, SDL_Renderer* renderer,tSonido* sonidoBotonCasilla);
int iniciarPartidaConSDL(Admin* admin, SDL_Texture* textKnight, SDL_Texture* textDemon, SDL_Renderer* renderer,tSonido* sonidoBotonCasilla);
void actualizarHoverCelda(int* hoverEnFila,int* hoverEnColumna,int mouseX,int mouseY,const size_t tamTablero,const int tamCelda);
void renderizarContadoresSDL(SDL_Renderer* renderer, const int tiempo, TTF_Font* font);
void renderizarMensajeDeQueNoHayPikas(SDL_Renderer* renderer, TTF_Font* font);
void mostrarMensajeDeInicioDeNivel(SDL_Renderer* renderer, TTF_Font* font);
void preguntarGuardarPartida(int* respuesta, SDL_Renderer* renderer, TTF_Font* font);

int postNivelUsandoSDL(Admin* admin, int resultado, SDL_Renderer* renderer, TTF_Font* font);
#endif
