#include "rules.h"
#include "cards.h"
#include "../players/player.h"
#include "../entities/room.h"
#include <stdio.h>

Carte faireSuspicion(Joueur* accusateur,Joueur* autre,int salle,int suspect,int arme)
{
    // téléporte le personnage soupçonné
    Joueur* cible =obtenirJoueurParSuspect(suspect,accusateur,autre);

    if(cible != NULL)
    {
        teleporterDansSalle(cible,salle,32.5,31);
    }

    // cherche une carte à montrer
    for(int i=0;i<autre->nbCartes;i++)
    {
        Carte c = autre->cartes[i];

        if((c.type==CARTE_SUSPECT && strcmp(c.nom,cartesSuspects[suspect].nom)==0) || (c.type==CARTE_ARME && strcmp(c.nom,cartesArmes[arme].nom)==0) || (c.type==CARTE_PIECE &&strcmp(c.nom,cartesPieces[salle].nom)==0))
        {
            return c;
        }
    }

    Carte vide;
    strcpy(vide.nom,"Aucune");
    return vide;
}

void verifierSuspicion(Joueur* accuse,int suspect,int arme,int salle)
{
    int choixPossibles[3];
    int nbChoix = 0;

    printf("\n%s vérifie ses cartes...\n",accuse->nom);


    for(int i=0;i<accuse->nbCartes;i++)
    {
        Carte c = accuse->cartes[i];

        if(c.type == CARTE_SUSPECT &&
           c.id == suspect)
        {
            choixPossibles[nbChoix++] = i;
        }


        if(c.type == CARTE_ARME &&
           c.id == arme)
        {
            choixPossibles[nbChoix++] = i;
        }


        if(c.type == CARTE_PIECE &&
           c.id == salle)
        {
            choixPossibles[nbChoix++] = i;
        }
    }


    if(nbChoix == 0)
    {
        printf("%s ne peut montrer aucune carte\n",accuse->nom);
        return;
    }


    printf("\nCartes possibles :\n");


    for(int i=0;i<nbChoix;i++)
    {
        Carte c = accuse->cartes[choixPossibles[i]];

        if(c.type == CARTE_SUSPECT)
            printf("%d : %s\n",i,cartesSuspects[c.id].nom);

        if(c.type == CARTE_ARME)
            printf("%d : %s\n",i,cartesArmes[c.id].nom);

        if(c.type == CARTE_PIECE)
            printf("%d : %s\n",i,cartesPieces[c.id].nom);
    }

    Carte montre = accuse->cartes[choixPossibles[0]];

    printf("\n%s montre une carte.\n",accuse->nom);
}

int verifierAccusation(int suspect,int arme,int salle)
{
    return (

        suspect == solution.suspect.id &&

        arme == solution.arme.id &&

        salle == solution.piece.id
    );
}