#ifndef IA_EASY_H
#define IA_EASY_H

#define MAX_CARTES 21

typedef struct {
    char nom[50];
    int type; // 0 = suspect, 1 = arme, 2 = lieu
    int id;
} Carte;

typedef struct {
    char nom[50];
    Carte main[10];
    int nbCartes;
    int id;
} Joueur;

typedef struct {
    int cartesVues[MAX_CARTES]; // 0 = inconnu, 1 = vu
} MemoireIA;

void initMemoireIA(MemoireIA *memoire);

void tourIA(Joueur *ia, Joueur joueurs[], int nbJoueurs,
            Carte suspects[], int nbSuspects,
            Carte armes[], int nbArmes,
            Carte lieux[], int nbLieux,
            int salleActuelle,
            MemoireIA *memoire);

Carte* trouver_carte(Joueur j, Carte suspect, Carte arme, Carte lieu);

#endif