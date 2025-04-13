#pragma once
#include <SDL3/SDL.h>
#include "gun.hpp"
#include "vector2d.hpp"
#include "weaponPickUp.hpp"
#include <vector>
#include "weaponPickUpManager.hpp"
#include "soundManager.hpp"

class player
{
public:
    player(const char* filepath, float spd, float xPos, float yPos, float w, float h, SDL_Renderer* ren);
    ~player();

    void handleInputs(SDL_Event event, SDL_FRect camera, weaponPickUpManager* weaponPickUps);
    void update(SDL_FRect camera);
    void render();
    void clean();

    std::vector<SDL_FRect> getBullets(){return weapon->bullets();}
    void bulletHit(int id);
    bool hasGun(){return weapon;}
    int damage(){return weapon->getDamage();}

    float angle(){return spinRot;}

    float x(){return worldPos->x + dstRect.w/2;}
    float y(){return worldPos->y + dstRect.h/2;}
    float dstX(){return dstRect.x + dstRect.w/2;}
    float dstY(){return dstRect.y + dstRect.h/2;}
private:
    SDL_Texture* playerTex;
    SDL_FRect dstRect;
    vector2d* worldPos;
    SDL_FlipMode flip = SDL_FLIP_NONE;
    SDL_Renderer* renderer;

    //WSDA, Im quirky like that
    bool directions[4] = {false, false, false, false};

    float speed;
    float angledSpeed;
    float baseSpeed;
    float mouseX, mouseY;

    float spinRot = 0;

    gun* weapon;
    bool autoFire = false;
    Uint32 lastFired = 0;

    soundManager* managerOfSounds;
};