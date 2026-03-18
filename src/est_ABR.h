#ifndef EST_ABR_H
#define EST_ABR_H

typedef struct noeud {
    int valeur;
    struct noeud *fg, *fd;
} Noeud, *Arbre;


int est_abr_naif(Arbre a);
int est_abr_naif_visites(Arbre a, long long *nb_visites);
int est_abr_definition_aux(Arbre a, int *min, int *max);
int est_abr_definition(Arbre a);
int est_abr_definition_visites(Arbre a, long long *nb_visites);

int infixe_croissant(Arbre a, Noeud **dernier_noeud);
int est_abr_infixe(Arbre a);
int est_abr_infixe_visites(Arbre a, long long *nb_visites);

int abr_min(Arbre a);
int abr_max(Arbre a);

Noeud *alloue_noeud(int valeur, Arbre fg, Arbre fd);
void libere_arbre(Arbre a);

#endif
