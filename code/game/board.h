#ifndef BOARD_H
#define BOARD_H

#include "game.h"

int estUnMur(int ligne,int colonne);

int peutAller(int ancienneX,int ancienneY,int nouvelleX,int nouvelleY,int tailleCaseX,int tailleCaseY);

void placerJoueurCase(Joueur* j,int col,int lig,int tailleCaseX,int tailleCaseY);

int obtenirCasePlateau(int x,int y,int tailleCaseX,int tailleCaseY);

#endif