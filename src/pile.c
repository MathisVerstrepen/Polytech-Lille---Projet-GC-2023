#include "pile.h"

void initPile(struct Pile* P){
    P->nbelem= 0;
    P->tete=NILMAILLON;
}

void clearPile(struct Pile *P){
    struct maillon *courant;
    struct maillon *suivant;
    int i;

    courant = P->tete;
    for (i = 0; i < P->nbelem; i++)
      {
          suivant = courant->next;
          free (courant);
          courant = suivant;
      }
}

void ajouter_en_tete(struct Pile* P, int d){
    struct maillon *nouveau;

    nouveau = (struct maillon *) malloc (sizeof (struct maillon));
    nouveau->value = d;         
    nouveau->next = P->tete;
    P->tete = nouveau;
    P->nbelem += 1;

}

int depiler(struct Pile* P){
    struct maillon *tete;
    int val;
    tete = P->tete;
    val = tete->value;
    P->tete = tete->next;
    P->nbelem -= 1;
    free (tete);
    return(val);
}

bool estPileVide(struct Pile* P){
    return (P->nbelem==0);
}

int renvoiTete(struct Pile* P){
    return P->tete->value;
}