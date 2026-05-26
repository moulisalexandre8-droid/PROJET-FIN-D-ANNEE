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

void distribuerCartes(Joueur* j1,Joueur* j2,ModeJeu* mode);

#endif