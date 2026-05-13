#include "game.h"
#include <stdio.h>

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
        1550,
        850,
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
    Joueur j = {100, 100};
    return j;
}

// mouvement du joueur
void bougerJoueur(const Uint8* etat, Joueur* j, int tailleCase, int* peutBouger)
{
    if (!*peutBouger)
        return;

    if (etat[SDL_SCANCODE_UP])    { j->y -= tailleCase; *peutBouger = 0; }
    else if (etat[SDL_SCANCODE_DOWN]) { j->y += tailleCase; *peutBouger = 0; }
    else if (etat[SDL_SCANCODE_LEFT]) { j->x -= tailleCase; *peutBouger = 0; }
    else if (etat[SDL_SCANCODE_RIGHT]) { j->x += tailleCase; *peutBouger = 0; }
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
    SDL_SetRenderDrawColor(rendu, 40, 40, 40, 255);

    for (int x = 0; x < 1550; x += tailleCase)
        SDL_RenderDrawLine(rendu, x, 0, x, 850);

    for (int y = 0; y < 850; y += tailleCase)
        SDL_RenderDrawLine(rendu, 0, y, 1550, y);
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
    int tailleCase = 50;

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
        appliquerLimites(&joueur, tailleCase, 1550, 850);

        // ================= RENDU UNIQUE =================
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