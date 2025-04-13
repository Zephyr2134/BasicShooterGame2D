#include "soundManager.hpp"

soundManager::soundManager()
{}
soundManager::~soundManager()
{}

void soundManager::makeSound(const char* filePath)
{
    sounds.push_back(new Sound(filePath));
}

void soundManager::update()
{
    for(int i = 0; i < sounds.size(); i++)
    {
        if(sounds[i]->playSound())
        {
            sounds.erase(sounds.begin() + i);
        }
    }
}