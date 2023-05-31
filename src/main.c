#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>

#include "chargement.h"

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define BLU "\x1B[34m"
#define RESET "\x1B[0m"

/*
 * Retourne la différence en millisecondes entre t1 et t0.
 */
float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 3)
    {
        fprintf(stderr, "Usage: main fichier-dimacs\n");
        exit(1);
    }
    bool debug = argv[2] != NULL && argv[2][0] == 'd';
    if (debug)
        printf("Debug mode\n\n");

    struct timeval t0;
    struct timeval t1;

    /* Importation des données et initialisation du graphe */
    struct graphe *G = (struct graphe *)malloc(sizeof(struct graphe));
    gettimeofday(&t0, 0);
    initGraph(argv[1], G);
    gettimeofday(&t1, 0);

    printf(BLU "Fin du chargement en %f ms\n" RESET, timedifference_msec(t0, t1));
    if (debug)
        printGraph(G);


    /* Calcul de l'ordre topologique */
    int *Sorted;
    Sorted = (int *)malloc(G->p * sizeof(int));

    gettimeofday(&t0, 0);
    ordreTopologique(G->p, G, 0, Sorted);
    gettimeofday(&t1, 0);

    printf(BLU "Fin du calcul de l'ordre topologique en %f ms\n" RESET, timedifference_msec(t0, t1));
    if (debug)
    {
        for (int i = 0; i < G->p; i++)
            printf("Sorted[%2d] = %4d\n", i, Sorted[i]);
    }

    /* Calcul des dates au plus tard avec l'algorithme de Bellman */
    gettimeofday(&t0, 0);
    G = bellman(G->p, G, Sorted);
    gettimeofday(&t1, 0);

    printf(BLU "Fin du calcul de Bellman en %f ms\n" RESET, timedifference_msec(t0, t1));
    if (debug)
    {
        for (int i = 0; i < G->p; i++)
            printf("DateTard[%2d] = %4d\n", i, G->Head[i].dateTard);
    }

    /* Calcul des dates au plus tot avec l'algorithme de Bellman Inversé */
    gettimeofday(&t0, 0);
    G = bellmanInv(G->p, G, Sorted);
    gettimeofday(&t1, 0);

    printf(BLU "Fin du calcul de Bellman Inversé en %f ms\n" RESET, timedifference_msec(t0, t1));
    if (debug)
    {
        for (int i = 0; i < G->p; i++)
            printf("DateTot[%2d] = %4d\n", i, G->Head[i].dateTot);
    }


    /* Affichage des tâches critiques */
    if (debug)
    {
        printf("\n");
        for (int i = 0; i < G->p; i++)
        {
            if (G->Head[i].estCritique)
                printf(RED "[%2d] est critique.\n" RESET, i);
            else
                printf(GRN "[%2d] n'est pas critique avec une marge de %d.\n" RESET, i, G->Head[i].marge);
        }
    }

    printf("\nValeur optimale : %d\n\n", G->Head[G->p - 1].dateTard);

    /* Libération de la mémoire */
    free(Sorted);
    free(G->Head);
    free(G->Pred);
    free(G->Succ);
    free(G);
}