#include "weaponPickUpManager.hpp"

weaponPickUpManager::weaponPickUpManager()
{}

weaponPickUpManager::~weaponPickUpManager()
{}

void weaponPickUpManager::addWeaponPickUp(SDL_Renderer* ren, float xPos, float yPos, const char* filepath, int srcFileWidth, int srcFileHeight, SDL_FRect camera)
{
    weaponPickUps.push_back(new weaponPickUp(ren, xPos, yPos, filepath, srcFileWidth, srcFileHeight, camera));
}

void weaponPickUpManager::removeWeaponPickUp(int id)
{
    weaponPickUps.erase(weaponPickUps.begin() + id);
}

void weaponPickUpManager::updatePickUps(SDL_FRect camera)
{
    for(weaponPickUp* w : weaponPickUps)
    {
        w->update(camera);
    }
}

void weaponPickUpManager::drawPickUps(SDL_Renderer* ren, float playerX, float playerY)
{
    for(weaponPickUp* w : weaponPickUps)
    {
        w->draw(ren);
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
        SDL_RenderLine(ren, w->x(), w->y(), playerX, playerY);
    }
}