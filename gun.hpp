#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "vector2d.hpp"
#include "UILabel.hpp"
#include "bullet.hpp"


class gun
{
public:
    gun(SDL_Renderer* ren, const char* rarity, const char* filePath, int mag, float nozzleOffsetFromX0, int timeToReload, int FireRate, int distanceFromPlayer, float width, float height, float srcW, float srcH);
    gun(const char* hasGun);
    ~gun();

    int fireRate(){return firerate;}
    void shoot(SDL_FRect camera);
    void handleMouseEvent(float pX, float pY, float mX, float mY, SDL_FRect camera);
    void reload();
    void update(SDL_FRect camera);
    std::vector<SDL_FRect> bullets(){ return bulletMngr->returnBullets();}
    void bulletHitSomething(int id);
    int getDamage(){return bulletMngr->damage();}
    void draw();
private:

    SDL_Renderer* renderer;

    SDL_FRect dstRect;
    SDL_FRect srcRect;

    SDL_Texture* gunTex;

    float playerX;
    float playerY;
    float mouseX;
    float mouseY;

    int distance;
    float nozzleOffset;

    UILabel* info;

    float rotation;

    SDL_FlipMode flip;

    //float xRot = 1.5f;
    //float yRot = 1.5f;

    float xRot;
    float yRot;


    int ammo;
    int magazine;
    int firerate;

    bool isReloading = false;
    Uint32 reloadStartTime;
    int reloadTime;

    bullet* bulletMngr;

    //bullets
    std::vector<SDL_FRect> bulRects;
    std::vector<vector2d> velocities;
};