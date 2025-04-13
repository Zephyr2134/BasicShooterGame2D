#include "gun.hpp"
#include <SDL3_image/SDL_image.h>
#include <cmath>
#include <iostream>
#include <string>
#include <cstdlib>

gun::gun(SDL_Renderer* ren, const char* rarity, const char* filePath, int mag, float nozzleOffsetFromX0, int timeToReload, int FireRate, int distanceFromPlayer, float width, float height, float srcW, float srcH) : renderer(ren)
{
    SDL_Surface* tmpSurf = IMG_Load(filePath);
    gunTex = SDL_CreateTextureFromSurface(renderer, tmpSurf);
    SDL_DestroySurface(tmpSurf);

    dstRect = {0,0,width,height};
    srcRect = {0,0,srcW,srcH};

    distance = distanceFromPlayer;
    nozzleOffset = nozzleOffsetFromX0;

    int magazineM;

    //srand(SDL_GetTicks());
    if(rarity == "Common"){
        magazineM = 101;
    }
    else if(rarity == "Legendary"){
        magazineM = 11;
    }

    magazine = rand() % magazineM;
    ammo = magazine;
    reloadTime = rand();
    firerate = FireRate;


    const char* text = (std::to_string(ammo) + "/" + std::to_string(magazine)).c_str();

    bulletMngr = new bullet(renderer, "bullet.png", 2, 8, 8, 10, 3);

    info = new UILabel(ren, text, 24, {0.0f, 0.0f, 120.0f, 100.0f});
}

gun::gun(const char* hasGun)
{
    SDL_Surface* tmpSurf = IMG_Load("top.png");
    gunTex = SDL_CreateTextureFromSurface(renderer, tmpSurf);
    SDL_DestroySurface(tmpSurf);

    dstRect = {0,0,0,0};
    srcRect = {0,0,0,0};

    distance = 0;
    nozzleOffset = 0;

    magazine = 0;
    ammo = 0;

    bulletMngr = new bullet(renderer, "grounded.png", 0, 0, 0, 0, 0);

    info = new UILabel(renderer, "Ammo", 0, {0.0f, 0.0f, 120.0f, 100.0f});
}

gun::~gun()
{}

void gun::shoot(SDL_FRect camera)
{
    if(!isReloading && ammo)
    {
        ammo -= 1;

        info->updateText((std::to_string(ammo) + "/" + std::to_string(magazine)).c_str());

        float xDif = (playerX - camera.x) - mouseX;
        float yDif = (playerY - camera.y) - mouseY;
        float hyp = sqrt(pow(xDif, 2) + pow(yDif, 2));

        float xVel = -(xDif/hyp);
        float yVel = -(yDif/hyp);
        
        float xPos = -(xVel * -1 * (distance + dstRect.w - 35)) + playerX;
        float yPos = -(yVel * -1 * (distance + dstRect.w - 35)) + playerY;

        bulletMngr->createBullet(xPos, yPos, xVel, yVel, rotation, camera);
    }
}

void gun::handleMouseEvent(float pX, float pY, float mX, float mY, SDL_FRect camera)
{   
    playerX = pX;
    playerY = pY;
    mouseX = mX;
    mouseY = mY;
    if(!isReloading)
    {
        rotation = SDL_atan2((playerY - camera.y) - mouseY, (playerX  - camera.x) - mouseX) * (180 / M_PI) - 180;
    }
    else
    {
        rotation += 20;
    }
}

void gun::reload()
{
    if(!isReloading && !ammo)
    {
        isReloading = true;
        reloadStartTime = SDL_GetTicks();
    }

    if(isReloading && SDL_GetTicks() - reloadStartTime >= reloadTime)
    {
        ammo = magazine;
        isReloading = false;

        info->updateText((std::to_string(ammo) + "/" + std::to_string(magazine)).c_str());
    }
}

void gun::update(SDL_FRect camera)
{   
    float xDif = (playerX - camera.x) - mouseX;
    float yDif = (playerY - camera.y) - mouseY;
    float hyp = sqrt(pow(xDif, 2) + pow(yDif, 2));
    

    if(rotation <= -270 || rotation >= -90)
    {
        flip = SDL_FLIP_NONE;
        if(rotation <= -270 && rotation >= -360)
        {   
            xRot = ((1+((rotation + 270)/90))*(srcRect.w-nozzleOffset*2))+nozzleOffset;
        }
        else
        {
            xRot = nozzleOffset;
        }
    }
    else
    {
        flip = SDL_FLIP_VERTICAL;
        
        if(rotation > -270 && rotation <= -180)
        {   
            xRot = ((1+((rotation + 180)/90))*(srcRect.w-nozzleOffset*2))+nozzleOffset;
        }
        else
        {
            xRot = srcRect.w - nozzleOffset;
        }
    }

    if(rotation >= -180 && rotation <= -90)
    {
        //1.5f to 4
        yRot = ((srcRect.h - ((srcRect.h/2)-nozzleOffset))-((((rotation + 90) / 90)*-((srcRect.h/2)-nozzleOffset))+nozzleOffset));
    }
    else if(rotation > -90 && rotation <= 0)
    {
        //4 to 1.5f
        yRot = (((rotation / 90)*((srcRect.h/2)-nozzleOffset))-nozzleOffset)*-1;
    }
    else
    {
        yRot = nozzleOffset;
    }

    //std::cout << xRot << "," << yRot << "," << rotation << std::endl;


    for(int i = 0; i < velocities.size(); i++)
    {
        bulRects[i].x += velocities[i].x;
        bulRects[i].y += velocities[i].y;
    }
    dstRect.x = -((xDif/hyp)*distance) + ((playerX - camera.x) - dstRect.w/srcRect.w*xRot);
    dstRect.y = -((yDif/hyp)*distance) + ((playerY - camera.y) - dstRect.h/srcRect.h*yRot);

    this->reload();

    bulletMngr->update(camera);
}

void gun::bulletHitSomething(int id)
{
    bulletMngr->destroyBullet(id);
}

void gun::draw()
{   
    SDL_RenderTextureRotated(renderer, gunTex, &srcRect, &dstRect, rotation,NULL, flip);
    SDL_SetTextureScaleMode(gunTex, SDL_SCALEMODE_NEAREST);
    info->draw();

    bulletMngr->draw();
    for(auto& b : bulRects)
    {
        SDL_RenderFillRect(renderer, &b);
    }
}