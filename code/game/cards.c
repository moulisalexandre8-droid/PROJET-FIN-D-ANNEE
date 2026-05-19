#include "cards.h"

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

#include <stdlib.h>
#include <stdio.h>

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