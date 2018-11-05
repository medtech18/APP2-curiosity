#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#ifdef NCURSES
#include <ncurses.h>
#endif
#include "listes.h"

/*
 *  Auteur(s) :
 *  Date :
 *  Suivi des Modifications :
 *
 */

void conversion (char *texte, sequence_t *seq)
{
  /* À compléter */
    cellule_t *cel;
    cel = nouvelleCellule();
    seq->tete = cel;
    cel->command = texte[0];

    for(int i = 1 ; texte[i] != '\0'; i++)
    {
        cel->suivant = nouvelleCellule();
        cel->suivant->command = texte[i];
        cel=cel->suivant;
    }

    cel->suivant = NULL;
}






void afficher (sequence_t* seq)
{
    /* À compléter */

    cellule_t *c = seq->tete;
    while(c->suivant != NULL)
    {
        printf("%c",c->command);
        c= c->suivant;
    }
    printf("%d",c->command);
}

cellule_t* nouvelleCellule (void)
{
    /* À compléter (utiliser malloc) */
    cellule_t* c = (cellule_t*) malloc(sizeof(cellule_t));
    return c;
}


void detruireCellule (cellule_t* cel)
{
    /* À compléter (utiliser free) */
    free(cel);
}
