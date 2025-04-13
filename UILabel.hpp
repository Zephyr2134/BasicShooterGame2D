#pragma once
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL.h>

class UILabel
{  
public:
    UILabel(SDL_Renderer* ren, const char* text, float size, SDL_FRect trnsfrm);
    ~UILabel();

    void draw();

    void updateText(const char* newTxt);

private:
    SDL_Texture* textTex;
    int textLength = 0;
    TTF_Font* font;
    SDL_FRect transform;
    SDL_Color textColor = {255, 255, 255, 255};

    SDL_Renderer* renderer;
};