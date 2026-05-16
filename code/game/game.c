#include "game.h"
#include "../ui/renderer.h"
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
    if (TTF_Init() == -1)
    {
        printf("Erreur SDL_ttf : %s\n", TTF_GetError());
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

// initialisation des joueurs
Joueur initialiserJoueur(int x, int y, SDL_Color couleur)
{
    Joueur j;

    j.x = x;
    j.y = y;
    j.couleur = couleur;

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

// nettoyer SDL
void nettoyer(SDL_Window* fenetre, SDL_Renderer* rendu)
{
    SDL_DestroyRenderer(rendu);
    SDL_DestroyWindow(fenetre);
    TTF_Quit();
    SDL_Quit();
}

// la boucle principale du jeu
void boucleJeu(SDL_Window* fenetre, SDL_Renderer* rendu)
{
    int tailleCase = 33;
    // Initialisation des joueurs
    SDL_Color rouge = {255, 0, 0, 255};
    SDL_Color bleu = {0, 0, 255, 255};

    Joueur joueur1 = initialiserJoueur(231, 0, rouge);
    Joueur joueur2 = initialiserJoueur(0, 231, bleu);

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

        bougerJoueur(etat, &joueur1, tailleCase, &peutBouger);
        appliquerLimites(&joueur1, tailleCase,925,860 );

        SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
        SDL_RenderClear(rendu);

        dessinerGrille(rendu,plateau, tailleCase);

        dessinerJoueur(
            rendu,
            joueur1.x,
            joueur1.y,
            tailleCase,
            joueur1.couleur
        );

        dessinerJoueur(
            rendu,
            joueur2.x,
            joueur2.y,
            tailleCase,
            joueur2.couleur
        );

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