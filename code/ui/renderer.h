#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../utils/constants.h"

void dessinerTexture(SDL_Renderer* rendu,SDL_Texture* texture,int x,int y,int largeur,int hauteur);

void dessinerJoueur(SDL_Renderer* rendu,SDL_Texture* texture,int x,int y,int tailleCaseX,int tailleCaseY);

void dessinerGrilleDebug(SDL_Renderer* rendu);

void dessinerInterfaceDroite(SDL_Renderer* rendu);

#endif