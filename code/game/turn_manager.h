#ifndef TURN_MANAGER_H
#define TURN_MANAGER_H

#include "game.h"
#include "rules.h"

void changerTour(int* tour,Joueur* joueurs,int nbJoueurs,EtatJeu* etat);

Joueur* prochainJoueur(Joueur* joueurs,int nbJoueurs,int tour);

#endif