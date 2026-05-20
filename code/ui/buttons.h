#ifndef BUTTONS_H
#define BUTTONS_H

#include <SDL2/SDL.h>

typedef struct
{
    SDL_Rect rect;
    char texte[50];
    int hover;
    int actif;
} Bouton;

Bouton creerBouton(int x, int y, int w, int h, const char* texte);

void updateHover(Bouton* b, int mouseX, int mouseY);

int boutonEstClique(Bouton* b, int mouseX, int mouseY);

void dessinerBouton(SDL_Renderer* rendu, Bouton* b);

#endif