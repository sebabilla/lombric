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
		
	if (a->tete == VRAI)
	{
		SDL_Rect rect_tete = {.w = 12, .h = 12};
		SDL_Rect rect_oeil = {.w = 10, .h = 10};
		
		switch(a->dir)
		{
			case HAUT:
				rect_tete.x = a->x + 6; rect_tete.y = a->y + 13;
				if (SDL_SetRenderDrawColor(r, 255, 180, 195, SDL_ALPHA_OPAQUE) != 0)
					SDL_ExitWithError("Impossible de changer la couleur du rendu");	
				if (SDL_RenderFillRect(r, &rect_tete) != 0)
					SDL_ExitWithError("Impossible de dessiner un rectangle");
				if (SDL_SetRenderDrawColor(r, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
					SDL_ExitWithError("Impossible de changer la couleur du rendu");	
				rect_oeil.x = a->x - 5; rect_oeil.y = a->y + 15;
				if (SDL_RenderFillRect(r, &rect_oeil) != 0)
					SDL_ExitWithError("Impossible de dessiner un rectangle");
				rect_oeil.x = a->x + 20; rect_oeil.y = a->y + 15;
				if (SDL_RenderFillRect(r, &rect_oeil) != 0)
					SDL_ExitWithError("Impossible de dessiner un rectangle");
				break;
			case BAS:
				rect_tete.x = a->x + 6; rect_tete.y = a->y;
				if (SDL_SetRenderDrawColor(r, 255, 180, 195, SDL_ALPHA_OPAQUE) != 0)
					SDL_ExitWithError("Impossible de changer la couleur du rendu");	
				if (SDL_RenderFillRect(r, &rect_tete) != 0)
					SDL_ExitWithError("Impossible de dessiner un rectangle");
				if (SDL_SetRenderDrawColor(r, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
					SDL_ExitWithError("Impossible de changer la couleur du rendu");	
				rect_oeil.x = a->x - 5; rect_oeil.y = a->y;
				if (SDL_RenderFillRect(r, &rect_oeil) != 0)
					SDL_ExitWithError("Impossible de dessiner un rectangle");
				rect_oeil.x = a->x + 20; rect_oeil.y = a->y;
				if (SDL_RenderFillRect(r, &rect_oeil) != 0)
					SDL_ExitWithError("Impossible de dessiner un rectangle");
				break;
			case GAUCHE:
				rect_tete.x = a->x + 13; rect_tete.y = a->y + 6;
				if (SDL_SetRenderDrawColor(r, 255, 180, 195, SDL_ALPHA_OPAQUE) != 0)
					SDL_ExitWithError("Impossible de changer la couleur du rendu");	
				if (SDL_RenderFillRect(r, &rect_tete) != 0)
					SDL_ExitWithError("Impossible de dessiner un rectangle");
				if (SDL_SetRenderDrawColor(r, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
					SDL_ExitWithError("Impossible de changer la couleur du rendu");	
				rect_oeil.x = a->x + 15; rect_oeil.y = a->y - 5;
				if (SDL_RenderFillRect(r, &rect_oeil) != 0)
					SDL_ExitWithError("Impossible de dessiner un rectangle");
				rect_oeil.x = a->x + 15; rect_oeil.y = a->y + 20;
				if (SDL_RenderFillRect(r, &rect_oeil) != 0)
					SDL_ExitWithError("Impossible de dessiner un rectangle");
				break;
			case DROITE:
				rect_tete.x = a->x; rect_tete.y = a->y + 6;
				if (SDL_SetRenderDrawColor(r, 255, 180, 195, SDL_ALPHA_OPAQUE) != 0)
					SDL_ExitWithError("Impossible de changer la couleur du rendu");	
				if (SDL_RenderFillRect(r, &rect_tete) != 0)
					SDL_ExitWithError("Impossible de dessiner un rectangle");
				if (SDL_SetRenderDrawColor(r, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
					SDL_ExitWithError("Impossible de changer la couleur du rendu");	
				rect_oeil.x = a->x; rect_oeil.y = a->y - 5;
				if (SDL_RenderFillRect(r, &rect_oeil) != 0)
					SDL_ExitWithError("Impossible de dessiner un rectangle");
				rect_oeil.x = a->x; rect_oeil.y = a->y + 20;
				if (SDL_RenderFillRect(r, &rect_oeil) != 0)
					SDL_ExitWithError("Impossible de dessiner un rectangle");
				break;
			default:
				break;
		}
	}
	else
	{
		rect_c->x = a->x;
		rect_c->y = a->y;		
		if (SDL_SetRenderDrawColor(r, 255, 180, 195, SDL_ALPHA_OPAQUE) != 0)
			SDL_ExitWithError("Impossible de changer la couleur du rendu");	
		if (SDL_RenderFillRect(r, rect_c) != 0)
			SDL_ExitWithError("Impossible de dessiner un rectangle");
		if (SDL_SetRenderDrawColor(r, 255, 0, 111, SDL_ALPHA_OPAQUE) != 0)
			SDL_ExitWithError("Impossible de changer la couleur du rendu");	
		if (SDL_RenderDrawRect(r, rect_c) != 0)
			SDL_ExitWithError("Impossible de dessiner un rectangle");
	}
}

SDL_Texture *TextureTexte(SDL_Renderer *r, char *texte, TTF_Font *f, CouleurTexte c)
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
									
	SDL_Surface *surface_message = TTF_RenderUTF8_Solid(f, texte, couleur);
	SDL_Texture *message = SDL_CreateTextureFromSurface(r, surface_message);
	SDL_FreeSurface(surface_message);
	
	return message;
}

void EcrireTexteProvisoire(SDL_Renderer *r, char *texte, TTF_Font *f, int X, int Y, int W, int H, CouleurTexte c)
{	
	SDL_Texture *message = TextureTexte(r, texte, f, c);	
	SDL_Rect MessageRect = {.x = X, .y = Y, .w = W, .h = H};
	SDL_RenderCopy(r, message, NULL, &MessageRect);
	SDL_DestroyTexture(message);
}

void AfficherPause(SDL_Renderer *r, TTF_Font *f)
{
	EcrireTexteProvisoire(r, "PAUSE", f, SHIFT + LARGEUR_TERRAIN / 2 - 75, SHIFT + HAUTEUR_TERRAIN / 2 - 50, 150, 100, JAUNE);
}

void AfficherPerdu(SDL_Renderer *r, TTF_Font *f)
{
	EcrireTexteProvisoire(r, "PERDU", f, SHIFT + LARGEUR_TERRAIN / 2 - 75, SHIFT + HAUTEUR_TERRAIN / 2 - 50, 150, 100, JAUNE);
}

void AfficherFelecitations(SDL_Renderer *r, TTF_Font *f)
{
	EcrireTexteProvisoire(r, "Niveau 8! FELICITATIONS!", f, SHIFT + LARGEUR_TERRAIN / 2 - 250, SHIFT + HAUTEUR_TERRAIN / 2 - 50, 500, 100, JAUNE);
	EcrireTexteProvisoire(r, "Je pensais cela impossible! 'Espace' pour aller encore plus loin!", f, SHIFT + LARGEUR_TERRAIN / 2 - 350, SHIFT + HAUTEUR_TERRAIN / 2 + 50, 700, 50, JAUNE);
	
}

void AfficherCommandes(SDL_Renderer *r, SDL_Texture **text_t)
{
	SDL_Rect message_rect1 = {.x = LARGEUR_FENETRE - 230, .y = HAUTEUR_FENETRE - 125, .w = 60, .h = 25};
	SDL_RenderCopy(r, text_t[0], NULL, &message_rect1);
	SDL_Rect message_rect2 = {.x = LARGEUR_FENETRE - 230, .y = HAUTEUR_FENETRE - 100, .w = 60, .h = 25};
	SDL_RenderCopy(r, text_t[1], NULL, &message_rect2);
	SDL_Rect message_rect3 = {.x = LARGEUR_FENETRE - 160, .y = HAUTEUR_FENETRE - 125, .w = 60, .h = 25};
	SDL_RenderCopy(r, text_t[2], NULL, &message_rect3);
	SDL_Rect message_rect4 = {.x = LARGEUR_FENETRE - 160, .y = HAUTEUR_FENETRE - 100, .w = 135, .h = 25};
	SDL_RenderCopy(r, text_t[3], NULL, &message_rect4);
	SDL_Rect message_rect5 = {.x = LARGEUR_FENETRE - 230, .y = HAUTEUR_FENETRE - 75, .w = 50, .h = 25};
	SDL_RenderCopy(r, text_t[4], NULL, &message_rect5);
	SDL_Rect message_rect6 = {.x = LARGEUR_FENETRE - 160, .y = HAUTEUR_FENETRE - 75, .w = 75, .h = 25};
	SDL_RenderCopy(r, text_t[5], NULL, &message_rect6);
}

void AfficherLegende(SDL_Renderer *r, SDL_Texture **text_t)
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
	
	SDL_Rect message_rect1 = {.x = LARGEUR_FENETRE - 228, .y = HAUTEUR_FENETRE - 260, .w = TUILE, .h = TUILE};
	SDL_RenderCopy(r, text_t[0], NULL, &message_rect1);
	SDL_Rect message_rect2 = {.x = LARGEUR_FENETRE - 228, .y = HAUTEUR_FENETRE - 230, .w = TUILE, .h = TUILE};
	SDL_RenderCopy(r, text_t[1], NULL, &message_rect2);
	SDL_Rect message_rect3 = {.x = LARGEUR_FENETRE - 228, .y = HAUTEUR_FENETRE - 200, .w = TUILE, .h = TUILE};
	SDL_RenderCopy(r, text_t[2], NULL, &message_rect3);
	SDL_Rect message_rect4 = {.x = LARGEUR_FENETRE - 190, .y = HAUTEUR_FENETRE - 260, .w = 170, .h = 25};
	SDL_RenderCopy(r, text_t[3], NULL, &message_rect4);
	SDL_Rect message_rect5 = {.x = LARGEUR_FENETRE - 190, .y = HAUTEUR_FENETRE - 230, .w = 180, .h = 25};
	SDL_RenderCopy(r, text_t[4], NULL, &message_rect5);
	SDL_Rect message_rect6 = {.x = LARGEUR_FENETRE - 190, .y = HAUTEUR_FENETRE - 200, .w = 95, .h = 25};
	SDL_RenderCopy(r, text_t[5], NULL, &message_rect6);

}

void AfficherRecords(SDL_Renderer *r, TTF_Font *f, Records *rt)
{
	char record_pts[20];
	char record_nv[20];
	char record_t[20];
	char record_lg[20];
	sprintf(record_pts, "%05d points", rt->points);
	sprintf(record_nv, "niveau %d sur 8", rt->niveau);
	sprintf(record_t, "%02dh %02dm %02ds", rt->temps / 3600, (rt->temps % 3600) / 60, (rt->temps % 3600) % 60);
	sprintf(record_lg, "%03d anneaux", rt->longueur);
	
	EcrireTexteProvisoire(r, "RECORDS", f, LARGEUR_TERRAIN / 2 + SHIFT - 40, HAUTEUR_FENETRE - 260, 80, 25, BLANC);
	EcrireTexteProvisoire(r, record_pts, f, LARGEUR_TERRAIN / 2 + SHIFT - 60, HAUTEUR_FENETRE - 230, 120, 25, BLANC);
	EcrireTexteProvisoire(r, record_nv, f, LARGEUR_TERRAIN / 2 + SHIFT - 65, HAUTEUR_FENETRE - 200, 130, 25, BLANC);
	EcrireTexteProvisoire(r, record_t, f, LARGEUR_TERRAIN / 2 + SHIFT - 55, HAUTEUR_FENETRE - 170, 110, 25, BLANC);
	EcrireTexteProvisoire(r, record_lg, f, LARGEUR_TERRAIN / 2 + SHIFT - 60, HAUTEUR_FENETRE - 140, 120, 25, BLANC);
	
	EcrireTexteProvisoire(r, "Par Sébastien Abilla, Licence MIT", f, LARGEUR_TERRAIN + SHIFT - 260, HAUTEUR_FENETRE - SHIFT - 20, 260, 20, GRIS);
}

