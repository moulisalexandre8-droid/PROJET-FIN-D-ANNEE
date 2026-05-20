#ifndef CARD_H
#define CARD_H

typedef enum
{
    CARTE_SUSPECT,
    CARTE_ARME,
    CARTE_PIECE

} TypeCarte;


typedef struct
{
    TypeCarte type;

    int id;

    char nom[30];

} Carte;

#endif