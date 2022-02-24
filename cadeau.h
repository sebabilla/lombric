#ifndef __CADEAU__H__
#define __CADEAU__H__

Cadeau *NouveauCadeau(Cadeau *c, Anneau *a);
Bool ResetCadeau(Cadeau *c, Cadeau *testc, Anneau *testa);
void CadeauSupplementaire(Cadeau *c, Cadeau *testc, Anneau *testa);
Evenement GestionCadeau(Cadeau *c, Cadeau *testc, Anneau *a);
void DiminuerCompteur(Cadeau *c, Cadeau *testc, Anneau *testa);
void LibererCadeaux(Cadeau *c);
Bool CollisionCadeau(Cadeau *c, int x, int y);

#endif
