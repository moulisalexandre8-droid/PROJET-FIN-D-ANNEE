#include "turn_manager.h"

void changerTour(Joueur** actif,Joueur* j1,Joueur* j2,EtatJeu* etatJeu)
{
    if (*actif == j1)
        *actif = j2;
    else
        *actif = j1;    
    *etatJeu = ETAT_ATTENTE_DE;
}