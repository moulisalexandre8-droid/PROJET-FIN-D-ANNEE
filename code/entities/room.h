#ifndef ROOM_H
#define ROOM_H
#include "../game/cards.h"

typedef struct
{
    int id;

    char nom[30];

} Piece;

extern Piece pieces[];
extern int salleX[NB_PIECES];
extern int salleY[NB_PIECES];

char* obtenirNomPiece(int id);

int estUneSalle(int id);
const char* obtenirNomSalle(int id);

#endif