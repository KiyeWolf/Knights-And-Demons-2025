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
#define RUTA_IMAGEN_BARRA_DE_ESTADO_KNIGHTS "./SDL3-Archivos/Visual/Imagenes/barraDeEstadoKnights.bmp"
#define RUTA_IMAGEN_BARRA_DE_ESTADO_DEMONS "./SDL3-Archivos/Visual/Imagenes/barraDeEstado-Demons.bmp"
#define RUTA_IMAGEN_DEMONIO  "./SDL3-Archivos/Visual/Imagenes/demonio.bmp"


#define RUTA_FUENTE_MENSAJE_INICIO_NIVEL "./SDL3-Archivos/Visual/freefont-20120503/FreeMonoOblique.ttf"
#define RUTA_FUENTE_TIEMPO_RESTANTE  "./SDL3-Archivos/Visual/freefont-20120503/FreeMonoOblique.ttf"
#define RUTA_FUENTE_BARRA_DE_ESTADO "./SDL3-Archivos/Visual/freefont-20120503/FreeMonoOblique.ttf"
#define RUTA_FUENTE_PIKAS_RESTANTES "./SDL3-Archivos/Visual/freefont-20120503/FreeMonoOblique.ttf"

#define TAMANIO_TEXTO_TIEMPO_RESTANTE 30
#define TAMANIO_TEXTO_INICIO_NIVEL 30
#define TAMANIO_FUENTE_BARRA_DE_ESTADO 24
#define TAMANIO_FUENTE_PIKAS_RESTANTES 24


#define RUTA_VFX_FUEGO "./SDL3-Archivos/Visual/VFX/VFX-Fuego.bmp"


#define TAMANIO_CELDA 40

// Barra de estado:
#define ANCHO_BARRA 600 
#define ALTO_BARRA 50
#define POS_X_BARRA 100 // Margen izquierdo
#define POS_Y_BARRA 20  // Margen superior
typedef struct {
    // Imagen para Caballeros
    SDL_Texture* texK;    
    // Imagen para Demonios
    SDL_Texture* texD; 
    // Para los textos de abajo    
    TTF_Font* fuente;          
} tRecursosHUD;
typedef struct EfectoVisual
{
    unsigned int id;
    SDL_Texture* text;
    TTF_Font* font;
    int alpha;
    int alpha2;
    int efectos[3];
    /* data */
}tVFX;


int jugar(Admin* manager,  SDL_Renderer* renderer, TTF_Font* font,tSonido* sonidoBotonCasilla);
int ciclarPartida(Admin* admin, SDL_Renderer* renderer,tSonido* sonidoBotonCasilla);
int iniciarPartidaConSDL(Admin* admin, SDL_Texture* textKnight, SDL_Texture* textDemon, SDL_Renderer* renderer,tSonido* sonidoBotonCasilla);
void actualizarHoverCelda(int* hoverEnFila,int* hoverEnColumna,int mouseX,int mouseY,const size_t tamTablero,const int tamCelda);
void renderizarContadoresSDL(SDL_Renderer* renderer, const int tiempo, TTF_Font* font);
void renderizarMensajeDeQueNoHayPikas(SDL_Renderer* renderer, TTF_Font* font);
void mostrarMensajeDeInicioDeNivel(SDL_Renderer* renderer, TTF_Font* font);
void preguntarGuardarPartida(int* respuesta, SDL_Renderer* renderer, TTF_Font* font);

void renderizarPikasRestantes(SDL_Renderer* renderer,tVFX* vfx, const int cantPikas,const int altoPantalla);

//Barra de estado
bool inicializarHUD(SDL_Renderer* renderer, tRecursosHUD* hud);
void renderizarBarraEstadoSDL(SDL_Renderer* renderer, tRecursosHUD* hud, int scoreCaballeros, int scoreDemonios);
void liberarHUD(tRecursosHUD* hud);


//VFX
bool inicializarVFX(SDL_Renderer* renderer, tVFX* vfx, const int id, const char* rutaIMG, const char* rutaFuente, const int tamFuente);
void activarEfecto(tVFX* vfx, const int efecto);
bool seActivoEfectoAlpha(tVFX* vfx);

//VFX Particular
void activarEfectoFuego(tVFX* fuego);

//Funcionees del post nivel
int postNivelUsandoSDL(Admin* admin, int resultado, SDL_Renderer* renderer, TTF_Font* font);
bool mostrarAgradecimientoCaballeros(SDL_Renderer* renderer, TTF_Font* font, const char* mensaje);
void chequearYMostrarFinalDelJuego(Admin* manager, SDL_Renderer* renderer, TTF_Font* font);
#endif
