#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "est_ABR.h"

/*
 * ============================================================================
 * UTILITAIRES DE TEST
 * ============================================================================
 */

/**
 * Affiche un titre de section
 */
void affiche_titre(const char *titre) {
    printf("\n");
    printf("=============================================================================\n");
    printf("  %s\n", titre);
    printf("=============================================================================\n");
}

/**
 * Affiche un sous-titre de test
 */
void affiche_sous_titre(const char *titre) {
    printf("\n--- %s ---\n", titre);
}

/**
 * Affiche le résultat d'un test
 */
void affiche_resultat(const char *nom_test, int resultat, int attendu, long long visites) {
    const char *status = (resultat == attendu) ? "✓ PASS" : "✗ FAIL";
    printf("%s | %-40s | Résultat: %d (attendu %d) | Visites: %lld\n",
           status, nom_test, resultat, attendu, visites);
}

/*
 * ============================================================================
 * CRÉATION D'ARBRES DE TEST
 * ============================================================================
 */

/**
 * Crée un ABR parfait
 *       5
 *      / \
 *     3   7
 *    / \ / \
 *   2  4 6  8
 */
Arbre creer_abr_parfait(void) {
    Arbre racine = alloue_noeud(5, NULL, NULL);
    racine->fg = alloue_noeud(3, NULL, NULL);
    racine->fd = alloue_noeud(7, NULL, NULL);
    racine->fg->fg = alloue_noeud(2, NULL, NULL);
    racine->fg->fd = alloue_noeud(4, NULL, NULL);
    racine->fd->fg = alloue_noeud(6, NULL, NULL);
    racine->fd->fd = alloue_noeud(8, NULL, NULL);
    return racine;
}

/**
 * Crée un arbre qui n'est PAS un ABR (violation à gauche)
 *       5
 *      / \
 *     6   7
 *    / \
 *   2  4
 */
Arbre creer_non_abr_gauche(void) {
    Arbre racine = alloue_noeud(5, NULL, NULL);
    racine->fg = alloue_noeud(6, NULL, NULL);  // Violation: 6 > 5
    racine->fd = alloue_noeud(7, NULL, NULL);
    racine->fg->fg = alloue_noeud(2, NULL, NULL);
    racine->fg->fd = alloue_noeud(4, NULL, NULL);
    return racine;
}

/**
 * Crée un arbre qui n'est PAS un ABR (violation à droite)
 *       5
 *      / \
 *     3   7
 *    / \ / \
 *   2  4 3  8
 */
Arbre creer_non_abr_droit(void) {
    Arbre racine = alloue_noeud(5, NULL, NULL);
    racine->fg = alloue_noeud(3, NULL, NULL);
    racine->fd = alloue_noeud(7, NULL, NULL);
    racine->fg->fg = alloue_noeud(2, NULL, NULL);
    racine->fg->fd = alloue_noeud(4, NULL, NULL);
    racine->fd->fg = alloue_noeud(3, NULL, NULL);  // Violation: 3 < 5
    racine->fd->fd = alloue_noeud(8, NULL, NULL);
    return racine;
}

/**
 * Crée un arbre dégénéré (liste chaînée à droite)
 *     1
 *      \
 *       2
 *        \
 *         3
 *          \
 *           4
 */
Arbre creer_abr_degenere(void) {
    Arbre racine = alloue_noeud(1, NULL, NULL);
    Arbre courant = racine;
    for (int i = 2; i <= 4; i++) {
        courant->fd = alloue_noeud(i, NULL, NULL);
        courant = courant->fd;
    }
    return racine;
}

/**
 * Crée un arbre vide (NULL)
 */
Arbre creer_arbre_vide(void) {
    return NULL;
}

/**
 * Crée un arbre avec un seul nœud
 */
Arbre creer_arbre_single(void) {
    return alloue_noeud(42, NULL, NULL);
}

/*
 * ============================================================================
 * TESTS POUR CHAQUE APPROCHE
 * ============================================================================
 */

/**
 * Tests pour l'approche naïve
 */
void tester_approche_naive(void) {
    affiche_titre("TESTS - APPROCHE NAÏVE (Section 1.1)");
    
    long long visites;
    
    // Test 1: ABR parfait
    affiche_sous_titre("Test 1: ABR parfait");
    Arbre abr_parfait = creer_abr_parfait();
    int res = est_abr_naif(abr_parfait);
    affiche_resultat("est_abr_naif - ABR parfait", res, 1, 0);
    
    res = est_abr_naif_visites(abr_parfait, &visites);
    affiche_resultat("est_abr_naif_visites - ABR parfait", res, 1, visites);
    libere_arbre(abr_parfait);
    
    // Test 2: Non-ABR (violation gauche)
    affiche_sous_titre("Test 2: Non-ABR (violation à gauche)");
    Arbre non_abr_g = creer_non_abr_gauche();
    res = est_abr_naif(non_abr_g);
    affiche_resultat("est_abr_naif - Non-ABR (gauche)", res, 0, 0);
    
    res = est_abr_naif_visites(non_abr_g, &visites);
    affiche_resultat("est_abr_naif_visites - Non-ABR (gauche)", res, 0, visites);
    libere_arbre(non_abr_g);
    
    // Test 3: Non-ABR (violation droite)
    affiche_sous_titre("Test 3: Non-ABR (violation à droite)");
    Arbre non_abr_d = creer_non_abr_droit();
    res = est_abr_naif(non_abr_d);
    affiche_resultat("est_abr_naif - Non-ABR (droit)", res, 0, 0);
    
    res = est_abr_naif_visites(non_abr_d, &visites);
    affiche_resultat("est_abr_naif_visites - Non-ABR (droit)", res, 0, visites);
    libere_arbre(non_abr_d);
    
    // Test 4: Arbre dégénéré
    affiche_sous_titre("Test 4: Arbre dégénéré (liste chaînée)");
    Arbre abr_deg = creer_abr_degenere();
    res = est_abr_naif(abr_deg);
    affiche_resultat("est_abr_naif - Arbre dégénéré", res, 1, 0);
    
    res = est_abr_naif_visites(abr_deg, &visites);
    affiche_resultat("est_abr_naif_visites - Arbre dégénéré", res, 1, visites);
    libere_arbre(abr_deg);
    
    // Test 5: Arbre vide
    affiche_sous_titre("Test 5: Arbre vide");
    Arbre arbre_vide = creer_arbre_vide();
    res = est_abr_naif(arbre_vide);
    affiche_resultat("est_abr_naif - Arbre vide", res, 1, 0);
    
    res = est_abr_naif_visites(arbre_vide, &visites);
    affiche_resultat("est_abr_naif_visites - Arbre vide", res, 1, visites);
    
    // Test 6: Arbre single
    affiche_sous_titre("Test 6: Arbre avec un seul nœud");
    Arbre arbre_single = creer_arbre_single();
    res = est_abr_naif(arbre_single);
    affiche_resultat("est_abr_naif - Single node", res, 1, 0);
    
    res = est_abr_naif_visites(arbre_single, &visites);
    affiche_resultat("est_abr_naif_visites - Single node", res, 1, visites);
    libere_arbre(arbre_single);
}

/**
 * Tests pour l'approche optimisée (propagation min/max)
 */
void tester_approche_optimisee(void) {
    affiche_titre("TESTS - APPROCHE OPTIMISÉE (Section 1.2)");
    
    long long visites;
    
    // Test 1: ABR parfait
    affiche_sous_titre("Test 1: ABR parfait");
    Arbre abr_parfait = creer_abr_parfait();
    int res = est_abr_definition(abr_parfait);
    affiche_resultat("est_abr_definition - ABR parfait", res, 1, 0);
    
    res = est_abr_definition_visites(abr_parfait, &visites);
    affiche_resultat("est_abr_definition_visites - ABR parfait", res, 1, visites);
    libere_arbre(abr_parfait);
    
    // Test 2: Non-ABR (violation gauche)
    affiche_sous_titre("Test 2: Non-ABR (violation à gauche)");
    Arbre non_abr_g = creer_non_abr_gauche();
    res = est_abr_definition(non_abr_g);
    affiche_resultat("est_abr_definition - Non-ABR (gauche)", res, 0, 0);
    
    res = est_abr_definition_visites(non_abr_g, &visites);
    affiche_resultat("est_abr_definition_visites - Non-ABR (gauche)", res, 0, visites);
    libere_arbre(non_abr_g);
    
    // Test 3: Non-ABR (violation droite)
    affiche_sous_titre("Test 3: Non-ABR (violation à droite)");
    Arbre non_abr_d = creer_non_abr_droit();
    res = est_abr_definition(non_abr_d);
    affiche_resultat("est_abr_definition - Non-ABR (droit)", res, 0, 0);
    
    res = est_abr_definition_visites(non_abr_d, &visites);
    affiche_resultat("est_abr_definition_visites - Non-ABR (droit)", res, 0, visites);
    libere_arbre(non_abr_d);
    
    // Test 4: Arbre dégénéré
    affiche_sous_titre("Test 4: Arbre dégénéré (liste chaînée)");
    Arbre abr_deg = creer_abr_degenere();
    res = est_abr_definition(abr_deg);
    affiche_resultat("est_abr_definition - Arbre dégénéré", res, 1, 0);
    
    res = est_abr_definition_visites(abr_deg, &visites);
    affiche_resultat("est_abr_definition_visites - Arbre dégénéré", res, 1, visites);
    libere_arbre(abr_deg);
    
    // Test 5: Arbre vide
    affiche_sous_titre("Test 5: Arbre vide");
    Arbre arbre_vide = creer_arbre_vide();
    res = est_abr_definition(arbre_vide);
    affiche_resultat("est_abr_definition - Arbre vide", res, 1, 0);
    
    res = est_abr_definition_visites(arbre_vide, &visites);
    affiche_resultat("est_abr_definition_visites - Arbre vide", res, 1, visites);
    
    // Test 6: Arbre single
    affiche_sous_titre("Test 6: Arbre avec un seul nœud");
    Arbre arbre_single = creer_arbre_single();
    res = est_abr_definition(arbre_single);
    affiche_resultat("est_abr_definition - Single node", res, 1, 0);
    
    res = est_abr_definition_visites(arbre_single, &visites);
    affiche_resultat("est_abr_definition_visites - Single node", res, 1, visites);
    libere_arbre(arbre_single);
}

/**
 * Tests pour l'approche par parcours infixe
 */
void tester_approche_infixe(void) {
    affiche_titre("TESTS - APPROCHE PARCOURS INFIXE (Section 1.3)");
    
    long long visites;
    
    // Test 1: ABR parfait
    affiche_sous_titre("Test 1: ABR parfait");
    Arbre abr_parfait = creer_abr_parfait();
    int res = est_abr_infixe(abr_parfait);
    affiche_resultat("est_abr_infixe - ABR parfait", res, 1, 0);
    
    res = est_abr_infixe_visites(abr_parfait, &visites);
    affiche_resultat("est_abr_infixe_visites - ABR parfait", res, 1, visites);
    libere_arbre(abr_parfait);
    
    // Test 2: Non-ABR (violation gauche)
    affiche_sous_titre("Test 2: Non-ABR (violation à gauche)");
    Arbre non_abr_g = creer_non_abr_gauche();
    res = est_abr_infixe(non_abr_g);
    affiche_resultat("est_abr_infixe - Non-ABR (gauche)", res, 0, 0);
    
    res = est_abr_infixe_visites(non_abr_g, &visites);
    affiche_resultat("est_abr_infixe_visites - Non-ABR (gauche)", res, 0, visites);
    libere_arbre(non_abr_g);
    
    // Test 3: Non-ABR (violation droite)
    affiche_sous_titre("Test 3: Non-ABR (violation à droite)");
    Arbre non_abr_d = creer_non_abr_droit();
    res = est_abr_infixe(non_abr_d);
    affiche_resultat("est_abr_infixe - Non-ABR (droit)", res, 0, 0);
    
    res = est_abr_infixe_visites(non_abr_d, &visites);
    affiche_resultat("est_abr_infixe_visites - Non-ABR (droit)", res, 0, visites);
    libere_arbre(non_abr_d);
    
    // Test 4: Arbre dégénéré
    affiche_sous_titre("Test 4: Arbre dégénéré (liste chaînée)");
    Arbre abr_deg = creer_abr_degenere();
    res = est_abr_infixe(abr_deg);
    affiche_resultat("est_abr_infixe - Arbre dégénéré", res, 1, 0);
    
    res = est_abr_infixe_visites(abr_deg, &visites);
    affiche_resultat("est_abr_infixe_visites - Arbre dégénéré", res, 1, visites);
    libere_arbre(abr_deg);
    
    // Test 5: Arbre vide
    affiche_sous_titre("Test 5: Arbre vide");
    Arbre arbre_vide = creer_arbre_vide();
    res = est_abr_infixe(arbre_vide);
    affiche_resultat("est_abr_infixe - Arbre vide", res, 1, 0);
    
    res = est_abr_infixe_visites(arbre_vide, &visites);
    affiche_resultat("est_abr_infixe_visites - Arbre vide", res, 1, visites);
    
    // Test 6: Arbre single
    affiche_sous_titre("Test 6: Arbre avec un seul nœud");
    Arbre arbre_single = creer_arbre_single();
    res = est_abr_infixe(arbre_single);
    affiche_resultat("est_abr_infixe - Single node", res, 1, 0);
    
    res = est_abr_infixe_visites(arbre_single, &visites);
    affiche_resultat("est_abr_infixe_visites - Single node", res, 1, visites);
    libere_arbre(arbre_single);
}

/**
 * Comparaison des trois approches
 */
void comparer_approches(void) {
    affiche_titre("COMPARAISON DES TROIS APPROCHES");
    
    long long visites_naif, visites_def, visites_infixe;
    
    // Créer un ABR parfait
    affiche_sous_titre("Comparaison sur un ABR parfait");
    Arbre abr = creer_abr_parfait();
    
    est_abr_naif_visites(abr, &visites_naif);
    est_abr_definition_visites(abr, &visites_def);
    est_abr_infixe_visites(abr, &visites_infixe);
    
    printf("Approche naïve:       %lld visites\n", visites_naif);
    printf("Approche optimisée:   %lld visites\n", visites_def);
    printf("Approche infixe:      %lld visites\n", visites_infixe);
    
    libere_arbre(abr);
    
    // Créer un arbre dégénéré
    affiche_sous_titre("Comparaison sur un arbre dégénéré");
    Arbre deg = creer_abr_degenere();
    
    est_abr_naif_visites(deg, &visites_naif);
    est_abr_definition_visites(deg, &visites_def);
    est_abr_infixe_visites(deg, &visites_infixe);
    
    printf("Approche naïve:       %lld visites\n", visites_naif);
    printf("Approche optimisée:   %lld visites\n", visites_def);
    printf("Approche infixe:      %lld visites\n", visites_infixe);
    
    libere_arbre(deg);
}

/*
 * ============================================================================
 * FONCTION PRINCIPALE
 * ============================================================================
 */

int main(void) {
    printf("\n");
    printf("╔═════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║           TEST COMPLET DES TROIS APPROCHES DE VÉRIFICATION D'ABR             ║\n");
    printf("╚═════════════════════════════════════════════════════════════════════════════╝\n");
    
    // Exécuter les tests
    tester_approche_naive();
    tester_approche_optimisee();
    tester_approche_infixe();
    comparer_approches();
    
    printf("\n");
    printf("╔═════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                        TESTS TERMINÉS                                       ║\n");
    printf("╚═════════════════════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    return 0;
}