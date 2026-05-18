#include "game.h"
#include "../ui/renderer.h"
#include "../utils/loader.h"

#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <string.h>

#define OFFSET_X 59
#define OFFSET_Y 40// pour les marges décoratives du plateau, à ajuster si besoin


// Plateau de jeu : 0 = sol, 1 = mur, 2/3/4/5/7/8/9/10/11 = pièces,6 = picine , 12 = porte haut, 13 = porte bas, 14 = porte gauche, 15 = porte droite

int plateau[25][26] =
{
    {1,1,1,1,1,1,1,1,1,0,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1},
    {1,2,2,2,2,1,1,0,0,0,1,3,3,1,0,0,0,1,1,4,4,4,4,4,1,1},
    {1,2,2,2,2,1,0,0,1,1,1,3,3,1,1,1,0,0,1,4,4,4,4,4,1,1},
    {1,2,2,2,2,1,0,0,1,3,3,3,3,3,3,1,0,0,1,4,4,4,4,4,1,1},
    {1,2,2,2,2,1,0,0,1,3,3,3,3,3,3,1,0,0,1,4,4,4,4,4,1,1},
    {1,2,2,2,2,1,0,0,14,3,3,3,3,3,3,15,0,0,0,14,1,1,1,1,1,1},
    {1,1,1,1,13,1,0,0,1,3,3,3,3,3,3,1,0,0,0,0,0,0,0,0,1,1},
    {0,0,0,0,0,0,0,0,1,13,1,1,1,1,13,1,0,0,0,0,0,0,0,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,14,7,7,7,7,7,7,1},
    {1,5,5,5,1,1,1,1,0,0,6,6,6,6,6,0,0,0,1,7,7,7,7,7,7,1},
    {1,5,5,5,5,5,5,1,0,0,6,6,6,6,6,0,0,0,1,7,7,7,7,7,7,1},
    {1,5,5,5,5,5,5,15,0,0,6,6,6,6,6,0,0,0,1,1,1,1,13,1,1,1},
    {1,5,5,5,5,5,5,1,0,0,6,6,6,6,6,0,0,0,0,0,0,0,0,1,1,1},
    {1,5,5,5,5,5,5,1,0,0,6,6,6,6,6,0,0,0,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,0,0,6,6,6,6,6,0,0,1,1,8,8,8,8,8,8,1},
    {1,0,0,0,0,0,0,0,0,0,6,6,6,6,6,0,0,0,14,8,8,8,8,8,8,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,8,8,8,8,8,8,1},
    {1,0,0,0,0,0,0,0,0,1,1,12,12,1,1,0,0,0,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,12,0,0,1,10,10,10,10,1,0,0,0,0,0,0,0,0,1,1},
    {1,9,9,9,9,9,1,0,0,1,10,10,10,10,15,0,0,0,0,0,0,0,1,1,1},
    {1,9,9,9,9,9,1,0,0,1,10,10,10,10,1,0,0,12,1,1,1,1,1,1,1},
    {1,9,9,9,9,9,1,0,0,1,10,10,10,10,1,0,0,1,11,11,11,11,11,11,1},
    {1,9,9,9,9,9,1,0,0,1,10,10,10,10,1,0,0,1,11,11,11,11,11,11,1},
    {1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1},
};

// Sdl et initialisation

int initialiserSDL(SDL_Window** fenetre, SDL_Renderer** rendu)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Erreur SDL : %s\n", SDL_GetError());
        return 0;
    }

    if (TTF_Init() == -1)
    {
        printf("Erreur TTF : %s\n", TTF_GetError());
        return 0;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        printf("Erreur SDL_image : %s\n", IMG_GetError());
        return 0;
    }

    *fenetre = SDL_CreateWindow("Cluelau",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        925, 860, 0);

    *rendu = SDL_CreateRenderer(*fenetre, -1, SDL_RENDERER_ACCELERATED);

    return (*fenetre && *rendu);
}

// joueur

Joueur initialiserJoueur(SDL_Renderer* rendu,int x,int y,const char* cheminImage,const char* nom)
{
    Joueur j;

    j.x = x;
    j.y = y;

    strcpy(j.nom, nom);

    j.mouvementsRestants = 6;

    SDL_Surface* surface = IMG_Load(cheminImage);

    if (surface == NULL)
    {
        printf("Erreur image %s : %s\n",cheminImage,IMG_GetError());}

    j.texture = SDL_CreateTextureFromSurface(rendu, surface);

    SDL_FreeSurface(surface);

    return j;
}

// colisions et limites

int estUnMur(int ligne, int colonne)
{
    return plateau[ligne][colonne] == 1;
}

int peutAller(int ancienneX,int ancienneY,
              int x,int y,
              int tailleCaseX,int tailleCaseY)
{
    int col = (x - OFFSET_X) / tailleCaseX;
    int lig = (y - OFFSET_Y) / tailleCaseY;

    int ancienneCol = (ancienneX - OFFSET_X) / tailleCaseX;
    int ancienneLig = (ancienneY - OFFSET_Y) / tailleCaseY;

    // limites tableau
    if (lig < 0 || lig >= 25 || col < 0 || col >= 26)
        return 0;

    int caseArrivee = plateau[lig][col];

    //chemainement de base : on peut aller sur les cases vides et la piscine

    if (caseArrivee == 0)
        return 1;

    //mur

    if (caseArrivee == 1)
        return 0;

    // piscine

    if (caseArrivee == 6)
        return 1;

    // portes : on doit venir d'une direction précise pour pouvoir les traverser

    // porte haut
    if (caseArrivee == 12)
    {
        return lig > ancienneLig;
    }

    // porte bas
    if (caseArrivee == 13)
    {
        return lig < ancienneLig;
    }

    // porte gauche
    if (caseArrivee == 14)
    {
        return col > ancienneCol;
    }

    // porte droite
    if (caseArrivee == 15)
    {
        return col < ancienneCol;
    }

    // pieces 
    if (caseArrivee >= 2 && caseArrivee <= 11)
        return 0;

    return 0;
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

// cleanup

void nettoyer(SDL_Window* f, SDL_Renderer* r)
{
    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(f);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void placerJoueurCase(Joueur* j,int col,int lig,int tailleCaseX,int tailleCaseY)
{
    j->x = OFFSET_X + col * tailleCaseX;
    j->y = OFFSET_Y + lig * tailleCaseY;
}

// boucle principale

void boucleJeu(SDL_Window* fenetre, SDL_Renderer* rendu)
{
    int tailleCaseX = 33;
    int tailleCaseY = 31;

    SDL_Texture* textureRose =chargerTexture(rendu,"code/assets/icons/Joueur1.png");
    SDL_Texture* textureMoutarde =chargerTexture(rendu,"code/assets/icons/Joueur2.png");
    SDL_Texture* plateauTexture =chargerTexture(rendu,"code/assets/board/cluedo_board.png");

    Joueur j1 = initialiserJoueur(rendu,0,0,"code/assets/icons/Joueur1.png","J1");
    Joueur j2 = initialiserJoueur(rendu,0,0,"code/assets/icons/Joueur2.png","J2");

    SDL_Surface* surfaceFond =IMG_Load("code/assets/board/cluedo_board.png");

    SDL_Texture* textureFond =SDL_CreateTextureFromSurface(rendu, surfaceFond);

    SDL_FreeSurface(surfaceFond);

    Joueur* actif = &j1;

    placerJoueurCase(&j1, 9, 0, tailleCaseX, tailleCaseY);
    placerJoueurCase(&j2, 0, 7, tailleCaseX, tailleCaseY);

    SDL_Event e;// contient les événements (clavier, souris, etc.)
    int run = 1;// nombre de bucles à faire avant de quitter le jeu(pas encore parametrée pour le vrai jeu)

    while (run)
    {
        while (SDL_PollEvent(&e))// lit les evenemment
        {
            if (e.type == SDL_QUIT)
                run = 0;

            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
            {
                actif = (actif == &j1) ? &j2 : &j1;
                actif->mouvementsRestants = 6;
            }
        }

        const Uint8* etat = SDL_GetKeyboardState(NULL);

        bougerJoueur(etat,actif,tailleCaseX,tailleCaseY);
        if (actif->mouvementsRestants <= 0)
        {
            if (actif == &j1)
                actif = &j2;
            else
                actif = &j1;
        
            actif->mouvementsRestants = 6;
        }
        appliquerLimites(actif,tailleCaseX,tailleCaseY,925,860);

        SDL_SetRenderDrawColor(rendu,0,0,0,255);
        SDL_RenderClear(rendu);

        dessinerTexture(rendu,plateauTexture,0,0,925,860);

        dessinerJoueur(rendu,j1.texture,j1.x,j1.y,tailleCaseX,tailleCaseY);

        dessinerJoueur(rendu,j2.texture,j2.x,j2.y,tailleCaseX,tailleCaseY);

        SDL_RenderPresent(rendu);// afiche le tout à l'écran
        SDL_Delay(16);
    }
}