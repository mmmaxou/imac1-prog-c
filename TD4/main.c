#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAILLE 100

typedef struct noeud {
  int etiquette;
  struct noeud *fg, *fd, *suiv, *prec;
} Noeud, *Arbre;

typedef struct tnoeud {
  char lettre;
  struct noeud *frg, *frd;
  struct noeud *fils;
} TNoeud, *Tarbre;

typedef struct doublelist {
	Arbre premier;
	Arbre dernier;	
	int taille;
} UListe;

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
Arbre recherche( Arbre a, int n );
int ajout( Arbre *a, int n );
Arbre extraitMin( Arbre *a );
Arbre extraitMax( Arbre *a );
Arbre extrait( Arbre *a, int n );
void afficheArbreJoli(Arbre a, int niv);
int estABR_rec ( Arbre a, Arbre *visited );
int estABR ( Arbre a );

/* Ce TP */
void afficherChemins( Arbre a );
void afficherChemins_rec ( Arbre a, UListe *path );
void ajouterFinUListe ( UListe *liste , Arbre a );
void afficherUListe ( UListe *liste );
int supprimerUListe ( UListe * liste, int etiquette );

/* I/O */
int creerArbreSequence( Arbre *arbre, int *buffer );
int construitArbreQuelconque( Arbre *a, FILE *in );
void ecritArbreQuelconque( Arbre a, FILE *out );
int noeudQuelconque ( Arbre *a, int *buffer );
int rec_dot_export(Arbre a, int nodeID, FILE *dot_file);
void dot_export(Arbre a, FILE *dot_file);
void free_arbre(Arbre a);

/* MAIN */
int main(int argc, char *argv[]) {
  
  FILE *file;
  Arbre arbre = NULL;
  
  /* OUVERTURE */
  file = fopen("tree.txt", "r");
  construitArbreQuelconque(&arbre, file);
  fclose(file);
  
  /* 1 */
  /*printf(" Est ABR ? %d\n", estABR( arbre ));*/
  
  /* 2 */
  afficherChemins( arbre );
  
  
  
  /* FERMETURE */
  file = fopen("tree.dot", "w+");
  dot_export( arbre, file );
  fclose(file);
  free_arbre( arbre );
  
  
	return 0;
}

/* ______________ FUNCTION DEF ___________________ */

void afficherChemins_rec ( Arbre a, UListe *path ) {
  if ( a ) {
    ajouterFinUListe( path, a );
    if( a->fg == NULL && a->fd == NULL ) {
      afficherUListe( path );
    }
    afficherChemins_rec( a->fg, path );
    afficherChemins_rec( a->fd, path );
    supprimerUListe( path, path->dernier->etiquette );
  }
}

void afficherChemins( Arbre a ) {
  UListe path;
  path.premier = NULL;
  path.dernier = NULL;
  path.taille = 0;
  if ( a ) {
    ajouterFinUListe( &path, a );
    if( a->fg == NULL && a->fd == NULL ) {
      afficherUListe( &path );
    }
    afficherChemins_rec( a->fg, &path );
    afficherChemins_rec( a->fd, &path );
    supprimerUListe( &path, path.dernier->etiquette );
  }
}

/*
	Ajoute un arbre au début de la liste doublement chainée
*/
void ajouterFinUListe ( UListe *liste , Arbre a ) {
	
	if ( liste->taille == 0 ) {
		liste->premier = a;
		liste->dernier = a;		
	} else {
		liste->dernier->suiv = a;
		a->prec = liste->dernier;
		liste->dernier = a;
	}
	
	liste->taille++;
}


/*
	Affiche la liste Tile	
*/
void afficherUListe ( UListe *liste ) {
	Arbre actuel = liste->premier;
  while ( actuel != NULL ) {
    printf("%d -> ", actuel->etiquette);
    actuel = actuel->suiv;
  }
  printf("NULL\n");
}

/*
	Supprime une unite de la liste doublement chainée Tile
	L'unite a supprimer est identifiée par son id
*/
int supprimerUListe ( UListe * liste, int etiquette ) {
	Arbre tmp = liste->premier;
	int found = 0;
	
	while ( tmp != NULL && !found ) {
		if ( tmp->etiquette == etiquette ) {
			if ( liste->taille == 1 ) {
				liste->premier = NULL;	
				liste->dernier = NULL;
			} else if ( tmp->suiv == NULL ) {
				liste->dernier = tmp->prec;
				liste->dernier->suiv = NULL;
			} else if ( tmp->prec == NULL ) {
				liste->premier = tmp->suiv;
				liste->premier->prec = NULL;				
			} else {
				tmp->suiv->prec = tmp->prec;
				tmp->prec->suiv = tmp->suiv;
			}
			found = 1;
			liste->taille--;
		} else {
			tmp = tmp->suiv;
		}
	}
	
	return 1;	
}


/* ______________ OLD ___________________ */

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
int estABR_rec ( Arbre a, Arbre *visited ) {
  if ( a ) {
    *visited = a;
    return ( estABR_rec(a->fg, visited) ) /* Fils gauche */
      && ( a->etiquette >= (*visited)->etiquette ) /* Infixe */
      && ( estABR_rec(a->fd, visited) ); /* Fils droit */
  }
  return 1;
}
int estABR ( Arbre a ) {
  Arbre *visited = NULL;
  visited = &a;
  if ( a ) {
    return ( estABR_rec(a->fg, visited) ) /* Fils gauche */
      && ( a->etiquette >= (*visited)->etiquette ) /* Infixe */
      && ( estABR_rec(a->fd, visited) ); /* Fils droit */
  }
  return 1;
}
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
  tmp->prec = NULL;
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