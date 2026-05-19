#ifndef CARDS_H
#define CARDS_H

#include "../entities/card.h"

#define NB_SUSPECTS 6
#define NB_ARMES 6
#define NB_PIECES 9

extern Carte cartesSuspects[NB_SUSPECTS];
extern Carte cartesArmes[NB_ARMES];
extern Carte cartesPieces[NB_PIECES];

void initialiserCartes();

typedef struct
{
    Carte suspect;
    Carte arme;
    Carte piece;

} Solution;


extern Solution solutionJeu;

void genererSolution();

#endif