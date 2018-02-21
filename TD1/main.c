#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 1000
#define TAILLE 51

typedef struct celmot {
  char * mot; /* Adresse de la chaine de caractere */
  int nombre; /* Nombre d'occurences */
  struct celmot *suivant;
} CelluleM, *ListeM;

ListeM alloueCellule ( char M[] );
int ajouteListe ( ListeM *L, char M[] );
ListeM recherche ( ListeM L, char M[] );
void afficherListe ( ListeM L );
int hachage ( char M[] );
int ajouteDico ( ListeM T[], char M[] );

int main(int argc, char *argv[]) {
	ListeM dico[N];
  ListeM mot;
  int i;
  
  for ( i=0; i<N; i++ ) {
    dico[i] = NULL;
  }
  
  
  ajouteListe( &mot, "Ecureuil" );
  ajouteListe( &mot, "Chat" );
  ajouteListe( &mot, "Ecureuil" );
  ajouteListe( &mot, "Tomate" );
  
  afficherListe( mot );
  
  printf("Chat: %d\n", hachage("Chat"));
  printf("Viande: %d\n", hachage("Viande"));
  printf("Tomate: %d\n", hachage("Tomate"));
  printf("Tomates: %d\n", hachage("Tomates"));
  
  
  ajouteDico( dico, "Ecureuil" );
  ajouteDico( dico, "Chat" );
  ajouteDico( dico, "Ecureuil" );
  ajouteDico( dico, "Tomate" );
  
  
	return 0;
}


/* 

  Alloue la mémoire nécessaire pour une nouvelle cellule 
*/
ListeM alloueCellule ( char M[] ) {
  
  ListeM tmp = ( ListeM ) malloc ( sizeof( tmp ) );
  char *mot = ( char* ) malloc( sizeof ( char ) * TAILLE );
  strcpy ( mot, M );
  if ( tmp ) {
    tmp->mot = mot;
    tmp->nombre = 1;
    tmp->suivant = NULL;
  } else {
    printf("Erreur allocation memoire\n");
  }
  return tmp;
}

/*

  Cherche une occurrence d'un mot dans une liste
  Si on le trouve, on incremente le mote
  Sinon, on l'ajoute
  Renvoi:
  -1: erreur
  0: ajoue
  1: incrementation
*/
int ajouteListe ( ListeM *L, char M[] ) {
  
  
  /* On va recherche le mot dans la liste */
  ListeM occurrence = recherche ( *L, M );
  if ( occurrence != NULL) {
    printf("Found : %s\n", occurrence->mot);
    occurrence->nombre++;
    return 1;
  } else {
    printf("Not found : Adding \n");
    ListeM new = alloueCellule ( M );
    
    if( *L == NULL ) {
      *L = new;
    } else {
      new->suivant = *L;
      *L = new;
    }
    return 0;    
  }  
  return -1;  
}

/*
  Recherche un mot dans une liste 
  renvoi :
   *ListeM si touvé
   NULL sinon
*/
ListeM recherche ( ListeM L, char M[] ) {
  if ( L == NULL ) {
    return NULL;
  } else if ( strcmp(L->mot, M) == 0 ) {
    return L;
  } else {
    return recherche( L->suivant, M);
  }
}
void afficherListe ( ListeM L ) {
  
  if ( L == NULL ) {
    printf("NULL\n");
    return;
  } else {
    printf("%s(%d) -> ", L->mot, L->nombre );
    return afficherListe( L->suivant );
  }
}

int hachage ( char M[] ) {
  int i = 0;
  int sum = 0;
  while ( M[i] != '\0' ) {
    sum += ( i+1 ) * ( M[i]);    
    i++;
  }
  
  return ( sum % N );
}
int ajouteDico ( ListeM T[], char M[] ) {
  int index = hachage ( M );
  return ajouteListe( &( T[index] ), M );
}



