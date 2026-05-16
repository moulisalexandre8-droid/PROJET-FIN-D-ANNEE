#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>

typedef struct
{
    int x;
    int y;

    SDL_Color couleur;

    char nom[30];

    int mouvementsRestants;

} Joueur;

// init SDL
int initialiserSDL(SDL_Window** fenetre, SDL_Renderer** rendu);

// joueur
Joueur initialiserJoueur(int x, int y, SDL_Color couleur, const char* nom);

// gameplay
void bougerJoueur(const Uint8* etat, Joueur* j, int tailleCase);
void appliquerLimites(Joueur* j, int tailleCase, int largeur, int hauteur);

int estUnMur(int ligne, int colonne);
int peutAller(int x, int y, int tailleCase);

void deplacerJoueur(Joueur* j, int nouvelleX, int nouvelleY);

// boucle principale
void boucleJeu(SDL_Window* fenetre, SDL_Renderer* rendu);

// nettoyage
void nettoyer(SDL_Window* fenetre, SDL_Renderer* rendu);

#endif