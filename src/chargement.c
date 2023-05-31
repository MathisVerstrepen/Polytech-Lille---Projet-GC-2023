#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "chargement.h"

#define MAX_LINE_SIZE 256

/*
 * Affiche le graphe G.
 * Pour chaque sommet, on affiche son poids, son successeur et son predecesseur.
 * On affiche aussi les listes des predecesseurs et des successeurs.
 */
void printGraph(struct graphe *G)
{
    int i;

    printf("\nNombre de sommets : %d\n", G->p);
    printf("Nombre d'aretes : %d\n", G->nbaretes);

    printf("\nHead : \n");
    printf("           Poids Succ Pred\n");
    for (i = 0; i < G->p; i++)
    {
        printf("Head[%2d] = %4d %4d %4d\n", i, G->Head[i].poids, G->Head[i].succIdx, G->Head[i].predIdx);
    }
    printf("\nPred : \n");
    for (i = 0; i <= G->nbaretes; i++)
    {
        printf("Pred[%2d] = %4d\n", i, G->Pred[i]);
    }
    printf("\nSucc : \n");
    for (i = 0; i < G->nbaretes; i++)
    {
        printf("Succ[%2d] = %4d\n", i, G->Succ[i]);
    }
}

/*
 * Extrait les donnees du fichier de nom file_name et les stocke dans G.
 * Provoque une erreur si le fichier est inaccessible.
 */
void initGraph(char *file_name, struct graphe *G)
{
    char ligne[MAX_LINE_SIZE]; // pour le passage de ligne
    int nb_noeuds;             // nombre de noeuds

    // Ouverture du fichier.
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "%s non accessible\n", file_name);
        exit(1);
    }

    // Passage des commentaires : lignes commençant par 'c'
    fgets(ligne, MAX_LINE_SIZE, fp);

    // Lecture du nombre de noeuds : ligne 'p num_noeuds'
    fgetc(fp);
    fgetc(fp);
    fscanf(fp, "%d", &nb_noeuds);

    // Initialisation du graphe G
    G->nbaretes = 0;
    G->p = nb_noeuds + 2;
    G->Head = (struct sommet *)malloc((G->p) * sizeof(struct sommet));

    /* Lecture des noeuds :
     *  ligne 'v num_noeud duree'
     *  Pour chaque noeud, on cree un sommet dans G->Head[num_noeud] et on initialise ses champs.
     *  On cree aussi un sommet fictif 0 (Début) qui sera le predecesseur de tous les sommets sans predecesseur.
     *  On cree aussi un sommet fictif G->p-1 (Fin) qui sera le successeur de tous les sommets sans successeur.
     *  On initialise les champs dateTard et dateTot de tous les sommets a -INT_MAX et INT_MAX respectivement
     *  pour simplifier les calculs lors de l'algorithme de Bellman et Bellman Inversé.
     */
    {
        int num_noeud;
        int duree;

        for (num_noeud = 1; num_noeud <= nb_noeuds; num_noeud++)
        {
            fgetc(fp);
            fgetc(fp); // on passe l'entete 'v '
            fscanf(fp, "%d", &num_noeud);
            fscanf(fp, "%d", &duree);

            G->Head[num_noeud].poids = duree;
            G->Head[num_noeud].succIdx = 0;
            G->Head[num_noeud].predIdx = 0;
            G->Head[num_noeud].dateTard = -INT_MAX;
            G->Head[num_noeud].dateTot = INT_MAX;
        }

        // Sommet fictif 0 (Début) et sommet fictif G->p-1 (Fin)
        G->Head[0].poids = 0;
        G->Head[0].succIdx = 0;
        G->Head[0].predIdx = 0;
        G->Head[0].dateTard = 0;
        G->Head[0].dateTot = 0;

        G->Head[G->p - 1].poids = 0;
        G->Head[G->p - 1].dateTard = -INT_MAX;
    }

    /* Lecture des aretes :
     *  ligne 'a num_pred num_noeud'
     *  Pour chaque arete, on incremente le nombre de successeurs de num_pred et le nombre de predecesseurs de num_noeud.
     *  On incremente aussi le nombre d'aretes du graphe.
     */
    {
        int num_pred;
        int num_noeud;
        char c;

        fgets(ligne, MAX_LINE_SIZE, fp);
        c = fgetc(fp);

        while (c == 'a')
        {
            fscanf(fp, "%d", &num_pred);
            fscanf(fp, "%d", &num_noeud);

            fgets(ligne, MAX_LINE_SIZE, fp);
            c = fgetc(fp);

            G->nbaretes += 1;
            G->Head[num_pred].succIdx += 1;
            G->Head[num_noeud].predIdx += 1;
        }
    }

    /* Detection des sommets sans predecesseurs et sans successeurs
     *  On cree deux tableaux no_succ et no_pred qui contiennent les numeros des sommets
     *  sans successeurs et sans predecesseurs respectivement.
     *  On incremente le nombre d'aretes du graphe pour chaque sommet sans successeur et sans predecesseur.
     *  On créé deux variables nb_no_succ et nb_no_pred qui contiennent le nombre de sommets sans successeurs et sans predecesseurs respectivement.
     */
    int *no_succ = (int *)malloc(nb_noeuds * sizeof(int));
    int *no_pred = (int *)malloc(nb_noeuds * sizeof(int));
    int nb_no_succ = 0;
    int nb_no_pred = 0;
    {
        for (int i = 1; i <= nb_noeuds; i++)
        {
            if (G->Head[i].succIdx == 0)
            {
                G->Head[i].succIdx = 1;
                no_succ[nb_no_succ] = i;

                G->nbaretes += 1;
                nb_no_succ += 1;
            }

            if (G->Head[i].predIdx == 0)
            {
                G->Head[i].predIdx = 1;
                no_pred[nb_no_pred] = i;

                G->nbaretes += 1;
                nb_no_pred += 1;
            }
        }

        nb_no_succ += 1;
    }

    /* Calcul des indices des successeurs et des predecesseurs
     * On parcourt le tableau des sommets de G et on fait la somme successive des nombres de successeurs et de predecesseurs.
     */
    {
        int tmpSucc = G->Head[1].succIdx;
        int tmpPred = G->Head[1].predIdx;

        G->Head[1].succIdx = nb_no_pred;
        G->Head[1].predIdx = 1;

        int succIdx, predIdx;

        for (int i = 2; i < G->p; i++)
        {
            succIdx = G->Head[i].succIdx;
            G->Head[i].succIdx = tmpSucc + G->Head[i - 1].succIdx;
            tmpSucc = succIdx;

            predIdx = G->Head[i].predIdx;
            G->Head[i].predIdx = tmpPred + G->Head[i - 1].predIdx;
            tmpPred = predIdx;
        }

        G->Head[G->p - 1].succIdx = G->nbaretes;
    }

    rewind(fp); // On se replace au debut du fichier
    char c = fgetc(fp);
    while (c != 'a') // On passe les lignes 'c', 'p' et 'v' pour arriver aux lignes 'a'
    {
        fgets(ligne, MAX_LINE_SIZE, fp);
        c = fgetc(fp);
    }

    /* Lecture des aretes :
     *  ligne 'a num_pred num_noeud'
     *  Allocation des tableaux des successeurs et des predecesseurs de G.
     *  Pour chaque arete, on ajoute le successeur num_noeud dans le tableau des successeurs de num_pred
     *      et on ajoute le predecesseur num_pred dans le tableau des predecesseurs de num_noeud.
     * On ajoute également les aretes des sommets sans successeurs et sans predecesseurs.
     */
    {
        int *cursor_succ = (int *)calloc(nb_noeuds + 2, sizeof(int)); // tableau temporaire de curseurs pour chaque sommet
        G->Succ = (int *)malloc((G->nbaretes + 3) * sizeof(int));     // tableau des successeurs
        G->Succ[0] = 1;

        int *cursor_pred = (int *)calloc(nb_noeuds + 2, sizeof(int)); // tableau temporaire de curseurs pour chaque sommet
        G->Pred = (int *)malloc((G->nbaretes + 4) * sizeof(int));     // tableau des predecesseurs
        G->Pred[0] = 0;
        G->Pred[1] = 0;

        int num_pred;
        int num_noeud;
        while (c == 'a')
        {
            fscanf(fp, "%d", &num_pred);
            fscanf(fp, "%d", &num_noeud);

            G->Succ[G->Head[num_pred].succIdx + cursor_succ[num_pred]] = num_noeud;
            cursor_succ[num_pred] += 1;

            G->Pred[G->Head[num_noeud].predIdx + cursor_pred[num_noeud]] = num_pred;
            cursor_pred[num_noeud] += 1;

            fgets(ligne, MAX_LINE_SIZE, fp);
            c = fgetc(fp);
        }

        // Ajout des aretes des sommets sans successeurs vers le sommet fin
        for (int i = 0; i < nb_no_succ - 1; i++)
        {
            G->Succ[G->Head[no_succ[i]].succIdx] = nb_noeuds + 1;
            G->Pred[G->Head[nb_noeuds + 1].predIdx + cursor_pred[nb_noeuds + 1]] = no_succ[i];
            cursor_pred[nb_noeuds + 1] += 1;
        }

        // Ajout des aretes du sommet debut vers les sommets sans predecesseurs
        for (int i = 0; i < nb_no_pred; i++)
        {
            G->Succ[G->Head[0].succIdx + cursor_succ[0]] = no_pred[i];
            cursor_succ[0] += 1;
            G->Pred[G->Head[no_pred[i]].predIdx] = 0;
        }

        // On libere les tableaux temporaires de curseurs
        free(cursor_succ);
        free(cursor_pred);
    }

    // On libere les tableaux des sommets sans successeurs et sans predecesseurs
    free(no_succ);
    free(no_pred);

    // On ferme le fichier
    fclose(fp);
}
