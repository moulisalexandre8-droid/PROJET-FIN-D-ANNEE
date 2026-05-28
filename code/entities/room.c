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
    2,  // ID 2 : Cuisine
    11, // ID 3 : Salle de bal
    20, // ID 4 : Véranda
    3,  // ID 5 : Salle à manger
    20, // ID 6 : Bibliothèque
    19, // ID 7 : Bureau
    3,  // ID 8 : Salon
    12, // ID 9 : Hall
    19  // ID 10 : Petit salon
};

int salleY[NB_PIECES] =
{
    4,  // ID 2 : Cuisine
    5,  // ID 3 : Salle de bal
    3,  // ID 4 : Véranda
    14, // ID 5 : Salle à manger
    10, // ID 6 : Bibliothèque
    16, // ID 7 : Bureau
    21, // ID 8 : Salon
    20, // ID 9 : Hall
    22  // ID 10 : Petit salon
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

int obtenirDestinationTrappe(int idSalle)
{
    if (idSalle == 2) return 7; // Cuisine vers Bureau
    if (idSalle == 7) return 2; // Bureau vers Cuisine
    if (idSalle == 4) return 8; // Véranda vers Salon
    if (idSalle == 8) return 4; // Salon vers Véranda
    
    return -1; //sinon y en a pas
}