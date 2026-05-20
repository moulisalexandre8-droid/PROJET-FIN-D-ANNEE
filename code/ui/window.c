#include "window.h"

void dessinerInterfaceDroite(SDL_Renderer* rendu)
{
    SDL_Rect panneau = {950,0,450,860};

    SDL_SetRenderDrawColor(rendu,35,35,35,255);
    SDL_RenderFillRect(rendu,&panneau);

    SDL_SetRenderDrawColor(rendu,0,0,0,255);
    SDL_RenderDrawLine(rendu,950,0,950,860);

    /* zone dé */

    SDL_Rect zoneDe = {1080,100,140,140};

    SDL_SetRenderDrawColor(rendu,220,220,220,255);
    SDL_RenderFillRect(rendu,&zoneDe);

    /* carnet */

    SDL_Rect carnet = {980,540,340,250};

    SDL_SetRenderDrawColor(rendu,60,60,60,255);
    SDL_RenderFillRect(rendu,&carnet);
}