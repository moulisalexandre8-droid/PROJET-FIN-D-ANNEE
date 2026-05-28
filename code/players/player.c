#include "../game/game.h"
#include "../utils/constants.h"
#include "../ui/renderer.h"
#include "../utils/loader.h"
#include <SDL2/SDL_image.h>
#include "../entities/room.h"
#include <stdio.h>
#include <string.h>
#include "../game/board.h"
#include "player.h"


int trouverProchainPasBFS(int startLig, int startCol, int cibleSalle, int* nextLig, int* nextCol) {
    int visites[25][26] = {0};
    PointBFS parent[25][26];
    PointBFS file[25 * 26];
    int debut = 0, fin = 0;

    file[fin++] = (PointBFS){startLig, startCol};
    visites[startLig][startCol] = 1;

    int dirs[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}}; // Haut, Bas, Gauche, Droite
    PointBFS but = {-1, -1};

    while(debut < fin) {
        PointBFS p = file[debut++];

        // Succès : on a trouvé une porte qui mène à la bonne salle
        if(estUnePorte(plateau[p.l][p.c]) && obtenirSalleDepuisPorte(p.l, p.c) == cibleSalle) {
            but = p;
            break;
        }

        // Tester les 4 directions
        for(int i = 0; i < 4; i++) {
            int nl = p.l + dirs[i][0];
            int nc = p.c + dirs[i][1];

            if(nl >= 0 && nl < 25 && nc >= 0 && nc < 26 && !visites[nl][nc]) {
                int caseArrivee = plateau[nl][nc];
                int ok = 0;

                // Est-ce qu'on a le droit de marcher ici ?
                if(caseArrivee == 0 || caseArrivee == 6) ok = 1; 
                else if(caseArrivee == 12 && nl > p.l) ok = 1;
                else if(caseArrivee == 13 && nl < p.l) ok = 1;
                else if(caseArrivee == 14 && nc > p.c) ok = 1;
                else if(caseArrivee == 15 && nc < p.c) ok = 1;
                
                if(ok) {
                    visites[nl][nc] = 1;
                    parent[nl][nc] = p;
                    file[fin++] = (PointBFS){nl, nc};
                }
            }
        }
    }

    // Remonter le chemin pour trouver le tout premier pas à faire
    if(but.l != -1) {
        PointBFS actuel = but;
        while(parent[actuel.l][actuel.c].l != startLig || parent[actuel.l][actuel.c].c != startCol) {
            actuel = parent[actuel.l][actuel.c];
        }
        *nextLig = actuel.l;
        *nextCol = actuel.c;
        return 1;
    }
    return 0;
}

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
    j.salleCible = -1;

    j.elimine = 0;

    j.ligneCarnet = 0;

    j.ligneAvantSalle = -1;
    j.colonneAvantSalle = -1;

    j.aFaitSoupcon = 0;



    SDL_Surface* surface = IMG_Load(cheminImage);

    if (surface == NULL)
    {
        printf("Erreur image %s : %s\n",cheminImage,IMG_GetError());
        j.texture = NULL;
    }
    else{
        j.texture = SDL_CreateTextureFromSurface(rendu, surface);
        SDL_FreeSurface(surface); 
    }

    return j;
}

void deplacerJoueur(Joueur* j, int nx, int ny)
{
    j->ancienneX = j->x;
    j->ancienneY = j->y;

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
        j->ancienneX = j->x;
        j->ancienneY = j->y;

        j->x = nx;
        j->y = ny;

        j->mouvementsRestants--;

        lock = 1;
    }
}

void teleporterDansSalle(Joueur* j, int salle, float tailleX, float tailleY)
{
    if(salle < 2 || salle > 10)
        return;

    int index = salle - 2;
    
    placerJoueurCase(j, salleX[index], salleY[index], tailleX, tailleY);
}

Joueur* obtenirJoueurParSuspect(int suspect,Joueur* j1,Joueur* j2)
{
    if(j1->personnage == suspect)
        return j1;

    if(j2->personnage == suspect)
        return j2;

    return NULL;
}