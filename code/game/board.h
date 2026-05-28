#ifndef BOARD_H
#define BOARD_H

#include "game.h"

extern int plateau[25][26];

int estUnMur(int ligne,int colonne);

int estUnePorte(int valeur);

int peutAller(int ancienneX,int ancienneY,int nouvelleX,int nouvelleY,float tailleCaseX,float tailleCaseY);

void placerJoueurCase(Joueur* j,int col,int lig,float tailleCaseX,float tailleCaseY);

int obtenirCasePlateau(int x,int y,float tailleCaseX,float tailleCaseY);

void obtenirLigneColonneDepuisCase(int valeurCase,int* ligne,int* colonne);

int obtenirSalleDepuisPorte(int lig,int col);

int trouverPortePourSalle(int idSalle, int* ligneSortie, int* colonneSortie);

#endif