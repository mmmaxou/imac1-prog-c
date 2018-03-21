#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PERE 0
#define RANG 1
#define N 50000
#define OPERATION 500000
static unsigned int cherchePere = 0;

void initialisation( int peres[][2] ) {
	int i;
	for( i=0; i<N; i++ ) {
		peres[i][PERE] = i;
		peres[i][RANG] = 1;
	}
}
void afficher( int peres[][2] ) {
	int i;
	for( i=0; i<N; i++ ) {
		printf("%d <= %d\n", peres[i][PERE], peres[i][RANG]);
	}
}
int trouve( int peres[][2], int x ) {
	while ( peres[x][PERE] != x ) {
		cherchePere++;
		x = peres[x][PERE];
	}
	return x;
}
void fusion( int peres[][2], int x, int y ) {
	int rangX = trouve(peres, x);
	int rangY = trouve(peres, y);
	if ( rangX != rangY ) {
		peres[rangY][PERE] = peres[rangX][PERE];
		peres[rangX][RANG] += peres[rangY][RANG];
		peres[rangY][RANG] = 0;
	}
}
int trouveCompresse( int peres[][2], int x ) {
	int n;
	cherchePere++;
	if ( peres[x][PERE] == x) {
		return x;
	} else {
		n = trouveCompresse( peres, peres[x][PERE] );
		peres[x][PERE] = n;
		return n;
	}
}
void fusionRang( int peres[][2], int x, int y ) {
	int rangX = trouve(peres, x);
	int rangY = trouve(peres, y);
	if (  rangX != rangY ) {
		if ( peres[rangX][RANG] > peres[rangY][RANG] ) {
			peres[rangY][PERE] = peres[rangX][PERE];
			peres[rangX][RANG] += peres[rangY][RANG];
			peres[rangY][RANG] = 0;
		} else {
			peres[rangX][PERE] = peres[rangY][PERE];
			peres[rangY][RANG] += peres[rangX][RANG];
			peres[rangX][RANG] = 0;			
		}
	}
}

/* On suppose a<b */
int rand_a_b(int a, int b){
 	return rand()%(b-a) +a;
}

/* ______________ MAIN ______________ */
int main(int argc, char *argv[]) {
	int i, nb1, nb2, nb3;
	int peres[N][2];
	
	initialisation(peres);
	srand(65138465);
	
	for( i=0; i< OPERATION ; i++ ) {
		nb1 = rand_a_b(0, N);
		nb2 = rand_a_b(0, N);
		nb3 = rand_a_b(0, 10);
		if ( nb3 <= 8 ) {
			trouve(peres, nb1);
		} else {
			fusion(peres, nb1, nb2);
		}		
	}
	
	printf("NB cherche pere : %d\n", cherchePere);
	cherchePere = 0;
	srand(65138465);
	
	for( i=0; i< OPERATION ; i++ ) {
		nb1 = rand_a_b(0, N);
		nb2 = rand_a_b(0, N);
		nb3 = rand_a_b(0, 10);
		if ( nb3 <= 8 ) {
			trouveCompresse(peres, nb1);
		} else {
			fusionRang(peres, nb1, nb2);
		}		
	}
	printf("NB cherche pere : %d\n", cherchePere);
	
	/*
	afficher(peres);
	*/
	
 	return 0;
}
