#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct _noeud {
    int valeur;
    struct _noeud *fg, *fd;
} Noeud, *Arbre;


// Fonction 1 : Valide si l'arbre est un ABR avec mise à jour de min et max
// Retourne 1 si ABR, 0 sinon
// Met à jour min et max uniquement si l'arbre est un ABR
int est_abr_definition_aux(Arbre a, int *min, int *max) {
    // Cas de base : arbre vide
    if (!a) {
        return 1;
    }

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


// Fonction 2 : Valide si l'arbre est un ABR
// Retourne 1 si ABR, 0 sinon
int est_abr_definition(Arbre a) {
    int min, max;
    return est_abr_definition_aux(a, &min, &max);
}


// Fonction 3 : Valide si l'arbre est un ABR et compte le nombre de nœuds visités
// Retourne 1 si ABR, 0 sinon
// Met à jour nb_visites avec le nombre total de nœuds visités
int est_abr_definition_avec_visites_aux(Arbre a, int *min, int *max, long long *nb_visites) {
    // Cas de base : arbre vide
    if (!a) {
        return 1;
    }

    // Incrémenter le compteur de visites
    (*nb_visites)++;

    // Valider le sous-arbre gauche
    int min_gauche = INT_MIN;
    int max_gauche = INT_MIN;

    if (a->fg) {
        if (!est_abr_definition_avec_visites_aux(a->fg, &min_gauche, &max_gauche, nb_visites)) {
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
        if (!est_abr_definition_avec_visites_aux(a->fd, &min_droit, &max_droit, nb_visites)) {
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


// Version finale : interface simple avec compteur de visites
int est_abr(Arbre a, long long *nb_visites) {
    int min, max;
    *nb_visites = 0;
    return est_abr_definition_avec_visites_aux(a, &min, &max, nb_visites);
}


int main(void) {
    return 0;
}
