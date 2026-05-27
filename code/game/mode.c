#include "mode.h"
#include "cards.h"
#include <stdlib.h>
#include <string.h>


ModeJeu creerModeClassique()
{
    ModeJeu mode;

    strcpy(mode.nom, "Classique");
    strcpy(mode.nomDossier, "classique");

    mode.nbSuspects = 6;
    mode.suspects = malloc(sizeof(Carte) * mode.nbSuspects);

    mode.suspects[0] = (Carte){CARTE_SUSPECT,0,"Rose"};
    mode.suspects[1] = (Carte){CARTE_SUSPECT,1,"Moutarde"};
    mode.suspects[2] = (Carte){CARTE_SUSPECT,2,"Olive"};
    mode.suspects[3] = (Carte){CARTE_SUSPECT,3,"Violet"};
    mode.suspects[4] = (Carte){CARTE_SUSPECT,4,"Leblanc"};
    mode.suspects[5] = (Carte){CARTE_SUSPECT,5,"Pervenche"};

    mode.nbArmes = 6;
    mode.armes = malloc(sizeof(Carte) * mode.nbArmes);
    mode.armes[0] = (Carte){CARTE_ARME,0,"Poignard"};
    mode.armes[1] = (Carte){CARTE_ARME,1,"Chandelier"};
    mode.armes[2] = (Carte){CARTE_ARME,2,"Revolver"};
    mode.armes[3] = (Carte){CARTE_ARME,3,"Corde"};
    mode.armes[4] = (Carte){CARTE_ARME,4,"Cle anglaise"};
    mode.armes[5] = (Carte){CARTE_ARME,5,"Matraque"};

    mode.nbPieces = 9;
    mode.pieces = malloc(sizeof(Carte) * mode.nbPieces);
    mode.pieces[0] = (Carte){CARTE_PIECE,0,"Cuisine"};
    mode.pieces[1] = (Carte){CARTE_PIECE,1,"Salon"};
    mode.pieces[2] = (Carte){CARTE_PIECE,2,"Bibliotheque"};
    mode.pieces[3] = (Carte){CARTE_PIECE,3,"Salle a manger"};
    mode.pieces[4] = (Carte){CARTE_PIECE,4,"Hall"};
    mode.pieces[5] = (Carte){CARTE_PIECE,5,"Veranda"};
    mode.pieces[6] = (Carte){CARTE_PIECE,6,"Studio"};
    mode.pieces[7] = (Carte){CARTE_PIECE,7,"Bureau"};
    mode.pieces[8] = (Carte){CARTE_PIECE,8,"Billard"};

    return mode;
}

ModeJeu creerModeHarryPotter()
{
    ModeJeu mode;

    strcpy(mode.nom, "Harry Potter");
    strcpy(mode.nomDossier, "harry_potter");

    mode.nbSuspects = 6;
    mode.suspects = malloc(sizeof(Carte) * mode.nbSuspects);

    mode.suspects[0] = (Carte){CARTE_SUSPECT,0,"Bellatrix Lestrange"};
    mode.suspects[1] = (Carte){CARTE_SUSPECT,1,"Crabbe et Goyle"};
    mode.suspects[2] = (Carte){CARTE_SUSPECT,2,"Dolores Umbrigde"};
    mode.suspects[3] = (Carte){CARTE_SUSPECT,3,"Drako Malfoy"};
    mode.suspects[4] = (Carte){CARTE_SUSPECT,4,"Lucius Malfoy"};
    mode.suspects[5] = (Carte){CARTE_SUSPECT,5,"Peter Pettigrow"};

    mode.nbArmes = 6;
    mode.armes = malloc(sizeof(Carte) * mode.nbArmes);
    mode.armes[0] = (Carte){CARTE_ARME,0,"Bague des Gaunt"};
    mode.armes[1] = (Carte){CARTE_ARME,1,"Epee de Gryffondor"};
    mode.armes[2] = (Carte){CARTE_ARME,2,"Lunettes de Luna"};
    mode.armes[3] = (Carte){CARTE_ARME,3,"Moto de Hagrid"};
    mode.armes[4] = (Carte){CARTE_ARME,4,"Pensine"};
    mode.armes[5] = (Carte){CARTE_ARME,5,"Polynectar"};

    mode.nbPieces = 9;
    mode.pieces = malloc(sizeof(Carte) * mode.nbPieces);
    mode.pieces[0] = (Carte){CARTE_PIECE,0,"salle Alchimie"};
    mode.pieces[1] = (Carte){CARTE_PIECE,1,"Salon Principal"};
    mode.pieces[2] = (Carte){CARTE_PIECE,2,"Salle de Cours"};
    mode.pieces[3] = (Carte){CARTE_PIECE,3,"Refectoire"};
    mode.pieces[4] = (Carte){CARTE_PIECE,4,"Dortoirs"};
    mode.pieces[5] = (Carte){CARTE_PIECE,5,"Bibliotheque"};
    mode.pieces[6] = (Carte){CARTE_PIECE,6,"Solarium"};
    mode.pieces[7] = (Carte){CARTE_PIECE,7,"Entree Principale"};
    mode.pieces[8] = (Carte){CARTE_PIECE,8,"Salle de Repos"};

    return mode;
}

void libererMode(ModeJeu* mode)
{
    free(mode->suspects);
    free(mode->armes);
    free(mode->pieces);
}