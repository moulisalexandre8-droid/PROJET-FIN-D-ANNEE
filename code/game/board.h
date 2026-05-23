#ifndef BOARD_H
#define BOARD_H

#include "game.h"

int estUnMur(int ligne,int colonne);

int peutAller(int ancienneX,int ancienneY,int nouvelleX,int nouvelleY,float tailleCaseX,float tailleCaseY);

void placerJoueurCase(Joueur* j,int col,int lig,float tailleCaseX,float tailleCaseY);

int obtenirCasePlateau(int x,int y,float tailleCaseX,float tailleCaseY);

#endif