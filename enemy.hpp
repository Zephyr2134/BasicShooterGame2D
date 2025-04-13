#pragma once
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "vector2d.hpp"
#include <cstdlib>
#include "UILabel.hpp"
#include "soundManager.hpp"

typedef struct enemy{

    enemy(int xpos, int ypos, int w, int h, int sc, int hp, int id){
        
        position.w = w*sc;
        position.h = h*sc;

        textureFrame.x = 0;
        textureFrame.y = 0;
        textureFrame.w = w;
        textureFrame.h = h;

        velocity = new vector2d(0,0);
        worldPos = new vector2d(xpos, ypos);

        health = hp;
        healthBar = {(xpos + w/2.0f) - 10.0f, ypos - 20.0f, (100.0f/health)*health, 10};
        
        srand(id);
        int randNum = rand()%101;
        minDistance = randNum + 200;
        maxDistance = (randNum*5) + 400;
    }

    SDL_FRect position;
    vector2d* worldPos;
    SDL_FRect textureFrame;
    vector2d* velocity;

    int health = 0;
    SDL_FRect healthBar;

    bool moveToPlayer = false;
    int rotation = 0;
    bool hit = false;
    Uint32 lastHit = 0;

    int minDistance;
    int maxDistance;

    float xDif = 0;
    float yDif = 0;
    float hyp = 0;
}enemy;

class enemyManager{
public:

    enemyManager(SDL_Renderer* ren, const char* textureFilePath, int w, int h, int sc, int hp, SDL_FRect camera);
    ~enemyManager();

    void createEnemy(int xpos, int ypos, int id);
    bool collision(SDL_FRect recA, int damage);
    void checkPlayerPos(float playerX, float playerY);
    void update(SDL_FRect camera);
    void draw(SDL_FRect camera);
    bool waveDone(){return (enemies.size() == 0);};


private:

    SDL_Renderer* renderer;

    const int width = 0;
    const int height = 0;
    const int scale = 0;
    const int health = 0;

    UILabel* enemyCount;

    SDL_Texture* enemyTexture;
    std::vector<enemy*> enemies;

    soundManager* managerOfSounds;
};