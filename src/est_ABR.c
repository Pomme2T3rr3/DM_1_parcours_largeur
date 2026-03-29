#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "est_ABR.h"


int abr_min(Arbre a) {
    if (!a) return INT_MAX;
    if (!a->fg) return a->valeur;
    return abr_min(a->fg);
}


int abr_max(Arbre a) {
    if (!a) return INT_MIN;
    if (!a->fd) return a->valeur;
    return abr_max(a->fd);
}


Noeud *alloue_noeud(int valeur, Arbre fg, Arbre fd) {
    Noeud *new = malloc(sizeof(Noeud));
    if (!new) return NULL;

    new->valeur = valeur;
    new->fg = fg;
    new->fd = fd;
    return new;
}


void libere_arbre(Arbre a) {
    if (!a) return;
    libere_arbre(a->fg);
    libere_arbre(a->fd);
    free(a);
}


int est_abr_naif_aux(Arbre a) {
    if (!a) return 1;

    // Vérifier récursivement les sous-arbres
    if (!est_abr_naif_aux(a->fg)) return 0;
    if (!est_abr_naif_aux(a->fd)) return 0;

    if (a->fg && abr_max(a->fg) >= a->valeur) return 0;

    if (a->fd && abr_min(a->fd) <= a->valeur) return 0;

    return 1;
}


int est_abr_naif(Arbre a) {
    return est_abr_naif_aux(a);
}


int est_abr_naif_visites_aux(Arbre a, long long *nb_visites) {
    if (!a) return 1;

    (*nb_visites)++;

    // Vérifier récursivement les sous-arbres
    if (!est_abr_naif_visites_aux(a->fg, nb_visites)) return 0;
    if (!est_abr_naif_visites_aux(a->fd, nb_visites)) return 0;

    // Vérifier que tous les nœuds du sous-arbre gauche sont < valeur
    if (a->fg && abr_max(a->fg) >= a->valeur) return 0;

    // Vérifier que tous les nœuds du sous-arbre droit sont > valeur
    if (a->fd && abr_min(a->fd) <= a->valeur) return 0;

    return 1;
}


int est_abr_naif_visites(Arbre a, long long *nb_visites) {
    *nb_visites = 0;
    return est_abr_naif_visites_aux(a, nb_visites);
}


int est_abr_definition_aux(Arbre a, int *min, int *max) {
    if (!a) return 1;

    // Valider le sous-arbre gauche
    int min_gauche = INT_MIN;
    int max_gauche = INT_MIN;

    if (a->fg) {
        if (!est_abr_definition_aux(a->fg, &min_gauche, &max_gauche)) {
            return 0;
        }
        // Vérifier que tous les éléments du sous-arbre gauche sont < valeur
        if (max_gauche >= a->valeur) {
            return 0;
        }
    }

    // Valider le sous-arbre droit
    int min_droit = INT_MAX;
    int max_droit = INT_MAX;

    if (a->fd) {
        if (!est_abr_definition_aux(a->fd, &min_droit, &max_droit)) {
            return 0;
        }
        // Vérifier que tous les éléments du sous-arbre droit sont > valeur
        if (min_droit <= a->valeur) {
            return 0;
        }
    }

    // Mettre à jour min et max pour ce sous-arbre
    *min = (a->fg) ? min_gauche : a->valeur;
    *max = (a->fd) ? max_droit : a->valeur;

    return 1;
}


int est_abr_definition(Arbre a) {
    int min, max;
    return est_abr_definition_aux(a, &min, &max);
}


static int est_abr_definition_visites_aux(Arbre a, int *min, int *max, long long *nb_visites) {
    if (!a) return 1;

    (*nb_visites)++;

    // Valider le sous-arbre gauche
    int min_gauche = INT_MIN;
    int max_gauche = INT_MIN;

    if (a->fg) {
        if (!est_abr_definition_visites_aux(a->fg, &min_gauche, &max_gauche, nb_visites)) {
            return 0;
        }
        // Vérifier que tous les éléments du sous-arbre gauche sont < valeur
        if (max_gauche >= a->valeur) {
            return 0;
        }
    }

    // Valider le sous-arbre droit
    int min_droit = INT_MAX;
    int max_droit = INT_MAX;

    if (a->fd) {
        if (!est_abr_definition_visites_aux(a->fd, &min_droit, &max_droit, nb_visites)) {
            return 0;
        }
        // Vérifier que tous les éléments du sous-arbre droit sont > valeur
        if (min_droit <= a->valeur) {
            return 0;
        }
    }

    // Mettre à jour min et max
    *min = (a->fg) ? min_gauche : a->valeur;
    *max = (a->fd) ? max_droit : a->valeur;

    return 1;
}


int est_abr_definition_visites(Arbre a, long long *nb_visites) {
    int min, max;
    *nb_visites = 0;
    return est_abr_definition_visites_aux(a, &min, &max, nb_visites);
}


int infixe_croissant(Arbre a, Noeud **dernier_noeud) {
    if (!a) return 1;

    // Parcourir le sous-arbre gauche
    if (!infixe_croissant(a->fg, dernier_noeud)) {
        return 0;
    }

    // Vérifier la condition de croissance
    if (*dernier_noeud && (*dernier_noeud)->valeur >= a->valeur) {
        return 0;
    }

    // Mettre à jour le dernier nœud visité
    *dernier_noeud = a;

    // Parcourir le sous-arbre droit
    if (!infixe_croissant(a->fd, dernier_noeud)) {
        return 0;
    }

    return 1;
}


int est_abr_infixe(Arbre a) {
    Noeud *dernier_noeud = NULL;
    return infixe_croissant(a, &dernier_noeud);
}


int infixe_croissant_visites(Arbre a, Noeud **dernier_noeud, long long *nb_visites) {
    if (!a) return 1;

    // Parcourir le sous-arbre gauche
    if (!infixe_croissant_visites(a->fg, dernier_noeud, nb_visites)) {
        return 0;
    }

    // Incrémenter le compteur de visites
    (*nb_visites)++;

    // Vérifier la condition de croissance
    if (*dernier_noeud && (*dernier_noeud)->valeur >= a->valeur) {
        return 0;
    }

    // Mettre à jour le dernier nœud visité
    *dernier_noeud = a;

    // Parcourir le sous-arbre droit
    if (!infixe_croissant_visites(a->fd, dernier_noeud, nb_visites)) {
        return 0;
    }

    return 1;
}


int est_abr_infixe_visites(Arbre a, long long *nb_visites) {
    Noeud *dernier_noeud = NULL;
    *nb_visites = 0;
    return infixe_croissant_visites(a, &dernier_noeud, nb_visites);
}
