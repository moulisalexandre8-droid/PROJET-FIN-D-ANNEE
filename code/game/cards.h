#ifndef CARDS_H
#define CARDS_H

#include "../entities/card.h"
#include "../game/game.h"
#include "mode.h"

#define NB_SUSPECTS 6
#define NB_ARMES 6
#define NB_PIECES 9

extern Carte cartesSuspects[NB_SUSPECTS];
extern Carte cartesArmes[NB_ARMES];
extern Carte cartesPieces[NB_PIECES];

void initialiserCartes(ModeJeu* mode);

typedef struct
{
    Carte suspect;
    Carte arme;
    Carte piece;

} Solution;


extern Solution solution;

void genererSolution(ModeJeu* mode);

void melanger(Carte* paquet, int nb);

void distribuerCartes(Joueur* joueurs,int nbJoueurs,ModeJeu* mode);

#endif