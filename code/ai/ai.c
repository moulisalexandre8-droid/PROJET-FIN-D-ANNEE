#include "ai.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Carte choisirAleatoire(Carte liste[], int taille)
{
    if (taille <= 0)
    {
        Carte vide = {0};
        strcpy(vide.nom, "Aucune");
        return vide;
    }

    return liste[rand() % taille];
}

int choisirJoueurCible(Joueur *ia, Joueur joueurs[], int nbJoueurs)
{
    if (ia == NULL || joueurs == NULL || nbJoueurs <= 1)
        return -1;

    int index;
    int securite = 0;

    do {
        index = rand() % nbJoueurs;
        securite++;
    } while (joueurs[index].personnage == ia->personnage && securite < 100);

    if (joueurs[index].personnage == ia->personnage)
        return -1;

    return index;
}

Carte* trouverCarte(Joueur *j, Carte a, Carte b, Carte c)
{
    if (j == NULL)
        return NULL;

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
    if (ia == NULL || joueurs == NULL || memoire == NULL)
        return;

    if (nbJoueurs <= 1 || nbSuspects <= 0 || nbArmes <= 0)
        return;

    int cibleIndex = choisirJoueurCible(ia, joueurs, nbJoueurs);

    if (cibleIndex == -1)
    {
        printf("Erreur IA : aucune cible disponible.\n");
        return;
    }

    Joueur *cible = &joueurs[cibleIndex];

    Carte suspect = suspects[rand() % nbSuspects];
    Carte arme = armes[rand() % nbArmes];

    Carte lieu = {0};
    strcpy(lieu.nom, "Salle");
    lieu.id = salleActuelle;
    lieu.type = CARTE_PIECE;

    printf("%s soupçonne %s avec %s dans salle %d\n",
           ia->nom, suspect.nom, arme.nom, salleActuelle);

    Carte *c = trouverCarte(cible, suspect, arme, lieu);

    if (c != NULL)
    {
        printf("Carte montrée : %s\n", c->nom);

        if (c->id >= 0 && c->id < MAX_CARTES)
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
    if (ia == NULL || joueurs == NULL || memoire == NULL)
        return;

    if (nbJoueurs <= 1 || nbSuspects <= 0 || nbArmes <= 0)
        return;

    printf("\n--- IA HARD : %s ---\n", ia->nom);

    int cibleIndex = choisirJoueurCible(ia, joueurs, nbJoueurs);

    if (cibleIndex == -1)
    {
        printf("Erreur IA HARD : aucune cible trouvée.\n");
        return;
    }

    Joueur *cible = &joueurs[cibleIndex];

    int bluff = rand() % 100 < 30;

    Carte suspect;
    Carte arme;

    if (bluff && ia->nbCartes > 0)
    {
        suspect = suspects[rand() % nbSuspects];
        arme = armes[rand() % nbArmes];
    }
    else
    {
        suspect = suspects[rand() % nbSuspects];
        arme = armes[rand() % nbArmes];
    }

    Carte lieu = {0};
    strcpy(lieu.nom, "Salle");
    lieu.id = salleActuelle;
    lieu.type = CARTE_PIECE;

    printf("%s HARD soupçonne %s avec %s\n",
           ia->nom, suspect.nom, arme.nom);

    Carte *c = trouverCarte(cible, suspect, arme, lieu);

    if (c != NULL)
    {
        printf("Révélation : %s\n", c->nom);

        if (c->id >= 0 && c->id < MAX_CARTES)
            memoire->cartesVues[c->id] = 1;

        Action *a = malloc(sizeof(Action));

        if (a != NULL)
        {
            a->source = ia->personnage;
            a->cible = cible->personnage;
            a->suspect = suspect.id;
            a->arme = arme.id;
            a->piece = salleActuelle;
            a->carteMontree = c->id;
            a->aMontre = 1;

            a->suiv = memoire->historique;
            memoire->historique = a;
        }
    }
}