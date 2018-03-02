#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAILLE 100

typedef struct tnoeud {
  char lettre;
  struct tnoeud *frg, *frd;
  struct tnoeud *fils;
} TNoeud, *Tarbre;


/* CE TP */
void affiche( Tarbre a );
int recherche( Tarbre a, char *m );
int nombreMot( Tarbre a );
int ajouteMot( Tarbre *a, char *m );

/* I/O */
int rec_dot_export(Tarbre a, int nodeID, FILE *dot_file);
void dot_export(Tarbre a, FILE *dot_file);
void free_arbre(Tarbre a);

/* MAIN */
int main(int argc, char *argv[]) {
  
  /*FILE *file;*/
  Tarbre arbre = NULL;
  
  ajouteMot( &arbre, "chat");
  
  
  
  /* FERMETURE */
  /*
  file = fopen("tree.dot", "w+");
  dot_export( arbre, file );
  fclose(file);
  free_arbre( arbre );
  */
  
	return 0;
}

/* ______________ FUNCTION DEF ___________________ */


int rec_dot_export(Tarbre a, int nodeID, FILE *dot_file) {
  int res_frg=0, res_frd=0, res_fils=0;
  char val;
  if (a != NULL) {
    val = a->lettre;
    fprintf(dot_file, "\tn%d [label=\"<gauche> | <valeur> %c | <fils> | <droit>\"];\n", nodeID, val);
  }
  if (a->frg != NULL) {
    fprintf(dot_file, "\tn%d:gauche:c -> n%d:valeur;\n", nodeID, nodeID+1);
    res_frg = rec_dot_export(a->frg, nodeID+1, dot_file);
  }
  if (a->frd != NULL) {
    fprintf(dot_file, "\tn%d:droit:c -> n%d:valeur;\n", nodeID, nodeID+res_frg+1);
    res_frd = rec_dot_export(a->frd, nodeID+res_frg+1, dot_file);
  }
  if (a->fils != NULL) {
    fprintf(dot_file, "\tn%d:fils:c -> n%d:valeur;\n", nodeID, nodeID+res_frg+res_frd+1);
    res_fils = rec_dot_export(a->fils, nodeID+res_frg+res_frd+1, dot_file);
  }
  return res_frg+res_frd+res_fils+1;
}
void dot_export(Tarbre a, FILE *dot_file) {
  fprintf(dot_file, "digraph arbre {\n");
  fprintf(dot_file, "\tnode [shape=record,height=.1]\n");
  fprintf(dot_file, "\tedge [tailclip=false,arrowtail=dot,dir=both]\n");
  rec_dot_export(a, 0, dot_file);
  fprintf(dot_file, "}\n");
}
void free_arbre(Tarbre a) {
  if (a != NULL) {
    free_arbre(a->frg);
    free_arbre(a->frd);
    free_arbre(a->fils);
    free(a);
  }
}

int recherche( Tarbre a, char *m ) {
  /* TODO */
  
  
}
int ajouteMot( Tarbre *a, char *m ) {
  /* TODO */
  
  
  
}