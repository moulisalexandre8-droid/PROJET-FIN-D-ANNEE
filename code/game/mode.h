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

    int nbJoueurs;      // total : humains + IA
    int nbHumains;      // nombre de joueurs humains
    int nbIA;           // nombre d'IA
    int iaDifficulte;   // 0 = facile, 1 = experte

} ModeJeu;

ModeJeu creerModeClassique();

ModeJeu creerModeHarryPotter();

void libererMode(ModeJeu* mode);

#endif