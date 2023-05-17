#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct matriz{
	int **matriz;
};

typedef struct matriz *Matriz;

struct grafo{
	int **MatrizDeAdjacencias;
    Matriz *vetorDeMatrizesDeAlcancabilidade;
    int ordemDaMatriz;
    int **Warshall;
};

typedef struct grafo *Grafo;

static void GerarMatrizDeAdjacencias(FILE *arquivo, Grafo G);
static void CalcularMatrizDeAlcancabilidade(Grafo G);
static int **InicializarMatriz(int ordemDaMatriz);
static void InicializarVetorDeMatrizes(Grafo G);
static void MostrarMatrizesDeAlcancabilidade(Grafo G);
static void MostrarMatrizWarshall(int **Matriz, int TAM);
static void CalcularMatrizResultante(Grafo G);
static void AlgoritmoDeWarshall(int **Matriz, int ordemDaMatriz);

int main(){

	FILE *arquivo;
	int i,j;

	Grafo MeuGrafo = malloc(sizeof(Grafo *));

	arquivo = fopen("matriz_de_adjacencias.txt", "r");

	GerarMatrizDeAdjacencias(arquivo, MeuGrafo);
	fclose(arquivo);

	MeuGrafo->Warshall = InicializarMatriz(MeuGrafo->ordemDaMatriz);
	for(i = 0; i< MeuGrafo->ordemDaMatriz; ++i)
		for(j = 0; j < MeuGrafo->ordemDaMatriz; ++j)
			MeuGrafo->Warshall[i][j] = MeuGrafo->MatrizDeAdjacencias[i][j];

	
	InicializarVetorDeMatrizes(MeuGrafo);

	CalcularMatrizDeAlcancabilidade(MeuGrafo);
	CalcularMatrizResultante(MeuGrafo);
	AlgoritmoDeWarshall(MeuGrafo->Warshall, MeuGrafo->ordemDaMatriz);
	MostrarMatrizesDeAlcancabilidade(MeuGrafo);
	MostrarMatrizWarshall(MeuGrafo->Warshall, MeuGrafo->ordemDaMatriz);

	return 0;
}

static void GerarMatrizDeAdjacencias(FILE *arquivo, Grafo G){
	int i = 0,j, k=0, MAX = 1000, ordemDaMatriz;
	int vetorTMP[MAX];

	while(!feof(arquivo)){
		fscanf(arquivo, "%d", &vetorTMP[i]);
		i++;	
	}

    ordemDaMatriz = sqrt(i);
    G->ordemDaMatriz = ordemDaMatriz;
	G->MatrizDeAdjacencias = InicializarMatriz(G->ordemDaMatriz);

	for(i = 0; i < ordemDaMatriz; ++i){
		for(j = 0; j < ordemDaMatriz; ++j){
			G->MatrizDeAdjacencias[i][j] = vetorTMP[k];
		    k++;
		}
	}

}


static int **InicializarMatriz(int ordemDaMatriz){
	int i;
	int **m = malloc(ordemDaMatriz * sizeof(int*));
	for(i=0; i< ordemDaMatriz;++i)
		m[i] = malloc(ordemDaMatriz * sizeof(int*));

	return m;
}

static void InicializarVetorDeMatrizes(Grafo G){
	int i;
	Matriz *vetor = malloc(G->ordemDaMatriz+1 * sizeof(Matriz *));
    Matriz tmp;
 

    for(i = 0; i<G->ordemDaMatriz+1; ++i){
    	tmp = malloc(sizeof(Matriz*));
    	tmp->matriz = InicializarMatriz(G->ordemDaMatriz);
    	vetor[i] = tmp;
    }

	G->vetorDeMatrizesDeAlcancabilidade = vetor;
}

static void CalcularMatrizDeAlcancabilidade(Grafo G){
	int TAM = G->ordemDaMatriz;
	int i = 0, j = 0,k,m, linha, count,coluna,l,Booleano[TAM], BoolTMP;

	Matriz novaMatriz;
	Matriz adjacencias;
	Matriz matrizInicial;
	Matriz atual;
	Matriz anterior;
	Matriz tmp;

	adjacencias = malloc(sizeof(Matriz*));
	novaMatriz = malloc(sizeof(Matriz*));
	matrizInicial = malloc(sizeof(Matriz*));
	tmp = malloc(sizeof(Matriz*));

	adjacencias->matriz = G->MatrizDeAdjacencias;
	matrizInicial->matriz = G->MatrizDeAdjacencias;
    tmp->matriz = G->MatrizDeAdjacencias;

    G->vetorDeMatrizesDeAlcancabilidade[0] = tmp;

    for(k=1;k < G->ordemDaMatriz;++k){
    	atual = G->vetorDeMatrizesDeAlcancabilidade[k];
    	anterior = G->vetorDeMatrizesDeAlcancabilidade[k-1];
		
		for(linha=0;linha<TAM;linha++){
			for(coluna = 0; coluna < TAM;++coluna){
				for(m=0;m<TAM;++m){
					Booleano[m] = adjacencias->matriz[linha][m] && anterior->matriz[m][coluna];
				}
				BoolTMP = Booleano[0];
				for(l=1;l<TAM;++l){
					BoolTMP = Booleano[l] || BoolTMP;
				}
				
				atual->matriz[linha][coluna] = BoolTMP;
				
			}
		}

	}
	    
}


static void MostrarMatrizesDeAlcancabilidade(Grafo G){
	int i,j,k;
	int TAM = G->ordemDaMatriz;
	Matriz atual;

	for(k=0;k < TAM+1;++k){
		if(k == TAM)
			printf("\nMatriz Resultante Geral R:\n");
		else
		    printf("\nMatriz de Alcançabilidade A%d:\n",k+1);

    	atual = G->vetorDeMatrizesDeAlcancabilidade[k];
		for(i = 0; i < TAM; ++i){
			for(j=0;j<TAM;++j){
			   printf("%d ",atual->matriz[i][j]);
			}
			printf("\n");
	    }

	}


}


static void CalcularMatrizResultante(Grafo G){
	int MAX = G->ordemDaMatriz;
	int i,j,k;

	Matriz Resultante, atual, TMP;
	Resultante = G->vetorDeMatrizesDeAlcancabilidade[MAX];
	TMP = G->vetorDeMatrizesDeAlcancabilidade[0];

    for(i = 0; i< MAX; ++i)
    	for(j =0; j< MAX; ++j)
    		Resultante->matriz[i][j] = TMP->matriz[i][j];


    for(k = 1; k < MAX; ++k){
    	atual = G->vetorDeMatrizesDeAlcancabilidade[k];
		for(i = 0; i <MAX; ++i)
			for(j = 0; j <MAX; ++j){
				Resultante->matriz[i][j] = Resultante->matriz[i][j] || atual->matriz[i][j];
			}
	}

}

static void AlgoritmoDeWarshall(int **Matriz, int ordemDaMatriz){
	int i,j,k;

	for(k = 0; k < ordemDaMatriz; ++k){
		for(i = 0; i < ordemDaMatriz; ++i){
			for(j =0; j < ordemDaMatriz; ++j){
				Matriz[i][j] = (Matriz[i][j] || Matriz[i][k] && Matriz[k][j]);
			}
		}
	}
}


static void MostrarMatrizWarshall(int **Matriz, int TAM){
	printf("\nMatriz de Alcançabilidade Geral M(Warshall):\n");
	for(int i=0;i<TAM;++i){
		for(int j=0; j<TAM;++j)
			printf("%d ", Matriz[i][j]);

		printf("\n");
	}

}

