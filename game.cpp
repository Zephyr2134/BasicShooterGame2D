#include "game.hpp"
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cmath>
#include <iostream>
#include <cstdlib>


Game::Game()
{}

Game::~Game()
{}

void Game::init(const char* title, int w, int h, bool fullscreen, const char* pFile)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Init(SDL_INIT_AUDIO);
    SDL_Init(SDL_INIT_EVENTS);
    TTF_Init();

    flags = 0;
    if(fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    window = SDL_CreateWindow(title, w, h, flags);
    SDL_SetWindowIcon(window, IMG_Load("lilguy.png"));

    renderer = SDL_CreateRenderer(window, NULL);

    running = true;

    character = new player("lilguy.png", 5.0f, 300.0f, 100.0f, 100.0f, 100.0f, renderer);

    camera.w = 800;
    camera.h = 800;
    int newWidth;
    int newHeight;
    SDL_GetWindowSize(window, &newWidth, &newHeight);

    camera.w = newWidth;
    camera.h = newHeight;
    camera.x = character->x()-(camera.w/2);
    camera.y = character->y()-(camera.h/2);

    basicEnemy = new enemyManager(renderer, "enemy.png", 8, 8, 10,10, camera);
    
    for(int i = 0; i < 100; i++){
    basicEnemy->createEnemy(rand()%11, rand()%11, i);
}

weaponPickUps = new weaponPickUpManager();

}

void Game::handleEvents()
{
    SDL_PollEvent(&event);
    if(event.type == SDL_EVENT_QUIT)
    {
        running = false;
    }
    character->handleInputs(event, camera, weaponPickUps);

    basicEnemy->checkPlayerPos(character->x(), character->y());

    camera.x = character->x()-(camera.w/2);
    camera.y = character->y()-(camera.h/2);
}

void Game::update()
{
    
    character->update(camera);

    basicEnemy->update(camera);

    weaponPickUps->updatePickUps(camera);

    if(basicEnemy->waveDone() && weaponPickUps->noPickUps() && !rewarded)
    {   
        rewarded = true;
        weaponPickUps->addWeaponPickUp(renderer, 400.0f, 500.0f, "top.png", 8, 8, camera);
    }
    else if(basicEnemy->waveDone() && weaponPickUps->noPickUps() && rewarded)
    {
        for(int i = SDL_GetTicks(); i < SDL_GetTicks() + 100; i++){
            srand(i);
            basicEnemy->createEnemy(rand()%11, rand()%11, i);
        }
        rewarded = false;
    }
    
    int id = 0;

    for(SDL_FRect rec : character->getBullets())
    {
        if(basicEnemy->collision(rec, character->damage()))
        {
            character->bulletHit(id);
        }
        id++;
    }
}

void Game::render()
{   
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderClear(renderer);
    character->render();
    basicEnemy->draw(camera);

    weaponPickUps->drawPickUps(renderer, character->dstX(), character->dstY());

    SDL_RenderPresent(renderer);
}

void Game::clean()
{}
