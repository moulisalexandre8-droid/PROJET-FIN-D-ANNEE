#include "turn_manager.h"

void changerTour(Joueur** actif,Joueur* j1,Joueur* j2,EtatJeu* etat)
{
    if(*actif == j1)
        *actif = j2;
    else
        *actif = j1;

    if((*actif)->elimine)
    {
        if(*actif == j1)
            *actif = j2;
        else
            *actif = j1;
    }

    *etat = ETAT_ATTENTE_DE;
}