#include <stdio.h>
#include <stdlib.h>
#include "lombric.h"
#include "cadeau.h"

Cadeau *NouveauCadeau(Cadeau *c, Anneau *a)
{
	Cadeau *n = malloc(sizeof(Cadeau));
	if (n== NULL)
	{
		free(n);
		printf("Erreur d'allocation mémoire\n");
		return NULL;
	}
	
	n->x = 0;	
	n->y = 0;
	n->suivant = NULL;
	ResetCadeau(n, c, a);
	
	return n;
}

void ResetCadeau(Cadeau *c, Cadeau *testc, Anneau *testa)
{
	int x, y;
	Bool tests = VRAI;
	while (tests == VRAI)
	{
		x = (rand() % ((LARGEUR_TERRAIN) / TUILE)) * TUILE;
		y = (rand() % ((HAUTEUR_TERRAIN) / TUILE)) * TUILE;
		if (CollisionLombric(testa, x, y, FAUX) == FAUX)
		{
			if (testc != NULL)
			{
				if (CollisionCadeau(testc, x, y) == FAUX)
					tests = FAUX;
			}
			else
			{
				tests = FAUX;
			}
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

void CadeauSupplementaire(Cadeau *c, Cadeau *testc, Anneau *testa)
{
	if (c->suivant != NULL)
	{
		CadeauSupplementaire(c->suivant, testc, testa);
	}
	if (c->suivant == NULL)	
		c->suivant = NouveauCadeau(testc, testa);		
}

Evenement GestionCadeau(Cadeau *c, Cadeau *testc, Anneau *a)
{	
	Evenement tmp;
	if (abs(a->x - c->x) < TUILE / 2 + TUILE && abs(a->y - c->y) < TUILE / 2 + TUILE)
	{
		tmp = c->evm[0];
		ResetCadeau(c, testc, a);
		return tmp;		
	}
	
	return GestionCadeau(c->suivant, testc, a);	
}

void DiminuerCompteur(Cadeau *c, Cadeau *testc, Anneau *testa)
{
	if(c->suivant != NULL)
		DiminuerCompteur(c->suivant, testc, testa);
		
	c->compteur--;
	if (c->compteur <= 0)
		ResetCadeau(c, testc, testa);
}

void LibererCadeaux(Cadeau *c)
{
	if(c->suivant != NULL)
		LibererCadeaux(c->suivant);
		
	free(c);
	c = NULL;
}

Bool CollisionCadeau(Cadeau *c, int x, int y)
{
	if (abs(c->x + TUILE /2 - x) < TUILE && abs(c->y + TUILE / 2 - y) < TUILE)
			return VRAI;
	
	if (c->suivant != 0)
		return CollisionCadeau(c->suivant, x, y);
	
	return FAUX;
}
