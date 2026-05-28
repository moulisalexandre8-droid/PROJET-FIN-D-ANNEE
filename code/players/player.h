#ifndef PLAYER_H
#define PLAYER_H
#include "../game/game.h"

typedef struct {
    int l;
    int c;
} PointBFS;

int trouverProchainPasBFS(int startLig, int startCol, int cibleSalle, int* nextLig, int* nextCol);

Joueur initialiserJoueur(SDL_Renderer* rendu,int x,int y,const char* cheminImage,const char* nom);

void deplacerJoueur(Joueur* j, int nx, int ny);

void appliquerLimites(Joueur* j,int tailleCaseX,int tailleCaseY,int largeur,int hauteur);

void teleporterDansSalle(Joueur* j,int salle,float tailleX,float tailleY);

Joueur* obtenirJoueurParSuspect(int suspect,Joueur* j1,Joueur* j2);

#endif