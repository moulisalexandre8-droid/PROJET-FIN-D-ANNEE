#ifndef MODE_H
#define MODE_H

#include "../entities/card.h"

typedef struct
{
    char nom[50];
    char nomDossier[50];

    Carte* suspects;
    int nbSuspects;

    Carte* armes;
    int nbArmes;

    Carte* pieces;
    int nbPieces;

} ModeJeu;

ModeJeu creerModeClassique();

ModeJeu creerModeHarryPotter();

void libererMode(ModeJeu* mode);

#endif