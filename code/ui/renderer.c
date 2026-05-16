#include "renderer.h"
#include <SDL2/SDL_ttf.h>

int estUnePiece(int valeur)
{
    return valeur >= 2 && valeur <= 10;
}

void afficherTexte(SDL_Renderer* rendu,TTF_Font* police,const char* texte,int x,int y)
{
    SDL_Color blanc = {255, 255, 255, 255};

    SDL_Surface* surface =
        TTF_RenderText_Blended(police, texte, blanc);

    SDL_Texture* texture =
        SDL_CreateTextureFromSurface(rendu, surface);

    SDL_Rect rectTexte =
    {
        x,
        y,
        surface->w,
        surface->h
    };

    SDL_RenderCopy(rendu, texture, NULL, &rectTexte);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void dessinerGrille(SDL_Renderer* rendu, int plateau[26][28], int tailleCase)
{
    TTF_Font* police = TTF_OpenFont("code/assets/fonts/Roboto-Regular.ttf", 20);

    if (police == NULL)
    {
        printf("Erreur police : %s\n", TTF_GetError());
        return;
    }
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

            int valeur = plateau[ligne][colonne];

            // Murs
            if (valeur == 1)
            {
                SDL_SetRenderDrawColor(rendu, 20, 20, 20, 255);
            }

            // Couloir
            else if (valeur == 0)
            {
                SDL_SetRenderDrawColor(rendu, 230, 220, 190, 255);
            }

            // Cuisine
            else if (valeur == 2)
            {
                SDL_SetRenderDrawColor(rendu, 120, 30, 30, 255);
            }

            // Salle de ball
            else if (valeur == 3)
            {
                SDL_SetRenderDrawColor(rendu, 200, 160, 40, 255);
            }

            // Veranda
            else if (valeur == 4)
            {
                SDL_SetRenderDrawColor(rendu, 70, 120, 50, 255);
            }

            // Bibliotheque
            else if (valeur == 5)
            {
                SDL_SetRenderDrawColor(rendu, 90, 40, 120, 255);
            }

            // Salon
            else if (valeur == 6)
            {
                SDL_SetRenderDrawColor(rendu, 40, 80, 150, 255);
            }

            // Bureau
            else if (valeur == 7)
            {
                SDL_SetRenderDrawColor(rendu, 40, 120, 120, 255);
            }

            // Salle a manger
            else if (valeur == 8)
            {
                SDL_SetRenderDrawColor(rendu, 130, 80, 30, 255);
            }

            // Salon de musique
            else if (valeur == 9)
            {
                SDL_SetRenderDrawColor(rendu, 20, 50, 100, 255);
            }

            // Hall
            else if (valeur == 10)
            {
                SDL_SetRenderDrawColor(rendu, 90, 90, 90, 255);
            }

            // Portes
            else if (valeur == 11)
            {
                SDL_SetRenderDrawColor(rendu, 200, 0, 0, 255);
            }

            SDL_RenderFillRect(rendu, &casePlateau);

            if (!estUnePiece(valeur))
            {
                SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
                SDL_RenderDrawRect(rendu, &casePlateau);
            }
        }
    }
    afficherTexte(rendu, police, "CUISINE", 80, 90);

    afficherTexte(rendu, police, "SALLE DE BAL", 415, 90);

    afficherTexte(rendu, police, "VERANDA", 780, 90);

    afficherTexte(rendu, police, "BIBLIOTHEQUE", 80, 350);

    afficherTexte(rendu, police, "SALON", 760, 350);

    afficherTexte(rendu, police, "BUREAU", 80, 660);

    afficherTexte(rendu, police, "SALLE A MANGER", 380, 660);

    afficherTexte(rendu, police, "SALON A THE", 745, 660);

    afficherTexte(rendu, police, "HALL", 455, 430);

    TTF_CloseFont(police);
}