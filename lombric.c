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
	
	a->x = LARGEUR_TERRAIN / 2;
	a->y = HAUTEUR_TERRAIN / 2;
	a->age = 0;
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
	l->pas = 0;
	l->vitesse = 3;
	l->longueur = 0;
	l->point = 0;
	l->niveau = 0;
	l->maintenant = 0;
	l->tete = a;

	for (int i = 0; i < (2 * TUILE); i++)
		AjouterQueue(l->tete, VRAI);

	return l;
	
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
		
		if (nouveau == VRAI)
			n->age = 0;
		else
			n->age = 2 * TUILE;
		
		n->dir = a->dir;
		n->evm = LOMBRIC;
		n->suivant = NULL;
		
		switch(n->dir)
		{
			case GAUCHE:
				n->x = a->x + 1;
				n->y = a->y;
				break;
			case DROITE:
				n->x = a->x - 1;
				n->y = a->y;
				break;
			case HAUT:
				n->x = a->x;
				n->y = a->y + 1;
				break;
			case BAS:
				n->x = a->x;
				n->y = a->y - 1;
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
			a->y = l->tete->y - 1;
			break;
		case BAS:
			a->x = l->tete->x;
			a->y = l->tete->y + 1;
			break;
		case GAUCHE:
			a->x = l->tete->x - 1;
			a->y = l->tete->y;
			break;
		case DROITE:
			a->x = l->tete->x + 1;
			a->y = l->tete->y;
			break;
		default:
			printf("Erreur: Le lombric n'a plus de direction'\n");
			return NULL;
	}

	if (l->tete->dir == l->tete->suivant->dir)
		a->evm = LOMBRIC;
	else
	{
		if (l->tete->dir != l->tete->suivant->dir)
			a->evm = LANGLE;
	}
	l->tete->suivant->age = 1;
	a->age = 0;
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
				if (l->tete->dir != BAS && l->pas <= 0)
				{
					l->tete->dir = HAUT;
					l->pas = TUILE + 1;
				}
				break;
			case BAS:
				if (l->tete->dir != HAUT && l->pas <= 0)
				{
					l->tete->dir = BAS;
					l->pas = TUILE + 1;
				}
				break;
			case GAUCHE:
				if (l->tete->dir != DROITE && l->pas <= 0)
				{
					l->tete->dir = GAUCHE;
					l->pas = TUILE + 1;
				}
				break;
			case DROITE:
				if (l->tete->dir != GAUCHE && l->pas <= 0)
				{
					l->tete->dir = DROITE;
					l->pas = TUILE + 1;
				}
				break;
			default:
				break;
		}
	
}

Lombric *Bouger(Lombric *l)
{
	for (int i = 0; i < l->vitesse; i++)
		NouvelleTete(l);
	
	switch(l->evm)
	{
		case RIEN:
			for (int i = 0; i < l->vitesse; i++)
				DetruireQueue(l->tete);
			break;
		case MO:
			l->evm = RIEN;
			for (int i = 0; i < TUILE; i ++)
				AjouterQueue(l->tete, FAUX);
			break;
		case VITPLUS:
			l->evm = RIEN;
			l->vitesse++;
			if (l->vitesse > TUILE)
				l->vitesse = TUILE;
			for (int i = 0; i < l->vitesse; i++)
				DetruireQueue(l->tete);
			break;
		case VITMOINS:
			l->evm = RIEN;
			l->vitesse--;
			if (l->vitesse < 1)
				l->vitesse = 1;
			for (int i = 0; i < l->vitesse; i++)
				DetruireQueue(l->tete);
			break;
		default:
			break;
	}
		
	return l;
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
	if (vieux == VRAI)
		if (a->age > TUILE && abs(a->x - x) < TUILE / 2 && abs(a->y - y) < TUILE / 2)
			return VRAI;
	if (vieux == FAUX)
		if (abs(a->x - x) < 2 * TUILE && abs(a->y - y) < 2 * TUILE)
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

void VieillirLombric(Anneau *a)
{
	a->age++;
	
	if (a->suivant != NULL)
		VieillirLombric(a->suivant);
}

void MiseAJourRecords(Lombric *l, Records *rt)
{
	if (rt->points < l->point)
		rt->points = l->point;
	if (rt->niveau < l->niveau)
		rt->niveau = l->niveau;
	if (rt->temps < l->maintenant)
		rt->temps = l->maintenant;
}
