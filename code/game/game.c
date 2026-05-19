#include "game.h"
#include "../utils/constants.h"
#include "../ui/renderer.h"
#include "../utils/loader.h"
#include "../ui/window.h"
#include "../ui/buttons.h"
#include "../ui/text.h"
#include "turn_manager.h"
#include "../entities/room.h"
#include "cards.h"
#include "rules.h"
#include <time.h>
#include "de.h"
#include "board.h"

#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <string.h>

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
        1400, 860, 0);

    *rendu = SDL_CreateRenderer(*fenetre, -1, SDL_RENDERER_ACCELERATED);

    srand(time(NULL));

    return (*fenetre && *rendu);
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

// boucle principale

void boucleJeu(SDL_Window* fenetre, SDL_Renderer* rendu)
{
    initialiserCartes();
    genererSolution();
    
    int tailleCaseX = 33;
    int tailleCaseY = 31;

    int valeurDe = 0;

    EtatJeu etatJeu = ETAT_ATTENTE_DE;

    SDL_Texture* textureRose =chargerTexture(rendu,"code/assets/icons/Joueur1.png");
    SDL_Texture* textureMoutarde =chargerTexture(rendu,"code/assets/icons/Joueur2.png");
    SDL_Texture* plateauTexture =chargerTexture(rendu,"code/assets/board/cluedo_board.png");

    SDL_Texture* diceTextures[6];

    diceTextures[0] = chargerTexture(rendu, "code/assets/de/de1.png");
    diceTextures[1] = chargerTexture(rendu, "code/assets/de/de2.png");
    diceTextures[2] = chargerTexture(rendu, "code/assets/de/de3.png");
    diceTextures[3] = chargerTexture(rendu, "code/assets/de/de4.png");
    diceTextures[4] = chargerTexture(rendu, "code/assets/de/de5.png");
    diceTextures[5] = chargerTexture(rendu, "code/assets/de/de6.png");

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

    Bouton boutonDe = creerBouton(1000, 200, 300, 60, "Lancer le de");;

    TTF_Font* font = TTF_OpenFont("code/assets/fonts/Roboto-Regular.ttf", 20);

    if (!font)
    {
        printf("Erreur font: %s\n", TTF_GetError());
    }

    while (run)
    {
        SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
        SDL_RenderClear(rendu);
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                run = 0;
        
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
            
                if (boutonEstClique(&boutonDe, x, y))
                {
                    if (etatJeu == ETAT_ATTENTE_DE)
                    {
                        valeurDe = lancerDe();
                        actif->mouvementsRestants = valeurDe;
                    
                        etatJeu = ETAT_DEPLACEMENT;
                    }
                }
            }
        }

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        updateHover(&boutonDe, mouseX, mouseY);

        const Uint8* etat = SDL_GetKeyboardState(NULL);
        if (etatJeu == ETAT_DEPLACEMENT)
        {
            bougerJoueur(etat, actif, tailleCaseX, tailleCaseY);
        }

        bougerJoueur(etat,actif,tailleCaseX,tailleCaseY);
        int caseActuelle =obtenirCasePlateau(actif->x,actif->y,tailleCaseX,tailleCaseY);

        if(caseActuelle >=2 &&caseActuelle <=11 &&caseActuelle !=6)
        {
            printf(
              "%s est dans : %s\n",
            
              actif->nom,
            
              obtenirNomPiece(
                   caseActuelle
              )
            );
        }
        if (etatJeu == ETAT_DEPLACEMENT && actif->mouvementsRestants <= 0)
        {
            changerTour(&actif,&j1,&j2,&etatJeu);
        }
        appliquerLimites(actif,tailleCaseX,tailleCaseY,925,860);

        SDL_SetRenderDrawColor(rendu, 255, 0, 0, 255);
        
        dessinerTexture(rendu, plateauTexture, 0, 0, 925, 860);

        // dessinerGrilleDebug(rendu, tailleCaseX, tailleCaseY);
        dessinerInterfaceDroite(rendu);

        dessinerBouton(rendu, &boutonDe);

        SDL_Color blanc = {255, 255, 255, 255};

        SDL_Texture* t1 = creerTexte(rendu, font, boutonDe.texte, blanc);

        dessinerTexteCentre(rendu,t1,boutonDe.rect.x,boutonDe.rect.y,boutonDe.rect.w,boutonDe.rect.h);

        if (valeurDe >= 1 && valeurDe <= 6)
        {
            dessinerTexture(rendu,diceTextures[valeurDe - 1],1020,50,120,120);
        }

        dessinerJoueur(rendu,j1.texture,j1.x,j1.y,tailleCaseX,tailleCaseY);
        dessinerJoueur(rendu,j2.texture,j2.x,j2.y,tailleCaseX,tailleCaseY);

        SDL_RenderPresent(rendu);// afiche le tout à l'écran
        SDL_Delay(16);
    }
    for (int i = 0; i < 6; i++)
    {
        SDL_DestroyTexture(diceTextures[i]);
    }
    TTF_CloseFont(font);
}
// a supp

/*void dessinerGrilleDebug(SDL_Renderer* rendu, int tailleCaseX, int tailleCaseY)
{
    SDL_SetRenderDrawColor(rendu, 255, 0, 0, 255);

    // lignes verticales
    for (int x = 0; x <= 26; x++)
    {
        SDL_RenderDrawLine(
            rendu,
            OFFSET_X + x * tailleCaseX,
            OFFSET_Y,
            OFFSET_X + x * tailleCaseX,
            OFFSET_Y + 25 * tailleCaseY
        );
    }

    // lignes horizontales
    for (int y = 0; y <= 25; y++)
    {
        SDL_RenderDrawLine(
            rendu,
            OFFSET_X,
            OFFSET_Y + y * tailleCaseY,
            OFFSET_X + 26 * tailleCaseX,
            OFFSET_Y + y * tailleCaseY
        );
    }
}*/