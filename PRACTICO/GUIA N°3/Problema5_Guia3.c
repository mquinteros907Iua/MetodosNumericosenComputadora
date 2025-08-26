#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 10

// ===== FUNCIONES AUXILIARES =====
void crearMatrizAumentada(double A[MAX][MAX], double b[MAX], double matriz[MAX][MAX+1], int n) {
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++)
			matriz[i][j] = A[i][j];
			matriz[i][n] = b[i];
	}
}

void intercambiarFilas(double matriz[MAX][MAX+1], int fila1, int fila2, int n) {
	for(int j = 0; j <= n; j++) {
		double temp = matriz[fila1][j];
		matriz[fila1][j] = matriz[fila2][j];
		matriz[fila2][j] = temp;
	}
}

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
		}
		if(matriz[i][i] == 0) {
			return 0;
		}
		for(int k = i+1; k < n; k++) {
			double factor = matriz[k][i] / matriz[i][i];
			for(int j = i; j <= n; j++)
				matriz[k][j] -= factor * matriz[i][j];
		}
	}
	return 1;
}

double determinante(double matriz[MAX][MAX+1], int n) {
	double det = 1.0;
	for(int i = 0; i < n; i++)
		det *= matriz[i][i];
		return det;
}

void sustitucionAtras(double matriz[MAX][MAX+1], int n, double x[MAX]) {
	for(int i = n-1; i >= 0; i--) {
		x[i] = matriz[i][n];
		for(int j = i+1; j < n; j++)
			x[i] -= matriz[i][j] * x[j];
			x[i] /= matriz[i][i];
	}
}

// Nueva función: norma de la matriz
double normaMatriz(double A[MAX][MAX], int n) {
	double suma = 0.0;
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
		suma += A[i][j] * A[i][j];
		return sqrt(suma);
}

// Nueva función: imprimir A y b
void imprimirAb(double A[MAX][MAX], double b[MAX], int n) {
	printf("Matriz A:\n");
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++)
			printf("%10.0lf ", A[i][j]);
		printf("\n");
	}
	printf("\nVector b:\n");
	for(int i = 0; i < n; i++)
		printf("%10.0lf\n", b[i]);
	printf("\n");
}

// ===== MAIN =====
int main() {
	int n = 4;
	double A[MAX][MAX] = {
		{20514, 4424, 978, 224},
		{4424,  978,  224,  54},
		{978,    224,   54,  14},
		{224,     54,   14,   4}
	};
	double b[MAX] = {20514, 4424, 978, 224};
	
	double Acopia[MAX][MAX], matriz[MAX][MAX+1], x[MAX];
	
	// ===== PRUEBA 1 =====
	printf("===== PRUEBA 1 (A[0][0] = 20514) =====\n");
	imprimirAb(A, b, n);
	
	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
		Acopia[i][j] = A[i][j];
		
		crearMatrizAumentada(A, b, matriz, n);
		triangularSuperiorPivoteo(matriz, n);
		double det1 = determinante(matriz, n);
		sustitucionAtras(matriz, n, x);
		double norma1 = normaMatriz(Acopia, n);
		
		printf("1. c1 = %.6lf , c2 = %.6lf , c3 = %.6lf , c4 = %.6lf , det(A) = %.0lf , ||A|| = %.0lf\n\n",
			   x[0], x[1], x[2], x[3], det1, norma1);
		
		// ===== PRUEBA 2 =====
		printf("===== PRUEBA 2 (A[0][0] = 20515) =====\n");
		A[0][0] = 20515;
		imprimirAb(A, b, n);
		
		for(int i=0;i<n;i++)
			for(int j=0;j<n;j++)
			Acopia[i][j] = A[i][j];
			
			crearMatrizAumentada(A, b, matriz, n);
			triangularSuperiorPivoteo(matriz, n);
			double det2 = determinante(matriz, n);
			sustitucionAtras(matriz, n, x);
			double norma2 = normaMatriz(Acopia, n);
			
			printf("2. c1 = %.6lf , c2 = %.6lf , c3 = %.6lf , c4 = %.6lf , det(A) = %.0lf , ||A|| = %.0lf\n",
				   x[0], x[1], x[2], x[3], det2, norma2);
			
			return 0;
}
