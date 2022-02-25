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

	SDL_bool program_launched = SDL_TRUE;
	Partie partie = ENCOURS;

//------------------Chargement Sauvegarde-------------------------------	
	
	Records *records_temp = malloc(sizeof(Records));
	if (records_temp == NULL)
	{
		free(records_temp);
		printf("Erreur d'allocation mémoire\n");
		exit(EXIT_FAILURE);		
	}
	if (VerificationSauvegarde("sauvegarde.txt") == VRAI)
	{
		FILE *sauvegarde = fopen("sauvegarde.txt", "r");
		fscanf(sauvegarde, "%d", &records_temp->points);
		fscanf(sauvegarde, "%d", &records_temp->niveau);
		fscanf(sauvegarde, "%d", &records_temp->temps);
		fscanf(sauvegarde, "%d", &records_temp->longueur);
		fclose(sauvegarde);
	}
	else
	{
		printf("Impossible de creer une sauvegarde");
		exit(EXIT_FAILURE);
	}

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
	
	srand(time(NULL)); // pour avoir un premier tirage differant à chaque jeu.

	unsigned int frame_limit = 0;
	
	Direction bouton = SANS;

	Lombric *lombric = NouveauLombric();
	Lombric *lombric_affiche = NouveauLombric();
	
	Cadeau *cadeau = NULL;
	
	SDL_Rect fond_fenetre = {.x = 0, .y = 0, .w = LARGEUR_FENETRE, .h = HAUTEUR_FENETRE};
	SDL_Point bord[5] = {{SHIFT, SHIFT},
						{LARGEUR_TERRAIN + SHIFT, SHIFT},
						{LARGEUR_TERRAIN + SHIFT, HAUTEUR_TERRAIN + SHIFT},
						{SHIFT, HAUTEUR_TERRAIN + SHIFT},
						{SHIFT, SHIFT}};
	SDL_Rect rect_cadeau = {.w = TUILE, .h = TUILE};
	
	SDL_Texture *texture_commande[6] = {TextureTexte(renderer, "Espace", font, GRIS),
										TextureTexte(renderer, "PAUSE", font, GRIS),
										TextureTexte(renderer, "Entrée", font, GRIS),
										TextureTexte(renderer, "RECOMMENCER", font, GRIS),
										TextureTexte(renderer, "Echap", font, GRIS),
										TextureTexte(renderer, "QUITTER", font, GRIS)};
	SDL_Texture *texture_legende[6] = {TextureTexte(renderer, "t++", font, JAUNE),
										TextureTexte(renderer, "v++", font, JAUNE),
										TextureTexte(renderer, "v--", font, JAUNE),
										TextureTexte(renderer, "Feuilles d'automne", font, BLANC),
										TextureTexte(renderer, "Exsudats racinaires", font, BLANC),
										TextureTexte(renderer, "Pesticides", font, BLANC)};
	
	char score[10];
	char record[10];
	char temps[10];
	int debut = SDL_GetTicks();
	int pause_maintenant;
	int anim_niveau = 0;
	int felicitations = 0;
	int mv = 1;
	
	//------------Lancement du jeu, en boucle infinie-----------------------
	
	while (program_launched)
	{
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
		
//------------Affichage des informations--------------------------------				

		sprintf(temps, "%02d : %02d", lombric->maintenant / 60, lombric->maintenant % 60);
		EcrireTexteProvisoire(renderer, temps, font, LARGEUR_FENETRE - 80, HAUTEUR_FENETRE - 25, 70, 25, BLANC);
		
		sprintf(score, "%5d", lombric->point);
		EcrireTexteProvisoire(renderer, score, font, LARGEUR_FENETRE - 205, 40, 150, 100, BLANC);
		
		sprintf(record, "%5d", records_temp->points);
		EcrireTexteProvisoire(renderer, record, font, LARGEUR_FENETRE - 50, 95, 48, 25, GRIS);
		
		if (SDL_RenderCopy(renderer, textureimage, NULL, &rect_image) != 0) // copier dans le rendu l'image en memoire
			SDL_ExitWithError("Impossible d'afficher la texture");			
		
		AfficherCommandes(renderer, texture_commande);
		
		AfficherLegende(renderer, texture_legende);
		
//------------Que veut faire le joueur SDL_Event-------------------------------
		
		bouton = EntreeJoueur();
		
		if (partie == ENCOURS)
		{
			ChangerDirection(bouton, lombric);
			if (mv == TUILE - lombric->vitesse)
			{
				Bouger(lombric);
				CopieLombric(lombric, lombric_affiche);
				mv = 1;
			}
			else
				faux_mouvement(lombric_affiche->tete, lombric->vitesse);
			
			mv++;
			
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
		
//------------Nouvelle position entraine-t'elle une collision ?---------

			if (CollisionCadeau(cadeau, lombric->tete->x, lombric->tete->y) == VRAI)
			{
				lombric->evm = GestionCadeau(cadeau, cadeau, lombric->tete);
				lombric->point += lombric->longueur;
			}
						
			if ((CollisionTeteMur(lombric->tete) == VRAI) || 
				(CollisionLombric(lombric->tete, lombric->tete->x, lombric->tete->y, VRAI) == VRAI))
				partie = PERDU;

//------------Actualiser les compteurs----------------------------------

			DiminuerCompteur(cadeau, cadeau, lombric->tete);
				lombric->maintenant = (SDL_GetTicks() - debut) / 1000;
		
			if (lombric->niveau == 8 && felicitations == 0)
			{
				pause_maintenant = SDL_GetTicks();
				partie = VICTOIRE;
			}
				
			lombric->longueur = LongueurLombric(lombric->tete) - 1;		
		
			MiseAJourRecords(lombric, records_temp);	
		}		
					
//------------Affichage des objets--------------------------------------

		AfficherCadeau(renderer, &rect_cadeau, cadeau);
		AfficherLombric(renderer, &rect_cadeau, lombric_affiche->tete);
		if (SDL_GetTicks() - anim_niveau < 1000)
			EcrireTexteProvisoire(renderer, "Nv++", font, lombric_affiche->tete->x, lombric_affiche->tete->y - 2 * TUILE, 50, 60, JAUNE);	
			
//------------Partie arrêtée--------------------------------------------

		switch(partie)
		{
			case ENPAUSE:
				AfficherPause(renderer, font);
				AfficherRecords(renderer, font, records_temp);
				if (bouton == PAUSE)
				{
					debut += SDL_GetTicks() - pause_maintenant;
					partie = ENCOURS;
					bouton = SANS;
				}
				if (bouton == COMMENCER)
					partie = ENCOURS;
				break;
				
			case VICTOIRE:
				felicitations = 1;
				AfficherFelecitations(renderer, font);
				AfficherRecords(renderer, font, records_temp);
				if (bouton == PAUSE)
				{
					debut += SDL_GetTicks() - pause_maintenant;
					partie = ENCOURS;
					bouton = SANS;
				}
				if (bouton == COMMENCER)
					partie = ENCOURS;
				break;
			
			case PERDU:
				AfficherPerdu(renderer, font);
				AfficherRecords(renderer, font, records_temp);
				if (bouton == COMMENCER)
					partie = ENCOURS;
				break;
			default:
				break;				
		}
		
		if (bouton == FERMERFENETRE)
			program_launched = SDL_FALSE;
		
		if (partie == ENCOURS && bouton == PAUSE)
		{
			partie = ENPAUSE;
			bouton = SANS;
			pause_maintenant = SDL_GetTicks();
		}
			
		if (partie == ENCOURS && bouton == COMMENCER)
		{
			LibererAnneaux(lombric->tete);
			free(lombric);
			lombric = NouveauLombric();
			LibererCadeaux(cadeau);
			cadeau = NULL;
			debut = SDL_GetTicks();
			bouton = SANS;
			felicitations = 0;
		}
								
//------------Générer l'image et attendre le tick-----------------------

		SDL_RenderPresent(renderer);
		
		frame_limit = SDL_GetTicks() + FPS; 
		limit_fps(frame_limit);
		frame_limit = SDL_GetTicks() + FPS;
		
	}
	
//------------------Sauvegarder les records ----------------------------

	FILE *sauvegarde = fopen("sauvegarde.txt", "w");
	fprintf(sauvegarde, "%d\n", records_temp->points);
	fprintf(sauvegarde, "%d\n", records_temp->niveau);
	fprintf(sauvegarde, "%d\n", records_temp->temps);
	fprintf(sauvegarde, "%d\n", records_temp->longueur);
	printf("sauvegarde mise a jour\n");
	fclose(sauvegarde);


//------------Sortie de la boucle, on nettoie tout----------------------
	
	for (int i = 0; i < 6; i++)
		SDL_DestroyTexture(texture_commande[i]);
	for (int i = 0; i < 6; i++)
		SDL_DestroyTexture(texture_legende[i]);
		
	LibererCadeaux(cadeau);
	LibererAnneaux(lombric->tete);
	LibererAnneaux(lombric_affiche->tete);
	free(lombric);
	free(lombric_affiche);
	free(records_temp);	
	TTF_CloseFont(font);
	TTF_Quit();	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return EXIT_SUCCESS;
}


