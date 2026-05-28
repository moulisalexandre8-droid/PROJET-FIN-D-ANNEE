#include "ai.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



Carte choisirAleatoire(Carte liste[], int taille)
{
    return liste[rand() % taille];
}

int choisirJoueurCible(Joueur *ia, Joueur joueurs[], int nbJoueurs)
{
    int index;

    do {
        index = rand() % nbJoueurs;
    } while (joueurs[index].personnage == ia->personnage);

    return index;
}

Carte* trouverCarte(Joueur *j, Carte a, Carte b, Carte c)
{
    for (int i = 0; i < j->nbCartes; i++)
    {
        if (j->cartes[i].id == a.id ||
            j->cartes[i].id == b.id ||
            j->cartes[i].id == c.id)
        {
            return &j->cartes[i];
        }
    }
    return NULL;
}

void tourIASimple(Joueur *ia,
                  Joueur joueurs[],
                  int nbJoueurs,
                  Carte suspects[],
                  int nbSuspects,
                  Carte armes[],
                  int nbArmes,
                  int salleActuelle,
                  MemoireIA *memoire)
{

    int cibleIndex;
    do {
        cibleIndex = rand() % nbJoueurs;
    } while (joueurs[cibleIndex].personnage == ia->personnage);

    Joueur *cible = &joueurs[cibleIndex];

    Carte suspect = suspects[rand() % nbSuspects];
    Carte arme = armes[rand() % nbArmes];

    Carte lieu = {0};
    strcpy(lieu.nom, "Salle");
    lieu.id = salleActuelle;
    lieu.type = 2;

    printf("%s accuse %s avec %s dans salle %d\n",
           ia->nom, suspect.nom, arme.nom, salleActuelle);

    Carte *c = trouverCarte(cible, suspect, arme, lieu);

    if (c)
    {
        printf("Carte montrée : %s\n", c->nom);
        memoire->cartesVues[c->id] = 1;
    }
}

void tourIAHard(Joueur *ia,
                Joueur joueurs[],
                int nbJoueurs,
                Carte suspects[],
                int nbSuspects,
                Carte armes[],
                int nbArmes,
                int salleActuelle,
                MemoireIA *memoire)
{
    printf("\n--- IA HARD : %s ---\n", ia->nom);

    int cibleIndex = -1;

    for (int i = 0; i < nbJoueurs; i++)
    {
        if (joueurs[i].personnage != ia->personnage)
        {
            cibleIndex = i;
            break;
        }
    }

    Joueur *cible = &joueurs[cibleIndex];

    int bluff = rand() % 100 < 30; //30% d utiliser la strategie

    Carte suspect, arme;

    if (bluff)
    {
        suspect = ia->cartes[rand() % ia->nbCartes];
        arme = ia->cartes[rand() % ia->nbCartes];
    }
    else
    {
        suspect = suspects[rand() % nbSuspects];
        arme = armes[rand() % nbArmes];
    }

    Carte lieu = {0};
    strcpy(lieu.nom, "Salle");
    lieu.id = salleActuelle;
    lieu.type = 2;

    printf("%s (HARD) accuse %s avec %s\n",
           ia->nom, suspect.nom, arme.nom);

    Carte *c = trouverCarte(cible, suspect, arme, lieu);

    if (c)
    {
        printf("Révélation : %s\n", c->nom);
        memoire->cartesVues[c->id] = 1;

        // stockage historique
        Action *a = malloc(sizeof(Action));
        a->source = ia->personnage;
        a->cible = cible->personnage;
        a->carteMontree = c->id;
        a->suiv = memoire->historique;
        memoire->historique = a;
    }
}