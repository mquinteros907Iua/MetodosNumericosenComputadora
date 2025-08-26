#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 20

// --- Utilidades ---
double norma_infinito_vec(const double v[], int n) {
	double m = 0.0;
	for (int i = 0; i < n; ++i) {
		double a = fabs(v[i]);
		if (a > m) m = a;
	}
	return m;
}

void multiplicarAx(const double A[MAX][MAX], const double x[], double Ax[], int n) {
	for (int i = 0; i < n; ++i) {
		double s = 0.0;
		for (int j = 0; j < n; ++j) s += A[i][j] * x[j];
		Ax[i] = s;
	}
}

double computar_residuo_infinito(const double A[MAX][MAX], const double x[], const double b[], double tmp[], int n) {
	multiplicarAx(A, x, tmp, n);
	for (int i = 0; i < n; ++i) tmp[i] -= b[i];
	return norma_infinito_vec(tmp, n);
}

// Imprimir matriz aumentada
void imprimirMatriz(double A[MAX][MAX], double b[MAX], int n) {
	printf("\nMatriz aumentada [A|b]:\n");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%10.4lf ", A[i][j]);
		}
		printf(" | %10.4lf\n", b[i]);
	}
}

// Verifica diagonal dominante (solo informa)
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

// --- Método de Jacobi ---
void jacobi(double A[MAX][MAX], double b[MAX], int n, double tol, int tipoError, int maxIter, double x0[MAX]) {
	double x[MAX] = {0.0};
	double xn[MAX] = {0.0};
	double tmp[MAX];
	if (x0 != NULL) for (int i = 0; i < n; ++i) x[i] = x0[i];
	
	int iter = 0;
	double error_iter = 1e9, residuo;
	
	printf("\n--- Jacobi ---\n");
	while (iter < maxIter) {
		iter++;
		// calcular xn
		for (int i = 0; i < n; ++i) {
			double s = b[i];
			for (int j = 0; j < n; ++j) if (j != i) s -= A[i][j] * x[j];
			xn[i] = s / A[i][i];
		}
		
		// error entre iteraciones
		double diff[MAX];
		for (int i = 0; i < n; ++i) diff[i] = xn[i] - x[i];
		error_iter = norma_infinito_vec(diff, n);
		double norm_xn = norma_infinito_vec(xn, n);
		double error_rel = (norm_xn > 1e-15) ? error_iter / norm_xn * 100.0 : 1e9;
		
		// actualizar x
		for (int i = 0; i < n; ++i) x[i] = xn[i];
		
		// residuo
		residuo = computar_residuo_infinito(A, x, b, tmp, n);
		
		// criterio de parada
		int stop = 0;
		if ((tipoError == 1 && error_iter <= tol) ||
			(tipoError == 2 && error_rel <= tol) ||
			(tipoError == 3 && residuo <= tol)) stop = 1;
		
		if (stop) break;
	}
	
	printf("\nResultado Jacobi:\n");
	for (int i = 0; i < n; ++i) printf("x[%d] = %.8lf\n", i+1, x[i]);
	printf("Iteraciones: %d\nResiduo ||Ax-b||_inf = %.6e\nError iter = %.6e\n", iter, residuo, error_iter);
}

// --- Método de Gauss-Seidel ---
void gaussSeidel(double A[MAX][MAX], double b[MAX], int n, double tol, int tipoError, int maxIter, double x0[MAX]) {
	double x[MAX] = {0.0};
	double tmp[MAX];
	if (x0 != NULL) for (int i = 0; i < n; ++i) x[i] = x0[i];
	
	int iter = 0;
	double error_iter = 1e9, residuo;
	
	printf("\n--- Gauss-Seidel ---\n");
	while (iter < maxIter) {
		iter++;
		double x_old[MAX];
		for (int i = 0; i < n; ++i) x_old[i] = x[i];
		
		for (int i = 0; i < n; ++i) {
			double s = b[i];
			for (int j = 0; j < n; ++j) if (j != i) s -= A[i][j] * x[j];
			x[i] = s / A[i][i];
		}
		
		// error entre iteraciones
		double diff[MAX];
		for (int i = 0; i < n; ++i) diff[i] = x[i] - x_old[i];
		error_iter = norma_infinito_vec(diff, n);
		double norm_x = norma_infinito_vec(x, n);
		double error_rel = (norm_x > 1e-15) ? error_iter / norm_x * 100.0 : 1e9;
		
		// residuo
		residuo = computar_residuo_infinito(A, x, b, tmp, n);
		
		// criterio de parada
		int stop = 0;
		if ((tipoError == 1 && error_iter <= tol) ||
			(tipoError == 2 && error_rel <= tol) ||
			(tipoError == 3 && residuo <= tol)) stop = 1;
		
		if (stop) break;
	}
	
	printf("\nResultado Gauss-Seidel:\n");
	for (int i = 0; i < n; ++i) printf("x[%d] = %.8lf\n", i+1, x[i]);
	printf("Iteraciones: %d\nResiduo ||Ax-b||_inf = %.6e\nError iter = %.6e\n", iter, residuo, error_iter);
}

int main() {
	int n, tipoError, maxIter;
	double A[MAX][MAX], b[MAX], tol;
	double x0[MAX];
	
	printf("Ingrese el número de ecuaciones: ");
	scanf("%d", &n);
	
	printf("Ingrese los coeficientes de la matriz A y el vector b:\n");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("A[%d][%d]: ", i+1, j+1);
			scanf("%lf", &A[i][j]);
		}
		printf("b[%d]: ", i+1);
		scanf("%lf", &b[i]);
	}
	
	imprimirMatriz(A, b, n);
	verificarDiagonalDominante(A, n);
	
	// vector inicial
	for (int i = 0; i < n; i++) x0[i] = 0.0; // por defecto en 0
	char resp;
	printf("¿Desea ingresar vector inicial distinto de cero? (s/n): ");
	scanf(" %c", &resp);
	if (resp == 's' || resp == 'S') {
		for (int i = 0; i < n; i++) {
			printf("x0[%d]: ", i+1);
			scanf("%lf", &x0[i]);
		}
	}
	
	printf("\nSeleccione criterio de parada:\n");
	printf("1. Error absoluto entre iteraciones\n");
	printf("2. Error relativo porcentual\n");
	printf("3. Residuo ||Ax-b||\n");
	printf("Opción: ");
	scanf("%d", &tipoError);
	
	printf("Ingrese tolerancia: ");
	scanf("%lf", &tol);
	
	printf("Ingrese máximo número de iteraciones: ");
	scanf("%d", &maxIter);
	
	jacobi(A, b, n, tol, tipoError, maxIter, x0);
	gaussSeidel(A, b, n, tol, tipoError, maxIter, x0);
	
	return 0;
}
