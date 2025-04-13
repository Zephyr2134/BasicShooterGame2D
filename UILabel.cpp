#include "UILabel.hpp"
#include <iostream>

UILabel::UILabel(SDL_Renderer* ren, const char* text, float size, SDL_FRect trnsfrm) : renderer(ren), transform(trnsfrm)
{   
    font = TTF_OpenFont("ARIAL.TTF", size);
    SDL_Surface* tmpSurf = TTF_RenderText_Solid(font, text, strlen(text), textColor);
    textTex = SDL_CreateTextureFromSurface(renderer, tmpSurf);
    SDL_DestroySurface(tmpSurf);
}

UILabel::~UILabel()
{}

void UILabel::draw()
{
    SDL_RenderTexture(renderer, textTex, NULL, &transform);
    SDL_SetTextureScaleMode(textTex, SDL_SCALEMODE_NEAREST);
}

void UILabel::updateText(const char* newTxt)
{
    SDL_Surface* tmpSurf = TTF_RenderText_Solid(font,newTxt, strlen(newTxt), textColor);
    textTex = SDL_CreateTextureFromSurface(renderer, tmpSurf);
    SDL_DestroySurface(tmpSurf);
}