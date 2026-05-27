#include "turn_manager.h"

void changerTour(int* tour,Joueur* joueurs,int nbJoueurs,EtatJeu* etat)
{

    do
    {
        *tour =(*tour+1)%nbJoueurs;
    }

    while(joueurs[*tour].elimine);
        *etat= ETAT_ATTENTE_DE;
}

Joueur* prochainJoueur(Joueur* joueurs,int nbJoueurs,int tour)
{
    int i=tour+1;

    while(1)
    {
        if(i>=nbJoueurs)
            i=0;

        if(!joueurs[i].elimine)
            return &joueurs[i];

        i++;
    }
}