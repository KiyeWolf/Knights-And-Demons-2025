#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED


#include <SDL3/SDL_audio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define CANAL_STEREO 2
#define CANAL_MONO 1

#define RUTA_SONIDO_BOTON_MENU "./SDL3-Archivos/Audio/Menu/BotonPresionadoRecortado.wav"
#define RUTA_SONIDO_BOTON_CASILLA "./SDL3-Archivos/Audio/Tablero/BotonPresionadoCasilla.wav"
#define RUTA_SONIDO_BOTON_PIKA "./SDL3-Archivos/Audio/Tablero/BotonPresionadoPikaCasilla.wav"
#define RUTA_BACKGROUND_MUSIC_FINAL_KNIGHTS "./SDL3-Archivos/Audio/Historia/endingHistoryTheme1.wav"
#define RUTA_BACKGROUND_MUSIC_FINAL_DEMONS "./SDL3-Archivos/Audio/Historia/endingHistoryTheme2.wav"
#define RUTA_BACKGROUND_MUSIC_FINAL_VERDADERO "./SDL3-Archivos/Audio/Historia/endingHistoryTheme3.wav"
//#define RUTA_BACKGROUND_MUSIC_FINAL_NEUTRAL "./SDL3-Archivos/Audio/Historia/endingHistoryTheme0.wav"
#define RUTA_BACKGROUND_MUSIC_FINAL "./SDL3-Archivos/Audio/Historia/endingMusicBackground.wav"



#define RUTA_BACKGROUND_MUSIC_HISTORIA_PRINCIPAL "./SDL3-Archivos/Audio/Historia/mainHistoryTheme.wav"
#define RUTA_BACKGROUND_MUSIC_MENU "./SDL3-Archivos/Audio/Menu/mainMenuTheme.wav"
#define RUTA_BACKGROUND_MUSIC_NIVEL "./SDL3-Archivos/Audio/Tablero/mainLevelTheme.wav"
#define RUTA_BACKGROUND_MUSIC_CREDITOS "./SDL3-Archivos/Audio/Menu/creditsMusic.wav"


typedef struct {

    // tan solo un identificador del dispositivo
    SDL_AudioDeviceID id;
    // formato REAL del dispositivo es esta de aca abajo(si)
    SDL_AudioSpec spec;

} tDispositivoAudio;

//NO me gusta usar estaticas pero la justificacion es muy buena: No tengo que mandar el parámetro para todos lados.
static tDispositivoAudio dispositivoDeAudio={0};
//este stream de aca abajo es para reproducir NO TIENE NADA QUE VER CON LA CONVERSION DE AUDIO
static SDL_AudioStream* streamActivo = NULL;
static SDL_AudioStream* streamBGM = NULL;

typedef struct AudioWAV
{
    /* Aquí van los atributos relacionados con el audio
     Para manejar audio en sdl, necesito una estructura que guarda datos del audio en sí
     como el formato, la frecuencia, longitud de los samples, otros datos de audio
    que no entieendo bien porque no estudio esa parte.*/
    //La parte que importa es que Manejamos WAV
    SDL_AudioSpec wavSpec;
    Uint8* buffer;
    Uint32 longitud;
    bool cargado;

} tSoundWav;

/*La estructura de tSonido se encarga de guardar en sí el sonido de sdl.
Es distinta a la anterior, porque esta no lo guarda en wav si no ya listo para ser ejecutado*/
typedef struct AudioSonido
{
    Uint8* buffer;
    Uint32 longitud;
    SDL_AudioSpec sonidoSpec;
}tSonido;


bool inicializarAudioSDL();
bool cargarUnSonidoNuevo(const char* rutaArch, tSonido* salida);
bool necesitaConversionDeAudio(SDL_AudioSpec* audio);

//SFX
bool reproducirSFX(const tSonido* sonido);
bool liberarSonido(tSonido* sonido);
void cerrarAudio();
//BGMusic
bool crearBackgroundMusic(const char* ruta,tSonido* s);
bool cargarUnaBGMNueva(const char* rutaArch, tSonido* salida);
bool reproducirBGM(const tSonido* musica);
bool liberarBGM(tSonido* sonido);
void detenerMusicaBGM();
bool terminoLaMusica();
void recargarLaBGM(tSonido* bgm);
bool bajarElVolumenDeLaMusica(float factorDecremento);
bool restaurarVolumenMusica();
void pausarMusicaBGM();

#endif // AUDIO_H_INCLUDED
