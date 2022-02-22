#include <stdio.h>
#include <stdlib.h>
#include "lombric.h"

Evenement **NouveauTerrain(void) 
{
	Evenement **t = (Evenement**)malloc(sizeof(Evenement *) * TERRAIN_HEIGHT + sizeof(Evenement) * TERRAIN_WIDTH * TERRAIN_HEIGHT);
	if (t == NULL)
	{
		free(t);
		printf("Erreur d'allocation mémoire\n");
		return NULL;
	}
	
	Evenement *ptr = (Evenement *)(t + TERRAIN_HEIGHT);
	for(int i = 0; i < TERRAIN_HEIGHT; i++)
        t[i] = (ptr + TERRAIN_WIDTH * i);
	
	NettoyerTerrain(t);

	return t;	
}

void NettoyerTerrain(Evenement **t)
{
	for (int i = TUILE; i < TERRAIN_WIDTH - TUILE; i++)
		for (int j = TUILE; j < TERRAIN_HEIGHT - TUILE; j++)
			t[i][j] = RIEN;
	
	for (int i = 0; i < TERRAIN_WIDTH; i++)
		for (int j = 0; j < TUILE; j++)
		{
	
			t[i][j] = BORD;
			t[i][TERRAIN_HEIGHT - TUILE + j] = BORD;
		}
	for (int j = 0; j < TERRAIN_HEIGHT; j++)
		for (int i = 0; i < TUILE; i++)
		{
	
			t[i][j] = BORD;
			t[TERRAIN_WIDTH - TUILE + i][j] = BORD;
		}
}

Lombric *NouveauLombric(void)
{
	Anneau *a = malloc(sizeof(Anneau));
	if (a == NULL)
	{
		free(a);
		printf("Erreur d'allocation mémoire\n");
		return NULL;
	}
	
	a->x = TERRAIN_WIDTH / 2;
	a->y = TERRAIN_HEIGHT / 2;
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
	l->vitesse = 2;
	l->longueur = 0;
	l->point = 0;
	l->niveau = 0;
	l->tete = a;

	for (int i = 0; i < (2 * TUILE); i++)
		AjouterQueue(l->tete);

	return l;
	
}

Anneau *AjouterQueue(Anneau *a)
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

		n->age = 1;
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
	
	AjouterQueue(a->suivant);
	return a;	
}

void InsererLombric(Evenement **t, Anneau *a)
{
	if(a->suivant != NULL)
		InsererLombric(t, a->suivant);
		
	if(a->suivant == NULL)
		PoserLosange(t, a, LOMBRICAGE);
	
	switch(a->evm)
	{
		case LOMBRIC:
			if (a->age == 1)
			{
				if (a->dir == GAUCHE || a->dir == DROITE)
					for (int i = 0; i < TUILE; i++)
						t[a->x][(a->y) - TUILE / 2 + i] = LOMBRICAGE;		
				else
					for (int i = 0; i < TUILE; i++)
						t[(a->x) - TUILE / 2 + i][a->y] = LOMBRICAGE;
			}
			else 
			{
				PoserLosange(t, a, LOMBRIC);
			}
			break;
		case LANGLE:
			PoserLosange(t, a, LOMBRIC);
			break;
		default:
			break;
	}	
}

void PoserLosange(Evenement **t, Anneau *a, Evenement e)
{
	for (int i = - TUILE / 2 ; i <=  0; i++)
				for (int j = 0; j >= - TUILE / 2 - i; j--)
					t[(a->x) + i][(a->y) + j] = e;
			for (int i = 0; i <=  TUILE / 2; i++)
				for (int j = 0; j >= - TUILE / 2 + i; j--)
					t[(a->x) + i][(a->y) + j] = e;
			for (int i = 0; i <= TUILE / 2; i++)
				for (int j = 0; j <= TUILE / 2 - i; j++)
					t[(a->x) + i][(a->y) + j] = e;
			for (int i = - TUILE / 2 ; i <=  0; i++)
				for (int j = 0; j <= TUILE / 2 + i; j++)
					t[(a->x) + i][(a->y) + j] = e;
}

void LibererAnneaux(Anneau *a)
{
	if(a->suivant != NULL)
		LibererAnneaux(a->suivant);
		
	free(a);
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
				AjouterQueue(l->tete);
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
	if (a->x < TUILE)
		return VRAI;
	if (a->x > TERRAIN_WIDTH - TUILE)
		return VRAI;
	if (a->y < TUILE)
		return VRAI;
	if (a->y > TERRAIN_HEIGHT - TUILE)
		return VRAI;
	return FAUX;
}

Bool CollisionTeteLombric(Anneau *a, Evenement **t)
{
	if (t[a->x][a->y] == LOMBRICAGE)
		return VRAI;
	return FAUX;
}

Bool CollisionTeteCadeau(Anneau *a, Evenement **t)
{
	if (a->dir == GAUCHE || a->dir == DROITE)
		for (int i = - TUILE / 2; i < TUILE / 2; i++)
			if (t[a->x][a->y + i] != RIEN && t[a->x][a->y + i] != BORD && t[a->x][a->y + i] != LOMBRIC && t[a->x][a->y + i] != LOMBRICAGE)
				return VRAI;
	if (a->dir == HAUT || a->dir == BAS)
		for (int i = - TUILE / 2; i < TUILE / 2; i++)
			if (t[a->x + i][a->y] != RIEN && t[a->x + i][a->y] != BORD && t[a->x + i][a->y] != LOMBRIC && t[a->x + i][a->y] != LOMBRICAGE)
				return VRAI;
		
	return FAUX;
}

int LongueurLombric(Anneau *a)
{
	int conteur = 0;
	while (a->suivant != 0)
	{
		conteur++;
		a = a->suivant;
	}
	return conteur;
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

Cadeau *NouveauCadeau(Evenement **t)
{
	Cadeau *c = malloc(sizeof(Cadeau));
	if (c == NULL)
	{
		free(c);
		printf("Erreur d'allocation mémoire\n");
		return NULL;
	}
	
	c->suivant = NULL;
	ResetCadeau(c, t);
	
	return c;
}

void ResetCadeau(Cadeau *c, Evenement **t)
{
	int x, y;
	Evenement test = LOMBRIC;
	while (test != RIEN)
	{
		x = (rand() % ((TERRAIN_WIDTH - 2 * TUILE) / TUILE)) * TUILE + TUILE + 1;
		y = (rand() % ((TERRAIN_HEIGHT - 2 * TUILE) / TUILE)) * TUILE + TUILE + 1;
		for (int i = 0; i < TUILE; i++)
		{
			for (int j = 0; j < TUILE; j++)
			{
				test = t[x + i][y + j];
				if (test != RIEN)
					break;
			}
			if (test != RIEN)
				break;
		}
	}

	c->x = x;
	c->y = y;
	
	int choix = rand() % 10;
	if (choix < 6) 
	{ 
		c->evm[0] = MO;
		c->evm[1] = MODISP;
		c->evm[2] = MODISPARITION;
	}
	else if (choix < 9)
	{
		c->evm[0] = VITPLUS;
		c->evm[1] = VITPLUSDISP;
		c->evm[2] = VITPLUSDISPARITION;
	}
	else
	{
		c->evm[0] = VITMOINS;
		c->evm[1] = VITMOINSDISP;
		c->evm[2] = VITMOINSDISPARITION;
	}
	
	c->compteur = TEMPSCADEAU;
}

void CadeauSupplementaire(Cadeau *c, Evenement **t)
{
	if (c->suivant != NULL)
	{
		CadeauSupplementaire(c->suivant, t);
	}
	if (c->suivant == NULL)	
		c->suivant = NouveauCadeau(t);		
}

void InsererCadeau(Evenement **t, Cadeau *c)
{
	for (int i = 0; i < TUILE; i++)
		for (int j = 0; j < TUILE; j++)
		{
			if (c->compteur > TEMPSCADEAU / 2)
				t[(c->x) + i][(c->y) + j] = c->evm[0];
			else if (c->compteur < TEMPSCADEAU / 5)
				t[(c->x) + i][(c->y) + j] = c->evm[2];
			else
				t[(c->x) + i][(c->y) + j] = c->evm[1];
		}
	
	if (c->suivant != NULL)
		InsererCadeau(t, c->suivant);
		
}

Evenement GestionCadeau(Evenement **t, Anneau *a, Cadeau *c)
{	
	Evenement tmp;
	if (abs(a->x - c->x) < TUILE / 2 + TUILE && abs(a->y - c->y) < TUILE / 2 + TUILE)
	{
		tmp = c->evm[0];
		ResetCadeau(c, t);
		return tmp;		
	}
	
	return GestionCadeau(t, a, c->suivant);	
}

void DiminuerCompteur(Cadeau *c, Evenement **t)
{
	if(c->suivant != NULL)
		DiminuerCompteur(c->suivant, t);
		
	c->compteur--;
	if (c->compteur <= 0)
		ResetCadeau(c, t);
}

void LibererCadeaux(Cadeau *c)
{
	if(c->suivant != NULL)
		LibererCadeaux(c->suivant);
		
	free(c);
	c = NULL;
}
