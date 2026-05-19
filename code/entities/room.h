#ifndef ROOM_H
#define ROOM_H

typedef struct
{
    int id;

    char nom[30];

} Piece;

extern Piece pieces[];

char* obtenirNomPiece(int id);

int estUneSalle(int id);
const char* obtenirNomSalle(int id);

#endif