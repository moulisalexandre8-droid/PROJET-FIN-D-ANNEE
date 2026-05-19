#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "../utils/constants.h"
#include <SDL2/SDL_image.h>
#include "rules.h"

typedef struct
{
    int x;
    int y;

    SDL_Texture* texture;

    char nom[30];

    int mouvementsRestants;

} Joueur;

// init SDL
int initialiserSDL(SDL_Window** fenetre, SDL_Renderer** rendu);

// joueur
Joueur initialiserJoueur(SDL_Renderer* rendu,int x,int y,const char* cheminImage,const char* nom);

// gameplay
void bougerJoueur(const Uint8* etat,Joueur* j,int tailleCaseX,int tailleCaseY);
void appliquerLimites(Joueur* j,int tailleCaseX,int tailleCaseY,int largeur,int hauteur);

int estUnMur(int ligne, int colonne);
int peutAller(int ancienneX,int ancienneY,int nouvelleX,int nouvelleY,int tailleCaseX,int tailleCaseY);

void deplacerJoueur(Joueur* j, int nouvelleX, int nouvelleY);

// boucle principale
void boucleJeu(SDL_Window* fenetre, SDL_Renderer* rendu);

// nettoyage
void nettoyer(SDL_Window* fenetre, SDL_Renderer* rendu);

void placerJoueurCase(Joueur* j,int col,int lig,int tailleCaseX,int tailleCaseY);

//temporaire pour afficher la grille de debug
void dessinerGrilleDebug(SDL_Renderer* rendu, int tailleCaseX,int tailleCaseY);

#endif