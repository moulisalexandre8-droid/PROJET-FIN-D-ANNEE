#include "game/game.h"

int main(int argc, char* argv[])
{
    SDL_Window* fenetre = NULL;
    SDL_Renderer* rendu = NULL;

    if (!initialiserSDL(&fenetre, &rendu))
        return 1;

    boucleJeu(fenetre, rendu);

    nettoyer(fenetre, rendu);

    return 0;
}