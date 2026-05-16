#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

int estUnePiece(int valeur);

SDL_Color obtenirCouleurCase(int valeur);

void afficherTexte(
    SDL_Renderer* rendu,
    TTF_Font* police,
    const char* texte,
    int x,
    int y
);

void afficherNomsPieces(
    SDL_Renderer* rendu,
    TTF_Font* police
);

void dessinerCase(
    SDL_Renderer* rendu,
    int valeur,
    int x,
    int y,
    int tailleCase
);

void dessinerPlateau(
    SDL_Renderer* rendu,
    int plateau[26][28],
    int tailleCase
);

void dessinerJoueur(
    SDL_Renderer* rendu,
    int x,
    int y,
    int tailleCase,
    SDL_Color couleur
);

void dessinerGrille(
    SDL_Renderer* rendu,
    int plateau[26][28],
    int tailleCase
);

#endif