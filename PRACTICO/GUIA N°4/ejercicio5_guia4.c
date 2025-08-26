#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 20

// Función para imprimir la matriz aumentada
void imprimirMatriz(double A[MAX][MAX], double b[MAX], int n) {
	printf("\nMatriz aumentada [A|b]:\n");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%10.4lf ", A[i][j]);
		}
		printf(" | %10.4lf\n", b[i]);
	}
}

// Verifica diagonal dominante
void verificarDiagonalDominante(double A[MAX][MAX], int n) {
	int dominante = 1;
	for (int i = 0; i < n; i++) {
		double suma = 0.0;
		for (int j = 0; j < n; j++) {
			if (j != i) suma += fabs(A[i][j]);
		}
		if (fabs(A[i][i]) < suma) {
			dominante = 0;
			break;
		}
	}
	if (!dominante)
		printf("\nLa matriz NO es diagonalmente dominante. El método iterativo puede no converger.\n");
	else
		printf("\nLa matriz es diagonalmente dominante.\n");
}

// Método de Relajación (SOR)
void gaussSeidelRelax(double A[MAX][MAX], double b[MAX], int n, double tol, int tipoError, double w) {
	double x[MAX] = {0};       // inicialización en 0
	double error = 0.0;
	double error_viejo = 1000; // inicializamos con un valor grande
	int iter = 0;
	int maxIter = 1000;
	
	printf("\nIteraciones Gauss-Seidel con Relajación (w = %.2lf):\n", w);
	
	do {
		iter++;
		error = 0.0;
		
		for (int i = 0; i < n; i++) {
			double suma = b[i];
			for (int j = 0; j < n; j++) {
				if (j != i)
					suma -= A[i][j] * x[j];
			}
			double x_gs = suma / A[i][i]; // valor Gauss-Seidel clásico
			double x_new = w * x_gs + (1 - w) * x[i]; // aplica relajación
			
			// Cálculo del error
			double e = fabs(x_new - x[i]);
			if (tipoError == 2 && fabs(x_new) > 1e-12) // error porcentual
				e = e / fabs(x_new) * 100.0;
			if (e > error) error = e;
			
			x[i] = x_new;
		}
		
		// Verificar si el error aumenta
		if (error > error_viejo) {
			printf("\nEl método no converge, el error aumentó en la iteración %d.\n", iter);
			return;
		}
		
		error_viejo = error; // actualizar error anterior
		
	} while (error > tol && iter < maxIter);
	
	printf("\n---- RESULTADO FINAL (Gauss-Seidel con Relajación, w=%.2lf) ----\n", w);
	for (int i = 0; i < n; i++) {
		printf("x[%d] = %.6lf\n", i + 1, x[i]);
	}
	printf("Error final: %.6lf\n", error);
	printf("Cantidad de iteraciones: %d\n", iter);
}

int main() {
	int n, tipoError;
	double A[MAX][MAX], b[MAX], tol;
	double w; // parámetro de relajación
	
	printf("Ingrese el número de ecuaciones: ");
	scanf("%d", &n);
	
	printf("Ingrese los coeficientes de la matriz A y el vector b:\n");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("A[%d][%d]: ", i + 1, j + 1);
			scanf("%lf", &A[i][j]);
		}
		printf("b[%d]: ", i + 1);
		scanf("%lf", &b[i]);
	}
	
	imprimirMatriz(A, b, n);
	verificarDiagonalDominante(A, n);
	
	printf("\nIngrese la tolerancia: ");
	scanf("%lf", &tol);
	
	printf("Seleccione tipo de error:\n1. Absoluto\n2. Relativo porcentual\nOpción: ");
	scanf("%d", &tipoError);
	
	// Ejecutamos dos veces: w=1.0 y w=1.1
	gaussSeidelRelax(A, b, n, tol, tipoError, 1.0);
	gaussSeidelRelax(A, b, n, tol, tipoError, 1.1);
	
	return 0;
}
