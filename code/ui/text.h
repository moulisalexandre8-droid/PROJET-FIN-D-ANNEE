#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

SDL_Texture* creerTexte(SDL_Renderer* rendu, TTF_Font* font, const char* texte, SDL_Color couleur);

void dessinerTexteCentre(SDL_Renderer* rendu, SDL_Texture* texture, int x, int y, int w, int h);

#endif