#include "game.h"
#include "../utils/constants.h"
#include "../ui/renderer.h"
#include "../utils/loader.h"
#include "../ui/window.h"
#include "../ui/buttons.h"
#include "../ui/text.h"

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

    Bouton boutonDe = creerBouton(1000, 200, 300, 60, "Lancer le de");
    Bouton boutonTour = creerBouton(1000, 300, 300, 60, "Fin du tour");

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
                    printf("DE LANCE\n");
            
                if (boutonEstClique(&boutonTour, x, y))
                    printf("FIN TOUR\n");
            }
        }

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        updateHover(&boutonDe, mouseX, mouseY);
        updateHover(&boutonTour, mouseX, mouseY);

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

        SDL_SetRenderDrawColor(rendu, 255, 0, 0, 255);
        
        dessinerTexture(rendu, plateauTexture, 0, 0, 925, 860);

        // dessinerGrilleDebug(rendu, tailleCaseX, tailleCaseY);
        dessinerInterfaceDroite(rendu);

        dessinerBouton(rendu, &boutonDe);
        dessinerBouton(rendu, &boutonTour);

        SDL_Color blanc = {255, 255, 255, 255};

        SDL_Texture* t1 = creerTexte(rendu, font, boutonDe.texte, blanc);
        SDL_Texture* t2 = creerTexte(rendu, font, boutonTour.texte, blanc);

        dessinerTexteCentre(rendu, t1,boutonDe.rect.x, boutonDe.rect.y,boutonDe.rect.w, boutonDe.rect.h);
            
        dessinerTexteCentre(rendu, t2,boutonTour.rect.x, boutonTour.rect.y,boutonTour.rect.w, boutonTour.rect.h);
        
        SDL_DestroyTexture(t1);
        SDL_DestroyTexture(t2);

        dessinerJoueur(rendu,j1.texture,j1.x,j1.y,tailleCaseX,tailleCaseY);
        dessinerJoueur(rendu,j2.texture,j2.x,j2.y,tailleCaseX,tailleCaseY);

        SDL_RenderPresent(rendu);// afiche le tout à l'écran
        SDL_Delay(16);
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