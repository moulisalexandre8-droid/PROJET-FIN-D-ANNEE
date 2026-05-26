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
    cartesSuspects[0]=(Carte){CARTE_SUSPECT,0,"Rose"};
    cartesSuspects[1]=(Carte){CARTE_SUSPECT,1,"Moutarde"};
    cartesSuspects[2]=(Carte){CARTE_SUSPECT,2,"Olive"};
    cartesSuspects[3]=(Carte){CARTE_SUSPECT,3,"Violet"};
    cartesSuspects[4]=(Carte){CARTE_SUSPECT,4,"Leblanc"};
    cartesSuspects[5]=(Carte){CARTE_SUSPECT,5,"Pervenche"};


    cartesArmes[0]=(Carte){CARTE_ARME,0,"Poignard"};
    cartesArmes[1]=(Carte){CARTE_ARME,1,"Chandelier"};
    cartesArmes[2]=(Carte){CARTE_ARME,2,"Revolver"};
    cartesArmes[3]=(Carte){CARTE_ARME,3,"Corde"};
    cartesArmes[4]=(Carte){CARTE_ARME,4,"Cle anglaise"};
    cartesArmes[5]=(Carte){CARTE_ARME,5,"Matraque"};


    cartesPieces[0]=(Carte){CARTE_PIECE,0,"Cuisine"};
    cartesPieces[1]=(Carte){CARTE_PIECE,1,"Salon"};
    cartesPieces[2]=(Carte){CARTE_PIECE,2,"Bibliotheque"};
    cartesPieces[3]=(Carte){CARTE_PIECE,3,"Salle a manger"};
    cartesPieces[4]=(Carte){CARTE_PIECE,4,"Hall"};
    cartesPieces[5]=(Carte){CARTE_PIECE,5,"Veranda"};
    cartesPieces[6]=(Carte){CARTE_PIECE,6,"Studio"};
    cartesPieces[7]=(Carte){CARTE_PIECE,7,"Bureau"};
    cartesPieces[8]=(Carte){CARTE_PIECE,8,"Billard"};
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



void distribuerCartes(Joueur* j1,Joueur* j2,ModeJeu* mode)
{
    int tour=0;


    for(int i=0;i<mode->nbSuspects;i++)
    {
        if(strcmp(mode->suspects[i].nom,solution.suspect.nom)!=0)
        {
            if(tour%2==0)
                ajouterCarte(j1,mode->suspects[i]);
            else
                ajouterCarte(j2,mode->suspects[i]);
            tour++;
        }
    }



    for(int i=0;i<mode->nbArmes;i++)
    {
        if(strcmp(mode->armes[i].nom,solution.arme.nom)!=0)
        {
            if(tour%2==0)
                ajouterCarte(j1,mode->armes[i]);

            else
                ajouterCarte(j2,mode->armes[i]);

            tour++;
        }
    }



    for(int i=0;i<mode->nbPieces;i++)
    {
        if(strcmp(mode->pieces[i].nom,solution.piece.nom)!=0)
        {
            if(tour%2==0)
                ajouterCarte(j1,mode->pieces[i]);

            else
                ajouterCarte(j2,mode->pieces[i]);

            tour++;
        }
    }
}
