#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Erreur SDL : %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* fenetre = SDL_CreateWindow(
        "Cluelau",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1550,
        850,
        0
    );

    SDL_Renderer* rendu = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);

    if (!rendu)
    {
        printf("Erreur renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(fenetre);
        SDL_Quit();
        return 1;
    }

    // Taille d'une case de la grille
    int tailleCase = 50;

    // Position du joueur
    int joueurX = 100;
    int joueurY = 100;

    int enCours = 1;
    SDL_Event evenement;

    int peutBouger = 1;

    while (enCours)
    {
        // Gestion des événements (fermeture fenêtre)
        while (SDL_PollEvent(&evenement))
        {
            if (evenement.type == SDL_QUIT)
            {
                enCours = 0;
            }

            // Réactivation du mouvement quand on relâche une touche
            if (evenement.type == SDL_KEYUP)
            {
                peutBouger = 1;
            }
        }

        // Lecture clavier en continu
        const Uint8* etatClavier = SDL_GetKeyboardState(NULL);

        if (peutBouger)
        {
            if (etatClavier[SDL_SCANCODE_UP])    { joueurY -= tailleCase; peutBouger = 0; }
            if (etatClavier[SDL_SCANCODE_DOWN])  { joueurY += tailleCase; peutBouger = 0; }
            if (etatClavier[SDL_SCANCODE_LEFT])  { joueurX -= tailleCase; peutBouger = 0; }
            if (etatClavier[SDL_SCANCODE_RIGHT]) { joueurX += tailleCase; peutBouger = 0; }
        }

        // Limites de la fenêtre
        if (joueurX < 0) joueurX = 0;
        if (joueurY < 0) joueurY = 0;

        if (joueurX > 1550 - tailleCase) joueurX = 1550 - tailleCase;
        if (joueurY > 850 - tailleCase) joueurY = 850 - tailleCase;

        // Fond noir
        SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
        SDL_RenderClear(rendu);

        // Dessin de la grille
        SDL_SetRenderDrawColor(rendu, 40, 40, 40, 255);

        for (int x = 0; x < 1550; x += tailleCase)
        {
            SDL_RenderDrawLine(rendu, x, 0, x, 850);
        }

        for (int y = 0; y < 850; y += tailleCase)
        {
            SDL_RenderDrawLine(rendu, 0, y, 1550, y);
        }

        // Dessin du joueur
        SDL_Rect joueur = {joueurX, joueurY, tailleCase, tailleCase};

        SDL_SetRenderDrawColor(rendu, 255, 0, 0, 255);
        SDL_RenderFillRect(rendu, &joueur);

        SDL_RenderPresent(rendu);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(rendu);
    SDL_DestroyWindow(fenetre);
    SDL_Quit();

    return 0;
}