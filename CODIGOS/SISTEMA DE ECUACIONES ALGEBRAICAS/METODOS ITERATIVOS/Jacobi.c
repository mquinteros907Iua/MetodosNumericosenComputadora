#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 20

/* ============================ PROTOTIPOS ============================ */
void imprimirMatriz(double A[MAX][MAX], double b[MAX], int n);
void verificarDiagonalDominante(double A[MAX][MAX], int n);
void jacobi(double A[MAX][MAX], double b[MAX], int n, double tol, int tipoError);


int leer_n_desde_archivo(const char *nombre_archivo, int *n_salida);
int leer_sistema_desde_archivo(const char *nombre_archivo, int n, double A[MAX][MAX], double b[MAX]);






/* ================================ MAIN ============================== */
int main() {
	int n, tipoError;
	double A[MAX][MAX], b[MAX], tol;
	int modo;
	
	printf("=== Metodo de Jacobi ===\n");
	printf("Seleccione modo de carga de datos:\n");
	printf("1) Ingresar por teclado\n");
	printf("2) Leer desde archivo 'datos.txt'\n");
	printf("Opcion: ");
	if (scanf("%d", &modo) != 1) {
		printf("Error de entrada.\n");
		return 0;
	}
	
	if (modo == 1) {
		/* ---- CARGA POR TECLADO ---- */
		printf("Ingrese el número de ecuaciones: ");
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
		
		
	} else if (modo == 2) {
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
	
	printf("Seleccione tipo de error:\n1. Absoluto\n2. Relativo porcentual\nOpción: ");
	scanf("%d", &tipoError);
	
	jacobi(A, b, n, tol, tipoError);
	
	return 0;
}






/* ======================= IMPLEMENTACION ======================= */

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





/* Función para imprimir la matriz aumentada */
void imprimirMatriz(double A[MAX][MAX], double b[MAX], int n) {
	printf("\nMatriz aumentada [A|b]:\n");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%10.4lf ", A[i][j]);
		}
		printf(" | %10.4lf\n", b[i]);
	}
}



/* Verifica diagonal dominante */												//La suma de todos lo valores de la fila excepto el pivote debe ser menor al pivote
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
		printf("\nLa matriz NO es diagonalmente dominante. El método Jacobi puede no converger.\n");
	else
		printf("\nLa matriz es diagonalmente dominante.\n");
}


/* Método de Jacobi */
void jacobi(double A[MAX][MAX], double b[MAX], int n, double tol, int tipoError) {
	double x[MAX] = {0};     
	double xn[MAX];          // vector de incognitas
	double error = 0.0;
	double error_viejo = 1000; 
	int iter = 0;
	int maxIter = 10000;
	
	printf("\nIteraciones Jacobi:\n");
	
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
				if (j != i)
					suma -= A[i][j] * x[j];
			}
			
			
			if (fabs(A[i][i]) < 1e-14) {										//Verifica que el pivote no sea cero a la hora de dividir
				printf("Error: pivote diagonal A[%d][%d] = %.3e es demasiado pequeño; no se puede dividir.\n", i + 1, i + 1, A[i][i]);
				return;
			}
			
			xn[i] = suma / A[i][i];
			
		}
		
		
		// Cálculo del error													//Se seleciona el error entre absoluto y porcentual
		for (int i = 0; i < n; i++) {
			double e = fabs(xn[i] - x[i]);
			if (tipoError == 2 && fabs(xn[i]) > 1e-12) // error porcentual
				e = e / fabs(xn[i]) * 100.0;
			if (e > error) error = e;				//Errror absoluto
			x[i] = xn[i];
		}
		
		// Verificar si el error aumenta										//Verifica que el error disminuya
		if (error > error_viejo) {
			printf("\nEl método no converge, el error aumentó en la iteración %d.\n", iter);
			return; // salir del método
		}
		
		error_viejo = error; // actualizar error anterior
		
	} while (error > tol);
	
	printf("\n---- RESULTADO FINAL ----\n");
	for (int i = 0; i < n; i++) {
		printf("x[%d] = %.6lf\n", i + 1, x[i]);
	}
	printf("Error final: %.6lf\n", error);
	printf("Cantidad de iteraciones: %d\n", iter);
}
