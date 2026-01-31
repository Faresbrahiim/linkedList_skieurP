#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Skieur {
    char nom[30];
    int dossard;
    int t1;
    int t2;
    int total;
    struct Skieur* suivant;
} Skieur;

void triBulle(Skieur* liste, int champ) {
    if (!liste) return;

    int swapped;
    do {
        swapped = 0;
        Skieur* p = liste;
        while (p->suivant) {
            Skieur* q = p->suivant;
            int valP = (champ == 1) ? p->t1 : p->total;
            int valQ = (champ == 1) ? q->t1 : q->total;
            if (valP > valQ) {
                Skieur temp = *p;
                *p = *q;
                *q = temp;
                Skieur* tmp = p->suivant;
                p->suivant = q->suivant;
                q->suivant = tmp;
                swapped = 1;
            }
            p = p->suivant;
        }
    } while (swapped);
}

/* Tri alphabétique des abandons */
void triAbandons(Skieur** abandon) {
    if (!*abandon) return;

    int swap;
    do {
        swap = 0;
        Skieur* prev = NULL;
        Skieur* cur = *abandon;

        while (cur && cur->suivant) {
            Skieur* nxt = cur->suivant;
            if (strcmp(cur->nom, nxt->nom) > 0) {
                if (prev)
                    prev->suivant = nxt;
                else
                    *abandon = nxt;

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

/* Lecture skieurs depuis un fichier */
Skieur* lireSkieurs(const char* nomFichier, Skieur** abandon, int* nb) {
    FILE* f = fopen(nomFichier, "r");


    Skieur* liste = NULL;
    int dossard = 1;
    char nom[30];
    int t1, t2;
    *nb = 0;

    while (fscanf(f, "%s %d %d", nom, &t1, &t2) == 3) {
        Skieur* s = malloc(sizeof(Skieur));
        if (!s) continue;

        strcpy(s->nom, nom);
        s->dossard = dossard++;
        s->t1 = t1;
        s->t2 = t2;
        s->total = t1 + t2;
        s->suivant = NULL;

        if (t1 == 0 || t2 == 0) {
            // abandon
            s->total = 0;
            s->suivant = *abandon;
            *abandon = s;
        } else {
            // finisher
            s->suivant = liste;
            liste = s;
        }

        (*nb)++;
    }

    fclose(f);
    return liste;
}

/* Sauvegarde des résultats */
void sauvegarderResultats(Skieur* liste, Skieur* abandon) {
    FILE* f1 = fopen("classement.txt", "w");


    if (liste) {
        int vainqueur = liste->total;
        for (Skieur* p = liste; p != NULL; p = p->suivant) {
            fprintf(f1, "%s | Dossard: %d | Total: %d | Ecart: %d\n",
                    p->nom, p->dossard, p->total, p->total - vainqueur);
        }
    }
    fclose(f1);

    FILE* f2 = fopen("abandons.txt", "w");


    for (Skieur* p = abandon; p != NULL; p = p->suivant) {
        fprintf(f2, "%s | Dossard: %d\n", p->nom, p->dossard);
    }

    fclose(f2);
}

int main() {
    Skieur* liste = NULL;
    Skieur* abandon = NULL;
    int nbSkieurs = 0;

    // lire skieurs depuis fichier "skieurs.txt"
    // format: Nom t1 t2  (t1 ou t2 = 0 si abandon)
    liste = lireSkieurs("skieurs.txt", &abandon, &nbSkieurs);
    if (!liste && !abandon) {
        printf("Aucun skieur à traiter.\n");
        return 0;
    }

    // TRI final par total
    triBulle(liste, 2);

    // TRI alphabétique abandons
    triAbandons(&abandon);

    // affichage console
    printf("\n=== Classement final ===\n");
    if (liste) {
        int vainqueur = liste->total;
        for (Skieur* p = liste; p != NULL; p = p->suivant) {
            printf("%s | Dossard: %d | Total: %d | Ecart: %d\n",
                   p->nom, p->dossard, p->total, p->total - vainqueur);
        }
    }

    printf("\n=== Abandons ===\n");
    for (Skieur* p = abandon; p != NULL; p = p->suivant) {
        printf("%s | Dossard: %d\n", p->nom, p->dossard);
    }

    // sauvegarde résultats
    sauvegarderResultats(liste, abandon);

    // libérer mémoire
    Skieur* tmp;
    while (liste) { tmp = liste->suivant; free(liste); liste = tmp; }
    while (abandon) { tmp = abandon->suivant; free(abandon); abandon = tmp; }

    return 0;
}
