#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define MAX 12


/* ______________ FUNCTIONS ______________ */
int enumPermutation(int T[], int premier, int n);
int enumTas(int T[], int premier, int n);
int estTas(int tab[], int taille);
int estTas_partiel(int tab[], int taille);
void afficherTab(int tab[], int taille);
int indiceParent(int i) { return (i-1)/2; }

/* ______________ MAIN ______________ */
int main(int argc, char *argv[]) {
  
  long start, end;
  struct timeval timecheck;
  int i;
  
  /* TEMPS START */
  gettimeofday(&timecheck, NULL);
  start = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;
  
  
  /* CREATION DU TAS */
  int tab[MAX];
  for( i=0; i<MAX; i++ ) {
    tab[i] = 0;    
  }
  /*
  printf("Enum permutation : %d\n", enumPermutation(tab, 1, MAX));
  */
  printf("Enum tas : %d\n", enumTas(tab, 1, MAX)); 
  
  /* TEMPS */
  gettimeofday(&timecheck, NULL);
  end = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;  
  printf("%ld milliseconds elapsed\n", (end - start));
  
	return 0;
}

/* ______________ FUNCTION DEF ___________________ */

int enumPermutation(int T[], int premier, int n) {
  
  int nbPermutations = 0;
  if( premier > n ) {
    return 1;
  } else {
    int i = 0;
    /* Tab pas rempli */
    while ( i < n ) {
      /* Position i n'est pas libre */
      while ( i < n && T[i] != 0 ) { 
        i++;
      }
      if ( i != n ) {
        T[i] = premier;
        nbPermutations += enumPermutation(T, premier+1, n);
        T[i] = 0;
        i++;
      }
    }
  }
  return nbPermutations;
}

int enumTas(int T[], int premier, int n) {
  int nbPermutations = 0;
  if( premier > n ) {
    return estTas(T, n);
  } else {
    int i = 0;
    while ( i < n ) {
      while ( i < n && T[i] != 0 ) { 
        i++;
      }
      if ( i != n ) {
        T[i] = premier;
        
        /* if ( estTas_partiel(T, n) ) */
          nbPermutations += enumTas(T, premier+1, n);
        T[i] = 0;
        i++;
      }
    }
  }
  return nbPermutations;
}

int estTas(int tab[], int taille) {
  int i, fg, fd;
  for (i=0; i<taille; i++) {
    fg = (2*i)+1;
    fd = (2*i)+2;
    /* Appartient au tableau ET est plus grand ou egal => FAUX */
    /* Sinon non changé */
    if (fg < taille && tab[i] >= tab[fg]) return 0;
    if (fd < taille && tab[i] >= tab[fd]) return 0;
  }
  return 1;
}

int estTas_partiel(int tab[], int taille) {
  int i, fg, fd;
  for (i=taille; i>=0; i--) {
    if ( tab[i] == 0 ) {
     return 1; 
    }
    fg = (2*i)+1;
    fd = (2*i)+2;
    /* Appartient au tableau ET est plus grand ou egal => FAUX */
    /* Sinon non changé */
    if (fg < taille && tab[i] >= tab[fg]) return 0;
    if (fd < taille && tab[i] >= tab[fd]) return 0;
  }
  return 1;
}

void afficherTab(int tab[], int taille) {
  int i;
  for( i=0; i<taille; i++ ) {
    printf("%d ", tab[i]);
  }
  printf("\n");
}


