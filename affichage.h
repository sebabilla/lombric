#ifndef __AFFICHAGE__H__
#define __AFFICHAGE__H__

void SDL_ExitWithError(const char *message); // Copiée telle quelle de Jason Champagne @ https://www.youtube.com/c/Formationvidéo8
void limit_fps(unsigned int limit); // Copiée telle quelle de Jason Champagne @ https://www.youtube.com/c/Formationvidéo8

void AfficherTerrain(SDL_Renderer *r, int i, int j, Evenement evm);
void EcrireTexte(SDL_Renderer *r, char *texte, TTF_Font *f, int X, int Y, int W, int H, CouleurTexte c);
void AfficherPause(SDL_Renderer *r, TTF_Font *f);
void AfficherPerdu(SDL_Renderer *r, TTF_Font *f);
void AfficherCommandes(SDL_Renderer *r, TTF_Font *f);
void AfficherLegende(SDL_Renderer *r, TTF_Font *f);

#endif