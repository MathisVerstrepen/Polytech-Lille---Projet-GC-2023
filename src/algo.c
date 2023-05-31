#include "algo.h"

/* Algorithme de tri topologique :
 * Tri topologique du graphe G de taille n à partir du sommet r et stockage du résultat dans Sorted
 *  n : nombre de sommets
 *  G : graphe
 *  r : indice du sommet racine
 *  Sorted : tableau des indices des sommets triés par ordre topologique
 */
void ordreTopologique(int n, struct graphe *G, int r, int Sorted[])
{
    bool Mark[n]; // Liste des sommets marqués
    int Next[n];  // Liste des indices des successeurs à visiter
    for (int i = 0; i < n; i++)
    {
        Mark[i] = 0;
        Next[i] = G->Head[i].succIdx;
    }

    Mark[0] = 1;
    Mark[n - 1] = 1;
    Sorted[n - 1] = n - 1;

    /* Initialisation de la pile et ajout du sommet r */
    struct Pile *Z = (struct Pile *)malloc(sizeof(struct Pile));
    initPile(Z);
    ajouter_en_tete(Z, r);

    int idxSorted = n - 1; // Indice courant dans la liste des indices de sommets triés par ordre topologique
    int courant;           // Indice du sommet courant
    int successeur;        // indice d'un successeur

    /* Parcours en profondeur tant que la pile n'est pas vide */
    while (!estPileVide(Z))
    {
        courant = renvoiTete(Z); // On récupère la valeur de la tête de la pile

        // Si le sommet courant n'a plus de successeurs, on l'ajoute à la liste des sommets triés
        if (Next[courant] >= G->Head[courant + 1].succIdx)
        {
            Sorted[idxSorted - 1] = courant;
            courant = depiler(Z);

            idxSorted -= 1;
        }
        // Sinon, on ajoute le successeur courant à la pile et on le marque comme visité
        else
        {
            successeur = G->Succ[Next[courant]];

            Next[courant] = Next[courant] + 1;
            if (Mark[successeur] == 0)
            {
                Mark[successeur] = 1;
                ajouter_en_tete(Z, successeur);
            }
        }
    }
    free(Z); // Libération de la mémoire allouée à la pile
}

/* Algorithme de calcul des dates au plus tard :
 * Calcul des dates au plus tard des sommets du graphe G de taille n et stockage du résultat dans G
 * avec la méthode de Bellman
 *  n : nombre de sommets
 *  G : graphe
 *  Sorted : tableau des indices des sommets triés par ordre topologique
 */
struct graphe *bellman(int n, struct graphe *G, int Sorted[])
{
    G->Head[Sorted[0]].dateTard = 0;
    int k, i;       // Indice de boucle
    int successeur; // Indice d'un successeur

    for (k = 1; k < n; k++)
    {
        successeur = Sorted[k];

        // Calcul de predIdx du prochain sommet et gestion du cas limite du sommet terminal
        int borne = k == n - 1 ? G->nbaretes + 1 : G->Head[successeur + 1].predIdx;

        // Pour chaque predecesseur calcul de la nouvelle dateTard et modification si plus grande que l'ancienne
        for (i = G->Head[successeur].predIdx; i < borne; i++)
        {
            int prede = G->Pred[i];
            int temp = G->Head[prede].dateTard + G->Head[prede].poids;

            if (temp > G->Head[successeur].dateTard)
                G->Head[successeur].dateTard = temp;
        }
    }

    return G;
}

/* Algorithme de calcul des dates au plus tôt :
 * Calcul des dates au plus tôt des sommets du graphe G de taille n et stockage du résultat dans G
 * avec la méthode de Bellman inversée
 * n : nombre de sommets
 * G : graphe
 * Sorted : tableau des indices des sommets triés par ordre topologique
 */
struct graphe *bellmanInv(int n, struct graphe *G, int Sorted[])
{
    G->Head[n - 1].dateTot = G->Head[n - 1].dateTard;
    int k, i;         // Indice de boucle
    int predecesseur; // Indice d'un predecesseur

    for (k = n - 2; k > -1; k--)
    {
        predecesseur = Sorted[k];

        // Pour chaque successeur calcul de la nouvelle dateTot et modification si plus petite que l'ancienne
        for (i = G->Head[predecesseur].succIdx; i < G->Head[predecesseur + 1].succIdx; i++)
        {
            int y = G->Succ[i];
            int temp = G->Head[y].dateTot - G->Head[predecesseur].poids;

            if (temp < G->Head[predecesseur].dateTot)
                G->Head[predecesseur].dateTot = temp;
        }

        // Calcul de la marge et de la criticité du sommet précédemmment traité
        G->Head[Sorted[k + 1]].marge = G->Head[Sorted[k + 1]].dateTot - G->Head[Sorted[k + 1]].dateTard;
        G->Head[Sorted[k + 1]].estCritique = G->Head[Sorted[k + 1]].marge == 0 ? 1 : 0;
    }

    G->Head[0].estCritique = 1;

    return G;
}