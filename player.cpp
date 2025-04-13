#include "player.hpp"
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <cmath>

player::player(const char* filepath, float spd, float xPos, float yPos, float w, float h, SDL_Renderer* ren) : renderer(ren)
{
    SDL_Surface* tmpSurf = IMG_Load(filepath);
    playerTex = SDL_CreateTextureFromSurface(ren, tmpSurf);
    SDL_DestroySurface(tmpSurf);

    dstRect = {xPos, yPos, w, h};

    baseSpeed = spd;
    angledSpeed = baseSpeed/1.414f;

    mouseX = mouseY = 0;

    
    weapon = new gun("no");
    worldPos = new vector2d(xPos, yPos);
    managerOfSounds = new soundManager();
}

player::~player()
{}

void player::handleInputs(SDL_Event event, SDL_FRect camera, weaponPickUpManager* weaponPickUps)
{
    
    if(event.type == SDL_EVENT_KEY_DOWN)
    {
        int pickUpId = 0;

        switch(event.key.key)
        {
            case SDLK_W:
                directions[0] = true;
                break;
            case SDLK_S:
                directions[1] = true;
                break;
            case SDLK_D:
                directions[2] = true;
                break;
            case SDLK_A:
                directions[3] = true;
                break;
            case SDLK_SPACE:
                autoFire = true;
                break;
            case SDLK_E:
            for(weaponPickUp* w : weaponPickUps->pickUps())
            {
                if(w->pickUp(worldPos))
                {
                    weaponPickUps->removeWeaponPickUp(pickUpId);
                    weapon = new gun(renderer, "Common", "gun.png", 16, 1.5f, 3000, 200,  100, 100, 100, 8, 8);
                    break;
                }
                pickUpId++;
            }
                break;
            case SDLK_Q:
                weapon = new gun(renderer,"Legendary", "top.png", 10000, 0.5f, 1000, 50,  100, 100, 100, 8, 8);
                break;
            default:
                break;
        }
    }
    if(event.type == SDL_EVENT_KEY_UP)
    {
        switch(event.key.key)
        {
            case SDLK_W:
                directions[0] = false;
                break;
            case SDLK_S:
                directions[1] = false;
                break;
            case SDLK_D:
                directions[2] = false;
                break;
            case SDLK_A:
                directions[3] = false;
                break;
            case SDLK_SPACE:
                autoFire = false;
                break;
            
            default:
                break;
        }
    }
    if(event.type == SDL_EVENT_MOUSE_MOTION)
    {
        mouseX = event.motion.x;
        mouseY = event.motion.y;
    }
    if(event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        if(event.button.button == 1)
        {
        autoFire = true;
        }
    }
    else if(event.type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        if(event.button.button == 1)
        {
            autoFire = false;
        }
    }

    weapon->handleMouseEvent((worldPos->x) + dstRect.w/2, (worldPos->y) + dstRect.h/2, mouseX, mouseY, camera);
    spinRot = SDL_atan2(((worldPos->y - camera.y) + dstRect.h/2) - mouseY, ((worldPos->x - camera.x) + dstRect.w/2) - mouseX) * (180 / M_PI);

    if(spinRot > -90 && spinRot < 90)
    {
        flip = SDL_FLIP_HORIZONTAL;
    }
    else
    {
        flip = SDL_FLIP_NONE;
    }
}

void player::update(SDL_FRect camera)
{
    if(directions[0] && directions[2] || directions[2] && directions[1] || directions[1] && directions[3] || directions[3] && directions[0])
    {
        speed = angledSpeed;
    }
    else
    {
        speed = baseSpeed;
    }
    if(directions[0] != directions[1])
    {
        if(directions[0])
        {
            worldPos->y -= speed;
        }
        else
        {
            worldPos->y += speed;
        }
    }
    
    if(directions[2] != directions[3])
    {
        if(directions[2])
        {
            worldPos->x += speed;
        }
        else
        {
            worldPos->x -= speed;
        }
    }

    weapon->update(camera);

    if(autoFire)
    {
        if(SDL_GetTicks() - lastFired >= weapon->fireRate())
        {
        managerOfSounds->makeSound("Assets/powerUp.wav");
        weapon->shoot(camera);
        lastFired = SDL_GetTicks();
        }
    }

    managerOfSounds->update();
    dstRect.x = worldPos->x - camera.x;
    dstRect.y = worldPos->y - camera.y;
}

void player::bulletHit(int id)
{
    weapon->bulletHitSomething(id);
}

void player::render()
{   
    SDL_RenderTextureRotated(renderer, playerTex, NULL, &dstRect, 0, NULL, flip);
    SDL_SetTextureScaleMode(playerTex, SDL_SCALEMODE_NEAREST);
    weapon->draw();
}