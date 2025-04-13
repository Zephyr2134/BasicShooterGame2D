#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include <vector>

typedef struct Sound{
    SDL_AudioStream* stream;
    Uint8* audioBuf;
    Uint32 audioLen;
    SDL_AudioSpec spec;

    Sound(const char* filePath) : stream(nullptr), audioBuf(nullptr), audioLen(0)
    {
        if(SDL_LoadWAV(filePath, &spec, &audioBuf, &audioLen))
        {
            stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
            if(!stream)
            {
            printf("Failed to open audio stream %s\n", SDL_GetError());
            SDL_free(audioBuf);
            audioBuf = nullptr;
            return;
            }
        }else{
            printf("Failed to load WAV file: %s\n", SDL_GetError());
        }
    }
    ~Sound()
    {
        if(stream){
        SDL_DestroyAudioStream(stream);
    }
    if(audioBuf){
        SDL_free(audioBuf);
    }
    }

    bool playSound()
    {
        if(!stream || !audioBuf)
        {
            printf("Error: Attempted to play sound on an invalid stream of buffer!\n");
            return true;
        }
        if(SDL_PutAudioStreamData(stream, audioBuf, audioLen) < 0)
        {
            printf("Failes to queue audio data: %s\n", SDL_GetError());
            return false;
        }

        SDL_ResumeAudioStreamDevice(stream);
        return true;
    }
}Sound;

class soundManager{
public:
    soundManager();
    ~soundManager();

    void makeSound(const char* filePath);

    void update();
private:
    std::vector<Sound*> sounds;
};