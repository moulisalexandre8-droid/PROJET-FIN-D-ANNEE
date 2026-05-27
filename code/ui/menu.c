#include "menu.h"
#include <stdio.h>

ModeJeu afficherMenu(SDL_Renderer* rendu)
{
    int choixMode;
    int nbJoueurs;

    printf("=== CLUELAU ===\n");
    printf("1 - Mode Classique\n");
    printf("2 - Mode Harry Potter\n");
    printf("Choix : ");
    scanf("%d",&choixMode);

    printf("\n");
    printf("Nombre de joueurs (2-6) : ");
    scanf("%d",&nbJoueurs);


    while(nbJoueurs <2 || nbJoueurs>6)
    {
        printf("Choisir entre 2 et 6 : ");
        scanf("%d",&nbJoueurs);
    }


    ModeJeu mode;

    if(choixMode==2)mode=creerModeHarryPotter();

    else
        mode=creerModeClassique();

    mode.nbJoueurs= nbJoueurs;

    return mode;
}