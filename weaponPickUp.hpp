#pragma once
#include <SDL3/SDL.h>
#include "vector2d.hpp"
#include <SDL3_image/SDL_image.h>
#include <cmath>
class weaponPickUp
{

public:
    weaponPickUp(SDL_Renderer* ren, float xPos, float yPos, const char* weaponImg, float ImgWidth, float ImgHeight, SDL_FRect camera);
    ~weaponPickUp();

    void update(SDL_FRect camera);

    bool pickUp(vector2d* playerPos);

    float x(){return dstRect.x + dstRect.w/2;}
    float y(){return dstRect.y + dstRect.h/2;}

    void draw(SDL_Renderer* ren);
private:
    vector2d* position;
    SDL_FRect dstRect;
    SDL_FRect srcRect;

    SDL_Texture* texture;
};