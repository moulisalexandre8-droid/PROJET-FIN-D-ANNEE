#include "window.h"

void dessinerInterfaceDroite(SDL_Renderer* rendu)
{
    SDL_Rect panneau = {1120,0,380,860};

    SDL_SetRenderDrawColor(rendu,35,35,35,255);
    SDL_RenderFillRect(rendu,&panneau);

    SDL_SetRenderDrawColor(rendu,0,0,0,255);
    SDL_RenderDrawLine(rendu,1120,0,1120,860);

    /* zone dé */

    SDL_Rect zoneDe = {1250,100,140,140};

    SDL_SetRenderDrawColor(rendu,220,220,220,255);
    SDL_RenderFillRect(rendu,&zoneDe);

}