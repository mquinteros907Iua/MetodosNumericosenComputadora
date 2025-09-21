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

/* ===========================================================
FUNCIÓN DE RELAJACIÓN
----------------------
- w = 1  ? Gauss-Seidel puro
- 0 < w < 1 ? sub-relajación
- 1 < w < 2 ? sobre-relajación
-----------------------------------------------------------
Si la docente pide cambiar la regla, SOLO se modifica aquí
=========================================================== */
double aplicarRelajacion(double x_old, double x_new, double w) {
	return (1.0 - w) * x_old + w * x_new;
}

// Método de Gauss-Seidel con coeficiente de relajación w
void gaussSeidel(double A[MAX][MAX], double b[MAX], int n,
	double tol, int tipoError, double w) {
	
	double x[MAX] = {0};       // inicialización en 0
	double error = 0.0;
	double error_viejo = 1000; // inicializamos con un valor grande
	int iter = 0;
	int maxIter = 1000;
	
	printf("\nIteraciones Gauss-Seidel (con w=%.2lf):\n", w);
	
	do {
		iter++;
		error = 0.0;
		
		for (int i = 0; i < n; i++) {
			double suma = b[i];
			for (int j = 0; j < n; j++) {
				if (j != i)
					suma -= A[i][j] * x[j];  // ya usamos valores actualizados
			}
			
			if (fabs(A[i][i]) < 1e-14) {
				printf("Error: pivote A[%d][%d] muy pequeño o nulo.\n", i+1, i+1);
				return;
			}
			
			double x_new = suma / A[i][i];
			
			// aplicar la función de relajación
			double x_relajado = aplicarRelajacion(x[i], x_new, w);
			
			// cálculo del error
			double e = fabs(x_relajado - x[i]);
			if (tipoError == 2 && fabs(x_relajado) > 1e-12) // error relativo porcentual
				e = e / fabs(x_relajado) * 100.0;
			if (e > error) error = e;
			
			x[i] = x_relajado;
		}
		
		// verificar si el error aumenta
		if (error > error_viejo) {
			printf("\nEl método no converge, el error aumentó en la iteración %d.\n", iter);
			return; // salir del método
		}
		
		error_viejo = error;
		
		// Mostrar valores en cada iteración (opcional)
		printf("Iter %d: ", iter);
		for (int k = 0; k < n; k++) {
			printf("%10.6lf ", x[k]);
		}
		printf("| error = %.6lf\n", error);
		
	} while (error > tol && iter < maxIter);
	
	printf("\n---- RESULTADO FINAL (Gauss-Seidel) ----\n");
	for (int i = 0; i < n; i++) {
		printf("x[%d] = %.6lf\n", i + 1, x[i]);
	}
	printf("Error final: %.6lf\n", error);
	printf("Cantidad de iteraciones: %d\n", iter);
}
	
int main() {
	int n, tipoError;
	double A[MAX][MAX], b[MAX], tol;
	double w; // coeficiente de relajación (0 < w < 2)
	
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
	
	printf("\nIngrese el coeficiente de relajación w (0 < w < 2): ");
	scanf("%lf", &w);
	
	gaussSeidel(A, b, n, tol, tipoError, w);
	
	return 0;
}
