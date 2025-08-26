#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 6

// Función para imprimir matriz aumentada
void imprimirMatriz(double matriz[MAX][MAX+1], int n) {
	for(int i = 0; i < n; i++) {
		for(int j = 0; j <= n; j++)
			printf("%8.3lf ", matriz[i][j]);
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
			printf("Se intercambiaron las filas %d y %d por pivoteo parcial:\n", i+1, filaMax+1);
			imprimirMatriz(matriz, n);
		}
		
		if(matriz[i][i] == 0) {
			printf("Error: pivote nulo en fila %d, el sistema puede ser singular.\n", i+1);
			return 0;
		}
		
		for(int k = i+1; k < n; k++) {
			double factor = matriz[k][i] / matriz[i][i];
			for(int j = i; j <= n; j++)
				matriz[k][j] -= factor * matriz[i][j];
		}
		
		printf("Matriz después de eliminar columna %d:\n", i+1);
		imprimirMatriz(matriz, n);
	}
	return 1;
}

// Función para calcular determinante de la matriz triangular superior
double determinante(double matriz[MAX][MAX+1], int n) {
	double det = 1.0;
	for(int i = 0; i < n; i++)
		det *= matriz[i][i];
		return det;
}

// Sustitución hacia atrás para resolver Ax = b
void sustitucionAtras(double matriz[MAX][MAX+1], int n, double x[MAX]) {
	for(int i = n-1; i >= 0; i--) {
		x[i] = matriz[i][n];
		for(int j = i+1; j < n; j++)
			x[i] -= matriz[i][j] * x[j];
			x[i] /= matriz[i][i];
	}
}

int main() {
	int n = 6; // 6 incógnitas P1..P6
	double A[MAX][MAX], b[MAX], matriz[MAX][MAX+1], x[MAX];
	
	// -------------------------
	// Sistema [A][P] = [b]
	// -------------------------
	// Ecuaciones de equilibrio en los nodos (resuelto del reticulado)
	double raiz2 = sqrt(2)/2.0;
	
	double tempA[6][6] = {
		{1, 1, 0, 0, 0, 0},
	{0, 0, sqrt(2), 1, 0, 0},
		{0, 0, -1.0/sqrt(2), 0, 1.0/sqrt(2), 1},
	{0, 0, 1.0/sqrt(2), 1, 1.0/sqrt(2), 0},
		{0, 1, 0, 0, 1.0/sqrt(2), 0},
	{0, 0, 0, 0, 1 + 1.0/sqrt(2), 0}
	};
	
	// Definición del vector b
	double tempb[6] = {0, 18, 0, 0, 0, 12};
	
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			A[i][j] = tempA[i][j];
		}
		b[i] = tempb[i];
	}
	
	// Crear matriz aumentada
	crearMatrizAumentada(A, b, matriz, n);
	
	printf("Matriz aumentada inicial [A|b]:\n");
	imprimirMatriz(matriz, n);
	
	if(!triangularSuperiorPivoteo(matriz, n)) {
		printf("No se puede continuar, sistema singular.\n");
		return 1;
	}
	
	double det = determinante(matriz, n);
	printf("Determinante de la matriz: %.6lf\n", det);
	if(det == 0) {
		printf("El sistema no tiene solución única.\n");
		return 1;
	}
	
	sustitucionAtras(matriz, n, x);
	
	printf("Solución del sistema:\n");
	for(int i = 0; i < n; i++)
		printf("P%d = %.6lf\n", i+1, x[i]);
	
	return 0;
}
