#include <stdio.h>
#include <stdlib.h>

#define MAX 100

typedef struct tnoeud {
  int taille;
  int arbre[MAX];
} Tas;

/* FUNCTIONS */
void afficherTas(Tas T);
int indiceParent(int i);
int ajout(Tas *t, int valeur);
int estTas(int tab[], int taille);
int fils(Tas T, int indice);
void change(Tas *T, int indice, int valeur);
int ajout(Tas *T, int valeur);
int extraireMin(Tas *T);


/* ______________ MAIN ______________ */
int main(int argc, char *argv[]) {
  
  int i;
  
  /* CREATION DU TAS */
  Tas tas;
  tas.taille = 0;
  for (i=0; i<10; i++) {
    tas.arbre[i] = 3*i;
    tas.taille += 1;
  }
  afficherTas(tas);
  
  /* Verification */
  printf( " > Est un tas ? %d\n", estTas(tas.arbre, tas.taille));
  
  /* Fils */
  printf( " > Fils de %d : %d\n", tas.arbre[1], fils(tas, 1));
  printf( " > Fils de %d : %d\n", tas.arbre[2], fils(tas, 2));
  
  /* Change */
  change(&tas, 9, 1);
  afficherTas(tas);  
  change(&tas, 1, 61);
  afficherTas(tas);
  
  /* Ajout */
  ajout(&tas, 99);
  ajout(&tas, 65);
  afficherTas(tas);
  
  
   /* FERMETURE */
	return 0;
}

/* ______________ FUNCTION DEF ___________________ */

void afficherTas(Tas T) {
  int i;
  for (i=0; i<T.taille; i++) {
    printf("%d ", T.arbre[i]);
  }
  printf("( taille=%d )\n", T.taille);
}
int indiceParent(int i) { return (i-1)/2; }
int estTas(int tab[], int taille) {
  int i, fg, fd;
  int valid = 1;
  for (i=0; i<taille; i++) {
    fg = (2*i)+1;
    fd = (2*i)+2;
    /* Appartient au tableau ET est plus grand ou egal => FAUX */
    /* Sinon non changé */
    if (fg < taille && tab[i] >= tab[fg]) valid = 0;
    if (fd < taille && tab[i] >= tab[fd]) valid = 0;
  }
  return valid;
}
int fils(Tas T, int indice) {
  
  int fils_min, fg, fd;
  fg = (2*indice)+1;
  fd = (2*indice)+2;
  
  /* TEST FG */
  fils_min = (fg < T.taille) ? fg : -1;
  /* TEST FD */
  if (fd < T.taille) {
    if (fils_min == -1) fils_min = fd;
    else if (T.arbre[fd] < T.arbre[fg]) {
      fils_min = fd;
    }
  }
  return fils_min;
}
void change(Tas *T, int indice, int valeur) {
  int p, f, tmp;
  
  /* On verifie que indice est inferieure à la taille du tas */
  if (indice > T->taille) {
    printf("L'indice donné est supérieur à la taille du tas\n");
    return;
  }
  
  /* On effectue le changement */
  T->arbre[indice] = valeur;
  
  /* On vérifie que les parents soient plus petits */
  p = indiceParent(indice);
  while (T->arbre[p] > T->arbre[indice]) {
    printf("Parent:%d | indice:%d\n", T->arbre[p], T->arbre[indice]);
    tmp = T->arbre[indice];
    T->arbre[indice] = T->arbre[p];
    T->arbre[p] = tmp;
    
    /* On change les valeurs du parent et de l'indice */
    indice = indiceParent(indice);
    p = indiceParent(indice);
  }
  
  /* On vérifie que les fils soient plus grands */
  f = fils(*T, indice);
  while (f != -1 && T->arbre[f] < T->arbre[indice]) {
    printf("Fils:%d | indice:%d\n", T->arbre[f], T->arbre[indice]);
    tmp = T->arbre[indice];
    T->arbre[indice] = T->arbre[f];
    T->arbre[f] = tmp;
    
    /* On change les valeurs du parent et de l'indice */
    indice = fils(*T, indice);
    p = fils(*T, indice);
  }

}
int ajout(Tas *T, int valeur) {
  /* ERROR */
  if (T->taille + 1 < MAX) {
    T->taille += 1;    
    change(T, T->taille, valeur);
    return 1;
  } else {
    return 0;
  }
}
