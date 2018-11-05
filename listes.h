#ifndef LISTES_H
#define LISTES_H

#define eprintf(...) fprintf (stderr, __VA_ARGS__)


struct cellule {
    char   command;
    /* vous pouvez rajouter d'autres champs ici */
    struct cellule *suivant;
};
typedef struct cellule cellule_t;

struct sequence {
    cellule_t *tete;
};
typedef struct sequence sequence_t;

void conversion (char *texte, sequence_t *seq);

void afficher (sequence_t* seq);

cellule_t* nouvelleCellule (void);

void detruireCellule (cellule_t*);


#endif
