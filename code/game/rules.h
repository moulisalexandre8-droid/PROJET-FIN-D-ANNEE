#ifndef RULES_H
#define RULES_H
#include "game.h"

typedef enum
{
    ETAT_ATTENTE_DE,
    ETAT_DEPLACEMENT,
    ETAT_SUSPICION

} EtatJeu;

typedef enum
{
    UI_PRINCIPALE,

    UI_SUSPICION,

    UI_ACCUSATION

} EtatInterface;

void faireSuspicion(Joueur* joueur,Joueur* autre,int salle);

void verifierSuspicion(Joueur* accuse,int suspect,int arme,int salle);

int verifierAccusation(int suspect,int arme,int salle);

#endif