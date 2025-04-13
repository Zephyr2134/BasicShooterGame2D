#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "player.hpp"
#include "UILabel.hpp"
#include "enemy.hpp"
#include "gun.hpp"
#include <vector>
#include "weaponPickUp.hpp"
#include "weaponPickUpManager.hpp"

class Game
{
public:

    Game();
    ~Game();

    void init(const char* title, int w, int h, bool fullscreen, const char* pFile);

    void handleEvents();

    void update();

    void render();

    void clean();

    bool isRunning() { return running; }


private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;

    SDL_FRect camera;


    SDL_Texture* playerTex;

    SDL_Event event;

    int flags;

    player* character;

    enemyManager* basicEnemy;

    weaponPickUpManager* weaponPickUps;

    bool rewarded = false;

    /*SDL_Color enemyColor = {255, 0, 0, 255};
    SDL_FRect Enemy;
    bool enemyHit = false;
    Uint32 enemyHitTime;
    int getUpTime = 500;
    int enemyHealth = 10;*/
};