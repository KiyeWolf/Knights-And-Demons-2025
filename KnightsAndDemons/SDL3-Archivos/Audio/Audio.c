/*Implementation file for audio subsystem.*/
#include "Audio.h"


bool inicializarAudioSDL()
{
    SDL_AudioSpec audio = {0};
    audio.freq = 44100;
    audio.format = SDL_AUDIO_S16;
    audio.channels = CANAL_STEREO;

    
    //primero encontrar el dispositivo de audio por defecto

    //esta es estatica, esta en el header:


    dispositivoDeAudio.id =  SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,&audio);
    if(!dispositivoDeAudio.id)
    {
        printf("[DEBUG] Error al inicializar el dispositivo de audio: %s", SDL_GetError());
        return false;
    }
    dispositivoDeAudio.spec = audio;
    SDL_GetAudioDeviceFormat(dispositivoDeAudio.id, &(dispositivoDeAudio.spec),NULL);
    SDL_PauseAudioDevice(dispositivoDeAudio.id);

    return true;    
}
bool cargarUnSonidoNuevo(const char* rutaArch, tSonido* salida)
{
    SDL_AudioSpec audio = {0};
    tSoundWav sound = {0};

    //LoadWav se encarga de buscar el archivo en la ruta, cargarlo a sound y audio que son auxiliares
    //antes de cargarlo a la salida
    if(!SDL_LoadWAV(rutaArch,&audio,&(sound.buffer), &(sound.longitud)))
    {
        printf("[DEBUG] Error cargando WAV: %s", SDL_GetError());
        return false;
    }
    //en esta parte hay que chequear si el formato del audio es compatible con el dispositivo
    if(necesitaConversionDeAudio(&audio))
    {
        printf("[DEBUG]Al parecer el sonido necesita conversion\n");
        //entonces hago la conversión
        SDL_AudioStream* stream = SDL_CreateAudioStream(&audio,&(dispositivoDeAudio.spec));
        if(!stream)
        {
            printf("[DEBUG] Error convirtiendo WAV, erro creando stream: %s", SDL_GetError());
            SDL_free(sound.buffer);
            return false;
        }
        if(!SDL_PutAudioStreamData(stream,sound.buffer, sound.longitud))
        {
            printf("[DEBUG] Error cargando WAV: error colocando stream: %s", SDL_GetError());
            SDL_DestroyAudioStream(stream);
            SDL_free(sound.buffer);
            return false;
        }
        //en este punto ya hice la conversión, ya cargue la data en sound.
        //hay que forzar el procesado con el cargado.
        //es decir le aviso que termine la carga de dato (bit de fin de transmision ponele)
        SDL_FlushAudioStream(stream);
        //ahora cuantos de ellos sse lograron leer correctamente
        int tamBytesListos = SDL_GetAudioStreamAvailable(stream);

        //Hay que guardar los datos convertidos: 1 elnuevo buffer y 2 la nueva longitud
        Uint8* nuevoBufferConvertido = malloc(tamBytesListos);
        if(!nuevoBufferConvertido)
        {
            printf("[DEBUG]: Error de memoria de malloc");
            SDL_DestroyAudioStream(stream);
            SDL_free(sound.buffer);
            return false;
        }
        //cantidad
        int bytesLeidos = SDL_GetAudioStreamData(stream,nuevoBufferConvertido,tamBytesListos);

        //ahora sí ya se puede guardar el sonido convertidoooo
        salida->buffer = nuevoBufferConvertido;
        salida->longitud = bytesLeidos;
        salida->sonidoSpec = dispositivoDeAudio.spec;
        SDL_DestroyAudioStream(stream);
        printf("[DEBUG]La conversion se logró\n");
        SDL_free(sound.buffer);
        return true;
    }
    salida->buffer = sound.buffer;
    salida->longitud = sound.longitud;
    salida->sonidoSpec = audio;

    printf("[DEBUG]Al parecer el sonido se cargo correctamente\n");
    return true;
}

bool necesitaConversionDeAudio(SDL_AudioSpec* audio)
{
    return (dispositivoDeAudio.spec.format != audio->format ||
       dispositivoDeAudio.spec.channels != audio->channels ||
       dispositivoDeAudio.spec.freq != audio->freq) ;
}


bool reproducirSFX(const tSonido* sonido)
{
    if(streamActivo)
    {
        SDL_UnbindAudioStream(streamActivo);
        SDL_DestroyAudioStream(streamActivo);
        streamActivo = NULL;
    }
    //un chequeo rapidito de que no esté vació
    if(!sonido || !sonido->buffer|| sonido->longitud <= 0)
    {
        //pues nada
        printf("[DEBUG]: Error sonido sin longitud o sonido sin buffer: %s", SDL_GetError());
        return false;
    }
    //no queda de otra opcion qeu destrozar el stream anterior y crear uno nuevo
    SDL_AudioStream* stream = SDL_CreateAudioStream(&sonido->sonidoSpec, &dispositivoDeAudio.spec);
    if (!stream) {
        printf("[DEBUG] Error creando stream de salida: %s\n", SDL_GetError());
        return false;
    }




    //aca es el mismo proceso, obtener los bytes, crear un espacio temporal, leerlos y luego encolarlos
    if (!SDL_PutAudioStreamData(stream, sonido->buffer, sonido->longitud)) 
    {
        printf("[DEBUG] Error SDL_PutAudioStreamData: %s\n", SDL_GetError());
        SDL_DestroyAudioStream(stream);
        return false;
    }
    printf("[DEBUG] Sonido cargado en el stream correctamente\n");
    SDL_FlushAudioStream(stream);
    // Dejamos que SDL consuma el stream automáticamente.
    // Esto funciona porque SDL3 "pulls" datos del stream.
    if(!SDL_BindAudioStream(dispositivoDeAudio.id, stream))
    {
        printf("[DEBUG] Error SDL_BindAudioStream: %s\n", SDL_GetError());
        SDL_DestroyAudioStream(stream);
        return false;
    }
    /*SDL_PauseAudioDevice(dispositivoDeAudio.id, false);
    guardarStreamActivo(stream);*/
   // SDL_PauseAudioDevice(dispositivoDeAudio.id);
    if (SDL_ResumeAudioDevice(dispositivoDeAudio.id) < 0) 
    {
         printf("[DEBUG] Error al resumir dispositivo: %s\n", SDL_GetError());
    }
   streamActivo = stream;
    printf("[DEBUG] Sonido reproducido\n");
    return true;    
}
bool liberarSonido(tSonido* sonido)
{
    if (!sonido) return false;

    if (sonido->buffer)
    {
        
        // libera buffer (sea WAV directo o convertido)
        SDL_free(sonido->buffer);
        sonido->buffer = NULL;
    }
    sonido->longitud = 0;
    return true;
}
void cerrarAudio()
{
    if (dispositivoDeAudio.id)
    {
        SDL_CloseAudioDevice(dispositivoDeAudio.id);
        dispositivoDeAudio.id = 0;
    }
}