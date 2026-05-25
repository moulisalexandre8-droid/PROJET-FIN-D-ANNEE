#include "../game/game.h"
#include "../utils/constants.h"
#include "../ui/renderer.h"
#include "../utils/loader.h"
#include <SDL2/SDL_image.h>
#include "../entities/room.h"
#include <stdio.h>
#include <string.h>

Joueur initialiserJoueur(SDL_Renderer* rendu,int x,int y,const char* cheminImage,const char* nom)
{
    Joueur j;
    for(int i=0;i<6;i++)
        j.notesSuspects[i]=0;
    
    for(int i=0;i<6;i++)
        j.notesArmes[i]=0;
    
    for(int i=0;i<9;i++)
        j.notesPieces[i]=0;

    j.x = x;
    j.y = y;

    strcpy(j.nom, nom);

    j.mouvementsRestants = 0;

    j.nbCartes = 0;

    j.ligneCarnet = 0;

    j.aFaitSoupcon = 0;

    SDL_Surface* surface = IMG_Load(cheminImage);

    if (surface == NULL)
    {
        printf("Erreur image %s : %s\n",cheminImage,IMG_GetError());}

    j.texture = SDL_CreateTextureFromSurface(rendu, surface);

    SDL_FreeSurface(surface);

    return j;
}

void deplacerJoueur(Joueur* j, int nx, int ny)
{
    j->x = nx;
    j->y = ny;
}

//limites du plateau

void appliquerLimites(Joueur* j,int tailleCaseX,int tailleCaseY,int largeur,int hauteur)
{
    if (j->x < 0)
        j->x = 0;

    if (j->y < 0)
        j->y = 0;

    if (j->x > largeur - tailleCaseX)
        j->x = largeur - tailleCaseX;

    if (j->y > hauteur - tailleCaseY)
        j->y = hauteur - tailleCaseY;
}

// deplacement

void bougerJoueur(const Uint8* etat,Joueur* j,int tailleCaseX,int tailleCaseY){
    if (j->mouvementsRestants <= 0)
        return;

    static int lock = 0;

    if (lock)
    {
        if (!etat[SDL_SCANCODE_UP] &&
            !etat[SDL_SCANCODE_DOWN] &&
            !etat[SDL_SCANCODE_LEFT] &&
            !etat[SDL_SCANCODE_RIGHT])
        {
            lock = 0;
        }
        return;
    }
    int dx = 0;
    int dy = 0;

    if (etat[SDL_SCANCODE_UP])
    dy = -tailleCaseY;

    else if (etat[SDL_SCANCODE_DOWN])
        dy = tailleCaseY;

    else if (etat[SDL_SCANCODE_LEFT])
        dx = -tailleCaseX;

    else if (etat[SDL_SCANCODE_RIGHT])
        dx = tailleCaseX;

    if (dx == 0 && dy == 0)
        return;

    int nx = j->x + dx;
    int ny = j->y + dy;

    if (peutAller(j->x,j->y,nx,ny,tailleCaseX,tailleCaseY))
    {
        j->x = nx;
        j->y = ny;

        j->mouvementsRestants--;

        lock = 1;
    }
}

void teleporterDansSalle(Joueur* j,int salle,float tailleX,float tailleY)
{
    placerJoueurCase(j,salleX[salle],salleY[salle],tailleX,tailleY);
}

Joueur* obtenirJoueurParSuspect(int suspect,Joueur* j1,Joueur* j2)
{
    if(j1->personnage == suspect)
        return j1;

    if(j2->personnage == suspect)
        return j2;

    return NULL;
}