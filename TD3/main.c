#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct noeud {
  int etiquette;
  struct noeud *fg, *fd, *suiv;
} Noeud, *Arbre;

Arbre creerArbre( int e );

/* OLD */
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

/* Ce TP */
Arbre recherche( Arbre a, int n );
int ajout( Arbre *a, int n );
Arbre extraitMin( Arbre *a );
Arbre extraitMax( Arbre *a );
Arbre extrait( Arbre *a, int n );
void afficheArbreJoli(Arbre a, int niv);
int estABR ( Arbre a, Arbre dernierVisite );

/* I/O */
int creerArbreSequence( Arbre *arbre, int *buffer );
int construitArbreQuelconque( Arbre *a, FILE *in );
void ecritArbreQuelconque( Arbre a, FILE *out );
int noeudQuelconque ( Arbre *a, int *buffer );
int rec_dot_export(Arbre a, int nodeID, FILE *dot_file);
void dot_export(Arbre a, FILE *dot_file);
void free_arbre(Arbre a);

int main(int argc, char *argv[]) {
  
  FILE *file;
  Arbre arbre = NULL;
  
  /* 1 */
  file = fopen("tree.txt", "r");
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
  
  
  printf("Extraction (17): %d\n", extrait( &arbre, 17 )->etiquette); 
  printf("Extraction (-58): %d\n", extrait( &arbre, -58 )->etiquette); 
  printf("Extraction (-6): %d\n", extrait( &arbre, -6 )->etiquette); 
  printf("Extraction (5): %d\n", extrait( &arbre, 5 )->etiquette); 
  
  printf("\nEst ABR ? %d\n", estABR(arbre, NULL));
  
  file = fopen("tree.dot", "w+");
  dot_export( arbre, file );
  fclose(file);
  free_arbre( arbre );
  
  
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
int rec_dot_export(Arbre a, int nodeID, FILE *dot_file) {
  int val, res_fg=0, res_fd=0;
  if (a != NULL) {
    val = a->etiquette;
    fprintf(dot_file, "\tn%d [label=\"<gauche> | <valeur> %d | <droit>\"];\n", nodeID, val);
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
void dot_export(Arbre a, FILE *dot_file) {
  fprintf(dot_file, "digraph arbre {\n");
  fprintf(dot_file, "\tnode [shape=record,height=.1]\n");
  fprintf(dot_file, "\tedge [tailclip=false,arrowtail=dot,dir=both]\n");
  rec_dot_export(a, 0, dot_file);
  fprintf(dot_file, "}\n");
}
void free_arbre(Arbre a) {
  if (a != NULL) {
    free_arbre(a->fg);
    free_arbre(a->fd);
    free(a);
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
Arbre extrait( Arbre *a, int n ) {
  Arbre tmp = NULL;
  
  if ( !a ) {
    return NULL;
  } else {
    if ( (*a)->etiquette == n ) {      
      tmp = *a;
      
      /* CAS 1 : 2 Feuilles */
      if ( (*a)->fg == NULL && (*a)->fd == NULL ) {
        *a = NULL;
      }
      /* CAS 2 : 1 Feuille : fg */
      else if ( (*a)->fg != NULL && (*a)->fd == NULL ) {
        *a = (*a)->fg;
      }
      /* CAS 2 BIS : 1 Feuille : fd */
      else if ( (*a)->fd != NULL && (*a)->fg == NULL ) {
        *a = (*a)->fd;
      }
      /* CAS 3 : 2 Fils */
      else {
        /* On récupère le maximum, on pourrait aussi faire pareil avec le minimum */
        Arbre max = extraitMax( &((*a)->fg) );
        
        /* On copie les enfants de l'arbre trouvé qu'il faut supprimer dans le maximum */
        max->fg = (*a)->fg;
        max->fd = (*a)->fd;
        
        /* On remplace l'arbre par le maximum */
        *a = max;
      }     
      return tmp;
    }
    /* RECHERCHE DE L'ELEMENT */
    else return (*a)->etiquette < n ? extrait( &((*a)->fd), n ) : extrait( &((*a)->fg), n );
  }    
}
int estABR ( Arbre a, Arbre dernierVisite ) {
  if ( a ) {
    estABR( a->fg, dernierVisite );
    printf("%d,", a->etiquette);
    estABR( a->fd, dernierVisite );
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