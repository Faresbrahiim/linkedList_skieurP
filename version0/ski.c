#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Structure d’un skieur */
typedef struct Skieur {
    char nom[30];
    int dossard;
    int t1;
    int t2;
    int total;
    struct Skieur* suivant;
} Skieur;

int main() {
    Skieur* liste = NULL;      // skieurs arrivés
    Skieur* abandon = NULL;    // skieurs abandons
    int n;
    int c = 1;  // dossard auto-increment

    printf("Nombre de skieurs : ");
    scanf("%d", &n);

    /* ====== SAISIE 1ère MANCHE ====== */
    for (int i = 0; i < n; i++) {
        Skieur* s = malloc(sizeof(Skieur));
        if (!s) return 1;

        s->suivant = NULL;

        printf("\nNom : ");
        scanf("%s", s->nom);

        s->dossard = c++;   // AUTO DOSSARD

        printf("Temps 1ere manche (0 si abandon) : ");
        scanf("%d", &s->t1);

        s->t2 = 0;
        s->total = s->t1;

        if (s->t1 == 0) {
            s->suivant = abandon;
            abandon = s;
        } else {
            s->suivant = liste;
            liste = s;
        }
    }

    /* ====== TRI BULLE 1ère MANCHE ====== */
    for (Skieur* end = NULL; end != liste; ) {
        for (Skieur* p = liste; p->suivant != end; p = p->suivant) {
            Skieur* q = p->suivant;
            if (p->t1 > q->t1) {
                Skieur temp = *p;
                *p = *q;
                *q = temp;
                Skieur* tmp = p->suivant;
                p->suivant = q->suivant;
                q->suivant = tmp;
            }
        }
        Skieur* p = liste;
        while (p->suivant != end) p = p->suivant;
        end = p;
    }

    /* ====== INVERSION LISTE (ordre inverse) ====== */
    Skieur* inverse = NULL;
    while (liste) {
        Skieur* tmp = liste->suivant;
        liste->suivant = inverse;
        inverse = liste;
        liste = tmp;
    }
    liste = inverse;

    /* ====== SAISIE 2ème MANCHE ====== */
    printf("\n--- 2eme manche ---\n");

    Skieur* p = liste;
    Skieur* prev = NULL;

    while (p != NULL) {
        printf("Temps 2eme manche pour %s (0 si abandon) : ", p->nom);
        scanf("%d", &p->t2);

        if (p->t2 == 0) {
            // retirer de liste
            Skieur* tmp = p;

            if (prev == NULL)
                liste = p->suivant;
            else
                prev->suivant = p->suivant;

            p = p->suivant;

            // ajouter aux abandons
            tmp->total = 0;
            tmp->suivant = abandon;
            abandon = tmp;
        } else {
            p->total += p->t2;
            prev = p;
            p = p->suivant;
        }
    }

    /* ====== TRI BULLE TEMPS TOTAL ====== */
    for (Skieur* end = NULL; end != liste; ) {
        for (Skieur* p = liste; p->suivant != end; p = p->suivant) {
            Skieur* q = p->suivant;
            if (p->total > q->total) {
                Skieur temp = *p;
                *p = *q;
                *q = temp;
                Skieur* tmp = p->suivant;
                p->suivant = q->suivant;
                q->suivant = tmp;
            }
        }
        Skieur* p = liste;
        while (p->suivant != end) p = p->suivant;
        end = p;
    }

    /* ====== TRI DES ABANDONS PAR NOM ====== */
    if (abandon) {
        int swap;
        do {
            swap = 0;
            Skieur* prev = NULL;
            Skieur* cur = abandon;

            while (cur && cur->suivant) {
                Skieur* nxt = cur->suivant;
                if (strcmp(cur->nom, nxt->nom) > 0) {
                    if (prev)
                        prev->suivant = nxt;
                    else
                        abandon = nxt;

                    cur->suivant = nxt->suivant;
                    nxt->suivant = cur;
                    prev = nxt;
                    swap = 1;
                } else {
                    prev = cur;
                    cur = cur->suivant;
                }
            }
        } while (swap);
    }

    /* ====== AFFICHAGE ====== */
    printf("\n=== CLASSEMENT FINAL ===\n");

    if (liste) {
        int vainqueur = liste->total;

        for (p = liste; p != NULL; p = p->suivant) {
            printf("%s | Dossard: %d | Total: %d | Ecart: %d\n",
                   p->nom, p->dossard, p->total, p->total - vainqueur);
        }
    } else {
        printf("Aucun skieur n'a terminé la course.\n");
    }

    printf("\n=== ABANDONS (triés par nom) ===\n");
    for (p = abandon; p != NULL; p = p->suivant) {
        printf("%s | Dossard: %d\n", p->nom, p->dossard);
    }

    return 0;
}
