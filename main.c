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


void affiche_liste_renversee(Liste lst) {
    printf("%d, ", lst->noeud->valeur);

    if (!lst->suivant) {
        return;
    }

    affiche_liste_renversee(lst->suivant);
}


File initialisation(void) {
    File f = malloc(sizeof(Queue));
    if (!f) return NULL;

    f->debut = NULL;
    f->fin = NULL;
    f->taille = 0;

    return f;
}


int est_vide(File f) {
    if (!f->debut)
        return 1;
    else return 0;
}


int enfiler(File f, Noeud* n) {
    Cellule* new = alloue_cellule(n);
    if (!new) return 0;

    if (!f->fin) {
        f->debut = new;
        f->fin = new;
    } else {
        f->fin->suivant = new;
        f->fin = new;
    }

    f->taille++;

    return 1;
}


Noeud* alloue_noeud(int val, Arbre fg, Arbre fd) {
    Noeud* new = malloc(sizeof(Noeud));

    if (!(new)) return NULL;

    new->valeur = val;
    new->fd = fd;
    new->fg = fg;

    return new;
}


int main (void) {
    Arbre racine = alloue_noeud(1, NULL, NULL);
    Arbre fg = alloue_noeud(2, NULL, NULL);
    Arbre fd = alloue_noeud(3, NULL, NULL);
    racine->fg = fg;
    racine->fd = fd;

    // test enfiler
    File f = initialisation();
    enfiler(f, racine);
    enfiler(f, fg);
    enfiler(f, fd);
    printf("%d\n", f->taille);
    free(f);
}
