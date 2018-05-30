#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAILLE 100

typedef struct tnoeud {
  char lettre;
  struct tnoeud *frg, *frd;
  struct tnoeud *fils;
} TNoeud, *TArbre;
typedef struct mot {
  char lettre;
  struct mot *suiv;
  struct mot *prec;
} *Mot;
typedef struct doublelist {
	Mot premier;
	Mot dernier;	
	int taille;
} UListe;

/* Affichage */
void makedot(TArbre a);
void ecrireDebut(FILE* stream);
void ecrireFin(FILE* stream);
void ecrireArb(FILE* stream, TArbre a);

/* CE TP */
void affiche( TArbre a );
int recherche( TArbre a, char *m );
int nombreMot( TArbre a );
int ajouteMot( TArbre *a, char *m );
TArbre creerArbre( char c );

void afficherChemins_rec(TArbre a, UListe *path);
void afficherChemins(TArbre a);
void ajouterFinUListe(UListe *liste , char c);
void afficherUListe(UListe *liste);
int supprimerUListe(UListe * liste, char lettre);
int supprimerDernierUListe(UListe *liste);

/* I/O */
int rec_dot_export(TArbre a, int nodeID, FILE *dot_file);
void dot_export(TArbre a, FILE *dot_file);
void free_arbre(TArbre a);

/* MAIN */
int main(int argc, char *argv[]) {
  
  TArbre arbre = NULL;
  
  ajouteMot( &arbre, "chat");
  ajouteMot( &arbre, "viande");
  ajouteMot( &arbre, "ecureuil");
  ajouteMot( &arbre, "harry");
  ajouteMot( &arbre, "potter");
  ajouteMot( &arbre, "manger");
  ajouteMot( &arbre, "boire");
  ajouteMot( &arbre, "mille");
  ajouteMot( &arbre, "milliards");
  ajouteMot( &arbre, "milliard");
  ajouteMot( &arbre, "million");
  ajouteMot( &arbre, "alcool");
  ajouteMot( &arbre, "jeudimac");
  ajouteMot( &arbre, "robe");
  ajouteMot( &arbre, "qui");
  ajouteMot( &arbre, "quoi");
  ajouteMot( &arbre, "torcy");
  ajouteMot( &arbre, "after");
  ajouteMot( &arbre, "arriver");
  ajouteMot( &arbre, "sexe");
  ajouteMot( &arbre, "drogue");
  ajouteMot( &arbre, "myriam");
  ajouteMot( &arbre, "nicolas");
  ajouteMot( &arbre, "quentin");
  ajouteMot( &arbre, "fenetre");
  ajouteMot( &arbre, "rideau");
  ajouteMot( &arbre, "chaise");
  ajouteMot( &arbre, "venceslas");
  ajouteMot( &arbre, "biri");
  ajouteMot( &arbre, "imac");
  ajouteMot( &arbre, "chien");
  ajouteMot( &arbre, "race");
  ajouteMot( &arbre, "patate");
  ajouteMot( &arbre, "vache");
  ajouteMot( &arbre, "le");
  ajouteMot( &arbre, "la");
  ajouteMot( &arbre, "les");
  
  printf("Nombre de mots: %d\n", nombreMot( arbre ));
  printf("Recherche de chat : %d\n", recherche(arbre, "chat"));
  printf("Recherche de chaton : %d\n", recherche(arbre, "chaton"));
  
  afficherChemins( arbre );
  
  /* FERMETURE */
  makedot( arbre );
  free_arbre( arbre );
	return 0;
}

/* ______________ FUNCTION DEF ___________________ */

Mot creerMot(char lettre) {
  Mot tmp = (Mot) malloc( sizeof( Mot ) );
  if ( !tmp ) {
    printf("Erreur allocation mémoire\n");
    exit(0);
  }
  tmp->lettre = lettre;
  tmp->suiv = NULL;
  tmp->prec = NULL;
  return tmp;
}
void afficherChemins_rec(TArbre a, UListe *path) {
  if ( a ) {
    afficherChemins_rec( a->frg, path );
    ajouterFinUListe( path, a->lettre );
    if( a->lettre == '\0' ) {
      afficherUListe( path );
    }
    afficherChemins_rec( a->fils, path );
    supprimerDernierUListe( path );
    afficherChemins_rec( a->frd, path );
  }
}
void afficherChemins(TArbre a) {
  UListe path;
  path.premier = NULL;
  path.dernier = NULL;
  path.taille = 0;
  afficherChemins_rec(a, &path);
}
/*
	Ajoute un arbre au début de la liste doublement chainée
*/
void ajouterFinUListe(UListe *liste , char c) {
	Mot m = creerMot( c );
  
	if ( liste->taille == 0 ) {
		liste->premier = m;
		liste->dernier = m;		
	} else {
		liste->dernier->suiv = m;
		m->prec = liste->dernier;
		liste->dernier = m;
	}
	
	liste->taille++;
}
/*
	Affiche la liste Tile	
*/
void afficherUListe ( UListe *liste ) {
	Mot actuel = liste->premier;
  while ( actuel != NULL ) {
    printf("%c", actuel->lettre);
    actuel = actuel->suiv;
  }
  printf("\n");
}
int supprimerDernierUListe ( UListe *liste ) {  
  if ( liste->taille == 0 ) {
    printf("Liste vide\n");
    return 0;
  }
  return supprimerUListe( liste, liste->dernier->lettre );
}

/*
	Supprime une unite de la liste doublement chainée Tile
	L'unite a supprimer est identifiée par son id
*/
int supprimerUListe ( UListe * liste, char lettre ) {
  
  
	Mot tmp = liste->premier;
	int found = 0;
	
	while ( tmp != NULL && !found ) {
		if ( tmp->lettre == lettre ) {
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

int recherche( TArbre a, char *m ) {
  /* TODO */
  if ( a == NULL ) {
    /*
      Le noeud ne possède pas de lettre 
      -> Non trouvé
    */
    return 0;
  } else if ( a->lettre == *m ) {
    /*
      C'est la bonne lettre on continue
    */
     if ( *m != '\0' ) {
        return recherche(a->fils, m+1);   
    } else {
       return 1;
     }
  } else {
    /*
      Ce n'est pas la bonne lettre, on cherche donc à gauche ou a droite.
    */
    
    if ( *m < a->lettre ) {
      /* Lettre plus petite lexicographiquement -> droite */
      return recherche(a->frg, m);
    } else if ( *m > a->lettre ) {
      /* Lettre plus grande lexicographiquement -> gauche */
      return recherche(a->frd, m);
    }
  }  
  return 0;
}
int ajouteMot( TArbre *a, char *m ) {
  if ( *a == NULL ) {
    /*
      Le noeud ne possède pas de lettre 
      -> on l'ajoute
    */
    TArbre new = creerArbre( *m );
    *a = new;
    if ( *m != '\0' ) {
        return ajouteMot(&((*a)->fils), m+1);   
    }
  } else if ( (*a)->lettre == *m ) {
    /*
      C'est la bonne lettre, on à pas besoin de l'ajouter 
    */
     if ( *m != '\0' ) {
        return ajouteMot(&((*a)->fils), m+1);   
    }
  } else {
    /*
      Ce n'est pas la bonne lettre, on cherche donc à gauche ou a droite.
    */
    
    if ( *m < (*a)->lettre ) {
      /* Lettre plus petite lexicographiquement -> droite */
      return ajouteMot(&((*a)->frg), m);
    } else if ( *m > (*a)->lettre ) {
      /* Lettre plus grande lexicographiquement -> gauche */
      return ajouteMot(&((*a)->frd), m);
    }
  }  
  return 1;
}
int nombreMot( TArbre a ) {
  int val = 0;
  if ( a ) {
    val += nombreMot( a->frg );
    val += nombreMot( a->frd );
    val += nombreMot( a->fils );
    val += a->lettre == '\0' ? 1 : 0;
  }
  return val;
}
TArbre creerArbre( char c ) {
  TNoeud *tmp = (TNoeud*) malloc( sizeof( TNoeud ) );
  if ( !tmp ) {
    printf("Erreur allocation mémoire\n");
    exit(0);
  }
  tmp->lettre = c;
  tmp->frg = NULL;
  tmp->frd = NULL;
  tmp->fils = NULL;
  return tmp;
}
void free_arbre(TArbre a) {
  if (a != NULL) {
    free_arbre(a->frg);
    free_arbre(a->frd);
    free_arbre(a->fils);
    free(a);
  }
}

/* Affichage */
void makedot(TArbre a) {
  FILE *out;
  if ((out = fopen("tree.dot", "w")) == NULL) {
    fprintf(stderr, "problem opening tarbre.dot\n");
    exit(1);
  }
  ecrireDebut(out);
  ecrireArb(out, a);
  ecrireFin(out);
  fclose(out);
  system("dot -Tpdf tree.dot -o tree.pdf");
}
void ecrireDebut(FILE *stream) {
  fprintf(stream, "digraph arbre {\n  node [shape=record, height=.1]\n");
  fprintf(stream, "edge [tailclip=false, arrowtail=dot, dir=both];\n\n");
}
void ecrireFin(FILE *stream) {
  fprintf(stream, "\n}\n");
}
void ecrireArb(FILE *stream, TArbre a) {
  if (a == NULL)
    return;
  if (a->lettre != '\0') {
    fprintf(stream, "n%p [label=\"{<valeur> %c | {<gauche> | <milieu> | <droit>}}\"];\n", a, a->lettre);
  } else {
    fprintf(stream, "n%p [label=\"{<valeur> \\\\0 | {<gauche> | <milieu> | <droit>}}\"];\n", a);
  }
  if (a->frg) {
    fprintf(stream, "n%p:gauche:c -> n%p;\n", a, a->frg);
    ecrireArb(stream, a->frg);
  }
  if (a->fils) {
    fprintf(stream, "n%p:milieu:c -> n%p;\n", a, a->fils);
    ecrireArb(stream, a->fils);
  }
  if (a->frd) {
    fprintf(stream, "n%p:droit:c -> n%p;\n", a, a->frd);
    ecrireArb(stream, a->frd);
  }
}