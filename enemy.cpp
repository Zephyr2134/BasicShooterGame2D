#include "enemy.hpp"
#include <iostream>
#include <cmath>

enemyManager::enemyManager(SDL_Renderer* ren, const char* textureFilePath, int w, int h, int sc, int hp, SDL_FRect camera) 
: renderer(ren), width(w), height(h), scale(sc), health(hp)
{
    SDL_Surface* tmpSurf = IMG_Load(textureFilePath);
    enemyTexture = SDL_CreateTextureFromSurface(renderer, tmpSurf);
    SDL_DestroySurface(tmpSurf);

    enemyCount = new UILabel(renderer, "Placeholder", 24, {(camera.w / 2) - 50, 100, 100, 50});
    managerOfSounds = new soundManager();
}

enemyManager::~enemyManager()
{}

void enemyManager::createEnemy(int xpos, int ypos, int id)
{
    enemy* Enemy = new enemy(xpos, ypos, width, height, scale, health, id);
    enemies.push_back(Enemy);
}

bool enemyManager::collision(SDL_FRect recA, int damage)
{
    for(int i = 0; i < enemies.size(); i++)
    {
        if(recA.x > enemies[i]->position.x && 
            recA.x < enemies[i]->position.x + enemies[i]->position.w && 
            recA.y > enemies[i]->position.y && 
            recA.y < enemies[i]->position.y + enemies[i]->position.h)
        {
            enemies[i]->health -= damage;
            enemies[i]->healthBar.w = 100.0f * ((enemies[i]->health + 0.0f)/health);

            enemies[i]->velocity->x = (enemies[i]->xDif / enemies[i]->hyp) * -70;
            enemies[i]->velocity->y = (enemies[i]->yDif / enemies[i]->hyp) * -70;

            if(enemies[i]->health <= 0)
            {
                enemies.erase(enemies.begin() + i);
                managerOfSounds->makeSound("Assets/explosion.wav");
            }
            return true;
        }
    }
    return false;
}

void enemyManager::checkPlayerPos(float playerX, float playerY)
{
    for(enemy* e : enemies)
    {   
        e->xDif = playerX - e->worldPos->x;
        e->yDif = playerY - e->worldPos->y;
        e->hyp = sqrt(pow(e->xDif, 2) + pow(e->yDif, 2));
    }
}

void enemyManager::update(SDL_FRect camera)
{
    for(enemy* e : enemies)
    {
        if(e->hyp > e->maxDistance)
        {
            e->velocity->x = (e->xDif / e->hyp) * (e->hyp - e->maxDistance)/10;
            e->velocity->y = (e->yDif / e->hyp) * (e->hyp - e->maxDistance)/10;
        }
        else if(e->hyp < e->minDistance)
        {
            e->velocity->x = (e->xDif / e->hyp) * (e->hyp - e->minDistance);
            e->velocity->y = (e->yDif / e->hyp) * (e->hyp - e->minDistance);
        }
        else
        {
            e->velocity->x = 0;
            e->velocity->y = 0;
        }
        e->worldPos->x += e->velocity->x;
        e->worldPos->y += e->velocity->y;
        e->healthBar.x = e->position.x - 10.0f;
        e->healthBar.y = e->position.y - 20.0f;

        e->position.x = e->worldPos->x - camera.x;
        e->position.y = e->worldPos->y - camera.y;
    }
    managerOfSounds->update();
    enemyCount->updateText(std::to_string(enemies.size()).c_str());
}


void enemyManager::draw(SDL_FRect camera)
{
    for(enemy* e : enemies)
    {

        SDL_RenderTextureRotated(renderer, enemyTexture, &e->textureFrame, &e->position, e->rotation, NULL, SDL_FLIP_NONE);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &e->healthBar);
    }
    SDL_SetTextureScaleMode(enemyTexture, SDL_SCALEMODE_NEAREST);
    enemyCount->draw();
}