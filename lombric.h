#ifndef __LOMBRIC__H__
#define __LOMBRIC__H__

#define TITRE "Lombric en C"
#define LARGEUR_FENETRE 1024
#define HAUTEUR_FENETRE 768
#define LARGEUR_TERRAIN 750
#define HAUTEUR_TERRAIN 750
#define SHIFT 9
#define TEMPSCADEAU 300
#define FPS 16
#define TUILE 25

typedef enum Bool
{
	FAUX, VRAI
} Bool;

typedef enum Partie
{
	ENCOURS, ENPAUSE, PERDU, NIVEAU8, FIN
} Partie;

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
	RIEN, PLUSDEPLACE,
	LOMBRIC,
	MO, MODISP, MODISPARITION, 
	VITPLUS, VITPLUSDISP, VITPLUSDISPARITION, 
	VITMOINS, VITMOINSDISP, VITMOINSDISPARITION
} Evenement;

typedef struct Records
{
	int points;
	int niveau;
	int temps;
	int longueur;
} Records;

typedef struct Anneau
{
	float x;
	float y;
	Bool tete;
	Direction dir;
	Evenement evm;
	struct Anneau *suivant;
} Anneau;

typedef struct Lombric
{
	Evenement evm;
	int vitesse;
	int longueur;
	int point;
	int niveau;
	int maintenant;
	Anneau *tete;
} Lombric;

Lombric *NouveauLombric(void);
void CopieLombric(Lombric *loriginal, Lombric *lcopie);
Anneau *CopierAnneaux(Anneau *a);
Anneau *AjouterQueue(Anneau *a, Bool nouveau);
void LibererAnneaux(Anneau *a);
Lombric *NouvelleTete(Lombric *l);
void DetruireQueue(Anneau *a);
void ChangerDirection(int b, Lombric *l);
Lombric *Bouger(Lombric *l);
void FauxMouvement(Anneau *acopie, int v);
void CollisionTeteMur(Anneau *a);
Bool CollisionLombric(Anneau *a, int x, int y, Bool vieux);
int LongueurLombric(Anneau *a);
Bool NiveauSupplementaire(Lombric *l);
void MiseAJourRecords(Lombric *l, Records *rt);

#endif
