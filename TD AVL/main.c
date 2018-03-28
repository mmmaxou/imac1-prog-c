#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX 100

typedef struct noeud {
  int valeur;
	int balance;
  struct noeud *fg, *fd;
} Noeud, *AVL;

/* FUNCTIONS */

int rec_dot_export(AVL a, int nodeID, FILE *dot_file);
void dot_export(AVL a);
void free_arbre(AVL a);
int max (int a, int b) {  return a>=b?a:b; }

AVL allocAVL( int valeur ) {
	AVL tmp = (AVL) malloc(sizeof( *tmp ));
	if ( !tmp ) {
		printf("Erreur allocation memoire.\n");
		exit(0);
	}
	tmp->valeur = valeur;
	tmp->balance = 0;
	tmp->fg = NULL;
	tmp->fd = NULL;
	return tmp;
}


/* Returns true if the given tree is an AVL and its 
   values are >= min and <= max. */
int estAVL_rec(AVL a, int min, int max) { 
  /* an empty tree is BST */
  if ( !a ) 
     return 1;
       
  /* false if this node violates the min/max constraint or balance is less/more than -1/1 */ 
  if (a->valeur < min || a->valeur > max || a->balance > 1 || a->balance < -1) {
		printf("Cette valeur fait rater : %d,%d\n", a->valeur, a->balance);
    return 0; 
	}
 
  return
    estAVL_rec(a->fg, min, a->valeur-1) &&  
    estAVL_rec(a->fd, a->valeur+1, max);
} 
int estAVL(AVL a) { 
  return(estAVL_rec(a, INT_MIN, INT_MAX)); 
}

/* Rotation gauche de l'arbre a */
void rotationG(AVL *a) {
	AVL tmp;
	int balRacine;
	int balGauche;
	
	
	/* On échange */
	tmp = (*a)->fd;
	(*a)->fd = tmp->fg;
	tmp->fg = *a;
	
	/* Calcul des balances */
	balRacine = (*a)->balance;
	balGauche = tmp->balance;
	
	/* Modification de la racine */	
	if ( balGauche<=0 )
		(*a)->balance = balRacine  - 1;
	else
		(*a)->balance = balRacine - balGauche - 1;
		
	/* Modification du fils gauche */	
	if ( (*a)->balance <= 0 )
		tmp->balance = balGauche + (*a)->balance - 1;
	else
		tmp->balance = balGauche - 1;
	
	/* On change le pointeur */
	*a = tmp;
	
}
/* Rotation droite de l'arbre a */
void rotationD(AVL *a) {
	AVL tmp;
	int balRacine;
	int balDroite;
	
	
	/* On échange */
	tmp = (*a)->fg;
	(*a)->fg = tmp->fd;
	tmp->fd = *a;
	
	/* Calcul des balances */
	balRacine = (*a)->balance;
	balDroite = tmp->balance;
	
	/* Modification de la racine */	
	if ( balDroite >= 0 )
		(*a)->balance = balRacine  + 1;
	else
		(*a)->balance = balRacine - balDroite + 1;
		
	/* Modification du fils Droit */	
	if ( (*a)->balance >= 0 )
		tmp->balance = balDroite + (*a)->balance + 1;
	else
		tmp->balance = balDroite + 1;
	
	/* On change le pointeur */
	*a = tmp;
	
}

void rotationGD(AVL *a) {
	rotationG((*a)->fd);
	rotationD(*a);
}

void rotationDG(AVL *a) { 
	rotationD((*a)->fg);
	rotationG(*a);	
}

/* Helper function for the first tree */
int calcBalance(AVL a) {
	if ( a ) {
		int Hg, Hd;
		Hg = calcBalance( a->fg );
		Hd = calcBalance( a->fd );
		a->balance = Hd - Hg;		
		return 1 + max(Hg, Hd);
	}	
	return -1;
}


/* ______________ MAIN ______________ */
int main(int argc, char *argv[]) {
  
	AVL avl = NULL;
	
  /* AVL de test */
  avl = allocAVL( 6 );
  
	/* Generation 1 */
	avl->fg = allocAVL( 5 );
	avl->fd = allocAVL( 15 );
	
	/* Generation 2 */
	avl->fd->fd = allocAVL( 25 );
	avl->fd->fg = allocAVL( 12 );
	
	calcBalance( avl );	
	rotationG( &avl );	
	rotationD( &avl );
	rotationG( &avl );	
	rotationD( &avl );
	rotationG( &avl );	
	rotationD( &avl );
	rotationG( &avl );	
	rotationD( &avl );
	printf("Est avl ? %d\n", estAVL( avl ));
	
  dot_export( avl );
  free_arbre( avl );
	
  /* FERMETURE */
	return 0;
}


int rec_dot_export(AVL a, int nodeID, FILE *dot_file) {
  int res_fg=0, res_fd=0;
  if (a != NULL) {
    fprintf(dot_file, "\tn%d [label=\"<gauche> | <valeur> %d | <balance> %d | <droit>\"];\n", nodeID, a->valeur, a->balance);
  }
  if (a->fg != NULL) {
    fprintf(dot_file, "\tn%d:gauche:c -> n%d:valeur;\n", nodeID, nodeID+1);
    res_fg = rec_dot_export(a->fg, nodeID+1, dot_file);
  }
  if (a->fd != NULL) {
    fprintf(dot_file, "\tn%d:droit:c -> n%d:valeur;\n", nodeID, nodeID+res_fg+1);
    res_fd = rec_dot_export(a->fd, nodeID+res_fg+1, dot_file);
  }
  return res_fg+res_fd+1;
}
void dot_export(AVL a) {
	
	FILE *file;	
	file = fopen("tree.dot", "w+");
	
  fprintf(file, "digraph arbre {\n");
  fprintf(file, "\tnode [shape=record,height=.1]\n");
  fprintf(file, "\tedge [tailclip=false,arrowtail=dot,dir=both]\n");
  rec_dot_export(a, 0, file);
  fprintf(file, "}\n");
	
  fclose(file);
}
void free_arbre(AVL a) {
  if (a != NULL) {
    free_arbre(a->fg);
    free_arbre(a->fd);
    free(a);
  }
}