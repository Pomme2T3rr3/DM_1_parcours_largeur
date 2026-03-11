#include <stdio.h>
#include <stdlib.h>


typedef struct _noeud {
    int valeur;
    struct _noeud *fg, *fd;
} Noeud, *Arbre;

typedef struct cell{
    Noeud *noeud;
    struct cell *suivant;
} Cellule, *Liste;

typedef struct {
    Liste debut;
    Liste fin;
    int taille;
} Queue, *File;


Cellule* alloue_cellule(Noeud * n) {
    Cellule* new = malloc(sizeof(Cellule));

    if (new) {
        new->noeud = n;
        return new;
    }
    return NULL;
}


void insere_en_tete(Liste* l, Cellule * c) {
    if (*l) {
        c->suivant = *l;
    }
    *l = c;
}


Cellule* extrait_tete(Liste * l) {
    Cellule* tmp = *l;
    if (*l) {
        *l = (*l)->suivant;
    }
    return tmp;
}
