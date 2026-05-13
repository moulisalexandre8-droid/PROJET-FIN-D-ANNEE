#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>

typedef struct
{
    int x;
    int y;
} Joueur;

// init SDL
int initialiserSDL(SDL_Window** fenetre, SDL_Renderer** rendu);

// joueur
Joueur initialiserJoueur();

// gameplay
void bougerJoueur(const Uint8* etat, Joueur* j, int tailleCase, int* peutBouger);
void appliquerLimites(Joueur* j, int tailleCase, int largeur, int hauteur);
void dessiner(SDL_Renderer* rendu, Joueur j, int tailleCase);

// boucle principale
void boucleJeu(SDL_Window* fenetre, SDL_Renderer* rendu);

// nettoyer SDL
void nettoyer(SDL_Window* fenetre, SDL_Renderer* rendu);

#endif