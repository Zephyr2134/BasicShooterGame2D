#pragma once
#include "weaponPickUp.hpp"
#include <vector>
#include <SDL3/SDL.h>

class weaponPickUpManager
{
public:

    weaponPickUpManager();
    ~weaponPickUpManager();

    void addWeaponPickUp(SDL_Renderer* ren, float xPos, float yPos, const char* filepath, int srcFileWidth, int srcFileHeight, SDL_FRect camera);
    void removeWeaponPickUp(int id);

    bool noPickUps(){return (weaponPickUps.size() == 0);}

    void updatePickUps(SDL_FRect camera);
    void drawPickUps(SDL_Renderer* ren, float playerX, float playerY);

    std::vector<weaponPickUp*> pickUps(){return weaponPickUps;}

private:

    std::vector<weaponPickUp*> weaponPickUps;

};