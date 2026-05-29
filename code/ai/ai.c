#include "ai.h"
#include "../entities/room.h"
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

void tourIAHard(Joueur *ia, Joueur joueurs[], int nbJoueurs, Carte suspects[], int nbSuspects, Carte armes[], int nbArmes, int salleActuelle, MemoireIA *memoire)
{
    printf("\n--- Reflexion IA HARD : %s ---\n", ia->nom);

    // 1. L'IA note ses propres cartes dans son carnet (si ce n'est pas déjà fait)
    for(int i = 0; i < ia->nbCartes; i++) {
        if(ia->cartes[i].type == CARTE_SUSPECT) ia->notesSuspects[ia->cartes[i].id] = 1;
        if(ia->cartes[i].type == CARTE_ARME) ia->notesArmes[ia->cartes[i].id] = 1;
        if(ia->cartes[i].type == CARTE_PIECE) ia->notesPieces[ia->cartes[i].id] = 1;
    }
    
    // Elle note la salle actuelle puisqu'elle y est
    ia->notesPieces[salleActuelle - 2] = 1;

    // 2. Faire l'inventaire des cartes Inconnues
    int suspectsInconnus[6]; int nbS = 0;
    int armesInconnues[6]; int nbA = 0;
    int piecesInconnues[9]; int nbP = 0;
    
    for(int i = 0; i < nbSuspects; i++) if(ia->notesSuspects[i] == 0) suspectsInconnus[nbS++] = i;
    for(int i = 0; i < nbArmes; i++) if(ia->notesArmes[i] == 0) armesInconnues[nbA++] = i;
    for(int i = 0; i < 9; i++) if(ia->notesPieces[i] == 0) piecesInconnues[nbP++] = i;

    // 3. VÉRIFICATION D'ACCUSATION (Elle a trouvé la solution !)
    if (nbS == 1 && nbA == 1 && nbP == 1) {
        printf("\n!!! EURÊKA ! %s A TROUVE LA SOLUTION !!!\n", ia->nom);
        printf("C'est %s, avec %s, dans %s.\n", suspects[suspectsInconnus[0]].nom, armes[armesInconnues[0]].nom, pieces[piecesInconnues[0]].nom);
        return; // Le jeu s'arrête ici pour l'IA (Tu pourras relier ça à UI_GAMEOVER plus tard)
    }

    // 4. PRÉPARER LE SOUPÇON (Mix entre Déduction et Bluff)
    int suspectChoisi = suspectsInconnus[rand() % nbS];
    int armeChoisie = armesInconnues[rand() % nbA];

    // BLUFF : 30% de chance de demander une carte qu'on possède pour forcer les autres
    // à montrer la deuxième carte qui nous intéresse vraiment !
    if ((rand() % 100) < 30 && ia->nbCartes > 0) {
        int indexBluff = rand() % ia->nbCartes;
        if (ia->cartes[indexBluff].type == CARTE_SUSPECT) suspectChoisi = ia->cartes[indexBluff].id;
        else if (ia->cartes[indexBluff].type == CARTE_ARME) armeChoisie = ia->cartes[indexBluff].id;
    }

    Carte suspectDemande = suspects[suspectChoisi];
    Carte armeDemandee = armes[armeChoisie];
    Carte lieuDemande = {CARTE_PIECE, salleActuelle, ""};
    
    for(int i = 0; i < 9; i++) {
        if(pieces[i].id == salleActuelle) strcpy(lieuDemande.nom, pieces[i].nom);
    }

    printf("=> %s soupconne %s avec %s dans %s\n", ia->nom, suspectDemande.nom, armeDemandee.nom, lieuDemande.nom);

    // 5. INTERROGER LES AUTRES JOUEURS (Véritable tour de table)
    int iaIndex = 0;
    for(int i = 0; i < nbJoueurs; i++) {
        if(&joueurs[i] == ia) iaIndex = i;
    }

    int indexQuestionne = (iaIndex + 1) % nbJoueurs;
    int carteTrouvee = 0;

    while (indexQuestionne != iaIndex) {
        if (!joueurs[indexQuestionne].elimine) {
            Carte* carteMontree = trouverCarte(&joueurs[indexQuestionne], suspectDemande, armeDemandee, lieuDemande);
            if (carteMontree != NULL) {
                printf("-> %s montre discretement une carte a %s.\n", joueurs[indexQuestionne].nom, ia->nom);
                
                // L'IA NOTE LA CARTE DANS SON CARNET !
                if (carteMontree->type == CARTE_SUSPECT) ia->notesSuspects[carteMontree->id] = 1;
                if (carteMontree->type == CARTE_ARME) ia->notesArmes[carteMontree->id] = 1;
                if (carteMontree->type == CARTE_PIECE) ia->notesPieces[carteMontree->id] = 1;
                
                carteTrouvee = 1;
                break;
            } else {
                printf("-> %s ne peut pas contredire la rumeur.\n", joueurs[indexQuestionne].nom);
            }
        }
        indexQuestionne = (indexQuestionne + 1) % nbJoueurs;
    }

    if (!carteTrouvee) {
        printf("-> PERSONNE n'a pu contredire %s !\n", ia->nom);
        // Si elle n'a pas bluffé, elle sait que ce sont les bonnes cartes !
    }
}