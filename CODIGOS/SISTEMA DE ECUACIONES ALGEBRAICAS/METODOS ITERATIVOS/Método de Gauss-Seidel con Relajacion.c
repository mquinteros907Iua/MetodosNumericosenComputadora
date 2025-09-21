#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 20



/* ============================ PROTOTIPOS ============================ */
void imprimirMatriz(double A[MAX][MAX], double b[MAX], int n);
void verificarDiagonalDominante(double A[MAX][MAX], int n);
double aplicarRelajacion(double x_old, double x_new, double w);
void gaussSeidel(double A[MAX][MAX], double b[MAX], int n, double tol, int tipoError, double w);

/* Lectura desde archivos*/
int leer_n_desde_archivo(const char *nombre_archivo, int *n_salida);
int leer_sistema_desde_archivo(const char *nombre_archivo, int n, double A[MAX][MAX], double b[MAX]);








/* ================================ MAIN ============================== */
int main() {
	int n, tipoError, modo_carga, modo_relajacion;
	double A[MAX][MAX], b[MAX], tol;
	double w = 1.0;  
	
	printf("=== Metodo de Gauss-Seider ===\n");
	printf("Seleccione modo de carga de datos:\n");
	printf("1) Ingresar por teclado\n");
	printf("2) Leer desde archivo 'datos.txt'\n");
	printf("Opcion: ");
	if (scanf("%d", &modo_carga) != 1) {
		printf("Error de entrada.\n");
		return 0;
	}
	
	if (modo_carga == 1) {
		/* ---- CARGA POR TECLADO ---- */
		printf("Ingrese el numero de ecuaciones: ");
		scanf("%d", &n);
		if (n <= 0 || n > MAX) {
			printf("n invalido (1..%d).\n", MAX);
			return 0;
		}
		
		printf("Ingrese los coeficientes de la matriz A y el vector b:\n");
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				printf("A[%d][%d]: ", i + 1, j + 1);
				scanf("%lf", &A[i][j]);
			}
			printf("b[%d]: ", i + 1);
			scanf("%lf", &b[i]);
		}
		
		
		
	} else if (modo_carga == 2) {
		/* ---- CARGA DESDE ARCHIVO ---- */
		const char *archivo = "datos.txt";
		if (!leer_n_desde_archivo(archivo, &n)) {
			printf("No se pudo leer n desde '%s'.\n", archivo);
			return 0;
		}
		if (n <= 0 || n > MAX) {
			printf("n invalido en archivo (1..%d).\n", MAX);
			return 0;
		}
		if (!leer_sistema_desde_archivo(archivo, n, A, b)) {
			printf("No se pudo leer el sistema completo desde '%s'.\n", archivo);
			return 0;
		}
	} else {
		printf("Opcion invalida.\n");
		return 0;
	}
	
	imprimirMatriz(A, b, n);
	verificarDiagonalDominante(A, n);
	
	printf("\nIngrese la tolerancia: ");
	scanf("%lf", &tol);
	
	printf("Seleccione tipo de error:\n1. Absoluto\n2. Relativo porcentual\nOpcion: ");
	scanf("%d", &tipoError);
	
	printf("\nModo de Gauss-Seidel:\n");
	printf("1) Sin relajacion\n");
	printf("2) Con relajacion\n");
	printf("Opcion: ");
	scanf("%d", &modo_relajacion);
	
	if (modo_relajacion == 1) {
		w = 1.0; 
	} else if (modo_relajacion == 2) {
		printf("Ingrese el coeficiente de relajacion w (0 < w < 2): ");
		scanf("%lf", &w);
	} else {
		printf("Opcion invalida.\n");
		return 0;
	}
	
	gaussSeidel(A, b, n, tol, tipoError, w);
	
	return 0;
}















/* ======================= IMPLEMENTAICONES ======================= */


int leer_n_desde_archivo(const char *nombre_archivo, int *n_salida) {
	FILE *f = fopen(nombre_archivo, "r");
	if (!f) {
		printf("No se pudo abrir '%s'.\n", nombre_archivo);
		return 0;
	}
	int ok = (fscanf(f, "%d", n_salida) == 1);
	fclose(f);
	return ok;
}


int leer_sistema_desde_archivo(const char *nombre_archivo, int n, double A[MAX][MAX], double b[MAX]) {
	FILE *f = fopen(nombre_archivo, "r");
	if (!f) {
		printf("No se pudo abrir '%s'.\n", nombre_archivo);
		return 0;
	}
	
	int nn;
	if (fscanf(f, "%d", &nn) != 1 || nn != n) {
		printf("Encabezado n inconsistente en el archivo.\n");
		fclose(f);
		return 0;
	}
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (fscanf(f, "%lf", &A[i][j]) != 1) {
				printf("Error leyendo A en fila %d, columna %d.\n", i + 1, j + 1);
				fclose(f);
				return 0;
			}
		}
	}
	
	for (int i = 0; i < n; i++) {
		if (fscanf(f, "%lf", &b[i]) != 1) {
			printf("Error leyendo b en posicion %d.\n", i + 1);
			fclose(f);
			return 0;
		}
	}
	
	fclose(f);
	return 1;
}








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
- w = 1      ? Gauss-Seidel puro
- 0 < w < 1  ? sub-relajación
- 1 < w < 2  ? sobre-relajación
=========================================================== */

double aplicarRelajacion(double x_old, double x_new, double w) {
	return (1.0 - w) * x_old + w * x_new;
}


/* Método de Gauss-Seidel con parámetro de relajación w
(cuando w=1, es Gauss-Seidel estándar) */
void gaussSeidel(double A[MAX][MAX], double b[MAX], int n, double tol, int tipoError, double w) {
	
	double x[MAX] = {0};       
	double error = 0.0;
	double error_viejo = 1000; 
	int iter = 0;
	int maxIter = 10000;
	
	
	if (fabs(w - 1.0) < 1e-15) {
		printf("\nIteraciones Gauss-Seidel (sin relajacion, w=1):\n");
	} else {
		printf("\nIteraciones Gauss-Seidel (con relajacion, w=%.6lf):\n", w);
	}
	
	do {
		iter++;
		error = 0.0;
		
		if (maxIter <= iter){
			
			printf("Se alcanzo el numero maximo de iteraciones\n");
			return;
		}
		
		
		for (int i = 0; i < n; i++) {
			double suma = b[i];
			for (int j = 0; j < n; j++) {
				if (j != i) {
					suma -= A[i][j] * x[j];  // usa valores ya actualizados de GS
				}
			}
			
			if (fabs(A[i][i]) < 1e-14) {
				printf("Error: pivote A[%d][%d] muy pequeño o nulo.\n", i + 1, i + 1);
				return;
			}
			
			double x_new = suma / A[i][i];
			
			/* Unifica GS puro y SOR:
			- Si w = 1 ? x_relajado = x_new (GS clásico)
			- Si w != 1 ? x_relajado = (1-w)*x_old + w*x_new (SOR) */
			double x_relajado = aplicarRelajacion(x[i], x_new, w);
			
			// Cálculo del error por componente 
			double e = fabs(x_relajado - x[i]);					//Error absoluto
			if (tipoError == 2 && fabs(x_relajado) > 1e-12) { // error relativo porcentual
				e = e / fabs(x_relajado) * 100.0;
			}
			if (e > error) {
				error = e;
			}
			
			x[i] = x_relajado;
		}
		
		// Verificar si el error aumenta
		if (error > error_viejo) {
			printf("\nEl método no converge, el error aumento en la iteracion %d.\n", iter);
			return; // salir del método
		}
		
		error_viejo = error;
		
		// (Opcional) Mostrar valores por iteración:
		// printf("Iter %d | error = %.6lf\n", iter, error);
		
	} while (error > tol);
	
	
	printf("\n---- RESULTADO FINAL (Gauss-Seidel) ----\n");
	for (int i = 0; i < n; i++) {
		printf("x[%d] = %.6lf\n", i + 1, x[i]);
	}
	printf("Error final: %.6lf\n", error);
	printf("Cantidad de iteraciones: %d\n", iter);
}
	
