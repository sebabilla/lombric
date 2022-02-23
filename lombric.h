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


typedef enum Direction
{
	HAUT, BAS, GAUCHE, DROITE, COMMENCER, PAUSE, FERMERFENETRE, SANS
} Direction;

typedef enum CouleurTexte
{
	BLANC, GRIS, JAUNE, NOIR
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

typedef struct Records
{
	int points;
	int niveau;
	int temps;
} Records;

typedef struct Lombric
{
	Evenement evm;
	int pas;
	int vitesse;
	int longueur;
	int point;
	int niveau;
	int maintenant;
	Anneau *tete;
} Lombric;

typedef struct Cadeau
{
	int x;
	int y;
	Evenement evm[3];
	int compteur;
	struct Cadeau *suivant;
} Cadeau;

Bool VerificationSauvegarde(const char *s);

Lombric *NouveauLombric(void);
Anneau *AjouterQueue(Anneau *a, Bool nouveau);
void LibererAnneaux(Anneau *a);
Lombric *NouvelleTete(Lombric *l);
void DetruireQueue(Anneau *a);
void ChangerDirection(int b, Lombric *l);
Lombric *Bouger(Lombric *l);
Bool CollisionTeteMur(Anneau *a);
Bool CollisionLombric(Anneau *a, int x, int y, Bool vieux);
int LongueurLombric(Anneau *a);
Bool NiveauSupplementaire(Lombric *l);
void VieillirLombric(Anneau *a);
void MiseAJourRecords(Lombric *l, Records *rt);

#endif
