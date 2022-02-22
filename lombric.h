#ifndef __LOMBRIC__H__
#define __LOMBRIC__H__

#define TITRE "Lombric en C"
#define LARGEUR_FENETRE 800
#define HAUTEUR_FENETRE 600
#define LARGEUR_TERRAIN 600
#define HAUTEUR_TERRAIN 600
#define TEMPSCADEAU 300
#define FPS 16
#define TUILE 20

typedef enum Bool
{
	FAUX, VRAI
} Bool;


typedef enum Direction
{
	HAUT, BAS, GAUCHE, DROITE, COMMENCER, PAUSE, FERMERFENETRE, SANS
} Direction;

typedef enum CouleurTexte
{
	BLANC, GRIS, JAUNE
} CouleurTexte;

typedef enum Evenement
{
	RIEN, BORD,
	LOMBRIC, LANGLE, LOMBRICAGE,
	MO, MODISP, MODISPARITION, 
	VITPLUS, VITPLUSDISP, VITPLUSDISPARITION, 
	VITMOINS, VITMOINSDISP, VITMOINSDISPARITION
} Evenement;

typedef struct Anneau
{
	int x;
	int y;
	int age;
	Direction dir;
	Evenement evm;
	struct Anneau *suivant;
} Anneau;

typedef struct Lombric
{
	Evenement evm;
	int pas;
	int vitesse;
	int longueur;
	int point;
	int niveau;
	Anneau *tete;
} Lombric;

Evenement **NouveauTerrain(void); // d'après www.geeksforgeeks.org/dynamically-allocate-2d-array-c/
void NettoyerTerrain(Evenement **t);

Lombric *NouveauLombric(void);
Anneau *AjouterQueue(Anneau *a);
void LibererAnneaux(Anneau *a);
void InsererLombric(Evenement **t, Anneau *a);
void PoserLosange(Evenement **t, Anneau *a, Evenement e);
Lombric *NouvelleTete(Lombric *l);
void DetruireQueue(Anneau *a);
void ChangerDirection(int b, Lombric *l);
Lombric *Bouger(Lombric *l);
Bool CollisionTeteMur(Anneau *a);
Bool CollisionTeteLombric(Anneau *a, Evenement **t);
Bool CollisionTeteCadeau(Anneau *a, Evenement **t);
int LongueurLombric(Anneau *a);
Bool NiveauSupplementaire(Lombric *l);

#endif
