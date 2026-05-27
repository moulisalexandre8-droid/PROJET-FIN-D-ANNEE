#include "room.h"

Piece pieces[] =
{
    {2,"Cuisine"},
    {3,"Salle de bal"},
    {4,"Veranda"},
    {5,"Salle a manger"},
    {6,"Bibliotheque"},
    {7,"Bureau"},
    {8,"Salon"},
    {9,"Hall"},
    {10,"Petit salon"}
};

int salleX[NB_PIECES] =
{
    2,   // Cuisine
    11,  // Salle de bal
    20,  // Véranda
    3,  // Bureau
    20, // Bibliothèque
    19, // Salon
    3, // Hall
    12, // Salle à manger
    19 // Petit salon
};

int salleY[NB_PIECES] =
{
    4,
    5,
    3,
    14,
    10,
    16,
    21,
    20,
    22
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

int estUneSalle(int id)
{
    return id >= 2 && id <= 11;
}

const char* obtenirNomSalle(int id)
{
    for(int i=0;i<9;i++)
    {
        if(pieces[i].id == id)
            return pieces[i].nom;
    }

    return "Inconnue";
}