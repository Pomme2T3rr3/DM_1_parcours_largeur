#include <stdlib.h>
#include "est_ABR.h"

// Forward declarations des fonctions auxiliaires statiques
static int construit_quelconque(Arbre *a, int **codage, int n);
static int puissance2(int k);
static int nb_noeuds_gauche(int n);
static void parcours_infixe_2_prefixe_presque_complet(int *prefixe, int *infixe, int n);
static void parcours_infixe_2_prefixe_quelconque_aleatoire(int *codage, int *infixe, int n);
static void filiforme_infixe_vers_codage_alea(int *codage, int *infixe, int n);
static void prefixe_presque_complet_vers_codage(int *codage, int *prefixe, int n);

// ============================================================================
// Implémentations des fonctions auxiliaires statiques
// ============================================================================

/**
 * puissance2: retourne 2^k
 */
static int puissance2(int k) {
    int resultat = 1;
    for (int i = 0; i < k; i++) {
        resultat *= 2;
    }
    return resultat;
}

/**
 * nb_noeuds_gauche: pour un arbre presque complet de n nœuds,
 * retourne le nombre de nœuds dans le sous-arbre gauche
 */
static int nb_noeuds_gauche(int n) {
    if (n <= 1) return 0;
    
    // Trouver la hauteur de l'arbre (en commençant à 0)
    int h = 0;
    int temp = n;
    while (temp > 1) {
        temp /= 2;
        h++;
    }
    
    // Nombre de nœuds au dernier niveau
    int noeuds_dernier = n - (puissance2(h) - 1);
    
    // Capacité du sous-arbre gauche au dernier niveau
    int capacite_gauche = puissance2(h - 1);
    
    // Nombre réel de nœuds à gauche au dernier niveau
    int gauche_dernier = (noeuds_dernier < capacite_gauche) ? noeuds_dernier : capacite_gauche;
    
    // Total sous-arbre gauche
    return (puissance2(h - 1) - 1) + gauche_dernier;
}

/**
 * construit_quelconque: construit l'arbre récursivement à partir du codage préfixe.
 * Si *codage pointe à -1, crée un nœud NULL.
 * Sinon crée un nœud et construit récursivement les fils gauche et droit.
 * Renvoie 1 si succès, 0 sinon.
 */
static int construit_quelconque(Arbre *a, int **codage, int n) {
    if (n <= 0 || !*codage) return 0;
    
    if (**codage == -1) {
        *a = NULL;
        (*codage)++;
        return 1;
    }
    
    *a = malloc(sizeof(Noeud));
    if (!*a) return 0;
    
    (*a)->valeur = **codage;
    (*codage)++;
    
    // Construire récursivement les fils
    if (!construit_quelconque(&((*a)->fg), codage, n) || 
        !construit_quelconque(&((*a)->fd), codage, n)) {
        libere_arbre(*a);
        *a = NULL;
        return 0;
    }
    
    return 1;
}

/**
 * parcours_infixe_2_prefixe_presque_complet: convertit un parcours infixe 
 * en parcours préfixe pour un arbre presque complet
 */
static void parcours_infixe_2_prefixe_presque_complet(int *prefixe, int *infixe, int n) {
    if (n <= 0) return;
    
    if (n == 1) {
        prefixe[0] = infixe[0];
        return;
    }
    
    // Pour un arbre presque complet, calcul du nombre de nœuds à gauche
    int ng = nb_noeuds_gauche(n);
    
    // La racine est à l'index ng en infixe
    prefixe[0] = infixe[ng];
    
    // Appel récursif pour sous-arbre gauche
    parcours_infixe_2_prefixe_presque_complet(prefixe + 1, infixe, ng);
    
    // Appel récursif pour sous-arbre droit
    parcours_infixe_2_prefixe_presque_complet(prefixe + 1 + ng, infixe + ng + 1, n - ng - 1);
}

/**
 * parcours_infixe_2_prefixe_quelconque_aleatoire_aux: version auxiliaire
 * qui choisit aléatoirement une racine et appelle récursivement
 */
static void parcours_infixe_2_prefixe_quelconque_aleatoire_aux(int *prefixe, int *infixe, int n, int *idx) {
    if (n <= 0) return;
    
    // Choisir aléatoirement une racine
    int racine_idx = rand() % n;
    
    prefixe[(*idx)++] = infixe[racine_idx];
    
    // Appel récursif pour sous-arbre gauche
    parcours_infixe_2_prefixe_quelconque_aleatoire_aux(prefixe, infixe, racine_idx, idx);
    
    // Appel récursif pour sous-arbre droit
    parcours_infixe_2_prefixe_quelconque_aleatoire_aux(prefixe, infixe + racine_idx + 1, n - racine_idx - 1, idx);
}

/**
 * parcours_infixe_2_prefixe_quelconque_aleatoire: choisit aléatoirement 
 * une structure d'arbre binaire et produit le codage préfixe correspondant
 */
static void parcours_infixe_2_prefixe_quelconque_aleatoire(int *codage, int *infixe, int n) {
    if (n <= 0) return;
    
    int idx = 0;
    parcours_infixe_2_prefixe_quelconque_aleatoire_aux(codage, infixe, n, &idx);
    
    // Remplir le reste avec -1 (pour les nœuds NULL)
    while (idx < 2 * n + 1) {
        codage[idx++] = -1;
    }
}

/**
 * filiforme_infixe_vers_codage_alea: encode aléatoirement (gauche ou droite) 
 * un arbre filiforme. Convertit l'infixe en codage préfixe.
 */
static void filiforme_infixe_vers_codage_alea(int *codage, int *infixe, int n) {
    if (n <= 0) return;
    
    int idx = 0;
    
    // Choisir aléatoirement gauche ou droite
    int gauche = rand() % 2;
    
    if (gauche) {
        // Chaîne à gauche: infixe[n-1], infixe[n-2], ..., infixe[0]
        for (int i = n - 1; i >= 0; i--) {
            codage[idx++] = infixe[i];
        }
    } else {
        // Chaîne à droite: infixe[0], NULL, infixe[1], NULL, infixe[2], ...
        for (int i = 0; i < n; i++) {
            codage[idx++] = infixe[i];
            if (i < n - 1) {
                codage[idx++] = -1;  // Pas d'enfant gauche
            }
        }
    }
    
    // Remplir le reste avec -1
    while (idx < 2 * n + 1) {
        codage[idx++] = -1;
    }
}

/**
 * prefixe_presque_complet_vers_codage: convertit préfixe en codage préfixe 
 * pour un arbre presque complet (les NULL sont codés -1)
 */
static void prefixe_presque_complet_vers_codage(int *codage, int *prefixe, int n) {
    if (n <= 0) return;
    
    // Initialiser tout à -1
    for (int i = 0; i < 2 * n + 1; i++) {
        codage[i] = -1;
    }
    
    // Copier le préfixe dans le codage
    for (int i = 0; i < n; i++) {
        codage[i] = prefixe[i];
    }
}

// ============================================================================
// Implémentations des 6 fonctions publiques de génération d'arbres
// ============================================================================

int ABR_presque_complet_alea(Arbre * a, int taille) {
    if (taille < 0) return 0;

    if (taille == 0) {
        *a = NULL;
        return 1;
    }

    int *infixe = malloc(taille * sizeof(int));
    if (!infixe) return 0;

    int *prefixe = malloc(taille * sizeof(int));
    if (!prefixe) {
        free(infixe);
        return 0;
    }

    int *codage = malloc((2 * taille + 1) * sizeof(int));
    if (!codage) {
        free(infixe);
        free(prefixe);
        return 0;
    }

    /* infixe trié => ABR */
    for (int i = 0; i < taille; i++) {
        infixe[i] = i + 1;
    }

    parcours_infixe_2_prefixe_presque_complet(prefixe, infixe, taille);
    prefixe_presque_complet_vers_codage(codage, prefixe, taille);

    int *p = codage;
    int tmp = construit_quelconque(a, &p, 2 * taille + 1);

    free(infixe);
    free(prefixe);
    free(codage);

    return tmp;
}

int non_ABR_presque_complet_alea(Arbre * a, int taille) {
    if (taille < 0) return 0;

    if (taille == 0) {
        *a = NULL;
        return 1;
    }

    int *infixe = malloc(taille * sizeof(int));
    if (!infixe) return 0;

    int *prefixe = malloc(taille * sizeof(int));
    if (!prefixe) {
        free(infixe);
        return 0;
    }

    int *codage = malloc((2 * taille + 1) * sizeof(int));
    if (!codage) {
        free(infixe);
        free(prefixe);
        return 0;
    }

    /* infixe trié => ABR */
    for (int i = 0; i < taille; i++) {
        infixe[i] = i + 1;
    }

    /* mélange pour que l'infixe ne soit pas trié */
    for (int i = taille - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int t = infixe[i];
        infixe[i] = infixe[j];
        infixe[j] = t;
    }

    parcours_infixe_2_prefixe_presque_complet(prefixe, infixe, taille);
    prefixe_presque_complet_vers_codage(codage, prefixe, taille);

    int *p = codage;
    int tmp = construit_quelconque(a, &p, 2 * taille + 1);

    free(infixe);
    free(prefixe);
    free(codage);

    return tmp;
}

int ABR_filiforme_alea(Arbre *a, int taille) {
    if (taille < 0) return 0;

    if (taille == 0) {
        *a = NULL;
        return 1;
    }

    int *infixe = malloc(taille * sizeof(int));
    if (!infixe) return 0;

    int *codage = malloc((2 * taille + 1) * sizeof(int));
    if (!codage) {
        free(infixe);
        return 0;
    }

    /* infixe trié => arbre binaire de recherche */
    for (int i = 0; i < taille; i++) {
        infixe[i] = i + 1;
    }

    filiforme_infixe_vers_codage_alea(codage, infixe, taille);

    int *p = codage;
    int tmp = construit_quelconque(a, &p, 2 * taille + 1);

    free(infixe);
    free(codage);

    return tmp;
}

int non_ABR_filiforme_alea(Arbre * a, int taille) {
    if (taille < 0) return 0;

    if (taille == 0) {
        *a = NULL;
        return 1;
    }

    int *infixe = malloc(taille * sizeof(int));
    if (!infixe) return 0;

    int *codage = malloc((2 * taille + 1) * sizeof(int));
    if (!codage) {
        free(infixe);
        return 0;
    }

    /* infixe trié => ABR */
    for (int i = 0; i < taille; i++) {
        infixe[i] = i + 1;
    }

    /* mélange pour que l'infixe ne soit pas trié */
    for (int i = taille - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int t = infixe[i];
        infixe[i] = infixe[j];
        infixe[j] = t;
    }

    filiforme_infixe_vers_codage_alea(codage, infixe, taille);

    int *p = codage;
    int tmp = construit_quelconque(a, &p, 2 * taille + 1);

    free(infixe);
    free(codage);

    return tmp;
}

int ABR_quelconque_alea(Arbre *a, int taille) {
    if (taille < 0) return 0;

    if (taille == 0) {
        *a = NULL;
        return 1;
    }

    int *infixe = malloc(taille * sizeof(int));
    if (!infixe) return 0;

    int *codage = malloc((2 * taille + 1) * sizeof(int));
    if (!codage) {
        free(infixe);
        return 0;
    }

    for (int i = 0; i < taille; i++) {
        infixe[i] = i + 1;
    }

    /* Choisit aléatoirement l'une des Cn formes */
    parcours_infixe_2_prefixe_quelconque_aleatoire(codage, infixe, taille);

    int *p = codage;
    int tmp = construit_quelconque(a, &p, 2 * taille + 1);

    free(infixe);
    free(codage);

    return tmp;
}

int non_ABR_quelconque_alea(Arbre *a, int taille) {
    if (taille < 0) return 0;

    if (taille == 0) {
        *a = NULL;
        return 1;
    }

    /* Un arbre à 1 nœud est toujours un ABR */
    if (taille == 1) {
        *a = malloc(sizeof(Noeud));
        if (!(*a)) return 0;
        (*a)->valeur = 1;
        (*a)->fg = NULL;
        (*a)->fd = NULL;
        return 1;
    }

    int *infixe = malloc(taille * sizeof(int));
    if (!infixe) return 0;

    int *codage = malloc((2 * taille + 1) * sizeof(int));
    if (!codage) {
        free(infixe);
        return 0;
    }

    for (int i = 0; i < taille; i++) {
        infixe[i] = i + 1;
    }

    for (int i = taille - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int t = infixe[i];
        infixe[i] = infixe[j];
        infixe[j] = t;
    }

    /* Choix aléatoire d'une Cn forme quelconque */
    parcours_infixe_2_prefixe_quelconque_aleatoire(codage, infixe, taille);

    int *p = codage;
    int tmp = construit_quelconque(a, &p, 2 * taille + 1);

    free(infixe);
    free(codage);

    return tmp;
}