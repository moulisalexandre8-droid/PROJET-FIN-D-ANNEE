#include "game.h"
#include "../ui/renderer.h"
#include <stdio.h>
#include <string.h>

// ----------------------
// PLATEAU
// ----------------------

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

// ----------------------
// SDL INIT
// ----------------------

int initialiserSDL(SDL_Window** fenetre, SDL_Renderer** rendu)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Erreur SDL : %s\n", SDL_GetError());
        return 0;
    }

    if (TTF_Init() == -1)
    {
        printf("Erreur TTF : %s\n", TTF_GetError());
        return 0;
    }

    *fenetre = SDL_CreateWindow("Cluelau",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        925, 860, 0);

    *rendu = SDL_CreateRenderer(*fenetre, -1, SDL_RENDERER_ACCELERATED);

    return (*fenetre && *rendu);
}

// ----------------------
// JOUEUR
// ----------------------

Joueur initialiserJoueur(int x, int y, SDL_Color couleur, const char* nom)
{
    Joueur j;

    j.x = x;
    j.y = y;
    j.couleur = couleur;

    strcpy(j.nom, nom);
    j.mouvementsRestants = 6;

    return j;
}

// ----------------------
// COLLISIONS
// ----------------------

int estUnMur(int ligne, int colonne)
{
    return plateau[ligne][colonne] == 1;
}

int peutAller(int x, int y, int tailleCase)
{
    int col = x / tailleCase;
    int lig = y / tailleCase;

    if (lig < 0 || lig >= 26 || col < 0 || col >= 28)
        return 0;

    return !estUnMur(lig, col);
}

// ----------------------
// DEPLACEMENT (FIX IMPORTANT)
// ----------------------

void bougerJoueur(const Uint8* etat, Joueur* j, int tailleCase)
{
    if (j->mouvementsRestants <= 0)
        return;

    static int lock = 0;

    if (lock)
    {
        if (!etat[SDL_SCANCODE_UP] &&
            !etat[SDL_SCANCODE_DOWN] &&
            !etat[SDL_SCANCODE_LEFT] &&
            !etat[SDL_SCANCODE_RIGHT])
        {
            lock = 0;
        }
        return;
    }
        int dx = 0, dy = 0;

    if (etat[SDL_SCANCODE_UP]) dy = -tailleCase;
    else if (etat[SDL_SCANCODE_DOWN]) dy = tailleCase;
    else if (etat[SDL_SCANCODE_LEFT]) dx = -tailleCase;
    else if (etat[SDL_SCANCODE_RIGHT]) dx = tailleCase;

    if (dx == 0 && dy == 0)
        return;
        int nx = j->x + dx;
    int ny = j->y + dy;

    if (peutAller(nx, ny, tailleCase))
    {
        j->x = nx;
        j->y = ny;
        j->mouvementsRestants--;

        lock = 1;
    }
}

void deplacerJoueur(Joueur* j, int nx, int ny)
{
    j->x = nx;
    j->y = ny;
}

// ----------------------
// LIMITES
// ----------------------

void appliquerLimites(Joueur* j, int tailleCase, int w, int h)
{
    if (j->x < 0) j->x = 0;
    if (j->y < 0) j->y = 0;

    if (j->x > w - tailleCase) j->x = w - tailleCase;
    if (j->y > h - tailleCase) j->y = h - tailleCase;
}

// ----------------------
// CLEAN
// ----------------------

void nettoyer(SDL_Window* f, SDL_Renderer* r)
{
    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(f);
    TTF_Quit();
    SDL_Quit();
}

// ----------------------
// LOOP
// ----------------------

void boucleJeu(SDL_Window* fenetre, SDL_Renderer* rendu)
{
    int tailleCase = 33;

    SDL_Color rouge = {255,0,0,255};
    SDL_Color bleu = {0,0,255,255};

    Joueur j1 = initialiserJoueur(231,0,rouge,"J1");
    Joueur j2 = initialiserJoueur(0,231,bleu,"J2");

    Joueur* actif = &j1;

    SDL_Event e;
    int run = 1;

    while (run)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                run = 0;

            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
            {
                actif = (actif == &j1) ? &j2 : &j1;
                actif->mouvementsRestants = 6;
            }
        }

        const Uint8* etat = SDL_GetKeyboardState(NULL);

        bougerJoueur(etat, actif, tailleCase);
        if (actif->mouvementsRestants <= 0)
        {
            if (actif == &j1)
                actif = &j2;
            else
                actif = &j1;
        
            actif->mouvementsRestants = 6;
        }
        appliquerLimites(actif, tailleCase, 925, 860);

        SDL_SetRenderDrawColor(rendu,0,0,0,255);
        SDL_RenderClear(rendu);

        dessinerGrille(rendu, plateau, tailleCase);

        dessinerJoueur(rendu, j1.x, j1.y, tailleCase, j1.couleur);
        dessinerJoueur(rendu, j2.x, j2.y, tailleCase, j2.couleur);

        SDL_RenderPresent(rendu);
        SDL_Delay(16);
    }
}