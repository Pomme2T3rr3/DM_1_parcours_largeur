#ifndef GENERE_ARBRE_BINAIRE_H
#define GENERE_ARBRE_BINAIRE_H

#include "est_ABR.h"

/* Fonctions publiques de génération d'arbres */
int ABR_presque_complet_alea(Arbre *a, int taille);
int non_ABR_presque_complet_alea(Arbre *a, int taille);
int ABR_filiforme_alea(Arbre *a, int taille);
int non_ABR_filiforme_alea(Arbre *a, int taille);
int ABR_quelconque_alea(Arbre *a, int taille);
int non_ABR_quelconque_alea(Arbre *a, int taille);

#endif