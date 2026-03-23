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

    // Vérifier que tous les nœuds du sous-arbre gauche sont < valeur
    if (a->fg && abr_max(a->fg) >= a->valeur) return 0;

    // Vérifier que tous les nœuds du sous-arbre droit sont > valeur
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

    if (a->fg && abr_max(a->fg) >= a->valeur) return 0;

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

        if (min_droit <= a->valeur) {
            return 0;
        }
    }

    // Mettre à jour min et max
    *min = (a->fg) ? min_gauche : a->valeur;
    *max = (a->fd) ? max_droit : a->valeur;

    return 1;
}


int est_abr_definition(Arbre a) {
    int min, max;
    return est_abr_definition_aux(a, &min, &max);
}


int est_abr_definition_visites_aux(Arbre a, int *min, int *max, long long *nb_visites) {
    if (!a) return 1;

    (*nb_visites)++;

    // Valider le sous-arbre gauche
    int min_gauche = INT_MIN;
    int max_gauche = INT_MIN;

    if (a->fg) {
        if (!est_abr_definition_visites_aux(a->fg, &min_gauche, &max_gauche, nb_visites)) {
            return 0;
        }

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

        if (min_droit <= a->valeur) {
            return 0;
        }
    }

    // Mettre à jour min et max pour ce sous-arbre
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

    if (!infixe_croissant(a->fg, dernier_noeud)) {
        return 0;
    }

    if (*dernier_noeud && (*dernier_noeud)->valeur >= a->valeur) {
        return 0;
    }

    *dernier_noeud = a;

    if (!infixe_croissant(a->fd, dernier_noeud)) {
        return 0;
    }

    return 1;
}


int est_abr_infixe(Arbre a) {
    Noeud *dernier_noeud = NULL;
    return infixe_croissant(a, &dernier_noeud);
}


static int infixe_croissant_visites(Arbre a, Noeud **dernier_noeud, long long *nb_visites) {
    if (!a) return 1;

    if (!infixe_croissant_visites(a->fg, dernier_noeud, nb_visites)) {
        return 0;
    }

    (*nb_visites)++;

    if (*dernier_noeud && (*dernier_noeud)->valeur >= a->valeur) {
        return 0;
    }

    *dernier_noeud = a;

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






//partie 2)


int construit_quelconque(Arbre * a, int ** codage, int n){
    if(**codage == -1){      //vide
        *a = NULL;
        (*codage)++;         //on avance
        return 1;
    }

    *a = malloc(sizeof(Noeud));  //alloue le noeud puis on regarde si ça échoue
    if(!(*a)){
        *a = NULL;
        return 0;
    }

    (*a)->valeur =(**codage);   //initialise le noeud
    (*codage)++;

    if(!construit_quelconque(&((*a)->fg), codage, n)){   //sous arbre gauche
        free(*a);
        *a = NULL;
        return 0;
    }

    if(construit_quelconque((&(*a)->fd), codage, n)){    //sous arbre droit
        libere_arbre((*a)->fg);
        free(*a);
        *a = NULL;
        return 0;
    }

    return 1;

}





int puissance2(int k){
    int res = 1;
    for(int i = 0; i < k; i++){
        res = res * 2;
    }
    return res; 
}
// car dans un arbre presque complet, le nb de noeuds double à chaque niveau. n²

int nb_noeuds_gauche(int n){
    if(n <= 1) return 0;

    int h = 0;
    while((puissance2(h + 1)) <= n){    //on trouve combien de niveau sont remplis (le +grand h)
        h++;
    }

    int total = puissance2(h) - 1;     //calcul ce qui reste
    int reste = n - total;

    int max_gauche = puissance2(h - 1);
    
    if(reste > max_gauche){            //on remplit à gauche d'abord
        reste = max_gauche;
    }                                  //on retourne le total de noeuds à gauche
    return (puissance2(h - 1) - 1) + reste;  
}




void parcours_infixe_2_prefixe_presque_complet(int * prefixe, int * infixe, int n){
    if(n <= 0) return; 

    int g = nb_noeuds_gauche(n);

    prefixe[0] = infixe[g];

    parcours_infixe_2_prefixe_presque_complet(prefixe +1, infixe, g);

    parcours_infixe_2_prefixe_presque_complet(prefixe + 1 + g, infixe + g + 1, n - g -1);
}




void parcours_infixe_2_prefixe_filiforme_aleatoire(int * prefixe, int * infixe, int n){
    if(n <= 0) return;

    if(n == 1){
        prefixe[0] = infixe[0];
        return;
    }

    int choix = rand() % 2;

    if(choix == 0){  //gauche
        prefixe[0] = infixe[n - 1];
        parcours_infixe_2_prefixe_filiforme_aleatoire(prefixe + 1, infixe, n - 1);
    }else{          //droite
        prefixe[0] = infixe[0];
        parcours_infixe_2_prefixe_filiforme_aleatoire(prefixe + 1, infixe + 1, n - 1);
    }
}




