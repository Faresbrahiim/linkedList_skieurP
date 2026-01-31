#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ===== STRUCTURE ===== */
typedef struct Skieur {
    char nom[30];
    int dossard;
    int t1;
    int t2;
    int total;
    struct Skieur* suivant;
} Skieur;

/* ===== PROTOTYPES ===== */
void ajouterDebut(Skieur** liste, Skieur* s);
void triBulleTemps(Skieur* liste, int mode);
Skieur* inverserListe(Skieur* liste);
void saisirPremiereManche(Skieur** liste, Skieur** abandon, int n);
void saisirDeuxiemeManche(Skieur** liste, Skieur** abandon);
void trierAbandonsParNom(Skieur** abandon);
void afficherResultats(Skieur* liste, Skieur* abandon);

/* ===== FONCTIONS ===== */

void ajouterDebut(Skieur** liste, Skieur* s) {
    s->suivant = *liste;
    *liste = s;
}

/* mode = 1 → tri t1 | mode = 2 → tri total */
void triBulleTemps(Skieur* liste, int mode) {
    if (!liste) return;

    for (Skieur* end = NULL; end != liste;) {
        for (Skieur* p = liste; p->suivant != end; p = p->suivant) {
            Skieur* q = p->suivant;
            int v1 = (mode == 1) ? p->t1 : p->total;
            int v2 = (mode == 1) ? q->t1 : q->total;

            if (v1 > v2) {
                Skieur tmp = *p;
                *p = *q;
                *q = tmp;

                Skieur* t = p->suivant;
                p->suivant = q->suivant;
                q->suivant = t;
            }
        }
        Skieur* p = liste;
        while (p->suivant != end) p = p->suivant;
        end = p;
    }
}

Skieur* inverserListe(Skieur* liste) {
    Skieur* inv = NULL;
    while (liste) {
        Skieur* tmp = liste->suivant;
        liste->suivant = inv;
        inv = liste;
        liste = tmp;
    }
    return inv;
}

void saisirPremiereManche(Skieur** liste, Skieur** abandon, int n) {
    int dossard = 1;

    for (int i = 0; i < n; i++) {
        Skieur* s = malloc(sizeof(Skieur));
        if (!s) exit(1);

        printf("\nNom : ");
        scanf("%s", s->nom);

        s->dossard = dossard++;
        printf("Temps 1ere manche (0 si abandon) : ");
        scanf("%d", &s->t1);

        s->t2 = 0;
        s->total = s->t1;
        s->suivant = NULL;

        if (s->t1 == 0)
            ajouterDebut(abandon, s);
        else
            ajouterDebut(liste, s);
    }
}

void saisirDeuxiemeManche(Skieur** liste, Skieur** abandon) {
    Skieur* p = *liste;
    Skieur* prev = NULL;

    printf("\n--- 2eme manche ---\n");

    while (p) {
        printf("Temps 2eme manche pour %s (0 si abandon) : ", p->nom);
        scanf("%d", &p->t2);

        if (p->t2 == 0) {
            Skieur* tmp = p;

            if (prev == NULL)
                *liste = p->suivant;
            else
                prev->suivant = p->suivant;

            p = p->suivant;

            tmp->total = 0;
            ajouterDebut(abandon, tmp);
        } else {
            p->total += p->t2;
            prev = p;
            p = p->suivant;
        }
    }
}

void trierAbandonsParNom(Skieur** abandon) {
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

void afficherResultats(Skieur* liste, Skieur* abandon) {
    printf("\n=== CLASSEMENT FINAL ===\n");

    if (liste) {
        int vainqueur = liste->total;
        for (Skieur* p = liste; p; p = p->suivant) {
            printf("%s | Dossard: %d | Total: %d | Ecart: %d\n",
                   p->nom, p->dossard, p->total, p->total - vainqueur);
        }
    } else {
        printf("Aucun skieur n'a terminé la course.\n");
    }

    printf("\n=== ABANDONS (triés par nom) ===\n");
    for (Skieur* p = abandon; p; p = p->suivant)
        printf("%s | Dossard: %d\n", p->nom, p->dossard);
}

/* ===== MAIN ===== */
int main() {
    Skieur* liste = NULL;
    Skieur* abandon = NULL;
    int n;

    printf("Nombre de skieurs : ");
    scanf("%d", &n);

    saisirPremiereManche(&liste, &abandon, n);
    triBulleTemps(liste, 1);
    liste = inverserListe(liste);
    saisirDeuxiemeManche(&liste, &abandon);
    triBulleTemps(liste, 2);
    trierAbandonsParNom(&abandon);
    afficherResultats(liste, abandon);
    return 0;
}
