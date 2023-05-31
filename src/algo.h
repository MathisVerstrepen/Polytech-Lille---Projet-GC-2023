#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "pile.h"

struct sommet
{
    int succIdx;
    int predIdx;
    int poids;
    int dateTot;
    int dateTard;
    int marge;
    bool estCritique;
};

struct graphe
{
    int p; 
    struct sommet* Head;
    int* Pred;
    int* Succ;
    int nbaretes;
};

#define NILSOMMET (struct* sommet)0

extern void ordreTopologique(int, struct graphe*, int, int*);

extern struct graphe* bellman(int, struct graphe*, int*);

extern struct graphe* bellmanInv(int, struct graphe*, int*);