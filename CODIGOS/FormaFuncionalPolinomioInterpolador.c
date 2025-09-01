#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 100

// ------------------------------
// Función para contar filas en archivo de texto
int contarFilas(const char *archivo) {
	FILE *fp = fopen(archivo, "r");
	if (!fp) {
		printf("Error: no se puede abrir %s\n", archivo);
		exit(1);
	}
	int filas = 0;
	char linea[256];
	while (fgets(linea, sizeof(linea), fp) != NULL) {
		if (linea[0] != '\n') filas++;
	}
	fclose(fp);
	return filas;
}

// ------------------------------
// Leer datos en x[], y[]
void leerDatos(const char *archivo, double x[], double y[], int n) {
	FILE *fp = fopen(archivo, "r");
	if (!fp) {
		printf("Error: no se puede abrir %s\n", archivo);
		exit(1);
	}
	for (int i = 0; i < n; i++) {
		fscanf(fp, "%lf %lf", &x[i], &y[i]);
	}
	fclose(fp);
}

void formaFuncional(double x[], double y[], double A[MAX][MAX], double b[MAX], int n) {
	// Construcción de A
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			A[i][j] = pow(x[i], j);
		}
		b[i] = y[i]; 
	}
}

// Eliminación Gaussiana con pivoteo parcial
void gaussPivot(double A[MAX][MAX], double b[MAX], double coef[MAX], int n) {
	for (int k = 0; k < n-1; k++) {
		// Pivoteo
		int maxRow = k;
		for (int i = k+1; i < n; i++) {
			if (fabs(A[i][k]) > fabs(A[maxRow][k])) {
				maxRow = i;
			}
		}
		// Intercambiar filas
		if (maxRow != k) {
			for (int j = k; j < n; j++) {
				double tmp = A[k][j];
				A[k][j] = A[maxRow][j];
				A[maxRow][j] = tmp;
			}
			double tmp = b[k];
			b[k] = b[maxRow];
			b[maxRow] = tmp;
		}
		// Eliminación
		for (int i = k+1; i < n; i++) {
			double factor = A[i][k] / A[k][k];
			for (int j = k; j < n; j++) {
				A[i][j] -= factor * A[k][j];
			}
			b[i] -= factor * b[k];
		}
	}
	// Sustitución hacia atrás
	for (int i = n-1; i >= 0; i--) {
		coef[i] = b[i];
		for (int j = i+1; j < n; j++) {
			coef[i] -= A[i][j] * coef[j];
		}
		coef[i] /= A[i][i];
	}
}

// ------------------------------
// Evaluar polinomio en un punto
double evaluar(double coef[MAX], int n, double xp) {
	double suma = 0.0;
	for (int i = 0; i < n; i++) {
		suma = suma + coef[i] * pow(xp, i);
	}
	return suma;
}

// ------------------------------
// Programa principal
int main() {
	const char *archivo = "datos.dat";
	double x[MAX], y[MAX], coef[MAX];
	double A[MAX][MAX], b[MAX];
	int n = contarFilas(archivo);
	
	leerDatos(archivo, x, y, n);
	
	printf("Se leyeron %d puntos.\n", n);
	
	// Armar sistema A * coef = y
	formaFuncional(x, y, A, b, n);
	
	// Resolver con Gauss-Pivoteo
	gaussPivot(A, b, coef, n);
	
	// Mostrar coeficientes
	printf("\nCoeficientes del polinomio interpolador:\n");
	for (int i = 0; i < n; i++) {
		printf("a[%d] = %lf\n", i, coef[i]);
	}
	
	// Evaluar en un valor
	double xp;
	printf("\nIngrese el valor de x para interpolar: ");
	scanf("%lf", &xp);
	
	double yp = evaluar(coef, n, xp);
	printf("\nP(%lf) = %lf\n", xp, yp);
	
	return 0;
}
