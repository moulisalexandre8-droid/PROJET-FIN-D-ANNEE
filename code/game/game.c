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
#include "mode.h"
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

    *fenetre = SDL_CreateWindow("Cluelau",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,FENETRE_LARGEUR,FENETRE_HAUTEUR,0);

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

void dessinerPortes(SDL_Renderer* rendu, float tailleCaseX, float tailleCaseY)
{
    for(int ligne = 0; ligne < 25; ligne++)
    {
        for(int col = 0; col < 26; col++)
        {
            int val = plateau[ligne][col];

            int x = PLATEAU_X + OFFSET_X + col * tailleCaseX;
            int y = OFFSET_Y + ligne * tailleCaseY;

            SDL_SetRenderDrawColor(rendu, 255, 0, 0, 255);

            // porte haut (entrée depuis le haut)
            if(val == 12)
            {
                SDL_Rect r = {x,y,tailleCaseX,4};
                SDL_RenderFillRect(rendu, &r);
            }

            // porte bas (entrée depuis le bas)
            if(val == 13)
            {
                SDL_Rect r = {x,y + tailleCaseY - 4,tailleCaseX,4};
                SDL_RenderFillRect(rendu, &r);
            }

            // porte gauche (entrée depuis la gauche)
            if(val == 14)
            {
                SDL_Rect r = {x,y,4,tailleCaseY};
                SDL_RenderFillRect(rendu, &r);
            }

            // porte droite (entrée depuis la droite)
            if(val == 15)
            {
                SDL_Rect r = {x + tailleCaseX - 4,y,4,tailleCaseY};
                SDL_RenderFillRect(rendu, &r);
            }
        }
    }
}

// boucle principale

void boucleJeu(SDL_Window* fenetre,SDL_Renderer* rendu,ModeJeu mode)
{
    ModeJeu modeActuel = mode;

    initialiserCartes(&modeActuel);
    genererSolution(&modeActuel);

    /*char cheminJ1[200];
    char cheminJ2[200];
    
    sprintf(
        cheminJ1,
        "code/assets/icons/%s/Joueur1.png",
        modeActuel.nomDossier
    );
    
    sprintf(
        cheminJ2,
        "code/assets/icons/%s/Joueur2.png",
        modeActuel.nomDossier
    );*/

    Joueur j1 = initialiserJoueur(rendu,0,0,"code/assets/icons/classique/Joueur1.png","J1");
    Joueur j2 = initialiserJoueur(rendu,0,0,"code/assets/icons/classique/Joueur2.png","J2");

    j1.personnage = 0; // Mme Rose
    j2.personnage = 1; // Moutarde

    distribuerCartes(&j1,&j2,&modeActuel);
    
    float tailleCaseX = 32.5;
    float tailleCaseY = 31;

    int valeurDe = 0;

    EtatJeu etatJeu = ETAT_ATTENTE_DE;
    EtatInterface etatUI = UI_PRINCIPALE;

    Carte carteMontree;
    int afficherCarte = 0;
    int attenteValidation = 0;

    int suspectChoisi = 0;
    int armeChoisie = 0;
    int salleChoisie = 0;

    char cheminPlateau[200];

    sprintf(cheminPlateau,"code/assets/board/%s/cluedo_board.png",modeActuel.nomDossier);

    SDL_Texture* plateauTexture = chargerTexture(rendu, cheminPlateau);

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

    Bouton boutonDe = creerBouton(1170, 280, 300, 60, "Lancer le de");

    Bouton boutonAccuser = creerBouton(1170, 370, 300, 60, "Accuser");

    Bouton boutonSoupcon = creerBouton(1170, 450, 300, 60, "Soupcon");

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
                
                    if(estUneSalle(caseActuelle) && !actif->aFaitSoupcon)
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
                    suspectChoisi = modeActuel.nbSuspects - 1;

                if(suspectChoisi >= modeActuel.nbSuspects)
                    suspectChoisi = 0;

                if(armeChoisie < 0)
                    armeChoisie = modeActuel.nbArmes - 1;

                if(armeChoisie >= modeActuel.nbArmes)
                    armeChoisie = 0;

                if(e.key.keysym.sym == SDLK_RETURN)
                {
                    Joueur* autre =(actif == &j1)? &j2: &j1;
                    carteMontree = faireSuspicion(actif,autre,caseActuelle - 2,suspectChoisi,armeChoisie);

                    actif->aFaitSoupcon = 1;

                    afficherCarte = 1;
                    attenteValidation = 1;
                    etatUI = UI_REVELATION;
                }
                if(e.key.keysym.sym == SDLK_ESCAPE)
                {
                    etatUI = UI_PRINCIPALE;
                }
            }

            if(etatUI == UI_REVELATION && e.type == SDL_KEYDOWN)
            {
                if(e.key.keysym.sym == SDLK_RETURN)
                {
                    if(attenteValidation)
                    {
                        attenteValidation = 0;
                    }
                    else
                    {
                        afficherCarte = 0;
                        actif->aFaitSoupcon = 0;
                    
                        changerTour(&actif,&j1,&j2,&etatJeu);
                    
                        etatUI = UI_PRINCIPALE;
                        etatJeu = ETAT_ATTENTE_DE;
                    }
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
                    suspectChoisi=modeActuel.nbSuspects-1;

                if(suspectChoisi>=modeActuel.nbSuspects)
                    suspectChoisi=0;

                if(armeChoisie<0)
                    armeChoisie=modeActuel.nbArmes-1;

                if(armeChoisie>=modeActuel.nbArmes)
                    armeChoisie=0;

                if(salleChoisie<0)
                    salleChoisie=modeActuel.nbPieces-1;

                if(salleChoisie>=modeActuel.nbPieces)
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
                
                if(actif->ligneCarnet > modeActuel.nbSuspects + modeActuel.nbArmes + modeActuel.nbPieces - 1)
                {
                    actif->ligneCarnet = modeActuel.nbSuspects + modeActuel.nbArmes + modeActuel.nbPieces - 1;
                }
            
            
                if(e.key.keysym.sym == SDLK_SPACE)
                {
                
                    if(actif->ligneCarnet < modeActuel.nbSuspects)
                    {
                        actif->notesSuspects[actif->ligneCarnet] ^=1;
                    }
                
                    else if(actif->ligneCarnet < modeActuel.nbSuspects + modeActuel.nbArmes)
                    {
                        actif->notesArmes[actif->ligneCarnet-modeActuel.nbSuspects] ^=1;
                    }
                
                    else
                    {
                        actif->notesPieces[actif->ligneCarnet-modeActuel.nbSuspects-modeActuel.nbArmes] ^=1;
                    }
                }
            }
            if(etatUI == UI_DEFAITE && e.type == SDL_KEYDOWN)
            {
                if(e.key.keysym.sym == SDLK_RETURN)
                {
                    actif->elimine = 1;
                    changerTour(&actif,&j1,&j2,&etatJeu);
                
                    if(j1.elimine && j2.elimine)
                    {
                        etatUI = UI_VICTOIRE;
                    }
                    else
                    {
                        etatUI = UI_PRINCIPALE;
                    }

                    etatUI = UI_PRINCIPALE;
                    etatJeu = ETAT_ATTENTE_DE;
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

        appliquerLimites(actif,tailleCaseX,tailleCaseY,220 + 925,860);

        SDL_SetRenderDrawColor(rendu, 255, 0, 0, 255);

        dessinerTexture(rendu,plateauTexture,PLATEAU_X,0,PLATEAU_LARGEUR,PLATEAU_HAUTEUR);

        dessinerTexture(rendu, plateauTexture,PLATEAU_X, 0,PLATEAU_LARGEUR,PLATEAU_HAUTEUR);

        dessinerPortes(rendu, tailleCaseX, tailleCaseY);

        dessinerInterfaceDroite(rendu);

        SDL_Rect zoneCartes = {0,0,220,860};

        SDL_SetRenderDrawColor(rendu,50,50,50,255);

        SDL_RenderFillRect(rendu,&zoneCartes);

        dessinerTexture(rendu,grilleTexture,1135,540,390,300);

        int curseurX;
        int curseurY;

        if(actif->ligneCarnet < modeActuel.nbSuspects)
        {
            curseurX = 1243;
            curseurY = 566 + actif->ligneCarnet * 19;
        }

        else if(actif->ligneCarnet <modeActuel.nbSuspects + modeActuel.nbArmes)
        {
            curseurX = 1243;
            curseurY = 716 +(actif->ligneCarnet-modeActuel.nbSuspects)* 19;
        }

        else
        {
            curseurX = 1440;
            curseurY =590 +(actif->ligneCarnet- modeActuel.nbSuspects- modeActuel.nbArmes) * 24;
        }  

        SDL_Texture* curseur = creerTexte(rendu,font,"<",noir);

        dessinerTexteCentre(rendu,curseur,curseurX,curseurY,20,20);

        SDL_DestroyTexture(curseur);

        SDL_SetRenderDrawColor(rendu,255,0,0,255);

        for(int i=0;i<modeActuel.nbSuspects;i++)
        {
            if(actif->notesSuspects[i])
            {
                SDL_Color noir = {0,0,0,255};
                SDL_Texture* tic = creerTexte(rendu,font,"X",noir);
            
                dessinerTexteCentre(rendu,tic,1228,566 + i*19,20,20);
                SDL_DestroyTexture(tic);
            }
        }


        for(int i=0;i<modeActuel.nbArmes;i++)
        {
            if(actif->notesArmes[i])
            {
                SDL_Color noir = {0,0,0,255};
                SDL_Texture* tic = creerTexte(rendu,font,"X",noir);
                
                dessinerTexteCentre(rendu,tic,1228,717 + i*19,20,20);
                SDL_DestroyTexture(tic);
            }
        }

        for(int i=0;i<modeActuel.nbPieces;i++)
        {
            if(actif->notesPieces[i])
            {
                SDL_Color noir = {0,0,0,255};
                SDL_Texture* tic = creerTexte(rendu,font,"X",noir);
            
                dessinerTexteCentre( rendu, tic, 1423, 589 + i*24, 20, 20);
                SDL_DestroyTexture(tic);
            }
        }

        SDL_Texture* txtTour = creerTexte(rendu,font,"Tour :",blanc);

        dessinerTexteCentre(rendu,txtTour,1150,25,120,40);

        SDL_Texture* txtNom = creerTexte(rendu,font,actif->nom,blanc);

        dessinerTexteCentre(rendu,txtNom,1270,25,120,40);

        /* icône joueur */

        dessinerTexture(rendu,actif->texture,1400,15,60,60);

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
            char espace[200];

            sprintf(espace,"Suspect : %s",modeActuel.suspects[suspectChoisi].nom);

            SDL_Texture* txt1= creerTexte(rendu,font,espace,blanc);

            dessinerTexteCentre(rendu,txt1,1175,260,300,40);


            sprintf(espace,"Arme : %s",modeActuel.armes[armeChoisie].nom);

            SDL_Texture* txt2= creerTexte(rendu,font,espace,blanc);

            dessinerTexteCentre(rendu,txt2,1175,320,300,40);


            sprintf(espace,"Salle : %s",modeActuel.pieces[salleChoisie].nom);

            SDL_Texture* txt3= creerTexte(rendu,font,espace,blanc);

            dessinerTexteCentre(rendu,txt3,1175,380,300,40);


            SDL_Texture* aide= creerTexte(rendu,font,"ENTRER=Valider ESC=Retour",blanc);

            dessinerTexteCentre(rendu,aide,1150,470,350,40);


            SDL_DestroyTexture(txt1);
            SDL_DestroyTexture(txt2);
            SDL_DestroyTexture(txt3);
            SDL_DestroyTexture(aide);
        }

        if(etatUI == UI_VICTOIRE)
        {
            SDL_Texture* txt= creerTexte(rendu,font,"VOUS AVEZ GAGNE",blanc);

            dessinerTexteCentre(rendu,txt,1175,300,350,60);

            SDL_DestroyTexture(txt);
        }

        if(etatUI == UI_DEFAITE)
        {
            SDL_Texture* txt = creerTexte(rendu,font,"ENTREE pour continuer la partie",blanc);
            dessinerTexteCentre(rendu,txt,1140,502,350,40);
            SDL_DestroyTexture(txt);

            SDL_Texture* titre = creerTexte(rendu,font,"MAUVAISE ACCUSATION",blanc);
            dessinerTexteCentre(rendu,titre,1150,250,350,60);
            SDL_DestroyTexture(titre);
        
            char espace[200];
        
            sprintf(espace,"Suspect : %s",solution.suspect.nom);
        
            SDL_Texture* txt1 =creerTexte(rendu,font,espace,blanc);
        
            dessinerTexteCentre(rendu,txt1,1150,340,350,40);
            SDL_DestroyTexture(txt1);
        
            sprintf(espace,"Arme : %s",solution.arme.nom);
        
            SDL_Texture* txt2 = creerTexte(rendu,font,espace,blanc);
            dessinerTexteCentre(rendu,txt2,1150,390,350,40);
            SDL_DestroyTexture(txt2);
        
        
        
            sprintf(espace,"Piece : %s",solution.piece.nom);
        
            SDL_Texture* txt3 =creerTexte(rendu,font,espace,blanc);
            dessinerTexteCentre(rendu,txt3,1150,440,350,40);
            SDL_DestroyTexture(txt3);
        }

        if(etatUI == UI_SUSPICION)
        {
            char espace[200];
        
            sprintf(espace,"Suspect : %s",mode.suspects[suspectChoisi].nom);
        
            SDL_Texture* txt1 = creerTexte(rendu,font,espace,blanc);
        
            dessinerTexteCentre(rendu,txt1,1175,250,300,40);


            sprintf(espace,"Arme : %s",mode.armes[armeChoisie].nom);
        
            SDL_Texture* txt2 = creerTexte(rendu,font,espace,blanc);
        
            dessinerTexteCentre(rendu,txt2,1175,320,300,40);
        
            SDL_Texture* aide = creerTexte(rendu,font,"Entrer=Valider ESC=Retour",blanc);
        
            dessinerTexteCentre(rendu,aide,1150,430,350,40);
        
            SDL_DestroyTexture(txt1);
            SDL_DestroyTexture(txt2);
            SDL_DestroyTexture(aide);
        }

        if(etatUI == UI_REVELATION)
        {
            SDL_Texture* txt = creerTexte(rendu,font,"Carte revelee - ENTREE pour continuer",blanc);
            dessinerTexteCentre(rendu,txt,1140,502,350,40);
        
            SDL_DestroyTexture(txt);
        
            char chemin[200];
        
            if(carteMontree.type == CARTE_SUSPECT)
                sprintf(chemin,"code/assets/cards/%s/suspects/%s.png",modeActuel.nomDossier,carteMontree.nom);
        
            else if(carteMontree.type == CARTE_ARME)
                sprintf(chemin,"code/assets/cards/%s/weapons/%s.png",modeActuel.nomDossier,carteMontree.nom);
        
            else
                sprintf(chemin,"code/assets/cards/%s/rooms/%s.png",modeActuel.nomDossier,carteMontree.nom);
        
            SDL_Texture* img = chargerTexture(rendu,chemin);
        
            if(img)
            {
                dessinerTexture(rendu,img,1230,250,180,260);
                SDL_DestroyTexture(img);
            }
        }

        if(!dansSalle)
        {
            boutonAccuser.hover = 0;
            boutonSoupcon.hover = 0;
        }


        if (valeurDe >= 1 && valeurDe <= 6)
        {
            dessinerTexture(rendu,diceTextures[valeurDe - 1],1260,110,120,120);
        }

        dessinerJoueur(rendu,j1.texture,j1.x,j1.y,tailleCaseX,tailleCaseY);
        dessinerJoueur(rendu,j2.texture,j2.x,j2.y,tailleCaseX,tailleCaseY);

        for(int i=0;i<actif->nbCartes;i++)
        {
            char chemin[200];
            Carte c = actif->cartes[i];
        
            if(c.type == CARTE_SUSPECT)
            {
                sprintf(chemin,"code/assets/cards/%s/suspects/%s.png",modeActuel.nomDossier,c.nom);
            }
        
            else if(c.type == CARTE_ARME)
            {
                sprintf(chemin,"code/assets/cards/%s/weapons/%s.png",modeActuel.nomDossier,c.nom);
            }
        
            else if(c.type == CARTE_PIECE)
            {
                sprintf(chemin,"code/assets/cards/%s/rooms/%s.png",modeActuel.nomDossier,c.nom);
            }

            SDL_Texture* carte = chargerTexture(rendu, chemin);
        
            if(carte != NULL)
            {
                int colonne = i % 2;
                int ligne = i / 2;
                            
                int x = 10 + colonne*100;
                int y = 30 + ligne*140;
                            
                dessinerTexture(rendu,carte,x,y,90,130);
                SDL_DestroyTexture(carte);
            }
        }
        // dessinerGrilleDebug(rendu);

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

/*void dessinerGrilleDebug(SDL_Renderer* rendu)
{
    SDL_SetRenderDrawColor(rendu,255,0,0,255);

    for(int x=0;x<=26;x++)
    {
        SDL_RenderDrawLine(rendu,PLATEAU_X + OFFSET_X + x*CASE_LARGEUR,OFFSET_Y,PLATEAU_X + OFFSET_X + x*CASE_LARGEUR,OFFSET_Y + 25*CASE_HAUTEUR);}

    for(int y=0;y<=25;y++)
    {
        SDL_RenderDrawLine(rendu,PLATEAU_X + OFFSET_X,OFFSET_Y + y*CASE_HAUTEUR,PLATEAU_X + OFFSET_X + 26*CASE_LARGEUR,OFFSET_Y + y*CASE_HAUTEUR);
    }
}*/