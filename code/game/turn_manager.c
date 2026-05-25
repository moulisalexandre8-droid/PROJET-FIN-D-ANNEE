#include "turn_manager.h"

void changerTour(Joueur** actif,Joueur* j1,Joueur* j2,EtatJeu* etatJeu)
{
    (*actif)->aFaitSoupcon = 0;

    if (*actif == j1)
        *actif = j2;
    else
        *actif = j1;
    (*actif)->aFaitSoupcon = 0;
    *etatJeu = ETAT_ATTENTE_DE;
}