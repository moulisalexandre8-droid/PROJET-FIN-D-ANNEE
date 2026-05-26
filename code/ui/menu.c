#include "menu.h"
#include <stdio.h>

ModeJeu afficherMenu(SDL_Renderer* rendu)
{
    int choix;

    printf("=== CLUELAU ===\n");
    printf("1. Mode Classique\n");
    printf("2. Mode Harry Potter\n");
    printf("Choix : ");

    scanf("%d",&choix);

    if(choix == 2)
        return creerModeHarryPotter();

    return creerModeClassique();
}