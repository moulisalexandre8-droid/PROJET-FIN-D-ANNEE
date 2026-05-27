#include "game.h"
#include "board.h"
#include "../utils/constants.h"
#include "../ui/renderer.h"
#include "../utils/loader.h"
#include "../entities/room.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>


// Plateau de jeu : 0 = sol, 1 = mur, 2/3/4/5/6/7/8/9/10 = pièces,6 = picine , 12 = porte haut, 13 = porte bas, 14 = porte gauche, 15 = porte droite

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
    {1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,14,6,6,6,6,6,6,1},
    {1,5,5,5,1,1,1,1,0,0,1,1,1,1,1,0,0,0,1,6,6,6,6,6,6,1},
    {1,5,5,5,5,5,5,1,0,0,1,1,1,1,1,0,0,0,1,6,6,6,6,6,6,1},
    {1,5,5,5,5,5,5,15,0,0,1,1,1,1,1,0,0,0,1,1,1,1,13,1,1,1},
    {1,5,5,5,5,5,5,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1},
    {1,5,5,5,5,5,5,1,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,13,1,0,0,1,1,1,1,1,0,0,1,1,7,7,7,7,7,7,1},
    {1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,14,7,7,7,7,7,7,7,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,7,7,7,7,7,7,1},
    {1,0,0,0,0,0,0,0,0,1,1,12,12,1,1,0,0,0,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,12,0,0,1,9,9,9,9,1,0,0,0,0,0,0,0,0,0,1},
    {1,8,8,8,8,8,1,0,0,1,9,9,9,9,15,0,0,0,0,0,0,0,0,1,1},
    {1,8,8,8,8,8,1,0,0,1,9,9,9,9,1,0,0,12,1,1,1,1,1,1,1},
    {1,8,8,8,8,8,1,0,0,1,9,9,9,9,1,0,0,1,10,10,10,10,10,10,1},
    {1,8,8,8,8,8,1,0,0,1,9,9,9,9,1,0,0,1,10,10,10,10,10,10,1},
    {1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1},
};

int estUnMur(int ligne, int colonne)
{
    return plateau[ligne][colonne] == 1;
}

int estUnePorte(int valeur)
{
    return valeur >= 12 && valeur <= 15;
}

int peutAller(int ancienneX,int ancienneY,int x,int y,float tailleCaseX,float tailleCaseY)
{
    int col = (x - PLATEAU_X - OFFSET_X) / tailleCaseX;
    int lig = (y - OFFSET_Y) / tailleCaseY;

    int ancienneCol = (ancienneX - PLATEAU_X - OFFSET_X) / tailleCaseX;

    int ancienneLig = (ancienneY - OFFSET_Y) / tailleCaseY;

    if(col < 0 || col >= 26 || lig < 0 || lig >= 25)
        return 0;

    int caseArrivee = plateau[lig][col];

    if(caseArrivee == 0) return 1;
    if(caseArrivee == 1) return 0;
    if(caseArrivee == 6) return 1;

    if(caseArrivee == 12)
        return lig > ancienneLig;

    if(caseArrivee == 13)
        return lig < ancienneLig;

    if(caseArrivee == 14)
        return col > ancienneCol;

    if(caseArrivee == 15)
        return col < ancienneCol;

    if(caseArrivee >= 2 && caseArrivee <= 11)
        return 1;

    return 0;
}

void placerJoueurCase(Joueur* j,int col,int lig,float tailleCaseX,float tailleCaseY)
{
    j->x =PLATEAU_X+ OFFSET_X+ col * tailleCaseX+ tailleCaseX/2;

    j->y =OFFSET_Y+ lig * tailleCaseY+ tailleCaseY/2;
}

int obtenirCasePlateau(int x,int y,float tailleCaseX,float tailleCaseY)
{
    int col = (x-PLATEAU_X-OFFSET_X)/tailleCaseX;

    int lig = (y - OFFSET_Y)/tailleCaseY;

    if(lig<0 || lig>=25 || col<0 || col>=26)
        return -1;

    return plateau[lig][col];
}

void obtenirLigneColonneDepuisCase(int valeurCase,int* ligne,int* colonne)
{
    for(int lig = 0; lig < 25; lig++)
    {
        for(int col = 0; col < 26; col++)
        {
            if(plateau[lig][col] == valeurCase)
            {
                *ligne = lig;
                *colonne = col;
                return;
            }
        }
    }
}

int obtenirSalleDepuisPorte(int lig,int col)
{
    int directions[8][2] = {{-1,0},{1,0},{0,-1},{0,1},{-1,-1},{-1,1},{1,-1},{1,1}};

    for(int i = 0; i < 8; i++)
    {
        int nl = lig + directions[i][0];
        int nc = col + directions[i][1];

        if(nl >= 0 && nl < 25 && nc >= 0 && nc < 26)
        {
            int val = plateau[nl][nc];

            if(estUneSalle(val))
            {
                return val;
            }
        }
    }
    return -1;
}