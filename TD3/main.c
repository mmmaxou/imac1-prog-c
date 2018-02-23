#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "tree.txt"

typedef struct noeud {
  int etiquette;
  struct noeud *fg, *fd, *suiv;
} Noeud, *Arbre;

Arbre creerArbre( int e );
int max (int a, int b);
int hauteur( Arbre a );
int nbNoeuds( Arbre a );
int nbNoeudsInternes( Arbre a );
int nbFeuilles( Arbre a );
int nb2fils( Arbre a );
void deepSearchPrefix( Arbre a );
void deepSearchInfix( Arbre a );
void deepSearchSuffix( Arbre a );
void shallowSearch( Arbre a );
void ajouterFin( Arbre list, Arbre ajout );
int estHG ( Arbre a );
int creerArbreSequence( Arbre *arbre, int *buffer );
int construitArbreQuelconque( Arbre *a, FILE *in );
int noeudQuelconque ( Arbre *a, int *buffer );
void ecritArbreQuelconque( Arbre a, FILE *out );
Arbre recherche( Arbre a, int n );
int ajout( Arbre *a, int n );
Arbre extraitMin( Arbre *a );
Arbre extraitMax( Arbre *a );
Arbre extrait( Arbre a, int n );
void afficheArbreJoli(Arbre a, int niv);

int main(int argc, char *argv[]) {
  
  FILE *file;
  Arbre arbre = NULL;
  
  /* 1 */
  file = fopen(FILENAME, "r");
  construitArbreQuelconque(&arbre, file);
  printf("Parcours en profondeur : ");
  deepSearchPrefix( arbre );
  printf("\n");
  fclose(file);
  
  /* 2 */
  file = fopen("TEMP.txt", "w+");
  ecritArbreQuelconque( arbre, file);
  fclose(file);
  
  /* 3 */
  printf("Recherche de 5 : %d\n", recherche(arbre, 5)->etiquette);
  printf("Recherche de 0 : %d\n", recherche(arbre, 0) || -0);
  printf("Recherche de 1 : %d\n", recherche(arbre, 1)->etiquette);
  printf("Recherche de -7 : %d\n", recherche(arbre, -7) || -0);
  printf("Recherche de 11 : %d\n", recherche(arbre, 11)->etiquette);
  printf("Recherche de 18 : %d\n", recherche(arbre, 18) || -0);
  
  /* 4 */
  ajout( &arbre, 3);
  ajout( &arbre, 7);
  ajout( &arbre, 24);
  ajout( &arbre, -6);
  ajout( &arbre, -59);
  ajout( &arbre, -58);
  ajout( &arbre, -57);
  ajout( &arbre, 14);
  ajout( &arbre, 17);
  
  afficheArbreJoli(arbre, 0);
  
  printf("Extraction : %d\n", extrait( arbre, 24 )->etiquette);
  printf("Extraction : %d\n", extrait( arbre, 89 ) || 0);
  printf("Extraction : %d\n", extrait( arbre, 2 )->etiquette); 
  
  afficheArbreJoli(arbre, 0);  
  
	return 0;
}

int construitArbreQuelconque( Arbre *a, FILE *in ) {
  int buffer[255];
  int id = 0;
  
  /* Gather datas */
  while ( fscanf(in, "%d ", buffer + id) == 1) {
    printf("%d ", buffer[id]);
    id++;
  }
  
  /* Create the tree from the data */
  printf("\n");
  noeudQuelconque( a, buffer );
  
  return 1;
}
int noeudQuelconque ( Arbre *a, int *buffer ) {
  int res_fg, res_fd;
  if ( *buffer == 3 ) {
    *a = creerArbre( *(buffer + 1));
    res_fg = noeudQuelconque( &((*a)->fg), buffer+2 );
    res_fd = noeudQuelconque( &((*a)->fd), buffer+res_fg+2 );
    return res_fg + res_fd + 2;
    
  } else if ( *buffer == 2 ) {    
    *a = creerArbre( *(buffer + 1));
    res_fg = noeudQuelconque( &((*a)->fg), buffer+2 );
    return res_fg + 2;
    
  } else if ( *buffer == 1 ) {    
    *a = creerArbre( *(buffer + 1));
    res_fd = noeudQuelconque( &((*a)->fd), buffer+2 );
    return res_fd + 2;
    
  } else {
    *a = creerArbre( *(buffer + 1));
    return 2;
  }
  
}
void ecritArbreQuelconque( Arbre a, FILE *out ) {  
  if ( a ) {
    if ( a->fg && a->fd ) {
      fprintf(out, "3 ");
    } else if ( a->fg && !(a->fd) ) {
      fprintf(out, "2 ");
    } else if ( !(a->fg) && a->fd ) {
      fprintf(out, "1 ");
    } else {
      fprintf(out, "0 ");
    }
    ecritArbreQuelconque( a->fg, out );
    ecritArbreQuelconque( a->fd, out );
  }
}

void afficheArbreJoli(Arbre a, int niv) {
/* 
  Affiche l'arbre a sous la forme d'une arborescence (fg en haut, fd en bas)
  Pour appeller le dessin de l'arbre a, taper "afficherArbJoli(a,0);" dans votre main
*/
  int i = 0;
  for (i=0; i<niv; i++) {
    printf("| ");
  }
  if (a == NULL)
    printf("NULL\n");
  else {
    printf("%d\n", a->etiquette);
    afficheArbreJoli(a->fg, 1 + niv);
    afficheArbreJoli(a->fd, 1 + niv);
  }
}

Arbre recherche( Arbre a, int n ) {
  if ( !a ) {
    return NULL;
  } else {
    if ( a->etiquette == n ) return a;
    else return a->etiquette < n ? recherche( a->fd, n ) : recherche( a->fg, n );
  }
}
int ajout( Arbre *a, int n ) {
  if ( !(*a) ) {
    Arbre tmp = creerArbre( n );
    *a = tmp;
    return 0;
  } else {
    if ( (*a)->etiquette != n ) {
      (*a)->etiquette < n ? 
        ajout( &((*a)->fd), n ): 
        ajout( &((*a)->fg), n );    
    }
    return 1;
  }
}
Arbre extraitMin( Arbre *a ) {
  if ( (*a)->fg != NULL ) {
    return extraitMin( &((*a)->fg) );
  }
  else {
    Arbre tmp = *a;
    *a = (*a)->fd;    
    return tmp;
  }    
}
Arbre extraitMax( Arbre *a ) {
  if ( (*a)->fd != NULL ) {
    return extraitMin( &((*a)->fd) );
  }
  else {
    Arbre tmp = *a;
    *a = (*a)->fg;    
    return tmp;
  }    
}
Arbre extrait( Arbre a, int n ) {
  Arbre *found = NULL;
  Arbre node = NULL;
  Arbre new = NULL;
  
  if ( !a ) {
    return NULL;
  } else {
    if ( a->etiquette == n ) {
      found = &a;
    } else if ( a->fg && a->fg->etiquette == n ) {
      found = &(a->fg);
    } else if ( a->fd && a->fd->etiquette == n ) {
      found = &(a->fd);      
    }
    
    if ( found ) {
      node = *found;
      if ( node->fg == NULL && node->fd == NULL ) {
        *found = NULL;
      } else if ( node->fg == NULL ) {
        new = extraitMax( &(node->fd) );
      } else {
        new = extraitMin( &(node->fg) );
      }
      if ( new ) {
        printf("new:%d  __ ", new->etiquette);
        new->fg = node->fg;
        new->fd = node->fd;
      }
      *found = new;
      return node;
    } else {
      return a->etiquette < n ? 
        extrait( a->fd, n ): 
        extrait( a->fg, n );
    } 
  }
}

/* OLD */
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
  Arbre *arbres = &a;
  
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
int estHG ( Arbre a ) {
  if ( a->fg == NULL || a->fd == NULL ) {
    return 1;
  } else {
    return estHG(a->fg) > estHG(a->fd);
  }
}

/* UTILS */
int max (int a, int b) {
  return a>=b?a:b;
}