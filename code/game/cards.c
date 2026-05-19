#include "cards.h"
#include "../game/game.h"
#include <stdio.h>
#include <string.h>

Carte cartesSuspects[NB_SUSPECTS];
Carte cartesArmes[NB_ARMES];
Carte cartesPieces[NB_PIECES];

Solution solutionJeu;

void initialiserCartes()
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

void genererSolution()
{
    solutionJeu.suspect = cartesSuspects[rand()%NB_SUSPECTS];

    solutionJeu.arme = cartesArmes[rand()%NB_ARMES];

    solutionJeu.piece = cartesPieces[rand()%NB_PIECES];


    printf("\n=== SOLUTION ===\n");

    printf("Suspect : %s\n",solutionJeu.suspect.nom);
    printf("Arme : %s\n",solutionJeu.arme.nom);
    printf("Salle : %s\n",solutionJeu.piece.nom);
    
    printf("================\n");
}

void ajouterCarte(Joueur* j,Carte c)
{
    j->cartes[j->nbCartes]=c;
    j->nbCartes++;
}



void distribuerCartes(Joueur* j1,Joueur* j2)
{
    int tour=0;


    for(int i=0;i<NB_SUSPECTS;i++)
    {
        if(strcmp(cartesSuspects[i].nom,solutionJeu.suspect.nom)!=0)
        {
            if(tour%2==0)
                ajouterCarte(j1,cartesSuspects[i]);
            else
                ajouterCarte(j2,cartesSuspects[i]);
            tour++;
        }
    }



    for(int i=0;i<NB_ARMES;i++)
    {
        if(strcmp(cartesArmes[i].nom,solutionJeu.arme.nom)!=0)
        {
            if(tour%2==0)
                ajouterCarte(j1,cartesArmes[i]);

            else
                ajouterCarte(j2,cartesArmes[i]);

            tour++;
        }
    }



    for(int i=0;i<NB_PIECES;i++)
    {
        if(strcmp(cartesPieces[i].nom,solutionJeu.piece.nom)!=0)
        {
            if(tour%2==0)
                ajouterCarte(j1,cartesPieces[i]);

            else
                ajouterCarte(j2,cartesPieces[i]);

            tour++;
        }
    }


    printf("\nCartes J1\n");

    for(int i=0;i<j1->nbCartes;i++)
        printf("%s\n",j1->cartes[i].nom);



    printf("\nCartes J2\n");

    for(int i=0;i<j2->nbCartes;i++)
        printf("%s\n",j2->cartes[i].nom);
}

#include <stdio.h>

void faireSuspicion(Joueur* joueur,int salleActuelle)
{
    int suspectChoisi;
    int armeChoisie;

    printf("\n%s fait une suspicion\n",joueur->nom);


    printf("\nSuspects\n");

    for(int i=0;i<NB_SUSPECTS;i++)
    {
        printf("%d : %s\n",i,cartesSuspects[i].nom);
    }

    scanf("%d",&suspectChoisi);



    printf("\nArmes\n");

    for(int i=0;i<NB_ARMES;i++)
    {
        printf("%d : %s\n",i,cartesArmes[i].nom);
    }

    scanf("%d",&armeChoisie);
    printf("\nSuspicion : %s / %s / %s\n",cartesSuspects[suspectChoisi].nom,cartesArmes[armeChoisie].nom,cartesPieces[salleActuelle].nom);
}