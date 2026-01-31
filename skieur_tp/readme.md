### README – Gestion d’une course de ski (V2)

### Pourquoi une liste chaînée ?

Le nombre de skieurs n’est pas fixe.
La liste chaînée permet :

* Ajouter facilement des skieurs
* Déplacer les skieurs qui abandonnent dans une autre liste

---

### Lecture du fichier et création de la liste

```cpp
Skieur* liste = nullptr;
FILE* f = fopen(nomFichier, "r");
```

* On initialise la liste principale vide.
* On ouvre le fichier `skieurs.txt` en lecture.
* Si le fichier n’existe pas, `f` sera `nullptr`.

**Boucle de lecture :**

```cpp
while (fscanf(f, "%s %d", nom, &dossard) == 2) { ... }
```

* Lit chaque ligne contenant le nom et le numéro de dossard.
* La boucle continue jusqu’à la fin du fichier.
* Le dossard est converti automatiquement en entier.

---

### Saisie des temps – 1ère manche

* On parcourt tous les skieurs de la liste principale (`liste`).
* On demande le temps pour chaque skieur :

```
0 si abandon
```

* `total = t1` au départ.
* Si le skieur abandonne (`t1 == 0`) :

  * `total = 0`
  * Déplacement du skieur dans la liste `abandon`

---

### Ajout d’un skieur

* La fonction `ajouterSkieur(Skieur*& liste, Skieur* s)` ajoute un skieur **au début** de la liste.
* Utilisée pour la liste des abandons ou lors de la lecture du fichier.

---

### Saisie des temps – 2ème manche

* La liste est **inversée** pour respecter l’ordre inverse du classement 1ère manche.
* Pour chaque skieur :

  * Saisie de `t2`
  * Si abandon : `total = 0` et ajout à la liste `abandon`
  * Sinon : `total += t2`

---

### Tri à bulle

* Pour trier la liste selon différents critères :

```cpp
triBulle(liste, 1); // Tri 1ère manche
triBulle(liste, 2); // Tri temps total
triBulle(abandon, 0); // Tri alphabétique pour les abandons
```

* Le tri se fait **sur les pointeurs**, pas besoin d’échanger les valeurs.

---

### Calcul du temps moyen

* La fonction `tempsMoyen(liste)` :

  * Ne prend en compte que les skieurs **ayant terminé les deux manches**
  * Retourne un double correspondant au temps moyen

---

### Fonctionnalités implémentées

| Fonctionnalité demandée                                | Code / Commentaire                                              |
| ------------------------------------------------------ | --------------------------------------------------------------- |
| Entrée de la 1ère manche (nom, dossard, temps)         | Oui, `lireFichier` + saisie `t1`                                |
| Liste chaînée des skieurs et abandons                  | Oui, `liste` et `abandon`                                       |
| Entrée de la 2ème manche, cumul ou abandon             | Oui, saisie `t2` + `total += t2` ou `total=0` + ajout à abandon |
| Tri pour classement                                    | Oui, `triBulle(liste, 1)` et `triBulle(liste, 2)`               |
| Affichage du classement final                          | Oui, `sauvegarderFichier` + `afficherFichier`                   |
| Affichage de la liste des abandons                     | Oui, dans `sauvegarderFichier`                                  |
| Temps moyen des skieurs ayant terminé les deux manches | Oui, `tempsMoyen(liste)`                                        |
| Récupération noms/dossards depuis fichier texte        | Oui, `lireFichier`                                              |
| Sauvegarde résultats dans fichier texte                | Oui, `sauvegarderFichier`                                       |
| Affichage du contenu du fichier résultat               | Oui, `afficherFichier`                                          |

---

### Exécution

1. Compiler :

```bash
g++ main.cpp skieur.cpp -o tp_ski
```

2. Exécuter :

```bash
./tp_ski.exe
```

* Le programme lira `skieurs.txt`, demandera les temps des deux manches, générera `resultats.txt` et affichera le contenu à l’écran.
