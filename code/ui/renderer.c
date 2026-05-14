#include "renderer.h"

void dessinerGrille(SDL_Renderer* rendu, int plateau[26][28], int tailleCase)
{
    for (int ligne = 0; ligne < 26; ligne++)
    {
        for (int colonne = 0; colonne < 28; colonne++)
        {
            SDL_Rect casePlateau =
            {
                colonne * tailleCase,
                ligne * tailleCase,
                tailleCase,
                tailleCase
            };

            // case jouable
            if (plateau[ligne][colonne] == 1)
            {
                SDL_SetRenderDrawColor(rendu, 20, 20, 20, 255);
            }
            else
            {
                // mur
                SDL_SetRenderDrawColor(rendu, 100, 100, 100, 255);
            }

            SDL_RenderFillRect(rendu, &casePlateau);

            // contour
            SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
            SDL_RenderDrawRect(rendu, &casePlateau);
        }
    }
}