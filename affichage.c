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

void AfficherCadeau(SDL_Renderer *r, SDL_Rect *rect_c, Cadeau *c)
{
	if (c->suivant != NULL)
		AfficherCadeau(r, rect_c, c->suivant);
		
	rect_c->x = c->x + SHIFT;
	rect_c->y = c->y + SHIFT;
	
	int R = 0, G = 0, B = 0;
	Evenement evm = RIEN;
	
	if (c->compteur > TEMPSCADEAU / 2)
		evm = c->evm[0];
	else if (c->compteur < TEMPSCADEAU / 5)
		evm = c->evm[2];
	else
		evm = c->evm[1];
	
	switch(evm)
	{
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
	
	if (SDL_SetRenderDrawColor(r, R, G, B, SDL_ALPHA_OPAQUE) != 0)
		SDL_ExitWithError("Impossible de changer la couleur du rendu");
		
	if (SDL_RenderFillRect(r, rect_c) != 0)
		SDL_ExitWithError("Impossible de dessiner un rectangle");
}

void AfficherLombric(SDL_Renderer *r, SDL_Rect *rect_c, Anneau *a)
{
	if (a->suivant != NULL)
		AfficherLombric(r, rect_c, a->suivant);
	
	if (SDL_SetRenderDrawColor(r, 255, 180, 195, SDL_ALPHA_OPAQUE) != 0)
		SDL_ExitWithError("Impossible de changer la couleur du rendu");	
	
	int X1, X2, Y1, Y2;
	
	switch(a->evm)
	{
		case LOMBRIC:
				if (a->dir == GAUCHE || a->dir == DROITE)
				{
					X1 = a->x + SHIFT; Y1 = a->y - TUILE / 2 + SHIFT; X2 = a->x + SHIFT; Y2 = a->y + TUILE / 2 + SHIFT;
				}		
				else
				{
					X1 = a->x - TUILE / 2 + SHIFT; Y1 = a->y + SHIFT; X2 = a->x + TUILE / 2 + SHIFT; Y2 = a->y + SHIFT;
				}
				if (SDL_RenderDrawLine(r, X1, Y1, X2, Y2) != 0)
					SDL_ExitWithError("Impossible de dessiner un rectangle");
			break;
		case LANGLE:
			rect_c->x = a->x - TUILE / 2 + SHIFT;
			rect_c->y = a->y - TUILE / 2 + SHIFT;
			if (SDL_RenderFillRect(r, rect_c) != 0)
				SDL_ExitWithError("Impossible de dessiner un rectangle");
			break;
		default:
			break;
	}
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
		couleur.r = 180; couleur.g = 180; couleur.b = 180, couleur.a = 0;
	}
	if (c == JAUNE)
	{
		couleur.r = 250; couleur.g = 250; couleur.b = 0, couleur.a = 0;
	}
	if (c == NOIR)
	{
		couleur.r = 0; couleur.g = 0; couleur.b = 0, couleur.a = 0;
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
	EcrireTexte(r, "PAUSE", f, SHIFT + LARGEUR_TERRAIN / 2 - 75, SHIFT + HAUTEUR_TERRAIN / 2 - 50, 150, 100, JAUNE);
}

void AfficherPerdu(SDL_Renderer *r, TTF_Font *f)
{
	EcrireTexte(r, "PERDU", f, SHIFT + LARGEUR_TERRAIN / 2 - 75, SHIFT + HAUTEUR_TERRAIN / 2 - 50, 150, 100, JAUNE);
}

void AfficherFelecitations(SDL_Renderer *r, TTF_Font *f)
{
	EcrireTexte(r, "Niveau 8! FELICITATIONS!", f, SHIFT + LARGEUR_TERRAIN / 2 - 250, SHIFT + HAUTEUR_TERRAIN / 2 - 50, 500, 100, JAUNE);
	EcrireTexte(r, "Je pensais cela impossible! ... 'Echap' pour aller encore plus loin!", f, SHIFT + LARGEUR_TERRAIN / 2 - 350, SHIFT + HAUTEUR_TERRAIN / 2 + 50, 700, 50, JAUNE);
	
}

void AfficherCommandes(SDL_Renderer *r, TTF_Font *f)
{
	EcrireTexte(r, "Espace", f, LARGEUR_FENETRE - 230, HAUTEUR_FENETRE - 125, 60, 25, GRIS);
	EcrireTexte(r, "PAUSE", f, LARGEUR_FENETRE - 160, HAUTEUR_FENETRE - 125, 60, 25, GRIS);
	EcrireTexte(r, "Entrée", f, LARGEUR_FENETRE - 230, HAUTEUR_FENETRE - 100, 60, 25, GRIS);
	EcrireTexte(r, "RECOMMENCER", f, LARGEUR_FENETRE - 160, HAUTEUR_FENETRE - 100, 135, 25, GRIS);
	EcrireTexte(r, "Echap", f, LARGEUR_FENETRE - 230, HAUTEUR_FENETRE - 75, 50, 25, GRIS);
	EcrireTexte(r, "QUITTER", f, LARGEUR_FENETRE - 160, HAUTEUR_FENETRE - 75, 75, 25, GRIS);
}

void AfficherLegende(SDL_Renderer *r, TTF_Font *f)
{
	SDL_Rect Rouge = {.x = LARGEUR_FENETRE - 255, .y = HAUTEUR_FENETRE - 260, .w = TUILE, .h = TUILE};
	if (SDL_SetRenderDrawColor(r, 255, 0, 0, SDL_ALPHA_TRANSPARENT) != 0)
		SDL_ExitWithError("Impossible de changer la couleur du rendu");
	if (SDL_RenderFillRect(r, &Rouge) != 0)
			SDL_ExitWithError("Impossible de dessiner un rectangle");
	
	SDL_Rect Bleu = {.x = LARGEUR_FENETRE - 255, .y = HAUTEUR_FENETRE - 230, .w = TUILE, .h = TUILE};
	if (SDL_SetRenderDrawColor(r, 30, 144, 205, SDL_ALPHA_TRANSPARENT) != 0)
		SDL_ExitWithError("Impossible de changer la couleur du rendu");
	if (SDL_RenderFillRect(r, &Bleu) != 0)
			SDL_ExitWithError("Impossible de dessiner un rectangle");
	
	SDL_Rect Vert = {.x = LARGEUR_FENETRE - 255, .y = HAUTEUR_FENETRE - 200, .w = TUILE, .h = TUILE};
	if (SDL_SetRenderDrawColor(r, 0, 255, 0, SDL_ALPHA_TRANSPARENT) != 0)
		SDL_ExitWithError("Impossible de changer la couleur du rendu");
	if (SDL_RenderFillRect(r, &Vert) != 0)
			SDL_ExitWithError("Impossible de dessiner un rectangle");
	
	EcrireTexte(r, "t++", f, LARGEUR_FENETRE - 228, HAUTEUR_FENETRE - 260, TUILE, TUILE, JAUNE);
	EcrireTexte(r, "v++", f, LARGEUR_FENETRE - 228, HAUTEUR_FENETRE - 230, TUILE, TUILE, JAUNE);
	EcrireTexte(r, "v--", f, LARGEUR_FENETRE - 228, HAUTEUR_FENETRE - 200, TUILE, TUILE, JAUNE);
	
	EcrireTexte(r, "Feuilles d'automne", f, LARGEUR_FENETRE - 190, HAUTEUR_FENETRE - 260, 170, 25, BLANC);
	EcrireTexte(r, "Exsudats racinaires", f, LARGEUR_FENETRE - 190, HAUTEUR_FENETRE - 230, 180, 25, BLANC);
	EcrireTexte(r, "Pesticides", f, LARGEUR_FENETRE - 190, HAUTEUR_FENETRE - 200, 95, 25, BLANC);
}

void AfficherRecords(SDL_Renderer *r, TTF_Font *f, Records *rt)
{
	char record_pts[20];
	char record_nv[20];
	char record_t[20];
	sprintf(record_pts, "%05d points", rt->points);
	sprintf(record_nv, "niveau %d sur 8", rt->niveau);
	sprintf(record_t, "%02dh %02dm %02ds", rt->temps / 3600, (rt->temps % 3600) / 60, (rt->temps % 3600) % 60);
	
	EcrireTexte(r, "RECORDS", f, LARGEUR_TERRAIN / 2 + SHIFT - 40, HAUTEUR_FENETRE - 260, 80, 25, BLANC);
	EcrireTexte(r, record_pts, f, LARGEUR_TERRAIN / 2 + SHIFT - 60, HAUTEUR_FENETRE - 230, 120, 25, BLANC);
	EcrireTexte(r, record_nv, f, LARGEUR_TERRAIN / 2 + SHIFT - 65, HAUTEUR_FENETRE - 200, 130, 25, BLANC);
	EcrireTexte(r, record_t, f, LARGEUR_TERRAIN / 2 + SHIFT - 55, HAUTEUR_FENETRE - 170, 110, 25, BLANC);
	
	EcrireTexte(r, "Par Sébastien Abilla, Licence MIT", f, LARGEUR_TERRAIN + SHIFT - 260, HAUTEUR_FENETRE - SHIFT - 20, 260, 20, GRIS);
}

