#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "est_ABR.h"
#include "genere_arbre_binaire.h"

#define NB_REPETITIONS 20
#define NB_EXECUTIONS_TEMPS 20

typedef int (*Generateur)(Arbre *, int);
typedef int (*Verificateur)(Arbre, long long *);

typedef struct {
    const char *nom;
    Generateur generer;
    int attendu;   // 1 si on attend un ABR, 0 sinon
} TypeArbre;

typedef struct {
    const char *nom;
    Verificateur verifier;
} Methode;

static long long diff_ns(struct timespec debut, struct timespec fin) {
    return (fin.tv_sec - debut.tv_sec) * 1000000000LL
         + (fin.tv_nsec - debut.tv_nsec);
}

static long long mesurer_temps_us(Verificateur verifier, Arbre a, long long *nb_visites) {
    struct timespec debut, fin;
    long long total_ns = 0;
    long long visites = 0;

    for (int i = 0; i < NB_EXECUTIONS_TEMPS; i++) {
        long long courant = 0;

        clock_gettime(CLOCK_MONOTONIC, &debut);
        verifier(a, &courant);
        clock_gettime(CLOCK_MONOTONIC, &fin);

        if (i == 0) {
            visites = courant;
        }

        total_ns += diff_ns(debut, fin);
    }

    *nb_visites = visites;
    return total_ns / (1000LL * NB_EXECUTIONS_TEMPS);   // microsecondes
}

int main(void) {
    FILE *f = fopen("mesures.csv", "w");
    if (!f) {
        perror("mesures.csv");
        return 1;
    }

    srand((unsigned int)time(NULL));

    fprintf(f, "Taille;Morphologie;Methode;Nb_visites;Temps\n");

    TypeArbre types[] = {
        {"ABR_presque_complet",     ABR_presque_complet_alea,     1},
        {"non_ABR_presque_complet", non_ABR_presque_complet_alea, 0},
        {"ABR_filiforme",           ABR_filiforme_alea,           1},
        {"non_ABR_filiforme",       non_ABR_filiforme_alea,       0},
        {"ABR_quelconque",          ABR_quelconque_alea,          1},
        {"non_ABR_quelconque",      non_ABR_quelconque_alea,      0}
    };

    Methode methodes[] = {
        {"naif",       est_abr_naif_visites},
        {"definition", est_abr_definition_visites},
        {"infixe",     est_abr_infixe_visites}
    };

    /* Taille 1 évitée :
       un arbre d’un seul noeud est toujours un ABR,
       donc ce n’est pas pertinent pour les non_ABR. */
    int tailles[] = {2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 4000};

    int nb_types = (int)(sizeof(types) / sizeof(types[0]));
    int nb_methodes = (int)(sizeof(methodes) / sizeof(methodes[0]));
    int nb_tailles = (int)(sizeof(tailles) / sizeof(tailles[0]));

    for (int i = 0; i < nb_tailles; i++) {
        int taille = tailles[i];
        printf("Taille %d...\n", taille);

        for (int t = 0; t < nb_types; t++) {
            for (int rep = 0; rep < NB_REPETITIONS; rep++) {
                Arbre a = NULL;

                if (!types[t].generer(&a, taille)) {
                    fprintf(stderr, "Erreur de génération : %s, taille=%d\n",
                            types[t].nom, taille);
                    fclose(f);
                    return 1;
                }

                for (int m = 0; m < nb_methodes; m++) {
                    long long nb_visites = 0;
                    long long temps_us = mesurer_temps_us(methodes[m].verifier, a, &nb_visites);

                    int resultat = methodes[m].verifier(a, &nb_visites);
                    if (resultat != types[t].attendu) {
                        fprintf(stderr,
                                "Avertissement : %s sur %s (taille=%d) renvoie %d au lieu de %d\n",
                                methodes[m].nom, types[t].nom, taille,
                                resultat, types[t].attendu);
                    }

                    fprintf(f, "%d;%s;%s;%lld;%lld\n",
                            taille,
                            types[t].nom,
                            methodes[m].nom,
                            nb_visites,
                            temps_us);
                }

                libere_arbre(a);
            }
        }
    }

    fclose(f);
    printf("mesures.csv généré avec succès.\n");
    return 0;
}
