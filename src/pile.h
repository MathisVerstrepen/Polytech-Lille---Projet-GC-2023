#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdbool.h>

struct maillon
{
    int value;
    struct maillon *next;
};

struct Pile
{
    struct maillon *tete;
    int nbelem;
};

#define NILMAILLON (struct maillon*)0

extern void initPile(struct Pile*);

extern void clearPile(struct Pile*);


extern void ajouter_en_tete(struct Pile*, int);

extern int depiler(struct Pile*);

extern bool estPileVide(struct Pile*);

extern int renvoiTete(struct Pile*);


