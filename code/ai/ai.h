#ifndef IA_H
#define IA_H

#include "../game/game.h"
#include "../game/cards.h"

#define MAX_CARTES 21

typedef struct Action
{
    int source;   
    int cible;    

    int suspect;
    int arme;
    int piece;

    int carteMontree;
    int aMontre;

    struct Action* suiv;
} Action;

typedef struct
{
    int cartesVues[MAX_CARTES]; 
    int suspectElimine[6];
    int armeElimine[6];
    int pieceElimine[9];

    Action* historique;
} MemoireIA;

void tourIASimple(Joueur *ia,
                  Joueur joueurs[],
                  int nbJoueurs,
                  Carte suspects[],
                  int nbSuspects,
                  Carte armes[],
                  int nbArmes,
                  int salleActuelle,
                  MemoireIA *memoire);

void tourIAHard(Joueur *ia,
                Joueur joueurs[],
                int nbJoueurs,
                Carte suspects[],
                int nbSuspects,
                Carte armes[],
                int nbArmes,
                int salleActuelle,
                MemoireIA *memoire);


Carte choisirAleatoire(Carte liste[], int taille);

int choisirJoueurCible(Joueur *ia, Joueur joueurs[], int nbJoueurs);

Carte* trouverCarte(Joueur *j, Carte a, Carte b, Carte c);

#endif