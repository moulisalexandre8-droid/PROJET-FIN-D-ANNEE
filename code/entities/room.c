#include "room.h"

Piece pieces[] =
{
    {2,"Cuisine"},
    {3,"Salle de bal"},
    {4,"Veranda"},
    {5,"Bureau"},
    {7,"Bibliotheque"},
    {8,"Salon"},
    {9,"Hall"},
    {10,"Salle a manger"},
    {11,"Petit salon"}
};

char* obtenirNomPiece(int id)
{
    for(int i=0;i<9;i++)
    {
        if(pieces[i].id == id)
            return pieces[i].nom;
    }

    return "Couloir";
}