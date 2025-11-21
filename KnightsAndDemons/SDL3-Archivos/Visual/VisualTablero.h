#ifndef VISUAL_TABLERO_H_INCLUDED
#define VISUAL_TABLERO_H_INCLUDED
#include <SDL3/SDL_surface.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "../../Tablero/TDATablero.h"
#include "VisualJuego.h"
#include <stdbool.h>
void renderizarTableroConSDL(SDL_Renderer *renderer,
                          char **tablero,
                          size_t tam,
                          SDL_Texture *texKnight,
                          SDL_Texture *texDemon,
                          const int tamCelda,
                          bool mostrarGrilla,
                          int luzHoverFila,
                          int luzHoverColumna);

bool hizoClickEnTablero(const int mouseX,const int mouseY,char** tablero,const size_t tamTablero,const int tamCelda);
int ejecutarJugadaSDL(const int orden,char** mat, int tamCelda,tJugada *jug);

#endif