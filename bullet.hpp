#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "vector2d.hpp"
#include <vector>

class bullet
{
public:

    bullet(SDL_Renderer* ren, const char* filePath, int size, float srcW, float srcH, int sped, int damag);
    ~bullet();

    void createBullet(float xPos, float yPos, float xVel, float yVel, float rotation, SDL_FRect camera);
    void destroyBullet(int id);
    void clearBullets(int i, SDL_FRect camera);
    void collision(std::vector<SDL_FRect> rects);
    int damage(){return dmg;}
    void update(SDL_FRect camera);
    std::vector<SDL_FRect> returnBullets(){ return dstRects; }
    void draw();

private:  
    SDL_Renderer* renderer;
    SDL_Texture* bulletTex;

    SDL_FRect srcRect;
    SDL_FRect baseDst;
    const int size;
    const float halfHeight;
    const float halfWidth;

    int dmg;

    std::vector<float> rotations;
    std::vector<vector2d*> positions;
    std::vector<SDL_FRect> dstRects;
    std::vector<vector2d> velocities;
    const int speed;
}; 