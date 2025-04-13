#include "bullet.hpp"
#include <iostream>

bullet::bullet(SDL_Renderer* ren, const char* filePath, int sise, float srcW, float srcH, int sped, int damag) : renderer(ren), halfHeight(srcH/2), halfWidth(srcW/2), size(sise), speed(sped)
{
    SDL_Surface* tmp = IMG_Load(filePath);
    bulletTex = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_DestroySurface(tmp);

    srcRect.x = srcRect.y = 0;
    srcRect.w = srcW;
    srcRect.h = srcH;

    baseDst.x = baseDst.y = 0;
    baseDst.w = srcW * sise;
    baseDst.h = srcH * sise;

    dmg = damag;
}

bullet::~bullet()
{}

void bullet::createBullet(float xPos, float yPos, float xVel, float yVel, float rotation, SDL_FRect camera)
{   
    positions.push_back(new vector2d(xPos  - (size*halfWidth), yPos - (size*halfHeight)));
    dstRects.push_back({(xPos  - (size*halfWidth) - camera.x), (yPos - (size*halfHeight) - camera.y), baseDst.w, baseDst.h});
    velocities.push_back({xVel * speed, yVel * speed});
    rotations.push_back(rotation);
}

void bullet::destroyBullet(int id)
{
    positions.erase(positions.begin() + id);
    dstRects.erase(dstRects.begin() + id);
    velocities.erase(velocities.begin() + id);
    rotations.erase(rotations.begin() + id);
}

void bullet::clearBullets(int i, SDL_FRect camera)
{
    if(dstRects[i].x + dstRects[i].w < 0 ||
       dstRects[i].x - dstRects[i].w > camera.w || 
       dstRects[i].y - dstRects[i].h > camera.h || 
       dstRects[i].y + dstRects[i].h < 0)
    {   
        destroyBullet(i);
    }
}

void bullet::collision(std::vector<SDL_FRect> rects)
{
    for(SDL_FRect bul : dstRects)
    {
        for(SDL_FRect rec : rects)
        {
            if(bul.x > rec.x && bul.x < rec.x + rec.w)
            {
                std::cout << "Hit" << std::endl;
            }
        }
    }
}

void bullet::update(SDL_FRect camera)
{
    for(int i = 0; i < dstRects.size(); i++)
    {
        positions[i]->x += velocities[i].x;
        positions[i]->y += velocities[i].y;

        dstRects[i].x = positions[i]->x - camera.x;
        dstRects[i].y = positions[i]->y - camera.y;
        clearBullets(i, camera);
    }
}

void bullet::draw()
{
    int rot = 0;
    for(auto& b : dstRects)
    {
        SDL_RenderTextureRotated(renderer, bulletTex, &srcRect, &b, rotations[rot], NULL, SDL_FLIP_NONE);
        rot++;
    }
    SDL_SetTextureScaleMode(bulletTex, SDL_SCALEMODE_NEAREST);
}