#include "buttons.h"
#include <string.h>

Bouton creerBouton(int x, int y, int w, int h, const char* texte)
{
    Bouton b;

    b.rect = (SDL_Rect){x, y, w, h};
    strcpy(b.texte, texte);

    b.hover = 0;
    b.actif = 1;

    return b;
}

void updateHover(Bouton* b, int mouseX, int mouseY)
{
    if (!b->actif) return;

    b->hover =
        mouseX >= b->rect.x &&
        mouseX <= b->rect.x + b->rect.w &&
        mouseY >= b->rect.y &&
        mouseY <= b->rect.y + b->rect.h;
}

int boutonEstClique(Bouton* b, int mouseX, int mouseY)
{
    if (!b->actif) return 0;

    return
        mouseX >= b->rect.x &&
        mouseX <= b->rect.x + b->rect.w &&
        mouseY >= b->rect.y &&
        mouseY <= b->rect.y + b->rect.h;
}

void dessinerBouton(SDL_Renderer* rendu, Bouton* b)
{
    if (!b->actif) return;

    // couleur hover / normal
    if (b->hover)
        SDL_SetRenderDrawColor(rendu, 70, 130, 180, 255); // bleu hover
    else
        SDL_SetRenderDrawColor(rendu, 45, 45, 45, 255);   // gris normal

    SDL_RenderFillRect(rendu, &b->rect);

    // bordure
    SDL_SetRenderDrawColor(rendu, 180, 180, 180, 255);
    SDL_RenderDrawRect(rendu, &b->rect);
}