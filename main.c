#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL_ttf.h>

#include "lombric.h"
#include "cadeau.h"
#include "affichage.h"
#include "controle.h"


int main(int argc, char *argv[])
{
//------------------Création d'une fenêtre et de son rendu--------------
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		SDL_ExitWithError("Initiation SDL");
		
	if (SDL_CreateWindowAndRenderer(LARGEUR_FENETRE, HAUTEUR_FENETRE, 0, &window, &renderer))
		SDL_ExitWithError("Impossible de creer la fenetre et le rendu");
	SDL_SetWindowTitle(window, TITRE);

//------------------Chargement de la police de texte--------------------

	if (TTF_Init() < 0)
		SDL_ExitWithError("Initiation TTF");
		
	TTF_Font *font = TTF_OpenFont("res/Orbitron.ttf", 100);
	if (!font)
		SDL_ExitWithError("Chargement de la police");

//------------------Chargement de l'icone-------------------------------
		
	SDL_Surface *image = NULL;
	SDL_Texture *textureimage = NULL;
	SDL_Rect rect_image = {.w = 100, .h = 100};
	rect_image.x = LARGEUR_FENETRE - (LARGEUR_FENETRE - HAUTEUR_FENETRE) / 2 - (rect_image.w / 2) ; 
	rect_image.y = HAUTEUR_FENETRE / 2;
	
	if ((image = SDL_LoadBMP("res/lombric.bmp")) == NULL)
		SDL_ExitWithError("Impossible de charger l'image");
	if ((textureimage = SDL_CreateTextureFromSurface(renderer, image)) == NULL)
		SDL_ExitWithError("Impossible de charger l'image");
	SDL_FreeSurface(image);

//------------------Chargement d'un éventuel joystic--------------------

	if (SDL_Init(SDL_INIT_JOYSTICK) != 0)
		SDL_ExitWithError("Initiation SDL");
	
	if (SDL_NumJoysticks() > 0)
		SDL_JoystickOpen(0);
	else
		printf("Pas de joystick\n");
		
//------------------Définition des compteurs et objets------------------
	
	SDL_bool program_launched = SDL_TRUE;
	srand(time(NULL)); // pour avoir un premier tirage differant à chaque jeu.

	unsigned int frame_limit = 0;
	
	Direction bouton = SANS;

	Lombric *lombric = NouveauLombric();
	Cadeau *cadeau = NULL;
	
	SDL_Rect fond_fenetre = {.x = 0, .y = 0, .w = LARGEUR_FENETRE, .h = HAUTEUR_FENETRE};
	SDL_Point bord[5] = {{SHIFT, SHIFT},
						{LARGEUR_TERRAIN + SHIFT, SHIFT},
						{LARGEUR_TERRAIN + SHIFT, HAUTEUR_TERRAIN + SHIFT},
						{SHIFT, HAUTEUR_TERRAIN + SHIFT},
						{SHIFT, SHIFT}};
	SDL_Rect rect_cadeau = {.w = TUILE, .h = TUILE};
	
	char score[10];
	char record[10];
	char temps[10];
	int record_temp = 0;
	int debut = SDL_GetTicks();
	int maintenant = SDL_GetTicks();
	int anim_niveau = 0;
	
//------------Lancement du jeu, en boucle infinie-----------------------
	
	while (program_launched)
	{
//------------Level up ?------------------------------------------------

		if (NiveauSupplementaire(lombric) == VRAI)
		{
			if (cadeau == NULL)
			{
				cadeau = NouveauCadeau(cadeau, lombric->tete); // Placé ici pour ne pas avoir de cadeau sur le premier lombric... fonctionne mieux, mais encore quelques fails...
				anim_niveau = -10000;
			}
			else
			{
				CadeauSupplementaire(cadeau, cadeau, lombric->tete);
				anim_niveau = SDL_GetTicks();
			}
		}
		
//------------Régénérer l'affichage dans la fenêtre---------------------
		
		if (SDL_RenderClear(renderer) != 0) 
			SDL_ExitWithError("Liberation du rendu echouee");
		
		if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE) != 0)
			SDL_ExitWithError("Impossible de changer la couleur du rendu");
		
		if (SDL_RenderFillRect(renderer, &fond_fenetre) != 0)
			SDL_ExitWithError("Impossible de dessiner un rectangle");
		
		if (SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
			SDL_ExitWithError("Impossible de changer la couleur du rendu");
			
		if (SDL_RenderDrawLines(renderer, bord, 5) != 0)
			SDL_ExitWithError("Impossible de dessiner un rectangle");
		
		
//------------Affichage des objets--------------------------------------

		AfficherCadeau(renderer, &rect_cadeau, cadeau);
		AfficherLombric(renderer, &rect_cadeau, lombric->tete);
		if (SDL_GetTicks() - anim_niveau < 1000)
			EcrireTexte(renderer, "Nv++", font, lombric->tete->x, lombric->tete->y - TUILE, 50, 60, JAUNE);	

//------------Affichage des informations--------------------------------				

		sprintf(temps, "%02d : %02d", maintenant / 60, maintenant % 60);
		EcrireTexte(renderer, temps, font, LARGEUR_FENETRE - 80, HAUTEUR_FENETRE - 25, 70, 25, BLANC);
		
		sprintf(score, "%5d", lombric->point);
		EcrireTexte(renderer, score, font, LARGEUR_FENETRE - 205, 40, 150, 100, BLANC);
		
		sprintf(record, "%5d", record_temp);
		EcrireTexte(renderer, record, font, LARGEUR_FENETRE - 50, 95, 48, 25, GRIS);
		
		if (SDL_RenderCopy(renderer, textureimage, NULL, &rect_image) != 0) // copier dans le rendu l'image en memoire
			SDL_ExitWithError("Impossible d'afficher la texture");			
		
		AfficherCommandes(renderer, font);
		
		AfficherLegende(renderer, font);
							
//------------Générer l'image et attendre le tick-----------------------

		SDL_RenderPresent(renderer);
			
		frame_limit = SDL_GetTicks() + FPS; 
		limit_fps(frame_limit);
		frame_limit = SDL_GetTicks() + FPS;

//------------Réagir à l'action du joueur-------------------------------
		
		bouton = EntreeJoueur();
		
		if (bouton == PAUSE)
		{
			maintenant = SDL_GetTicks();
			AfficherPause(renderer, font);
			SDL_RenderPresent(renderer);
			bouton = Attendre();
			debut += SDL_GetTicks() - maintenant;
		}
		
		if (bouton == FERMERFENETRE)
			program_launched = SDL_FALSE;
		
		ChangerDirection(bouton, lombric);
		
//------------Actualiser les compteurs----------------------------------

		if (lombric->pas > 0)
			lombric->pas -= lombric->vitesse;
			
		VieillirLombric(lombric->tete);
		
		lombric->longueur = LongueurLombric(lombric->tete);
		
		maintenant = (SDL_GetTicks() - debut) / 1000;

//------------Nouvelle position entraine-t'elle une collision ?---------		
		
		Bouger(lombric);
		
		if (CollisionCadeau(cadeau, lombric->tete->x, lombric->tete->y) == VRAI)
		{
			lombric->evm = GestionCadeau(cadeau, cadeau, lombric->tete);
			lombric->point += lombric->longueur / TUILE / 2 + 1;
		}
		
		DiminuerCompteur(cadeau, cadeau, lombric->tete);
			
		if ((CollisionTeteMur(lombric->tete) == VRAI) || 
			(CollisionLombric(lombric->tete, lombric->tete->x, lombric->tete->y, VRAI) == VRAI))
		{
			AfficherPerdu(renderer, font);
			SDL_RenderPresent(renderer);
			bouton = Attendre();
			if (bouton == PAUSE)
				bouton = COMMENCER;
			if (bouton == FERMERFENETRE)
				program_launched = SDL_FALSE;
		}
				
		if (bouton == COMMENCER)
		{
			if (record_temp < lombric->point)
				record_temp = lombric->point;
			LibererAnneaux(lombric->tete);
			free(lombric);
			lombric = NouveauLombric();
			LibererCadeaux(cadeau);
			cadeau = NULL;
			debut = SDL_GetTicks();
			bouton = SANS;
		}
		
	}

//------------Sortie de la boucle, on nettoie tout----------------------
	
	LibererCadeaux(cadeau);
	LibererAnneaux(lombric->tete);
	free(lombric);
	lombric = NULL;
	TTF_CloseFont(font);
	TTF_Quit();	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return EXIT_SUCCESS;
}


