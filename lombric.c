#include <stdio.h>
#include <stdlib.h>
#include "lombric.h"

Bool VerificationSauvegarde(const char *s)
{
	FILE *sauvegarde;
	if ((sauvegarde = fopen(s, "r")))
	{
		printf("Sauvegarde trouvee\n");
		fclose(sauvegarde);
		return VRAI;
	}
		
	sauvegarde = fopen(s, "w");
	if (sauvegarde == NULL)
	{
		printf("impossible de creer une sauvegarde\n");
		exit(EXIT_FAILURE);
	}
	fprintf(sauvegarde, "%d\n", 0);
	fprintf(sauvegarde, "%d\n", 0);
	fprintf(sauvegarde, "%d\n", 0);
	fprintf(sauvegarde, "%d\n", 0);
	printf("sauvegarde creee\n");
	fclose(sauvegarde);
	return VRAI;
}

Lombric *NouveauLombric()
{
	Anneau *a = malloc(sizeof(Anneau));
	if (a == NULL)
	{
		free(a);
		printf("Erreur d'allocation mémoire\n");
		return NULL;
	}
	
	a->x = LARGEUR_TERRAIN / TUILE * TUILE / 2 + SHIFT; // pour être sur une tuile
	a->y = HAUTEUR_TERRAIN / TUILE * TUILE / 2 + SHIFT;
	a->tete = VRAI;
	a->dir = GAUCHE;
	a->evm = LOMBRIC;
	a->suivant = NULL;
	
	Lombric *l = malloc(sizeof(Lombric));
	if (l == NULL)
	{
		free(l);
		free(a);
		printf("Erreur d'allocation mémoire\n");
		return NULL;
	}
	
	l->evm = RIEN;
	l->vitesse = 15;
	l->longueur = 0;
	l->point = 0;
	l->niveau = 0;
	l->maintenant = 0;
	l->tete = a;

	for (int i = 0; i < 2; i++)
		AjouterQueue(l->tete, VRAI);

	return l;
	
}

void CopieLombric(Lombric *loriginal, Lombric *lcopie)
{
	LibererAnneaux(lcopie->tete);
	lcopie->tete = CopierAnneaux(loriginal->tete);	
}

Anneau *CopierAnneaux(Anneau *a)
{
	if (a == NULL)
		return NULL;
	
	Anneau *nouveau = malloc(sizeof(Anneau));
	if (nouveau == NULL)
	{
		free(nouveau);
		printf("Erreur d'allocation mémoire\n");
		return NULL;
	}
  
    nouveau->x = a->x;
    nouveau->y = a->y;
    nouveau->tete = a->tete;
	nouveau->dir = a->dir;
        
    nouveau->suivant = CopierAnneaux(a->suivant);
  
    return nouveau;
}

Anneau *AjouterQueue(Anneau *a, Bool nouveau)
{
	if (a->suivant == NULL)
	{
		Anneau *n = malloc(sizeof(Anneau));
		if (n == NULL)
		{
			free(n);
			printf("Erreur d'allocation mémoire\n");
			return NULL;
		}
		
		n->dir = a->dir;
		n->evm = LOMBRIC;
		n->suivant = NULL;
		
		switch(n->dir)
		{
			case GAUCHE:
				n->x = a->x + TUILE;
				n->y = a->y;
				break;
			case DROITE:
				n->x = a->x - TUILE;
				n->y = a->y;
				break;
			case HAUT:
				n->x = a->x;
				n->y = a->y + TUILE;
				break;
			case BAS:
				n->x = a->x;
				n->y = a->y - TUILE;
				break;
			default:
				break;
		}
		
		a->suivant = n;
		
		return a;
	}
	
	AjouterQueue(a->suivant, nouveau);
	return a;	
}

void LibererAnneaux(Anneau *a)
{
	if(a->suivant != NULL)
		LibererAnneaux(a->suivant);
		
	free(a);
	a = NULL;
}

Lombric *NouvelleTete(Lombric *l)
{
	Anneau *a = malloc(sizeof(Anneau));
	if (a == NULL)
	{
		free(a);
		printf("Erreur d'allocation mémoire\n");
		return NULL;
	}
	
	switch (l->tete->dir)
	{
		case HAUT:
			a->x = l->tete->x;
			a->y = l->tete->y - TUILE;
			break;
		case BAS:
			a->x = l->tete->x;
			a->y = l->tete->y + TUILE;
			break;
		case GAUCHE:
			a->x = l->tete->x - TUILE;
			a->y = l->tete->y;
			break;
		case DROITE:
			a->x = l->tete->x + TUILE;
			a->y = l->tete->y;
			break;
		default:
			printf("Erreur: Le lombric n'a plus de direction'\n");
			return NULL;
	}
	l->tete->tete = FAUX;
	a->tete = VRAI;
	a->dir = l->tete->dir;
	a->suivant = l->tete;
	l->tete = a;
	return l;
}

void DetruireQueue(Anneau *a)
{
	if(a->suivant->suivant == NULL)
	{
		free(a->suivant);
		a->suivant = NULL;
		return;		
	}		
	DetruireQueue(a->suivant);	
}

void ChangerDirection(int b, Lombric *l)
{
	switch(b)
		{
			case HAUT:
				if (l->tete->dir != BAS)
					l->tete->dir = HAUT;
				break;
			case BAS:
				if (l->tete->dir != HAUT)
					l->tete->dir = BAS;
				break;
			case GAUCHE:
				if (l->tete->dir != DROITE)
					l->tete->dir = GAUCHE;
				break;
			case DROITE:
				if (l->tete->dir != GAUCHE)
					l->tete->dir = DROITE;
				break;
			default:
				break;
		}
	
}

Lombric *Bouger(Lombric *l)
{
	NouvelleTete(l);
	
	switch(l->evm)
	{
		case RIEN:
			DetruireQueue(l->tete);
			break;
		case MO:
			l->evm = RIEN;
			break;
		case VITPLUS:
			l->evm = RIEN;
			l->vitesse++;
			if (l->vitesse > TUILE - 4)
				l->vitesse = TUILE - 4;
			DetruireQueue(l->tete);
			break;
		case VITMOINS:
			l->evm = RIEN;
			l->vitesse--;
			if (l->vitesse < 10);
			DetruireQueue(l->tete);
			break;
		default:
			break;
	}
		
	return l;
}

void faux_mouvement(Anneau *acopie, int v)
{
	if (acopie->suivant != NULL)
		faux_mouvement(acopie->suivant, v);
	
	switch(acopie->dir)
		{
			case HAUT:
				acopie->y -= (float)TUILE / (TUILE - v);
				break;
			case BAS:
				acopie->y += (float)TUILE / (TUILE - v);
				break;
			case GAUCHE:
				acopie->x -= (float)TUILE / (TUILE - v);
				break;
			case DROITE:
				acopie->x += (float)TUILE / (TUILE - v);
				break;
			default:
				break;
		}
}

Bool CollisionTeteMur(Anneau *a)
{
	if (a->x < 0)
		return VRAI;
	if (a->x > LARGEUR_TERRAIN)
		return VRAI;
	if (a->y < 0)
		return VRAI;
	if (a->y > HAUTEUR_TERRAIN)
		return VRAI;
	return FAUX;
}

Bool CollisionLombric(Anneau *a, int x, int y, Bool vieux)
{	
	if (vieux != VRAI || a->tete != VRAI)
		if ((a->x == x && abs(a->y - y) < TUILE) ||
			(a->y == y && abs(a->x - x) < TUILE))
				return VRAI;
	
	if (a->suivant != 0)
		return CollisionLombric(a->suivant, x, y, vieux);
	
	return FAUX;
}

int LongueurLombric(Anneau *a)
{
	if (a->suivant == NULL)
		return 1;
		
	return 	1 + LongueurLombric(a->suivant);
}

Bool NiveauSupplementaire(Lombric *l)
{
	Bool sup = FAUX;
	
	switch(l->niveau)
	{
		case 0:
			sup = VRAI;
			break;
		case 1:
			if (l->point >= 10)
				sup = VRAI;
			break;
		case 2:
			if (l->point >= 50)
				sup = VRAI;
			break;
		case 3:
			if (l->point >= 100)
				sup = VRAI;
			break;
		case 4:
			if (l->point >= 500)
				sup = VRAI;
			break;
		case 5:
			if (l->point >= 1000)
				sup = VRAI;
			break;
		case 6:
			if (l->point >= 5000)
				sup = VRAI;
			break;
		case 7:
			if (l->point >= 10000)
				sup = VRAI;
			break;
		case 8:
			if (l->point >= 50000)
				sup = VRAI;
			break;
		default:
			break;		
	}
	
	if (sup == VRAI)
	{
		l->niveau++;
		return VRAI;
	}
		
	return FAUX;
}

/*
void VieillirLombric(Anneau *a)
{
	a->age++;
	
	if (a->suivant != NULL)
		VieillirLombric(a->suivant);
}
*/

void MiseAJourRecords(Lombric *l, Records *rt)
{
	if (rt->points < l->point)
		rt->points = l->point;
	if (rt->niveau < l->niveau)
		rt->niveau = l->niveau;
	if (rt->temps < l->maintenant)
		rt->temps = l->maintenant;
	if (rt->longueur < l->longueur)
		rt->longueur = l->longueur;
}
