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
	
	Evenement **terrain = NouveauTerrain(); 
	Lombric *lombric = NouveauLombric();
	Cadeau *cadeau = NULL;
	
	SDL_Rect fond_fenetre = {.x = 0, .y = 0, .w = LARGEUR_FENETRE, .h = HAUTEUR_FENETRE};
	SDL_Point bord[POINTS_COUNT] = {{TUILE, TUILE},
										{LARGEUR_TERRAIN - TUILE, TUILE},
										{LARGEUR_TERRAIN - TUILE, HAUTEUR_TERRAIN - TUILE},
										{TUILE, HAUTEUR_TERRAIN - TUILE}};
	
	char score[10];
	char record[10];
	char temps[10];
	int record_temp = 0;
	int debut = SDL_GetTicks();
	int maintenant = SDL_GetTicks();
	
//------------Lancement du jeu, en boucle infinie-----------------------
	
	while (program_launched)
	{
//------------Régénérer la matrice du terrain---------------------------
		
		NettoyerTerrain(terrain);
		InsererLombric(terrain, lombric->tete);
		
		if (NiveauSupplementaire(lombric) == VRAI)
		{
			if (cadeau == NULL)
				cadeau = NouveauCadeau(terrain); // Placé ici pour ne pas avoir de cadeau sur le lombric... fonctionne mieux, mais encore quelques fails...
			else
				CadeauSupplementaire(cadeau, terrain);
		}
		
		InsererCadeau(terrain, cadeau);
		
//------------Régénérer l'affichage dans la fenêtre---------------------
		
		if (SDL_RenderClear(renderer) != 0) 
			SDL_ExitWithError("Liberation du rendu echouee");
		
		if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE) != 0)
			SDL_ExitWithError("Impossible de changer la couleur du rendu");
		
		if (SDL_RenderFillRect(renderer, &fond_fenetre) != 0)
			SDL_ExitWithError("Impossible de dessiner un rectangle");
		
		if (SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
			SDL_ExitWithError("Impossible de changer la couleur du rendu");
			
		if (SDL_RenderDrawLines(renderer, bord, 4) != 0)
			SDL_ExitWithError("Impossible de dessiner un rectangle");
		
		
//------------Régénérer l'affichage de la matrice du terrain------------

		for (int i = 0; i < LARGEUR_TERRAIN; i ++)
			for (int j = 0; j < HAUTEUR_TERRAIN; j ++)
				AfficherTerrain(renderer, i, j, terrain[i][j]);

//------------Affichage des informations--------------------------------				

		sprintf(temps, " %5d s", maintenant);
		EcrireTexte(renderer, temps, font, LARGEUR_FENETRE - 80, HAUTEUR_FENETRE - 25, 70, 25, BLANC);
		
		sprintf(score, "%5d", lombric->point);
		EcrireTexte(renderer, score, font, LARGEUR_FENETRE - 195, 40, 150, 100, BLANC);
		
		sprintf(record, "%5d", record_temp);
		EcrireTexte(renderer, record, font, LARGEUR_FENETRE - 40, 95, 36, 25, GRIS);
		
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
			
		lombric->longueur = LongueurLombric(lombric->tete);
		
		maintenant = (SDL_GetTicks() - debut) / 1000;

//------------Nouvelle position entraine-t'elle une collision ?---------		
		
		Bouger(lombric);
		
		if (CollisionTeteCadeau(lombric->tete, terrain) == VRAI)
		{
			lombric->evm = GestionCadeau(terrain, lombric->tete, cadeau);
			lombric->point += lombric->longueur / TUILE / 2 + 1;
		}
		
		DiminuerCompteur(cadeau, terrain);
			
		if ((CollisionTeteMur(lombric->tete) == VRAI) || 
			(CollisionTeteLombric(lombric->tete, terrain) == VRAI))
		{
			AfficherPerdu(renderer, font);
			SDL_RenderPresent(renderer);
			bouton = Attendre();
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
	free(terrain);
	terrain = NULL;	
	TTF_CloseFont(font);
	TTF_Quit();	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return EXIT_SUCCESS;
}


