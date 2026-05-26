#include "game/game.h"
#include "game/mode.h"
#include "ui/menu.h"

int main(int argc, char* argv[])
{
    SDL_Window* fenetre = NULL;
    SDL_Renderer* rendu = NULL;

    if (!initialiserSDL(&fenetre, &rendu))
        return 1;

    ModeJeu mode = afficherMenu(rendu);

    boucleJeu(fenetre,rendu,mode);

    nettoyer(fenetre, rendu);

    return 0;
}
// ne pas ublier de bien se placer dans le repère avec un cd ( cd /c/Users/teort/OneDrive/Documents/Junia/Informatique/PROJET-FIN-D-ANNEE)
// La commande est make pour compiler le projet
// et ensuite ./cluelau.exe pour lancer le jeu