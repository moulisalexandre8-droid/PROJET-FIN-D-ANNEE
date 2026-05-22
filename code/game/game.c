#include "game.h"
#include "../entities/room.h"
#include "cards.h"
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

    Joueur j1 = initialiserJoueur(rendu,0,0,"code/assets/icons/Joueur1.png","J1");
    Joueur j2 = initialiserJoueur(rendu,0,0,"code/assets/icons/Joueur2.png","J2");

    distribuerCartes(&j1,&j2);
    
    int tailleCaseX = 33;
    int tailleCaseY = 31;

    int valeurDe = 0;

    EtatJeu etatJeu = ETAT_ATTENTE_DE;
    EtatInterface etatUI = UI_PRINCIPALE;
    int suspectChoisi = 0;
    int armeChoisie = 0;
    int salleChoisie = 0;

    SDL_Texture* plateauTexture =chargerTexture(rendu,"code/assets/board/cluedo_board.png");
    SDL_Texture* grilleTexture = chargerTexture(rendu,"code/assets/grille/grille.png");

    SDL_Texture* diceTextures[6];

    diceTextures[0] = chargerTexture(rendu, "code/assets/de/de1.png");
    diceTextures[1] = chargerTexture(rendu, "code/assets/de/de2.png");
    diceTextures[2] = chargerTexture(rendu, "code/assets/de/de3.png");
    diceTextures[3] = chargerTexture(rendu, "code/assets/de/de4.png");
    diceTextures[4] = chargerTexture(rendu, "code/assets/de/de5.png");
    diceTextures[5] = chargerTexture(rendu, "code/assets/de/de6.png");

    Joueur* actif = &j1;
    int derniereSalle = -1;

    placerJoueurCase(&j1, 9, 0, tailleCaseX, tailleCaseY);
    placerJoueurCase(&j2, 0, 7, tailleCaseX, tailleCaseY);

    SDL_Event e;// contient les événements (clavier, souris, etc.)
    int run = 1;// nombre de bucles à faire avant de quitter le jeu(pas encore parametrée pour le vrai jeu)

    Bouton boutonDe = creerBouton(1000, 280, 300, 60, "Lancer le de");

    Bouton boutonAccuser = creerBouton(1000, 370, 300, 60, "Accuser");

    Bouton boutonSoupcon = creerBouton(1000, 450, 300, 60, "Soupcon");

    TTF_Font* font = TTF_OpenFont("code/assets/fonts/Roboto-Regular.ttf", 20);

    if (!font)
    {
        printf("Erreur font: %s\n", TTF_GetError());
    }

    while (run)
    {
        SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
        SDL_RenderClear(rendu);
        SDL_Color blanc = {255, 255, 255, 255};
        SDL_Color noir = {0,0,0,255};

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
                if(boutonEstClique(&boutonAccuser,x,y))
                {
                    int caseActuelle =obtenirCasePlateau(actif->x,actif->y,tailleCaseX,tailleCaseY);

                    if(estUneSalle(caseActuelle))
                    {
                        etatUI = UI_ACCUSATION;
                    }
                }
                if(boutonEstClique(&boutonSoupcon,x,y))
                {
                    int caseActuelle =obtenirCasePlateau(actif->x,actif->y,tailleCaseX,tailleCaseY);

                    if(estUneSalle(caseActuelle))
                    {
                        etatUI = UI_SUSPICION;
                    }
                }
            }
            if(etatUI == UI_SUSPICION &&e.type == SDL_KEYDOWN)
            {
                int caseActuelle = obtenirCasePlateau(actif->x,actif->y,tailleCaseX,tailleCaseY);

                if(e.key.keysym.sym == SDLK_UP)
                    suspectChoisi--;

                if(e.key.keysym.sym == SDLK_DOWN)
                    suspectChoisi++;

                if(e.key.keysym.sym == SDLK_LEFT)
                    armeChoisie--;

                if(e.key.keysym.sym == SDLK_RIGHT)
                    armeChoisie++;

                if(suspectChoisi < 0)
                    suspectChoisi = NB_SUSPECTS - 1;

                if(suspectChoisi >= NB_SUSPECTS)
                    suspectChoisi = 0;

                if(armeChoisie < 0)
                    armeChoisie = NB_ARMES - 1;

                if(armeChoisie >= NB_ARMES)
                    armeChoisie = 0;

                if(e.key.keysym.sym == SDLK_RETURN)
                {
                    Joueur* autre =(actif == &j1)? &j2: &j1;
                    faireSuspicion(actif,autre,caseActuelle - 2,suspectChoisi,armeChoisie);
                    etatUI = UI_PRINCIPALE;
                }
                if(e.key.keysym.sym == SDLK_ESCAPE)
                {
                    etatUI =UI_PRINCIPALE;
                }
            }
            if(etatUI == UI_ACCUSATION && e.type == SDL_KEYDOWN)
            {
            
                if(e.key.keysym.sym == SDLK_UP)
                    suspectChoisi--;

                if(e.key.keysym.sym == SDLK_DOWN)
                    suspectChoisi++;

                if(e.key.keysym.sym == SDLK_LEFT)
                    armeChoisie--;

                if(e.key.keysym.sym == SDLK_RIGHT)
                    armeChoisie++;

                if(e.key.keysym.sym == SDLK_a)
                    salleChoisie--;

                if(e.key.keysym.sym == SDLK_z)
                    salleChoisie++;

                if(suspectChoisi<0)
                    suspectChoisi=NB_SUSPECTS-1;

                if(suspectChoisi>=NB_SUSPECTS)
                    suspectChoisi=0;

                if(armeChoisie<0)
                    armeChoisie=NB_ARMES-1;

                if(armeChoisie>=NB_ARMES)
                    armeChoisie=0;

                if(salleChoisie<0)
                    salleChoisie=NB_PIECES-1;

                if(salleChoisie>=NB_PIECES)
                    salleChoisie=0;

                if(e.key.keysym.sym == SDLK_RETURN)
                {
                    if(verifierAccusation(suspectChoisi,armeChoisie,salleChoisie))
                    {
                        etatUI = UI_VICTOIRE;
                    }
                    else
                    {
                        etatUI = UI_DEFAITE;
                    }
                }
                if(e.key.keysym.sym == SDLK_ESCAPE)
                    etatUI = UI_PRINCIPALE;
            }
            if(e.type == SDL_KEYDOWN && etatUI == UI_PRINCIPALE)
            {
        
                if(e.key.keysym.sym == SDLK_w)
                    actif->ligneCarnet--;
                
                if(e.key.keysym.sym == SDLK_s)
                    actif->ligneCarnet++;
                
                
                if(actif->ligneCarnet < 0)
                    actif->ligneCarnet = 0;
                
                if(actif->ligneCarnet > NB_SUSPECTS + NB_ARMES + NB_PIECES - 1)
                {
                    actif->ligneCarnet = NB_SUSPECTS + NB_ARMES + NB_PIECES - 1;
                }
            
            
                if(e.key.keysym.sym == SDLK_SPACE)
                {
                
                    if(actif->ligneCarnet < NB_SUSPECTS)
                    {
                        actif->notesSuspects[actif->ligneCarnet] ^=1;
                    }
                
                    else if(actif->ligneCarnet < NB_SUSPECTS + NB_ARMES)
                    {
                        actif->notesArmes[actif->ligneCarnet-NB_SUSPECTS] ^=1;
                    }
                
                    else
                    {
                        actif->notesPieces[actif->ligneCarnet-NB_SUSPECTS-NB_ARMES] ^=1;
                    }
                
                }
            }
        }

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        int caseActuelle = obtenirCasePlateau(actif->x,actif->y,tailleCaseX,tailleCaseY);

        int dansSalle = estUneSalle(caseActuelle);

        updateHover(&boutonDe, mouseX, mouseY);
        if(dansSalle)
        {
            updateHover(&boutonAccuser,mouseX,mouseY);
            updateHover(&boutonSoupcon,mouseX,mouseY);
        }
        else
        {
            boutonAccuser.hover = 0;
            boutonSoupcon.hover = 0;
        }

        const Uint8* etat = SDL_GetKeyboardState(NULL);
        if(etatJeu == ETAT_DEPLACEMENT &&etatUI == UI_PRINCIPALE)
        {
            bougerJoueur(etat, actif, tailleCaseX, tailleCaseY);
        }

        if(estUneSalle(caseActuelle))
        {
            if(caseActuelle != derniereSalle)
            {
                derniereSalle = caseActuelle;
            }
        }
        else
        {
            derniereSalle = -1;
        }

        if(etatJeu == ETAT_DEPLACEMENT && actif->mouvementsRestants <= 0)
        {
            changerTour(&actif,&j1,&j2,&etatJeu);
        }

        appliquerLimites(actif,tailleCaseX,tailleCaseY,925,860);

        SDL_SetRenderDrawColor(rendu, 255, 0, 0, 255);

        dessinerTexture(rendu, plateauTexture, 0, 0, 925, 860);

        dessinerInterfaceDroite(rendu);

        dessinerTexture(rendu,grilleTexture,965,540,390,300);

        int curseurX;
        int curseurY;

        if(actif->ligneCarnet < NB_SUSPECTS)
        {
            curseurX = 1073;
            curseurY = 566 + actif->ligneCarnet * 19;
        }

        else if(actif->ligneCarnet <NB_SUSPECTS + NB_ARMES)
        {
            curseurX = 1073;
            curseurY = 716 +(actif->ligneCarnet-NB_SUSPECTS)* 19;
        }

        else
        {
            curseurX = 1270;
            curseurY =590 +(actif->ligneCarnet- NB_SUSPECTS- NB_ARMES) * 24;
        }  

        SDL_Texture* curseur = creerTexte(rendu,font,"<",noir);

        dessinerTexteCentre(rendu,curseur,curseurX,curseurY,20,20);

        SDL_DestroyTexture(curseur);

        SDL_SetRenderDrawColor(rendu,255,0,0,255);

        for(int i=0;i<NB_SUSPECTS;i++)
        {
            if(actif->notesSuspects[i])
            {
                SDL_Color noir = {0,0,0,255};
                SDL_Texture* tic = creerTexte(rendu,font,"X",noir);
            
                dessinerTexteCentre(rendu,tic,1058,566 + i*19,20,20);
                SDL_DestroyTexture(tic);
            }
        }


        for(int i=0;i<NB_ARMES;i++)
        {
            if(actif->notesArmes[i])
            {
                SDL_Color noir = {0,0,0,255};
                SDL_Texture* tic = creerTexte(rendu,font,"X",noir);
                
                dessinerTexteCentre(rendu,tic,1058,717 + i*19,20,20);
                SDL_DestroyTexture(tic);
            }
        }

        for(int i=0;i<NB_PIECES;i++)
        {
            if(actif->notesPieces[i])
            {
                SDL_Color noir = {0,0,0,255};
                SDL_Texture* tic = creerTexte(rendu,font,"X",noir);
            
                dessinerTexteCentre( rendu, tic, 1253, 589 + i*24, 20, 20);
                SDL_DestroyTexture(tic);
            }
        }

        SDL_Texture* txtTour = creerTexte(rendu,font,"Tour :",blanc);

        dessinerTexteCentre(rendu,txtTour,980,25,120,40);

        SDL_Texture* txtNom = creerTexte(rendu,font,actif->nom,blanc);

        dessinerTexteCentre(rendu,txtNom,1100,25,120,40);

        /* icône joueur */

        dessinerTexture(rendu,actif->texture,1230,15,60,60);

        SDL_DestroyTexture(txtTour);
        SDL_DestroyTexture(txtNom);

        SDL_Texture* t1 = creerTexte(rendu, font, boutonDe.texte, blanc);;
        SDL_Texture* txtAccuser = creerTexte(rendu,font,boutonAccuser.texte,blanc);
        SDL_Texture* txtSoupcon = creerTexte(rendu,font,boutonSoupcon.texte,blanc);

        if(etatUI == UI_PRINCIPALE)
    {
        dessinerBouton(rendu,&boutonDe);
        dessinerBouton(rendu,&boutonAccuser);
        dessinerBouton(rendu,&boutonSoupcon);

        dessinerTexteCentre(rendu,t1,boutonDe.rect.x,boutonDe.rect.y,boutonDe.rect.w,boutonDe.rect.h);
        dessinerTexteCentre(rendu,txtAccuser,boutonAccuser.rect.x,boutonAccuser.rect.y,boutonAccuser.rect.w,boutonAccuser.rect.h);
        dessinerTexteCentre(rendu,txtSoupcon,boutonSoupcon.rect.x,boutonSoupcon.rect.y,boutonSoupcon.rect.w,boutonSoupcon.rect.h);
    }

        if(etatUI == UI_ACCUSATION)
    {
        char buffer[200];

        sprintf(buffer,"Suspect : %s",cartesSuspects[suspectChoisi].nom);

        SDL_Texture* txt1= creerTexte(rendu,font,buffer,blanc);

        dessinerTexteCentre(rendu,txt1,1000,260,300,40);


        sprintf(buffer,"Arme : %s",cartesArmes[armeChoisie].nom);

        SDL_Texture* txt2= creerTexte(rendu,font,buffer,blanc);

        dessinerTexteCentre(rendu,txt2,1000,320,300,40);


        sprintf(buffer,"Salle : %s",cartesPieces[salleChoisie].nom);

        SDL_Texture* txt3= creerTexte(rendu,font,buffer,blanc);

        dessinerTexteCentre(rendu,txt3,1000,380,300,40);


        SDL_Texture* aide= creerTexte(rendu,font,"ENTRER=Valider ESC=Retour",blanc);

        dessinerTexteCentre(rendu,aide,980,470,350,40);


        SDL_DestroyTexture(txt1);
        SDL_DestroyTexture(txt2);
        SDL_DestroyTexture(txt3);
        SDL_DestroyTexture(aide);
    }

        if(etatUI == UI_VICTOIRE)
    {
        SDL_Texture* txt= creerTexte(rendu,font,"VOUS AVEZ GAGNE",blanc);

        dessinerTexteCentre(rendu,txt,980,300,350,60);

        SDL_DestroyTexture(txt);
    }

        if(etatUI == UI_DEFAITE)
    {
        SDL_Texture* txt= creerTexte(rendu,font,"MAUVAISE ACCUSATION",blanc);
        
        dessinerTexteCentre(rendu,txt,980,300,350,60);
        
        SDL_DestroyTexture(txt);
    }

        if(etatUI == UI_SUSPICION)
    {
        char buffer[200];
    
        sprintf(buffer,"Suspect : %s",cartesSuspects[suspectChoisi].nom);
    
        SDL_Texture* txt1 = creerTexte(rendu,font,buffer,blanc);
    
        dessinerTexteCentre(rendu,txt1,1000,250,300,40);
        
        
        sprintf(buffer,"Arme : %s",cartesArmes[armeChoisie].nom);
    
        SDL_Texture* txt2 = creerTexte(rendu,font,buffer,blanc);
    
        dessinerTexteCentre(rendu,txt2,1000,320,300,40);
    
        SDL_Texture* aide = creerTexte(rendu,font,"Entrer=Valider ESC=Retour",blanc);
    
        dessinerTexteCentre(rendu,aide,980,430,350,40);
    
        SDL_DestroyTexture(txt1);
        SDL_DestroyTexture(txt2);
        SDL_DestroyTexture(aide);
    }

        if(!dansSalle)
        {
            boutonAccuser.hover = 0;
            boutonSoupcon.hover = 0;
        }


        if (valeurDe >= 1 && valeurDe <= 6)
        {
            dessinerTexture(rendu,diceTextures[valeurDe - 1],1090,110,120,120);
        }

        dessinerJoueur(rendu,j1.texture,j1.x,j1.y,tailleCaseX,tailleCaseY);
        dessinerJoueur(rendu,j2.texture,j2.x,j2.y,tailleCaseX,tailleCaseY);

        SDL_RenderPresent(rendu);// afiche le tout à l'écran
        SDL_Delay(16);

        SDL_DestroyTexture(t1);
        SDL_DestroyTexture(txtAccuser);
        SDL_DestroyTexture(txtSoupcon);
    }
    for (int i = 0; i < 6; i++)
    {
        SDL_DestroyTexture(diceTextures[i]);
    }
    SDL_DestroyTexture(grilleTexture);
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

/*char buffer[100];

        int xSuspects = 990;
        int xArmes    = 1100;
        int xPieces   = 1210;

        int yDepart = 570;
        int espace = 22;

        SDL_Texture* titre1 = creerTexte(rendu,font,"Suspects",blanc);

        dessinerTexteCentre(rendu,titre1,xSuspects,540,100,20);

        SDL_DestroyTexture(titre1);

        for(int i=0;i<NB_SUSPECTS;i++)
        {
        
            sprintf(buffer,"%c %s",notesSuspects[i]? 'X': 'O',cartesSuspects[i].nom);
        
            SDL_Texture* txt = creerTexte(rendu,font,buffer,blanc);
        
            dessinerTexteCentre(rendu,txt,xSuspects,yDepart+i*espace,120,20);
        
            SDL_DestroyTexture(txt);
        }

        SDL_Texture* titre2 = creerTexte(rendu,font,"Armes",blanc);

        dessinerTexteCentre(rendu,titre2,xArmes,540,100,20);

        SDL_DestroyTexture(titre2);

        for(int i=0;i<NB_ARMES;i++)
        {
            sprintf(buffer,"%c %s",notesArmes[i]? 'X': 'O',cartesArmes[i].nom);
        
            SDL_Texture* txt = creerTexte(rendu,font,buffer,blanc);
        
            dessinerTexteCentre(rendu,txt,xArmes,yDepart+i*espace,120,20);
        
            SDL_DestroyTexture(txt);
        }

        SDL_Texture* titre3 =creerTexte(rendu,font,"Salles",blanc);

        dessinerTexteCentre(rendu,titre3,xPieces,540,100,20);

        SDL_DestroyTexture(titre3);

        for(int i=0;i<NB_PIECES;i++)
        {
        
            sprintf(buffer,"%c %s",notesPieces[i]? 'X': 'O',cartesPieces[i].nom);
        
            SDL_Texture* txt = creerTexte(rendu,font,buffer,blanc);
        
            dessinerTexteCentre(rendu,txt,xPieces,yDepart+i*espace,120,20);
        
            SDL_DestroyTexture(txt);
        }*/