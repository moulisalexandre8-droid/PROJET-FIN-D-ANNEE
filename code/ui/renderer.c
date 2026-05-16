#include "renderer.h"
#include <stdio.h>

int estUnePiece(int valeur)
{
    return valeur >= 2 && valeur <= 10;
}

SDL_Color obtenirCouleurCase(int valeur)
{
    switch (valeur)
    {
        case 0: return (SDL_Color){230,220,190,255};
        case 1: return (SDL_Color){20,20,20,255};
        case 2: return (SDL_Color){120,30,30,255};
        case 3: return (SDL_Color){200,160,40,255};
        case 4: return (SDL_Color){70,120,50,255};
        case 5: return (SDL_Color){90,40,120,255};
        case 6: return (SDL_Color){40,80,150,255};
        case 7: return (SDL_Color){40,120,120,255};
        case 8: return (SDL_Color){130,80,30,255};
        case 9: return (SDL_Color){20,50,100,255};
        case 10:return (SDL_Color){90,90,90,255};
        case 11:return (SDL_Color){200,0,0,255};

        default:
            return (SDL_Color){255,255,255,255};
    }
}

void afficherTexte(
    SDL_Renderer* rendu,
    TTF_Font* police,
    const char* texte,
    int x,
    int y
)
{
    SDL_Color blanc = {255,255,255,255};

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

void afficherNomsPieces(
    SDL_Renderer* rendu,
    TTF_Font* police
)
{
    afficherTexte(rendu, police, "CUISINE", 80, 90);

    afficherTexte(rendu, police, "SALLE DE BAL", 415, 90);

    afficherTexte(rendu, police, "VERANDA", 780, 90);

    afficherTexte(rendu, police, "BIBLIOTHEQUE", 80, 350);

    afficherTexte(rendu, police, "SALON", 760, 350);

    afficherTexte(rendu, police, "BUREAU", 80, 660);

    afficherTexte(rendu, police, "SALLE A MANGER", 380, 660);

    afficherTexte(rendu, police, "SALON A THE", 745, 660);

    afficherTexte(rendu, police, "HALL", 455, 430);
}

void dessinerCase(
    SDL_Renderer* rendu,
    int valeur,
    int x,
    int y,
    int tailleCase
)
{
    SDL_Rect casePlateau =
    {
        x,
        y,
        tailleCase,
        tailleCase
    };

    SDL_Color couleur =
        obtenirCouleurCase(valeur);

    SDL_SetRenderDrawColor(
        rendu,
        couleur.r,
        couleur.g,
        couleur.b,
        couleur.a
    );

    SDL_RenderFillRect(rendu, &casePlateau);

    if (!estUnePiece(valeur))
    {
        SDL_SetRenderDrawColor(rendu, 0,0,0,255);

        SDL_RenderDrawRect(rendu, &casePlateau);
    }
}

void dessinerPlateau(
    SDL_Renderer* rendu,
    int plateau[26][28],
    int tailleCase
)
{
    for (int ligne = 0; ligne < 26; ligne++)
    {
        for (int colonne = 0; colonne < 28; colonne++)
        {
            dessinerCase(
                rendu,
                plateau[ligne][colonne],
                colonne * tailleCase,
                ligne * tailleCase,
                tailleCase
            );
        }
    }
}

void dessinerJoueur(
    SDL_Renderer* rendu,
    int x,
    int y,
    int tailleCase,
    SDL_Color couleur
)
{
    SDL_Rect joueur =
    {
        x,
        y,
        tailleCase,
        tailleCase
    };

    SDL_SetRenderDrawColor(
        rendu,
        couleur.r,
        couleur.g,
        couleur.b,
        couleur.a
    );

    SDL_RenderFillRect(rendu, &joueur);
}

void dessinerGrille(
    SDL_Renderer* rendu,
    int plateau[26][28],
    int tailleCase
)
{
    TTF_Font* police =
        TTF_OpenFont(
            "code/assets/fonts/Roboto-Regular.ttf",
            20
        );

    if (police == NULL)
    {
        printf("Erreur police : %s\n", TTF_GetError());
        return;
    }

    dessinerPlateau(rendu, plateau, tailleCase);

    afficherNomsPieces(rendu, police);

    TTF_CloseFont(police);
}