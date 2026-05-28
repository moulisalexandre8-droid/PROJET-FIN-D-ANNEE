#include "menu.h"

#include "../ui/buttons.h"

#include "../ui/text.h"

#include "../game/mode.h"

#include <SDL2/SDL.h>

#include <SDL2/SDL_ttf.h>

#include <stdio.h>

#include <stdlib.h>



void afficherTexte(SDL_Renderer* renderer, TTF_Font* font, const char* texte, int x, int y)

{

    SDL_Color blanc = {255, 255, 255, 255};

    SDL_Surface* surfaceTexte = TTF_RenderText_Solid(font, texte, blanc);

    SDL_Texture* textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);



    SDL_Rect rectTexte;

    rectTexte.x = x;

    rectTexte.y = y;

    rectTexte.w = surfaceTexte->w;

    rectTexte.h = surfaceTexte->h;



    SDL_RenderCopy(renderer, textureTexte, NULL, &rectTexte);



    SDL_FreeSurface(surfaceTexte);

    SDL_DestroyTexture(textureTexte);

}



ModeJeu afficherMenu(SDL_Renderer* renderer)

{

    TTF_Font* font = TTF_OpenFont("code/assets/fonts/Roboto-Regular.ttf", 32);

    if (!font) {

        printf("Erreur font : %s\n", TTF_GetError());

        ModeJeu fallback = creerModeClassique();

        fallback.nbHumains = 1;
        fallback.nbIA = 2;
        fallback.nbJoueurs = 3;
        fallback.iaDifficulte = 0;
            
        return fallback;

    }



    int running = 1;

    SDL_Event event;



    int nbJoueurs = 0;

    int nbIA = 0;

    int iaDifficulte = 0;

    int erreurParticipants = 0;



    while(running)

    {

        while(SDL_PollEvent(&event))

        {

            if(event.type == SDL_QUIT)

            {

                exit(0);

            }

            

            if(event.type == SDL_MOUSEBUTTONDOWN)

            {

                int mx = event.button.x;

                int my = event.button.y;

                erreurParticipants = 0;



                

                if (mx >= 850 && mx <= 930 && my >= 240 && my <= 290) {

                    nbJoueurs = (nbJoueurs + 1) % 7;

                    if (nbJoueurs + nbIA > 6) {

                        nbIA = 6 - nbJoueurs;

                    }

                }

                

                if (mx >= 850 && mx <= 930 && my >= 340 && my <= 390) {

                    nbIA = (nbIA + 1) % 7;

                    if (nbJoueurs + nbIA > 6) {

                        nbJoueurs = 6 - nbIA;

                    }

                }



                if (mx >= 600 && mx <= 750 && my >= 440 && my <= 490) iaDifficulte = 0;

                if (mx >= 800 && mx <= 950 && my >= 440 && my <= 490) iaDifficulte = 1;



                if (mx >= 600 && mx <= 900 && my >= 600 && my <= 680) {

                    if (nbJoueurs + nbIA >= 2) {

                        running = 0;

                    } else {

                        erreurParticipants = 1;

                    }

                }

            }

        }



        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        SDL_RenderClear(renderer);



        afficherTexte(renderer, font, "========= CLUELAU =========", 520, 100);



        char txtJoueurs[50];

        sprintf(txtJoueurs, "Joueurs humains : %d", nbJoueurs);

        afficherTexte(renderer, font, txtJoueurs, 450, 250);



        SDL_Rect boutonJoueur = {850, 240, 80, 50};

        SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);

        SDL_RenderFillRect(renderer, &boutonJoueur);

        afficherTexte(renderer, font, "+", 880, 245);



        char txtIA[50];

        sprintf(txtIA, "Joueurs IA : %d", nbIA);

        afficherTexte(renderer, font, txtIA, 450, 350);



        SDL_Rect boutonIA = {850, 340, 80, 50};

        SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);

        SDL_RenderFillRect(renderer, &boutonIA);

        afficherTexte(renderer, font, "+", 880, 345);



        afficherTexte(renderer, font, "Difficulte IA :", 350, 450);

        

        SDL_Rect boutonFacile = {600, 440, 150, 50};

        SDL_Rect boutonExpert = {800, 440, 150, 50};



        if (iaDifficulte == 0) SDL_SetRenderDrawColor(renderer, 50, 200, 50, 255);

        else SDL_SetRenderDrawColor(renderer, 50, 120, 50, 255);

        SDL_RenderFillRect(renderer, &boutonFacile);



        if (iaDifficulte == 1) SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);

        else SDL_SetRenderDrawColor(renderer, 120, 50, 50, 255);

        SDL_RenderFillRect(renderer, &boutonExpert);



        afficherTexte(renderer, font, "FACILE", 620, 450);

        afficherTexte(renderer, font, "EXPERTE", 815, 450);





        SDL_Rect boutonJouer = {600, 600, 300, 80};

        

        if (nbJoueurs + nbIA >= 2) {

            SDL_SetRenderDrawColor(renderer, 180, 180, 0, 255);

        } else {

            SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);

        }

        

        SDL_RenderFillRect(renderer, &boutonJouer);

        afficherTexte(renderer, font, "JOUER", 705, 620);



        if (erreurParticipants) {

            SDL_Color rouge = {255, 50, 50, 255};

            SDL_Surface* surfErreur = TTF_RenderText_Solid(font, "Il faut au moins 2 participants !", rouge);

            SDL_Texture* texErreur = SDL_CreateTextureFromSurface(renderer, surfErreur);

            SDL_Rect rectErreur = {550, 700, surfErreur->w, surfErreur->h};

            SDL_RenderCopy(renderer, texErreur, NULL, &rectErreur);

            SDL_FreeSurface(surfErreur);

            SDL_DestroyTexture(texErreur);

        }



        SDL_RenderPresent(renderer);

        SDL_Delay(16);

    }



    TTF_CloseFont(font);



    ModeJeu mode = creerModeClassique();

    mode.nbHumains = nbJoueurs;
    mode.nbIA = nbIA;
    mode.nbJoueurs = nbJoueurs + nbIA;
    mode.iaDifficulte = iaDifficulte;

    return mode;

}