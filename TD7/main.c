#include <stdio.h>
#include <stdlib.h>

#define MAX 100
typedef struct {
	int nbSommets;
	int matrice[MAX][MAX];
} Graphe;


void createGrapheFromFile(Graphe *g, FILE *file) {
	int i, j;
	fscanf(file, "%d", &(g->nbSommets));
	for(i=0; i<g->nbSommets; i++)	{
		for(j=0; j<g->nbSommets; j++)	{
			fscanf(file, "%d", &(g->matrice[i][j]));
		}
	}
}
void afficherGraphe(Graphe *g) {
	int i, j;
	printf("NB SOMMETS: %d\n", g->nbSommets);
	for(i=0; i<g->nbSommets; i++)	{
		for(j=0; j<g->nbSommets; j++)	{
			printf("%d", g->matrice[i][j]);
		}
		printf("\n");
	}
}
void fermetureNaive(Graphe g, Graphe *f) {
	int s = g.nbSommets;
	int i,j,k,p;

	/* On affecte F à G */
	f->nbSommets = s;
	for (i=0; i<s; i++) {
		for (j=0; j<s; j++) {
			f->matrice[i][j] = g.matrice[i][j];			
		}
	}

	for (p=0; p<s; p++) {
		
		/* On calcule les nouveaux Mij */
		for (i=0; i<s; i++) {
			for (j=0; j<s; j++) {
				for (k=0; k<s; k++) {
					f->matrice[i][j] = f->matrice[i][k] || f->matrice[k][j];

				}
			}
		}
		
		/* On ajoute le tout */
		
	}
}




/* ============== MAIN ============== */
int main(int argc, char *argv[]) {
	FILE *file;
	Graphe g;
	Graphe fNaive;
	/* Graphe fermetureWarshall;*/ 


	/* Ouverture du fichier */
	file = fopen("graphe.txt", "r");
	if (!file) {
		fprintf(stderr, "Erreur lecture du fichier\n");
		exit(1);
	}
	createGrapheFromFile(&g, file);
	printf("\nGraphe :\n");
	afficherGraphe(&g);
	
	fermetureNaive(g, &fNaive);
	printf("\nFermeture:\n");
	afficherGraphe(&g);


	fclose(file);
	return 0;
}