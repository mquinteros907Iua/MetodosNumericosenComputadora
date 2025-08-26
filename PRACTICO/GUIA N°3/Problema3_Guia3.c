#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 10

// Función para imprimir matriz aumentada
void imprimirMatriz(double matriz[MAX][MAX+1], int n) {
	for(int i = 0; i < n; i++) {
		for(int j = 0; j <= n; j++)
			printf("%8.0lf ", matriz[i][j]);
		printf("\n");
	}
	printf("\n");
}

// Función para armar la matriz aumentada [A|b]
void crearMatrizAumentada(double A[MAX][MAX], double b[MAX], double matriz[MAX][MAX+1], int n) {
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++)
			matriz[i][j] = A[i][j];
			matriz[i][n] = b[i];
	}
}

// Función para intercambiar dos filas
void intercambiarFilas(double matriz[MAX][MAX+1], int fila1, int fila2, int n) {
	for(int j = 0; j <= n; j++) {
		double temp = matriz[fila1][j];
		matriz[fila1][j] = matriz[fila2][j];
		matriz[fila2][j] = temp;
	}
}

// Función para reducir la matriz aumentada a triangular superior con pivoteo parcial
int triangularSuperiorPivoteo(double matriz[MAX][MAX+1], int n) {
	for(int i = 0; i < n-1; i++) {
		// Pivoteo parcial
		int filaMax = i;
		double max = fabs(matriz[i][i]);
		for(int k = i+1; k < n; k++) {
			if(fabs(matriz[k][i]) > max) {
				max = fabs(matriz[k][i]);
				filaMax = k;
			}
		}
		if(filaMax != i) {
			intercambiarFilas(matriz, i, filaMax, n);
		}
		if(matriz[i][i] == 0) {
			printf("Error: pivote nulo en fila %d, el sistema puede ser singular.\n", i+1);
			return 0;
		}
		// Eliminación hacia adelante
		for(int k = i+1; k < n; k++) {
			double factor = matriz[k][i] / matriz[i][i];
			for(int j = i; j <= n; j++)
				matriz[k][j] -= factor * matriz[i][j];
		}
	}
	return 1; 
}

// Sustitución hacia atrás
void sustitucionAtras(double matriz[MAX][MAX+1], int n, double x[MAX]) {
	for(int i = n-1; i >= 0; i--) {
		x[i] = matriz[i][n];
		for(int j = i+1; j < n; j++)
			x[i] -= matriz[i][j] * x[j];
			x[i] /= matriz[i][i];
	}
}

int main() {
	int n = 4;
	double A[MAX][MAX], b[MAX], matriz[MAX][MAX+1], x[MAX];
	
	// Matriz A según los puntos dados
	double tempA[4][4] = {
		{1, 0, 0, 0},   // (0,0)
		{1, 1, 1, 1},   // (1,1)
		{1, 2, 4, 8},   // (2,3)
		{1, 3, 9, 27}   // (3,4)
	};
	
	// Copiar en A
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			A[i][j] = tempA[i][j];
	
	// Vector b
	double tempb[4] = {0, 1, 3, 4};
	for(int i = 0; i < n; i++)
		b[i] = tempb[i];
	
	// Crear matriz aumentada
	crearMatrizAumentada(A, b, matriz, n);
	
	printf("Matriz aumentada inicial [A|b]:\n");
	imprimirMatriz(matriz, n);
	
	// Reducir a triangular superior con pivoteo parcial
	if(!triangularSuperiorPivoteo(matriz, n)) {
		printf("No se puede continuar, sistema singular.\n");
		return 1;
	}
		
	// Sustitución hacia atrás
	sustitucionAtras(matriz, n, x);
	
	// Mostrar solución
	printf("Coeficientes del polinomio:\n");
	printf("A = %.3f\n", x[0]);
	printf("B = %.3f\n", x[1]);
	printf("C = %.3f\n", x[2]);
	printf("D = %.3f\n", x[3]);
		
	// Mostrar polinomio
	printf("\nPolinomio: y = %.3f + %.3f*x + %.3f*x^2 + %.3f*x^3\n",
	x[0], x[1], x[2], x[3]);
	
	return 0;
}
