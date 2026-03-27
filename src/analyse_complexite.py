import os
import sys

import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np
import pandas as pd

# Chargement du fichier


CSV_FILE = "mesures.csv"

if not os.path.exists(CSV_FILE):
    print(f"Erreur : fichier '{CSV_FILE}' introuvable.")
    print("Lancez d'abord l'exécutable main pour générer les mesures.")
    sys.exit(1)

df = pd.read_csv(CSV_FILE, sep=";")

# Vérification des colonnes attendues
colonnes_attendues = {"Taille", "Morphologie", "Methode", "Nb_visites", "Temps"}
if not colonnes_attendues.issubset(df.columns):
    print(f"Erreur : colonnes manquantes. Trouvées : {list(df.columns)}")
    sys.exit(1)

print(f"Données chargées : {len(df)} lignes")
print(f"Morphologies : {sorted(df['Morphologie'].unique())}")
print(f"Méthodes     : {sorted(df['Methode'].unique())}")
print(f"Tailles      : {sorted(df['Taille'].unique())}\n")


df_moy = df.groupby(["Taille", "Morphologie", "Methode"], as_index=False).agg(
    Nb_visites=("Nb_visites", "mean"), Temps=("Temps", "mean")
)


# Couleur par méthode
COULEURS = {
    "naif": "#e74c3c",  # rouge
    "definition": "#2980b9",  # bleu
    "infixe": "#27ae60",  # vert
}


STYLES = {
    True: "-",  # ABR
    False: "--",  # non-ABR
}

MARQUEURS = {
    "naif": "o",
    "definition": "s",
    "infixe": "^",
}

# Morphologies regroupées pour les sous-graphiques
MORPHOLOGIES_ABR = ["ABR_presque_complet", "ABR_filiforme", "ABR_quelconque"]
MORPHOLOGIES_NON_ABR = [
    "non_ABR_presque_complet",
    "non_ABR_filiforme",
    "non_ABR_quelconque",
]


def label_morpho(m):
    """Renvoie un libellé court pour la légende."""
    return m.replace("_", " ").replace("non ABR", "non-ABR")


# Fonction utilitaire de tracé


def tracer_courbes(ax, data, y_col, morphologies, methodes, titre, ylabel, log=False):
    """
    Trace sur `ax` une courbe par (morphologie, méthode).
    - data       : DataFrame agrégé
    - y_col      : "Nb_visites" ou "Temps"
    - morphologies : liste de morphologies à afficher
    - methodes   : liste de méthodes à afficher
    - log        : True => échelle logarithmique sur les deux axes
    """
    for morpho in morphologies:
        est_abr = not morpho.startswith("non_")
        for methode in methodes:
            sous = data[
                (data["Morphologie"] == morpho) & (data["Methode"] == methode)
            ].sort_values("Taille")

            if sous.empty:
                continue

            couleur = COULEURS.get(methode, "black")
            style = STYLES[est_abr]
            marqueur = MARQUEURS.get(methode, "x")

            ax.plot(
                sous["Taille"],
                sous[y_col],
                linestyle=style,
                color=couleur,
                marker=marqueur,
                markersize=4,
                linewidth=1.5,
                label=f"{methode} / {label_morpho(morpho)}",
            )

    if log:
        ax.set_xscale("log")
        ax.set_yscale("log")

    ax.set_title(titre, fontsize=11, fontweight="bold")
    ax.set_xlabel("Taille de l'arbre (n)", fontsize=9)
    ax.set_ylabel(ylabel, fontsize=9)
    ax.legend(fontsize=7, ncol=1, loc="upper left")
    ax.grid(True, which="both", linestyle=":", alpha=0.6)


methodes = list(COULEURS.keys())

fig1, axes1 = plt.subplots(1, 2, figsize=(14, 5))
fig1.suptitle("Nombre de nœuds visités — Arbres ABR", fontsize=13, fontweight="bold")

tracer_courbes(
    axes1[0],
    df_moy,
    "Nb_visites",
    MORPHOLOGIES_ABR,
    methodes,
    titre="Échelle linéaire",
    ylabel="Nœuds visités (moyenne)",
    log=False,
)
tracer_courbes(
    axes1[1],
    df_moy,
    "Nb_visites",
    MORPHOLOGIES_ABR,
    methodes,
    titre="Échelle logarithmique",
    ylabel="Nœuds visités (moyenne)",
    log=True,
)

fig1.tight_layout()
fig1.savefig("fig1_visites_ABR.png", dpi=150)
print("Figure 1 sauvegardée : fig1_visites_ABR.png")


fig2, axes2 = plt.subplots(1, 2, figsize=(14, 5))
fig2.suptitle(
    "Nombre de nœuds visités — Arbres non-ABR", fontsize=13, fontweight="bold"
)

tracer_courbes(
    axes2[0],
    df_moy,
    "Nb_visites",
    MORPHOLOGIES_NON_ABR,
    methodes,
    titre="Échelle linéaire",
    ylabel="Nœuds visités (moyenne)",
    log=False,
)
tracer_courbes(
    axes2[1],
    df_moy,
    "Nb_visites",
    MORPHOLOGIES_NON_ABR,
    methodes,
    titre="Échelle logarithmique",
    ylabel="Nœuds visités (moyenne)",
    log=True,
)

fig2.tight_layout()
fig2.savefig("fig2_visites_non_ABR.png", dpi=150)
print("Figure 2 sauvegardée : fig2_visites_non_ABR.png")


# Figure 3 : Comparaison ABR vs non-ABR


formes = ["presque_complet", "filiforme", "quelconque"]
fig3, axes3 = plt.subplots(1, 3, figsize=(18, 5))
fig3.suptitle(
    "ABR vs non-ABR : impact de l'arrêt précoce (Nb_visites, échelle log)",
    fontsize=13,
    fontweight="bold",
)

for ax, forme in zip(axes3, formes):
    morphos_forme = [f"ABR_{forme}", f"non_ABR_{forme}"]
    tracer_courbes(
        ax,
        df_moy,
        "Nb_visites",
        morphos_forme,
        methodes,
        titre=forme.replace("_", " ").capitalize(),
        ylabel="Nœuds visités (moyenne)",
        log=True,
    )

fig3.tight_layout()
fig3.savefig("fig3_ABR_vs_nonABR.png", dpi=150)
print("Figure 3 sauvegardée : fig3_ABR_vs_nonABR.png")


# Figure 4 : Temps d'exécution, toutes morphologies


toutes_morphos = MORPHOLOGIES_ABR + MORPHOLOGIES_NON_ABR

fig4, axes4 = plt.subplots(1, 2, figsize=(14, 5))
fig4.suptitle("Temps d'exécution (toutes morphologies)", fontsize=13, fontweight="bold")

tracer_courbes(
    axes4[0],
    df_moy,
    "Temps",
    toutes_morphos,
    methodes,
    titre="Échelle linéaire",
    ylabel="Temps (µs, moyenne)",
    log=False,
)
tracer_courbes(
    axes4[1],
    df_moy,
    "Temps",
    toutes_morphos,
    methodes,
    titre="Échelle logarithmique",
    ylabel="Temps (µs, moyenne)",
    log=True,
)

fig4.tight_layout()
fig4.savefig("fig4_temps.png", dpi=150)
print("Figure 4 sauvegardée : fig4_temps.png")


# Figure 5 — Courbes de référence théoriques


tailles = np.array(sorted(df["Taille"].unique()), dtype=float)
if len(tailles) >= 2:
    fig5, ax5 = plt.subplots(figsize=(8, 5))
    fig5.suptitle("Références théoriques (normalisées)", fontsize=13, fontweight="bold")

    refs = {
        "O(n)": tailles,
        "O(n log n)": tailles * np.log2(tailles + 1),
        "O(n²)": tailles**2,
    }
    styles_ref = ["-", "--", ":"]
    for (label, vals), st in zip(refs.items(), styles_ref):
        vals_norm = vals / vals[-1]  # normalisation pour comparer les formes
        ax5.plot(tailles, vals_norm, linestyle=st, linewidth=2, label=label)

    ax5.set_xlabel("Taille de l'arbre (n)", fontsize=9)
    ax5.set_ylabel("Valeur normalisée", fontsize=9)
    ax5.set_title("Formes théoriques sur la plage de tailles testée", fontsize=10)
    ax5.legend(fontsize=9)
    ax5.grid(True, linestyle=":", alpha=0.6)

    fig5.tight_layout()
    fig5.savefig("fig5_references_theoriques.png", dpi=150)
    print("Figure 5 sauvegardée : fig5_references_theoriques.png")


plt.show()
print("\nAnalyse terminée.")
