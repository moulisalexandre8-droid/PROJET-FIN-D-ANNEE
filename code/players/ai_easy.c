#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ia_easy.h"

void initMemoireIA(MemoireIA *memoire) {
    for (int i = 0; i < MAX_CARTES; i++) {
        memoire->cartesVues[i] = 0;
    }
}

Carte choisirAleatoire(Carte liste[], int taille) {
    return liste[rand() % taille];
}

Carte* trouver_carte(Joueur j, Carte suspect, Carte arme, Carte lieu) {
    for (int i = 0; i < j.nbCartes; i++) {
        if (j.main[i].id == suspect.id ||
            j.main[i].id == arme.id ||
            j.main[i].id == lieu.id) {
            return &j.main[i];
        }
    }
    return NULL;
}

int choisirJoueurCible(Joueur *ia, Joueur joueurs[], int nbJoueurs) {
    int index;
    do {
        index = rand() % nbJoueurs;
    } while (joueurs[index].id == ia->id);

    return index;
}

void tourIA(Joueur *ia, Joueur joueurs[], int nbJoueurs,
            Carte suspects[], int nbSuspects,
            Carte armes[], int nbArmes,
            Carte lieux[], int nbLieux,
            int salleActuelle,
            MemoireIA *memoire) {

    printf("\n--- Tour de %s (IA) ---\n", ia->nom);

    int cibleIndex = choisirJoueurCible(ia, joueurs, nbJoueurs);
    Joueur *cible = &joueurs[cibleIndex];

    Carte suspect = choisirAleatoire(suspects, nbSuspects);
    Carte arme = choisirAleatoire(armes, nbArmes);
    Carte lieu = lieux[salleActuelle]; // salle actuelle imposée

    printf("%s accuse à %s : %s avec %s dans %s\n",
           ia->nom,
           cible->nom,
           suspect.nom,
           arme.nom,
           lieu.nom);

    Carte *carteMontree = trouver_carte(*cible, suspect, arme, lieu);

    if (carteMontree != NULL) {
        printf("%s montre une carte à %s\n", cible->nom, ia->nom);

        memoire->cartesVues[carteMontree->id] = 1;
    } else {
        printf("%s ne peut rien montrer\n", cible->nom);
    }
}