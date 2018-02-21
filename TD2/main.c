#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct noeud {
  int etiquette;
  struct noeud *fg, *fd, *suiv;
} Noeud, *Arbre;
Arbre creerArbre( int e );
int max (int a, int b);

/* PARTIE 2 */
int hauteur( Arbre a );
int nbNoeuds( Arbre a );
int nbNoeudsInternes( Arbre a );
int nbFeuilles( Arbre a );
int nb2fils( Arbre a );

/* PARTIE 3 */
void deepSearchPrefix( Arbre a );
void deepSearchInfix( Arbre a );
void deepSearchSuffix( Arbre a );
void shallowSearch( Arbre a );
void ajouterFin( Arbre list, Arbre ajout );
int estHG ( Arbre a );

/* PARTIE 4 */
int creerArbreSequence( Arbre *arbre, int *buffer );

int main(int argc, char *argv[]) {
  
  int sequence1[20] = {8,3,2,1,0,0,5,0,0,7,0,0,9,4,0,0,6,0,0};
  Arbre arbreSeq1 = NULL;
  
  /* Création arbre FIG 1 */
  /*
  Arbre a = creerArbre( 'a' );
  a->fd = creerArbre( 'c' );
  a->fd->fd = creerArbre( 'g' );
  a->fd->fd->fd = creerArbre( 'd' );
  a->fd->fd->fg = creerArbre( 'e' );
  a->fg = creerArbre( 'z' );
  a->fg->fg = creerArbre( 'x' );
  a->fg->fg->fg = creerArbre( 'h' );
  a->fg->fg->fd = creerArbre( 'n' );
  a->fg->fd = creerArbre( 'e' );
  a->fg->fd->fg = creerArbre( 'j' );
  a->fg->fd->fg->fd = creerArbre( 'i' );
  */
  
  
  /* PARTIE 2 */
  /*
  printf(" > Hauteur : %d\n", hauteur(a));
  printf(" > nb de noeuds : %d\n", nbNoeuds(a));
  printf(" > nb de feuilles : %d\n", nbFeuilles(a));
  printf(" > nb de noeuds internes : %d\n", nbNoeudsInternes(a));
  printf(" > nb de noeuds possédants exactement 2 fils : %d\n", nb2fils(a));
  */
  
  /* PARTIE 3 */
  /*
  printf(" > Parcours préfixe : ");
  deepSearchPrefix( a );
  printf("\n");
  printf(" > Parcours infixe : ");
  deepSearchInfix( a );
  printf("\n");
  printf(" > Parcours suffixe : ");
  deepSearchSuffix( a );
  printf("\n");
  */
  
  creerArbreSequence( &arbreSeq1, sequence1 );
  printf(" > Parcours préfixe : ");
  deepSearchPrefix( arbreSeq1 );
  printf("\n > Parcours infixe : ");
  deepSearchInfix( arbreSeq1 );
  printf("\n > Parcours suffixe : ");
  deepSearchSuffix( arbreSeq1 );
  printf("\n");  
  
  /* En niveaux */
  shallowSearch( arbreSeq1 );
  
  /* PARTIE 8 */
  int sequenceHG[20] = {5,3,6,0,0,2,0,0,2,0,0};
  Arbre arbreHG = NULL;
  creerArbreSequence( &arbreHG, sequenceHG );
  printf(" > Parcours préfixe : ");
  deepSearchPrefix( arbreHG );
  printf("\n");
  shallowSearch( arbreHG );  
  printf("\nEst HG ? %d\n", estHG( arbreHG ));
	  
	return 0;
}

/* Creer un arbre */

Arbre creerArbre( int e ) {
  Noeud * tmp = (Noeud*) malloc( sizeof( Noeud ) );
  if ( !tmp ) {
    printf("Erreur allocation mémoire\n");
    exit(0);
  }
  tmp->etiquette = e;
  tmp->fg = NULL;
  tmp->fd = NULL;
  tmp->suiv = NULL;
  return tmp;
}

/* PARTIE 2 */
int hauteur( Arbre a ) {
  
  return a ? max(hauteur(a->fg), hauteur(a->fd)) + 1 : -1;
}
int nbNoeuds( Arbre a ) {
  
  return a ? nbNoeuds(a->fg) + nbNoeuds(a->fd) + 1 : 0;  
}
int nbFeuilles( Arbre a ) {
  /*
  if ( a == NULL ) {
    return 0;
  } else {
    if ( !(a->fg) && !(a->fd) ) {
      return 1;
    } else {
      return nbFeuilles(a->fg) + nbFeuilles(a->fd);
    }
  }
  */  
  return a ? ( !(a->fg) && !(a->fd) ) ? 1 : nbFeuilles(a->fg) + nbFeuilles(a->fd) : 0; 
}
int nbNoeudsInternes( Arbre a ) {
  /*
  int sum = 0;
  if ( a->fg ) {
    sum += nbNoeudsInternes(a->fg);
  }
  if ( a->fd ) {
    sum += nbNoeudsInternes(a->fd);
  }
  if ( a->fg || a->fd ) {
    sum += 1;
  }
  return sum;
  */
  return a ? nbNoeudsInternes(a->fg) + nbNoeudsInternes(a->fd) + (a->fg||a->fd):0;  
}
int nb2fils( Arbre a ) {
  return a ? nb2fils(a->fg) + nb2fils(a->fd) + (a->fg && a->fd) : 0;  
}

/* PARTIE 3 */
void deepSearchPrefix( Arbre a ) {
  if ( a ) {
    printf( "%d,",a->etiquette );
    deepSearchPrefix( a->fg );
    deepSearchPrefix( a->fd );
  }
}
void deepSearchInfix( Arbre a ) {
  if ( a ) {
    deepSearchInfix( a->fg );
    printf( "%d,",a->etiquette );
    deepSearchInfix( a->fd );
  }
}
void deepSearchSuffix( Arbre a ) {
  if ( a ) {
    deepSearchSuffix( a->fg );
    deepSearchSuffix( a->fd );
    printf( "%d,",a->etiquette );
  }
}
void shallowSearch( Arbre a ) {
  printf(" > Parcours en Niveaux : ");

  Arbre *arbres = NULL;
  arbres = &a;
  
  while ( *arbres ) {
    printf("%d,", (*arbres)->etiquette);
    ajouterFin( *arbres, (*arbres)->fg);
    ajouterFin( *arbres, (*arbres)->fd);
    *arbres = (*arbres)->suiv;
  }
  
  printf("\n");
}
void ajouterFin( Arbre list, Arbre ajout ) {
  while ( list->suiv ) {
    list = list->suiv;
  }
  list->suiv = ajout;
}


/* PARTIE 4 */
int creerArbreSequence( Arbre *arbre, int *buffer ) {
  int res_fg, res_fd;
  if ( *buffer != 0) {
    *arbre = creerArbre( *buffer );
    res_fg = creerArbreSequence( &((*arbre)->fg), buffer+1 );
    res_fd = creerArbreSequence( &((*arbre)->fd), buffer+res_fg+1 );
    return res_fg+res_fd+1;
  } else {
    return 1;
  }
}

/* PARTIE 8 */
int estHG ( Arbre a ) {
  if ( a->fg == NULL || a->fd == NULL ) {
    return 1;
  } else {
    return (nbFeuilles(a->fg) >= nbFeuilles(a->fd)) 
      && estHG(a->fg) 
      && estHG(a->fd);
  }
}


/* UTILS */
int max (int a, int b) {
  return a>=b?a:b;
}