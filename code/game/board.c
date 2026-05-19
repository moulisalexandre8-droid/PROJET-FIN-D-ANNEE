#include "game.h"
#include "board.h"
#include "../utils/constants.h"
#include "../ui/renderer.h"
#include "../utils/loader.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>


// Plateau de jeu : 0 = sol, 1 = mur, 2/3/4/5/7/8/9/10/11 = pièces,6 = picine , 12 = porte haut, 13 = porte bas, 14 = porte gauche, 15 = porte droite

int plateau[25][26] =
{
    {1,1,1,1,1,1,1,1,1,0,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1},
    {1,2,2,2,2,1,1,0,0,0,1,3,3,1,0,0,0,1,1,4,4,4,4,4,1,1},
    {1,2,2,2,2,1,0,0,1,1,1,3,3,1,1,1,0,0,1,4,4,4,4,4,1,1},
    {1,2,2,2,2,1,0,0,1,3,3,3,3,3,3,1,0,0,1,4,4,4,4,4,1,1},
    {1,2,2,2,2,1,0,0,1,3,3,3,3,3,3,1,0,0,1,4,4,4,4,4,1,1},
    {1,2,2,2,2,1,0,0,14,3,3,3,3,3,3,15,0,0,0,14,1,1,1,1,1,1},
    {1,1,1,1,13,1,0,0,1,3,3,3,3,3,3,1,0,0,0,0,0,0,0,0,1,1},
    {0,0,0,0,0,0,0,0,1,13,1,1,1,1,13,1,0,0,0,0,0,0,0,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,14,7,7,7,7,7,7,1},
    {1,5,5,5,1,1,1,1,0,0,1,1,1,1,1,0,0,0,1,7,7,7,7,7,7,1},
    {1,5,5,5,5,5,5,1,0,0,1,1,1,1,1,0,0,0,1,7,7,7,7,7,7,1},
    {1,5,5,5,5,5,5,15,1,1,1,1,1,1,1,0,0,0,1,1,1,1,13,1,1,1},
    {1,5,5,5,5,5,5,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1},
    {1,5,5,5,5,5,5,1,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,1,1,8,8,8,8,8,8,1},
    {1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,14,8,8,8,8,8,8,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,8,8,8,8,8,8,1},
    {1,0,0,0,0,0,0,0,0,1,1,12,12,1,1,0,0,0,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,12,0,0,1,10,10,10,10,1,0,0,0,0,0,0,0,0,0,1},
    {1,9,9,9,9,9,1,0,0,1,10,10,10,10,15,0,0,0,0,0,0,0,0,1,1},
    {1,9,9,9,9,9,1,0,0,1,10,10,10,10,1,0,0,12,1,1,1,1,1,1,1},
    {1,9,9,9,9,9,1,0,0,1,10,10,10,10,1,0,0,1,11,11,11,11,11,11,1},
    {1,9,9,9,9,9,1,0,0,1,10,10,10,10,1,0,0,1,11,11,11,11,11,11,1},
    {1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1},
};

int estUnMur(int ligne, int colonne)
{
    return plateau[ligne][colonne] == 1;
}

int peutAller(int ancienneX,int ancienneY,
              int x,int y,
              int tailleCaseX,int tailleCaseY)
{
    int col = (x - OFFSET_X) / tailleCaseX;
    int lig = (y - OFFSET_Y) / tailleCaseY;

    int ancienneCol = (ancienneX - OFFSET_X) / tailleCaseX;
    int ancienneLig = (ancienneY - OFFSET_Y) / tailleCaseY;

    // limites tableau
    if (lig < 0 || lig >= 25 || col < 0 || col >= 26)
        return 0;

    int caseArrivee = plateau[lig][col];

    //chemainement de base : on peut aller sur les cases vides et la piscine

    if (caseArrivee == 0)
        return 1;

    //mur

    if (caseArrivee == 1)
        return 0;

    // piscine

    if (caseArrivee == 6)
        return 1;

    // portes : on doit venir d'une direction précise pour pouvoir les traverser

    // porte haut
    if (caseArrivee == 12)
    {
        return lig > ancienneLig;
    }

    // porte bas
    if (caseArrivee == 13)
    {
        return lig < ancienneLig;
    }

    // porte gauche
    if (caseArrivee == 14)
    {
        return col > ancienneCol;
    }

    // porte droite
    if (caseArrivee == 15)
    {
        return col < ancienneCol;
    }

    // pieces 
    if (caseArrivee >= 2 && caseArrivee <= 11)
        return 1;

    return 0;
}

void placerJoueurCase(Joueur* j,int col,int lig,int tailleCaseX,int tailleCaseY)
{
    j->x = OFFSET_X + col * tailleCaseX;
    j->y = OFFSET_Y + lig * tailleCaseY;
}

int obtenirCasePlateau(int x,int y,int tailleCaseX,int tailleCaseY)
{
    int col =
    (x - OFFSET_X)/tailleCaseX;

    int lig =
    (y - OFFSET_Y)/tailleCaseY;

    if(lig<0 || lig>=25 ||
       col<0 || col>=26)
        return -1;

    return plateau[lig][col];
}