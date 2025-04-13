#include "weaponPickUp.hpp"

weaponPickUp::weaponPickUp(SDL_Renderer* ren, float xPos, float yPos, const char* weaponImg, float ImgWidth, float ImgHeight, SDL_FRect camera)
{
    SDL_Surface* tmpSurf = IMG_Load(weaponImg);
    texture = SDL_CreateTextureFromSurface(ren, tmpSurf);
    SDL_DestroySurface(tmpSurf);

    position = new vector2d(xPos, yPos);
    srcRect = {0,0, ImgWidth, ImgHeight};
    dstRect = {position->x - camera.x, position->y - camera.y, 100, 100};
}

weaponPickUp::~weaponPickUp()
{}

void weaponPickUp::update(SDL_FRect camera)
{
    dstRect.x = position->x - camera.x;
    dstRect.y = position->y - camera.y;
}

bool weaponPickUp::pickUp(vector2d* playerPos)
{
    float difX = playerPos->x - position->x;
    float difY = playerPos->y - position->y;
    float hyp = sqrt(pow(difX, 2) + pow(difY, 2));

    if(hyp < 100)
    {
        return true;
    }
    return false;
}

void weaponPickUp::draw(SDL_Renderer* ren)
{
    SDL_RenderTexture(ren, texture, &srcRect, &dstRect);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
}