#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "../utils/constants.h"
#include <SDL2/SDL_image.h>
#include "../entities/card.h"
#include "../utils/constants.h"
#include "mode.h"

typedef struct Joueur
{
    int x;
    int y;

    SDL_Texture* texture;

    char nom[30];

    int mouvementsRestants;

    Carte cartes[15];
    int nbCartes;
    int personnage;

    int notesSuspects[6];
    int notesArmes[6];
    int notesPieces[9];
    int elimine;

    int ligneCarnet;
    int aFaitSoupcon;

} Joueur;

// init SDL
int initialiserSDL(SDL_Window** fenetre, SDL_Renderer** rendu);

// joueur
Joueur initialiserJoueur(SDL_Renderer* rendu,int x,int y,const char* cheminImage,const char* nom);

// gameplay
void bougerJoueur(const Uint8* etat,Joueur* j,int tailleCaseX,int tailleCaseY);
void appliquerLimites(Joueur* j,int tailleCaseX,int tailleCaseY,int largeur,int hauteur);

int estUnMur(int ligne, int colonne);
int peutAller(int ancienneX,int ancienneY,int nouvelleX,int nouvelleY,float tailleCaseX,float tailleCaseY);

void deplacerJoueur(Joueur* j, int nouvelleX, int nouvelleY);

void dessinerPortes(SDL_Renderer* rendu, float tailleCaseX, float tailleCaseY);

// boucle principale

void boucleJeu(SDL_Window* fenetre,SDL_Renderer* rendu,ModeJeu mode);

// nettoyage
void nettoyer(SDL_Window* fenetre, SDL_Renderer* rendu);

void placerJoueurCase(Joueur* j,int col,int lig,float tailleCaseX,float tailleCaseY);

//temporaire pour afficher la grille de debug
void dessinerGrilleDebug(SDL_Renderer* rendu);

#endif