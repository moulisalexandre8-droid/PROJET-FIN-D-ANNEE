#include "cards.h"
#include "../game/game.h"
#include "mode.h"
#include <stdio.h>
#include <string.h>

Carte cartesSuspects[NB_SUSPECTS];
Carte cartesArmes[NB_ARMES];
Carte cartesPieces[NB_PIECES];

Solution solution;

void initialiserCartes(ModeJeu* mode)
{
    for(int i = 0; i < mode->nbSuspects; i++)
    {
        cartesSuspects[i] = mode->suspects[i];
    }

    for(int i = 0; i < mode->nbArmes; i++)
    {
        cartesArmes[i] = mode->armes[i];
    }

    for(int i = 0; i < mode->nbPieces; i++)
    {
        cartesPieces[i] = mode->pieces[i];
    }
}

void genererSolution(ModeJeu* mode)
{
    solution.suspect = mode->suspects[rand() % mode->nbSuspects];

    solution.arme = mode->armes[rand() % mode->nbArmes];

    solution.piece = mode->pieces[rand() % mode->nbPieces];
}

void ajouterCarte(Joueur* j,Carte c)
{
    j->cartes[j->nbCartes]=c;
    j->nbCartes++;
}



void distribuerCartes(Joueur* joueurs,int nbJoueurs,ModeJeu* mode)
{
    Carte paquet[50];
    int nb = 0;

    for(int i=0;i<mode->nbSuspects;i++)
        paquet[nb++] = mode->suspects[i];

    for(int i=0;i<mode->nbArmes;i++)
        paquet[nb++] = mode->armes[i];

    for(int i=0;i<mode->nbPieces;i++)
        paquet[nb++] = mode->pieces[i];

    melanger(paquet,nb);
    int joueurActuel=0;

    for(int i=0;i<nb;i++)
    {
        joueurs[joueurActuel].cartes[joueurs[joueurActuel].nbCartes++] = paquet[i];
        joueurActuel++;

        if(joueurActuel>=nbJoueurs)
            joueurActuel=0;
    }
}

void melanger(Carte* paquet, int nb)
{
    if(nb <= 1) return;

    for(int i = nb - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);

        Carte tmp = paquet[i];
        paquet[i] = paquet[j];
        paquet[j] = tmp;
    }
}
