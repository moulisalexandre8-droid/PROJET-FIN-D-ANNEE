#include "game.h"
#include <SDL2/SDL.h>
#include <stdio.h>

// On définit le plateau de jeu : 1 = mur, 0 = case jouable,2 à 10 pieces)
int plateau[26][28] =
{
    {1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1},
    {1,2,2,2,2,2,1,0,1,3,3,3,3,3,3,3,3,3,3,3,1,0,1,4,4,4,4,1},
    {1,2,2,2,2,2,1,0,1,3,3,3,3,3,3,3,3,3,3,3,1,0,1,4,4,4,4,1},
    {1,2,2,2,2,2,1,0,1,3,3,3,3,3,3,3,3,3,3,3,1,0,1,4,4,4,4,1},
    {1,2,2,2,2,2,1,0,1,3,3,3,3,3,3,3,3,3,3,3,1,0,1,4,4,4,4,1},
    {1,2,2,2,2,2,11,0,1,3,3,3,3,3,3,3,3,3,3,3,1,0,11,4,4,4,4,1},
    {1,1,1,1,1,1,1,0,1,1,1,3,3,3,3,3,3,3,1,1,1,0,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,1,1,1,11,1,1,1,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
    {1,5,5,5,5,5,5,5,1,0,0,0,1,1,1,1,1,0,0,0,1,6,6,6,6,6,6,1},
    {1,5,5,5,5,5,5,5,1,0,0,0,1,10,10,10,1,0,0,0,1,6,6,6,6,6,6,1},
    {1,5,5,5,5,5,5,5,11,0,0,0,1,10,10,10,1,0,0,0,11,6,6,6,6,6,6,1},
    {1,5,5,5,5,5,5,5,1,0,0,0,1,10,10,10,1,0,0,0,1,6,6,6,6,6,6,1},
    {1,5,5,5,5,5,5,5,1,0,0,0,1,10,10,10,1,0,0,0,1,6,6,6,6,6,6,1},
    {1,5,5,5,5,5,5,5,1,0,0,0,1,10,10,10,1,0,0,0,1,6,6,6,6,6,6,1},
    {1,1,1,1,1,1,1,1,1,0,0,0,1,1,11,1,1,0,0,0,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,0,0,0,0,1,1,1,11,1,1,1,0,0,0,1,1,1,1,1,1,1},
    {1,7,7,7,7,7,1,0,0,1,1,8,8,8,8,8,8,1,1,0,0,1,9,9,9,9,9,1},
    {1,7,7,7,7,7,1,0,0,1,8,8,8,8,8,8,8,8,1,0,0,1,9,9,9,9,9,1},
    {1,7,7,7,7,7,1,0,0,1,8,8,8,8,8,8,8,8,1,0,0,1,9,9,9,9,9,1},
    {1,7,7,7,7,7,1,0,0,1,8,8,8,8,8,8,8,8,1,0,0,1,9,9,9,9,9,1},
    {1,7,7,7,7,7,11,0,0,1,8,8,8,8,8,8,8,8,1,0,0,11,9,9,9,9,9,1},
    {1,7,7,7,7,7,1,0,0,1,8,8,8,8,8,8,8,8,1,0,0,1,9,9,9,9,9,1},
    {1,7,7,7,7,7,1,0,0,1,8,8,8,8,8,8,8,8,1,0,0,1,9,9,9,9,9,1},
    {1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1}
};

// INIT SDL
int initialiserSDL(SDL_Window** fenetre, SDL_Renderer** rendu)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Erreur SDL : %s\n", SDL_GetError());
        return 0;
    }

    *fenetre = SDL_CreateWindow(
        "Cluelau",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        925,
        860,
        0
    );

    if (!*fenetre)
    {
        printf("Erreur fenetre : %s\n", SDL_GetError());
        return 0;
    }

    *rendu = SDL_CreateRenderer(*fenetre, -1, SDL_RENDERER_ACCELERATED);

    if (!*rendu)
    {
        printf("Erreur renderer : %s\n", SDL_GetError());
        return 0;
    }

    return 1;
}

// initialisation du joueur
Joueur initialiserJoueur()
{
    Joueur j = {231,0};
    return j;
}
// pas sortir de l'écran
void appliquerLimites(Joueur* j, int tailleCase, int largeur, int hauteur)
{
    if (j->x < 0) j->x = 0;
    if (j->y < 0) j->y = 0;

    if (j->x > largeur - tailleCase) j->x = largeur - tailleCase;
    if (j->y > hauteur - tailleCase) j->y = hauteur - tailleCase;
}

void dessinerGrille(SDL_Renderer* rendu, int tailleCase)
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
                SDL_SetRenderDrawColor(rendu, 80, 80, 80, 255);
            }
            else
            {
                // mur
                SDL_SetRenderDrawColor(rendu, 30, 30, 30, 255);
            }

            SDL_RenderFillRect(rendu, &casePlateau);

            // contour
            SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
            SDL_RenderDrawRect(rendu, &casePlateau);
        }
    }
}

// nettoyer SDL
void nettoyer(SDL_Window* fenetre, SDL_Renderer* rendu)
{
    SDL_DestroyRenderer(rendu);
    SDL_DestroyWindow(fenetre);
    SDL_Quit();
}

// la boucle principale du jeu
void boucleJeu(SDL_Window* fenetre, SDL_Renderer* rendu)
{
    int tailleCase = 33;

    Joueur joueur = initialiserJoueur();

    int enCours = 1;
    int peutBouger = 1;

    SDL_Event event;

    while (enCours)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                enCours = 0;

            if (event.type == SDL_KEYUP)
                peutBouger = 1;
        }

        const Uint8* etat = SDL_GetKeyboardState(NULL);

        bougerJoueur(etat, &joueur, tailleCase, &peutBouger);
        appliquerLimites(&joueur, tailleCase,925,860 );

        SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
        SDL_RenderClear(rendu);

        dessinerGrille(rendu, tailleCase);

        SDL_Rect player = { joueur.x, joueur.y, tailleCase, tailleCase };

        SDL_SetRenderDrawColor(rendu, 255, 0, 0, 255);
        SDL_RenderFillRect(rendu, &player);

        SDL_RenderPresent(rendu);

        SDL_Delay(16);
    }
}

int estUnMur(int ligne, int colonne)
{
    return plateau[ligne][colonne] == 1;
}

int peutAller(int x, int y, int tailleCase)
{
    int colonne = x / tailleCase;
    int ligne = y / tailleCase;

    if (estUnMur(ligne, colonne))
    {
        return 0;
    }

    return 1;
}

void deplacerJoueur(Joueur* j, int nouvelleX, int nouvelleY)
{
    j->x = nouvelleX;
    j->y = nouvelleY;
}

//mouchette pour le déplacement du joueur

void bougerJoueur(const Uint8* etat, Joueur* j, int tailleCase, int* peutBouger)
{
    if (!*peutBouger)
        return;

    int nouvelleX = j->x;
    int nouvelleY = j->y;

    if (etat[SDL_SCANCODE_UP])
    {
        nouvelleY -= tailleCase;
        *peutBouger = 0;
    }

    if (etat[SDL_SCANCODE_DOWN])
    {
        nouvelleY += tailleCase;
        *peutBouger = 0;
    }

    if (etat[SDL_SCANCODE_LEFT])
    {
        nouvelleX -= tailleCase;
        *peutBouger = 0;
    }

    if (etat[SDL_SCANCODE_RIGHT])
    {
        nouvelleX += tailleCase;
        *peutBouger = 0;
    }

    if (peutAller(nouvelleX, nouvelleY, tailleCase))
    {
        deplacerJoueur(j, nouvelleX, nouvelleY);
    }
}