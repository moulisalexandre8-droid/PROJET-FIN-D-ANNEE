#ifndef RENDERER_H
#define RENDERER_H
#include <SDL2/SDL_ttf.h>

#include <SDL2/SDL.h>

int estUnePiece(int valeur);

void afficherTexte(SDL_Renderer* rendu,TTF_Font* police,const char* texte,int x,int y);

void dessinerGrille(SDL_Renderer* rendu, int plateau[26][28], int tailleCase);

#endif