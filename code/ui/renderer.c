#include "renderer.h"
#include <stdio.h>

#define OFFSET_X 59
#define OFFSET_Y 40// pour les marges décoratives du plateau, à ajuster si besoin

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

void afficherTexte(SDL_Renderer* rendu,TTF_Font* police,const char* texte,int x,int y)
{
    SDL_Color blanc = {255,255,255,255};

    SDL_Surface* surface = TTF_RenderText_Blended(police, texte, blanc);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(rendu, surface);

    SDL_Rect rectTexte ={x,y,surface->w,surface->h};

    SDL_RenderCopy(rendu, texture, NULL, &rectTexte);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void afficherNomsPieces(SDL_Renderer* rendu,TTF_Font* police)
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

void dessinerCase(SDL_Renderer* rendu,int valeur,int x,int y,int tailleCaseX,int tailleCaseY)
{
    SDL_Rect casePlateau ={x,y,tailleCaseX,tailleCaseY};

    SDL_Color couleur = obtenirCouleurCase(valeur);// obtient la couleur de la case en fonction de sa valeur

    SDL_SetRenderDrawColor(rendu,couleur.r,couleur.g,couleur.b,couleur.a);// configure le renderer pour dessiner avec la couleur de la case

    SDL_RenderFillRect(rendu, &casePlateau);

    if (!estUnePiece(valeur))// si c pas uune pièce, on dessine un contour noir pour les murs et les couloirs
    {
        SDL_SetRenderDrawColor(rendu, 0,0,0,255);

        SDL_RenderDrawRect(rendu, &casePlateau);
    }
}

void dessinerPlateau(SDL_Renderer* rendu,int plateau[26][28],int tailleCaseX,int tailleCaseY)
{
    for (int ligne = 0; ligne < 26; ligne++)
    {
        for (int colonne = 0; colonne < 28; colonne++)
        {
            dessinerCase(rendu,plateau[ligne][colonne],colonne * tailleCaseX,ligne * tailleCaseY,tailleCaseX,tailleCaseY);
        }
    }
}

void dessinerJoueur(SDL_Renderer* rendu,SDL_Texture* texture,int x,int y,int tailleCaseX,int tailleCaseY)
{
    SDL_Rect dest ={x,y,tailleCaseX,tailleCaseY};

    SDL_RenderCopy(rendu, texture, NULL, &dest);
}

void dessinerGrille(SDL_Renderer* rendu,int plateau[26][28],int tailleCaseX,int tailleCaseY)
{
    TTF_Font* police = TTF_OpenFont("code/assets/fonts/Roboto-Regular.ttf",20);

    if (police == NULL)
    {
        printf("Erreur police : %s\n", TTF_GetError());
        return;
    }

    dessinerPlateau(rendu, plateau, tailleCaseX, tailleCaseY);

    afficherNomsPieces(rendu, police);

    TTF_CloseFont(police);
}

void dessinerTexture(SDL_Renderer* rendu,SDL_Texture* texture,int x,int y,int largeur,int hauteur)
{
    SDL_Rect dest ={x,y,largeur,hauteur};

    SDL_RenderCopy(rendu, texture, NULL, &dest);
}

void dessinerFond(SDL_Renderer* rendu, SDL_Texture* textureFond)
{
    SDL_Rect fond ={0,0,925,860};

    SDL_RenderCopy(rendu, textureFond, NULL, &fond);
}

// a suppprimer après les tests
void dessinerGrilleDebug(SDL_Renderer* rendu,
                         int tailleCaseX,
                         int tailleCaseY)
{
    SDL_SetRenderDrawColor(rendu, 255, 0, 0, 255);

    for (int lig = 0; lig < 26; lig++)
    {
        for (int col = 0; col < 25; col++)
        {
            SDL_Rect rect =
            {
                OFFSET_X + col * tailleCaseX,
                OFFSET_Y + lig * tailleCaseY,
                tailleCaseX,
                tailleCaseY
            };

            SDL_RenderDrawRect(rendu, &rect);
        }
    }
}