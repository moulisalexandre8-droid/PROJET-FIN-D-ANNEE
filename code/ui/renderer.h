#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

int estUnePiece(int valeur);

SDL_Color obtenirCouleurCase(int valeur);

void afficherTexte(SDL_Renderer* rendu,TTF_Font* police,const char* texte,int x,int y);

void afficherNomsPieces(SDL_Renderer* rendu,TTF_Font* police);

void dessinerCase(SDL_Renderer* rendu,int valeur,int x,int y,int tailleCaseX,int tailleCaseY);

void dessinerPlateau(SDL_Renderer* rendu,int plateau[26][28],int tailleCaseX,int tailleCaseY);

void dessinerJoueur(SDL_Renderer* rendu,SDL_Texture* texture,int x,int y,int tailleCaseX,int tailleCaseY);

void dessinerFond(SDL_Renderer* rendu, SDL_Texture* textureFond);

void dessinerTexture(SDL_Renderer* rendu,SDL_Texture* texture,int x,int y,int largeur,int hauteur);

void dessinerFond(SDL_Renderer* rendu, SDL_Texture* textureFond);

//asupprimer après les tests
void dessinerGrilleDebug(SDL_Renderer* rendu,int tailleCaseX,int tailleCaseY);

#endif