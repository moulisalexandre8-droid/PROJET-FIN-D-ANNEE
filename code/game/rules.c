#include "rules.h"
#include "cards.h"
#include <stdio.h>

void faireSuspicion(Joueur* joueur,Joueur* autre,int salle)
{
    int suspectChoisi;
    int armeChoisie;

    printf("\n%s fait une suspicion\n",joueur->nom);

    printf("\nSuspects :\n");

    for(int i=0;i<NB_SUSPECTS;i++)
    {
        printf("%d : %s\n",i,cartesSuspects[i].nom);
    }

    scanf("%d",&suspectChoisi);


    printf("\nArmes :\n");

    for(int i=0;i<NB_ARMES;i++)
    {
        printf("%d : %s\n",i,cartesArmes[i].nom);
    }

    scanf("%d",&armeChoisie);


    printf("\nSuspicion : %s / %s / %s\n",cartesSuspects[suspectChoisi].nom,cartesArmes[armeChoisie].nom,cartesPieces[salle].nom);

    verifierSuspicion(autre,suspectChoisi,armeChoisie,salle);
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


    int choix;

    printf("\nChoisir carte à montrer : ");
    scanf("%d",&choix);


    Carte montre =accuse->cartes[choixPossibles[choix]];


    printf("\n%s montre une carte.\n",accuse->nom);
}

void faireAccusation(Joueur* joueur)
{
    int suspect;
    int arme;
    int salle;

    printf("\n%s fait une ACCUSATION\n",joueur->nom);


    printf("\nChoisir suspect : ");
    scanf("%d",&suspect);

    printf("\nChoisir arme : ");
    scanf("%d",&arme);

    printf("\nChoisir salle : ");
    scanf("%d",&salle);


    if(suspect == solution.suspect.id && arme == solution.arme.id && salle == solution.piece.id)
    {
        printf("\n%s gagne la partie !\n",joueur->nom);
    }
    else
    {
        printf("\nMauvaise accusation.\n");

        printf("%s est éliminé.\n",joueur->nom);
    }
}