#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "game.hpp"

Game* game = nullptr;

int main()
{

    const int FPS = 60;
    const int frameDelay = 1000/FPS;

    Uint32 frameStart;
    int frameTime;

    game = new Game();

    game->init("SDL is fun", 800, 800, false, "lilguy.png");

    while(game->isRunning())
    {

        frameStart = SDL_GetTicks();

        game->handleEvents();
        game->update();
        game->render();

        frameTime = SDL_GetTicks() - frameStart;

        if(frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    game->clean();

    return 0;
}