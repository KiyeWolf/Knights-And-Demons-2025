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
        SDL_Log("[DEBUG] Error al inicializar el dispositivo de audio: %s", SDL_GetError());
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
        SDL_Log("[DEBUG] Error cargando WAV: %s", SDL_GetError());
        return false;
    }
    //en esta parte hay que chequear si el formato del audio es compatible con el dispositivo
    //entonces hago la conversión
        SDL_AudioStream* stream = SDL_CreateAudioStream(&audio,&(dispositivoDeAudio.spec));
        if(!stream)
        {
            SDL_Log("[DEBUG] Error convirtiendo WAV, erro creando stream: %s", SDL_GetError());
            SDL_free(sound.buffer);
            return false;
        }
        if(!SDL_PutAudioStreamData(stream,sound.buffer, sound.longitud))
        {
            SDL_Log("[DEBUG] Error cargando WAV: error colocando stream: %s", SDL_GetError());
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
        Uint8* nuevoBufferConvertido = SDL_malloc(tamBytesListos);
        if(!nuevoBufferConvertido)
        {
            SDL_Log("[DEBUG]: Error de memoria de malloc");
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
        //SDL_Log("[DEBUG]La conversion se logró\n");
        SDL_free(sound.buffer);
        return true;
}
bool cargarUnaBGMNueva(const char* rutaArch, tSonido* salida)
{
    SDL_AudioSpec audio = {0};
    tSoundWav sound = {0};

    //LoadWav se encarga de buscar el archivo en la ruta, cargarlo a sound y audio que son auxiliares
    //antes de cargarlo a la salida
    if(!SDL_LoadWAV(rutaArch,&audio,&(sound.buffer), &(sound.longitud)))
    {
        SDL_Log("[DEBUG] Error cargando WAV: %s", SDL_GetError());
        return false;
    }
    //en esta parte hay que chequear si el formato del audio es compatible con el dispositivo
    if(necesitaConversionDeAudio(&audio))
    {
        //SDL_Log("[DEBUG]Al parecer el sonido necesita conversion\n");
        //entonces hago la conversión
        SDL_AudioStream* stream = SDL_CreateAudioStream(&audio,&(dispositivoDeAudio.spec));
        if(!stream)
        {
            SDL_Log("[DEBUG] Error convirtiendo WAV, erro creando stream: %s", SDL_GetError());
            SDL_free(sound.buffer);
            return false;
        }
        if(!SDL_PutAudioStreamData(stream,sound.buffer, sound.longitud))
        {
            SDL_Log("[DEBUG] Error cargando WAV: error colocando stream: %s", SDL_GetError());
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
        Uint8* nuevoBufferConvertido = SDL_malloc(tamBytesListos);
        if(!nuevoBufferConvertido)
        {
            SDL_Log("[DEBUG]: Error de memoria de malloc");
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
        SDL_Log("[DEBUG]La conversion se logró\n");
        //SDL_free(sound.buffer);
        /*SDL_Log("[DEBUG] Spec BGM -> freq:%d  channels:%d  format:%d\n",
        salida->sonidoSpec.freq,
        salida->sonidoSpec.channels,
         salida->sonidoSpec.format);
        */
       SDL_free(sound.buffer);

        return true;
    }
    //ahora convierto siempre
    SDL_AudioStream* stream = SDL_CreateAudioStream(&audio, &dispositivoDeAudio.spec);
    if (!stream)
    {
        SDL_Log("[DEBUG] Error creando stream WAV->Device: %s\n", SDL_GetError());
        SDL_free(sound.buffer);
        return false;
    }

    SDL_PutAudioStreamData(stream, sound.buffer, sound.longitud);
    SDL_FlushAudioStream(stream);

    // Obtener bytes convertidos
    int tam = SDL_GetAudioStreamAvailable(stream);
    Uint8* nuevoBuffer = SDL_malloc(tam);
    SDL_GetAudioStreamData(stream, nuevoBuffer, tam);

    // Guardar salida
    salida->buffer = nuevoBuffer;
    salida->longitud = tam;
    salida->sonidoSpec = dispositivoDeAudio.spec;
    SDL_free(sound.buffer);
    SDL_DestroyAudioStream(stream);
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
        SDL_Log("[DEBUG]: Error sonido sin longitud o sonido sin buffer: %s", SDL_GetError());
        return false;
    }
    //SDL_ClearAudioStream(streamActivo);
    //no queda de otra opcion qeu destrozar el stream anterior y crear uno nuevo
    SDL_AudioStream* stream = SDL_CreateAudioStream(&sonido->sonidoSpec, &dispositivoDeAudio.spec);
    if (!stream) {
        SDL_Log("[DEBUG] Error creando stream de salida: %s\n", SDL_GetError());
        return false;
    }




    //aca es el mismo proceso, obtener los bytes, crear un espacio temporal, leerlos y luego encolarlos
    if (!SDL_PutAudioStreamData(stream, sonido->buffer, sonido->longitud))
    {
        SDL_Log("[DEBUG] Error SDL_PutAudioStreamData: %s\n", SDL_GetError());
        SDL_DestroyAudioStream(stream);
        return false;
    }
    SDL_Log("[DEBUG] Sonido cargado en el stream correctamente\n");
    SDL_FlushAudioStream(stream);
    // Dejamos que SDL consuma el stream automáticamente.
    // Esto funciona porque SDL3 "pulls" datos del stream.
    if(!SDL_BindAudioStream(dispositivoDeAudio.id, stream))
    {
        SDL_Log("[DEBUG] Error SDL_BindAudioStream: %s\n", SDL_GetError());
        SDL_DestroyAudioStream(stream);
        return false;
    }
    /*SDL_PauseAudioDevice(dispositivoDeAudio.id, false);
    guardarStreamActivo(stream);*/
   // SDL_PauseAudioDevice(dispositivoDeAudio.id);
    if (!SDL_ResumeAudioDevice(dispositivoDeAudio.id))
    {
         SDL_Log("[DEBUG] Error al resumir dispositivo: %s\n", SDL_GetError());
    }
   streamActivo = stream;
    //SDL_Log("[DEBUG] Sonido reproducido\n");
    return true;
}
bool liberarSonido(tSonido* sonido)
{
    if (!sonido) return false;

    if (sonido->buffer)
    {
        //SDL_Log("yes llegue hasta liberarSonido");
        // libera buffer (sea WAV directo o convertido)

       // SDL_Log("[DEBUG liberarSonido] buffer=%p, longitud=%d\n",
           //sonido->buffer, sonido->longitud);
        SDL_free(sonido->buffer);
        sonido->buffer = NULL;
    }
    sonido->longitud = 0;
    return true;
}
bool liberarBGM(tSonido* sonido)
{
    if (!sonido) return false;

     //SDL_Log("[DEBUG liberarSonido] buffer=%p, longitud=%d\n",
           //sonido->buffer, sonido->longitud);

    if (sonido->buffer)
    {
        //SDL_Log("yes llegue hasta liberarBGM");
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
bool crearBackgroundMusic(const char* ruta, tSonido* s)
{
    if (!cargarUnaBGMNueva(ruta,
                            s))
    {
        return false;
    }
    return true;
}
bool reproducirBGM(const tSonido* musica)
{
     if (!musica || !musica->buffer || musica->longitud <= 0)
    {
        SDL_Log("[DEBUG] BGM inválida: %s\n", SDL_GetError());
        return false;
    }

    // Si ya había una BGM sonando la saco, y borro
    if (streamBGM)
    {
        SDL_UnbindAudioStream(streamBGM);
        SDL_DestroyAudioStream(streamBGM);
        streamBGM = NULL;
    }
    /*SDL_Log("[DEBUG] Spec BGM -> freq:%d  channels:%d  format:%d\n",
       musica->sonidoSpec.freq,
       musica->sonidoSpec.channels,
       musica->sonidoSpec.format);
    SDL_Log("[DEBUG] Spec DEVICE -> freq:%d  channels:%d  format:%d\n",
       dispositivoDeAudio.spec.freq,
       dispositivoDeAudio.spec.channels,
       dispositivoDeAudio.spec.format);
    */
    // Crear un nuevo stream para la BGM usando el formato ya convertido del sonido
    streamBGM = SDL_CreateAudioStream(&(musica->sonidoSpec), &dispositivoDeAudio.spec);
    if (!streamBGM)
    {
        SDL_Log("[DEBUG] Error creando stream BGM   : %s\n", SDL_GetError());
        return false;
    }

    // Cargar el audio completo al stream
    if (!SDL_PutAudioStreamData(streamBGM, musica->buffer, musica->longitud))
    {
        SDL_Log("[DEBUG] Error cargando datos BGM: %s\n", SDL_GetError());
        SDL_DestroyAudioStream(streamBGM);
        streamBGM = NULL;
        return false;
    }

    SDL_FlushAudioStream(streamBGM);

    // Vincular stream BGM al dispositivo de salida
    if (!SDL_BindAudioStream(dispositivoDeAudio.id, streamBGM))
    {
        SDL_Log("[DEBUG] Error al bindear BGM: %s\n", SDL_GetError());
        SDL_DestroyAudioStream(streamBGM);
        streamBGM = NULL;
        return false;
    }

    // Asegurar que el audio esté reproduciéndose
    SDL_ResumeAudioDevice(dispositivoDeAudio.id);
    SDL_Log("[DEBUG] BGM reproduciéndose correctamente.\n");
    return true;
}
void detenerMusicaBGM()
{
    if (streamBGM)
    {
        SDL_UnbindAudioStream(streamBGM);
        SDL_DestroyAudioStream(streamBGM);
        streamBGM = NULL;
        SDL_Log("[DEBUG] BGM detenida y stream destruido correctamente.\n");
    }
    else
    {
        SDL_Log("[DEBUG] Intente detener BGM pero el puntero era NULL (¿Ya estaba detenida?)\n");
    }
}
bool terminoLaMusica()
{
    return (streamBGM && SDL_GetAudioStreamAvailable(streamBGM) == 0);
}
void recargarLaBGM(tSonido* bgm)
{
    // Recargar la música para el siguiente loop
        SDL_PutAudioStreamData(streamBGM, bgm->buffer, bgm->longitud);
        SDL_FlushAudioStream(streamBGM);
}
bool bajarElVolumenDeLaMusica(float factorDecremento)
{
    return SDL_SetAudioStreamGain(streamBGM,factorDecremento);
}
bool restaurarVolumenMusica()
{
    return SDL_SetAudioStreamGain(streamBGM,1.0f);
}
void pausarMusicaBGM()
{
    SDL_PauseAudioDevice(dispositivoDeAudio.id);
}
