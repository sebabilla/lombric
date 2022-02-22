#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#include "lombric.h"
#include "affichage.h"

void SDL_ExitWithError(const char *message)
{
	SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
	SDL_Quit();
	exit(EXIT_FAILURE);
}

void limit_fps(unsigned int limit)
{
	unsigned t = SDL_GetTicks();
	
	if (limit < t)
		return; // laisser tourner l'affichage
	else if (limit > t + FPS)
		SDL_Delay(FPS); // faire une pause
	else
		SDL_Delay(limit - t); // faire une petite pause
}

void AfficherTerrain(SDL_Renderer *r, int i, int j, Evenement evm)
{
	int R = 0, G = 0, B = 0;
	
	switch(evm)
	{
		case RIEN:
			R = 0; G = 0; B = 0;
			break;
		case BORD:
			R = 50; G = 50; B = 50;
			break;
		case LOMBRIC:
			R = 255, G = 180; B = 195;
			break;
		case LOMBRICAGE:
			R = 225, G = 145; B = 160;
			break;
		case MO:
			R = 255;
			break;
		case MODISP:
			R = 205;
			break;
		case MODISPARITION:
			R = 155;
			break;
		case VITPLUS:
			R = 30; G = 144; B = 255;
			break;
		case VITPLUSDISP:
			R = 30; G = 94; B = 205;
			break;
		case VITPLUSDISPARITION:
			R = 30; G = 44; B = 155;
			break;
		case VITMOINS:
			G = 255;
			break;
		case VITMOINSDISP:
			G = 205;
			break;
		case VITMOINSDISPARITION:
			G = 155;
			break;
		default:
			break;
	}
	
	
	if (SDL_SetRenderDrawColor(r, R, G, B, SDL_ALPHA_TRANSPARENT) != 0)
		SDL_ExitWithError("Impossible de changer la couleur du rendu");
	
	if (SDL_RenderDrawPoint(r, i, j) != 0)
		SDL_ExitWithError("Impossible de changer la couleur du point");
}

void EcrireTexte(SDL_Renderer *r, char *texte, TTF_Font *f, int X, int Y, int W, int H, CouleurTexte c)
{
	SDL_Color couleur;
	
	if (c == BLANC)
	{
		couleur.r = 250; couleur.g = 250; couleur.b = 250, couleur.a = 0;
	}
	if (c == GRIS)
	{
		couleur.r = 160; couleur.g = 160; couleur.b = 160, couleur.a = 0;
	}
	if (c == JAUNE)
	{
		couleur.r = 250; couleur.g = 250; couleur.b = 0, couleur.a = 0;
	}
									
	SDL_Surface *surfaceMessage = TTF_RenderUTF8_Blended(f, texte, couleur);
	SDL_Texture *Message = SDL_CreateTextureFromSurface(r, surfaceMessage);
	SDL_Rect MessageRect = {.x = X, .y = Y, .w = W, .h = H};
	SDL_RenderCopy(r, Message, NULL, &MessageRect);
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);
}

void AfficherPause(SDL_Renderer *r, TTF_Font *f)
{
	EcrireTexte(r, "PAUSE", f, TERRAIN_HEIGHT / 2 - 75, TERRAIN_WIDTH / 2 - 50, 150, 100, JAUNE);
}

void AfficherPerdu(SDL_Renderer *r, TTF_Font *f)
{
	EcrireTexte(r, "PERDU", f, TERRAIN_HEIGHT / 2 - 75, TERRAIN_WIDTH / 2 - 50, 150, 100, JAUNE);
}

void AfficherCommandes(SDL_Renderer *r, TTF_Font *f)
{
	EcrireTexte(r, "Espace", f, WINDOW_WIDTH - 190, WINDOW_HEIGHT - 125, 50, 25, GRIS);
	EcrireTexte(r, "PAUSE", f, WINDOW_WIDTH - 125, WINDOW_HEIGHT - 125, 50, 25, GRIS);
	EcrireTexte(r, "Entrée", f, WINDOW_WIDTH - 190, WINDOW_HEIGHT - 100, 50, 25, GRIS);
	EcrireTexte(r, "RECOMMENCER", f, WINDOW_WIDTH - 125, WINDOW_HEIGHT - 100, 110, 25, GRIS);
	EcrireTexte(r, "Echap", f, WINDOW_WIDTH - 190, WINDOW_HEIGHT - 75, 50, 25, GRIS);
	EcrireTexte(r, "QUITTER", f, WINDOW_WIDTH - 125, WINDOW_HEIGHT - 75, 62, 25, GRIS);
}

void AfficherLegende(SDL_Renderer *r, TTF_Font *f)
{
	EcrireTexte(r, "Feuilles d'automne", f, WINDOW_WIDTH - 170, WINDOW_HEIGHT - 250, 170, 25, BLANC);
	EcrireTexte(r, "Exsudats racinaires", f, WINDOW_WIDTH - 170, WINDOW_HEIGHT - 225, 170, 25, BLANC);
	EcrireTexte(r, "Pesticides", f, WINDOW_WIDTH - 170, WINDOW_HEIGHT - 200, 80, 25, BLANC);

	SDL_Rect Rouge = {.x = WINDOW_WIDTH - 195, .y = WINDOW_HEIGHT - 250, .w = TUILE, .h = TUILE};
	if (SDL_SetRenderDrawColor(r, 255, 0, 0, SDL_ALPHA_TRANSPARENT) != 0)
		SDL_ExitWithError("Impossible de changer la couleur du rendu");
	if (SDL_RenderFillRect(r, &Rouge) != 0)
			SDL_ExitWithError("Impossible de dessiner un rectangle");
	
	SDL_Rect Bleu = {.x = WINDOW_WIDTH - 195, .y = WINDOW_HEIGHT - 225, .w = TUILE, .h = TUILE};
	if (SDL_SetRenderDrawColor(r, 30, 144, 255, SDL_ALPHA_TRANSPARENT) != 0)
		SDL_ExitWithError("Impossible de changer la couleur du rendu");
	if (SDL_RenderFillRect(r, &Bleu) != 0)
			SDL_ExitWithError("Impossible de dessiner un rectangle");
	
	SDL_Rect Vert = {.x = WINDOW_WIDTH - 195, .y = WINDOW_HEIGHT - 200, .w = TUILE, .h = TUILE};
	if (SDL_SetRenderDrawColor(r, 0, 255, 0, SDL_ALPHA_TRANSPARENT) != 0)
		SDL_ExitWithError("Impossible de changer la couleur du rendu");
	if (SDL_RenderFillRect(r, &Vert) != 0)
			SDL_ExitWithError("Impossible de dessiner un rectangle");
	
}

