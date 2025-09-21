#include <stdio.h>
#include <math.h>



/* --- Prototipos --- */
int eliminacion_gaussiana_pivoteo_parcial(int n, double A[n][n], double b[n]);
int leer_n_desde_archivo(const char *nombre_archivo, int *n_out);
int leer_sistema_desde_archivo(const char *nombre_archivo, int n, double A[n][n], double b[n]);






/* =============================== MAIN =============================== */
int main(void) {
	int n, i, j;
	int modo;
	
	printf("\n=== Eliminacion Gaussiana con pivoteo parcial ===\n");
	printf("Seleccione modo de carga de datos:\n");
	printf("1) Ingresar por teclado\n");
	printf("2) Leer desde archivo 'datos.txt'\n");
	printf("Opcion: ");
	if (scanf("%d", &modo) != 1) {
		printf("Error de entrada.\n");
		return 0;
	}
	
	if (modo == 1) {
		printf("\nIngrese el tamanio del sistema (n): ");
		if (scanf("%d", &n) != 1 || n <= 0) {
			printf("n invalido.\n");
			return 0;
		}
		
		double A[n][n];
		double b[n];
		
		printf("\nIngrese la matriz A (n x n):\n");
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				printf("A[%d][%d]: ", i, j);
				if (scanf("%lf", &A[i][j]) != 1) {
					printf("Entrada invalida.\n");
					return 0;
				}
			}
		}
		
		printf("\nIngrese el vector b (n):\n");
		for (i = 0; i < n; i++) {
			printf("b[%d]: ", i);
			if (scanf("%lf", &b[i]) != 1) {
				printf("Entrada invalida.\n");
				return 0;
			}
		}
		
		eliminacion_gaussiana_pivoteo_parcial(n, A, b);
	}
	else if (modo == 2) {
		const char *archivo = "datos.txt";
		if (!leer_n_desde_archivo(archivo, &n)) {
			printf("No se pudo leer n desde '%s'. Verifique formato.\n", archivo);
			return 0;
		}
		if (n <= 0) {
			printf("n invalido en archivo.\n");
			return 0;
		}
		
		double A[n][n];
		double b[n];
		
		if (!leer_sistema_desde_archivo(archivo, n, A, b)) {
			printf("No se pudo leer el sistema completo desde '%s'.\n", archivo);
			return 0;
		}
		
		eliminacion_gaussiana_pivoteo_parcial(n, A, b);
	}
	else {
		printf("Opcion invalida.\n");
		return 0;
	}
	
	return 0;
}








/* =================== IMPLEMENTACIONES =================== */

int leer_n_desde_archivo(const char *nombre_archivo, int *n_out) {
	FILE *f = fopen(nombre_archivo, "r");
	if (!f) {
		printf("No se pudo abrir '%s'.\n", nombre_archivo);
		return 0;
	}
	int ok = (fscanf(f, "%d", n_out) == 1);
	fclose(f);
	return ok;
}

int leer_sistema_desde_archivo(const char *nombre_archivo, int n, double A[n][n], double b[n]) {
	FILE *f = fopen(nombre_archivo, "r");
	if (!f) {
		printf("No se pudo abrir '%s'.\n", nombre_archivo);
		return 0;
	}
	
	int nn;
	if (fscanf(f, "%d", &nn) != 1 || nn != n) {
		printf("Encabezado (n) inconsistente o invalido en archivo.\n");
		fclose(f);
		return 0;
	}
	
	/* Leer matriz A (n x n) */
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (fscanf(f, "%lf", &A[i][j]) != 1) {
				printf("Error leyendo A en fila %d, columna %d.\n", i, j);
				fclose(f);
				return 0;
			}
		}
	}
	
	/* Leer vector b (n valores, en una o varias lineas) */
	for (int i = 0; i < n; i++) {
		if (fscanf(f, "%lf", &b[i]) != 1) {
			printf("Error leyendo b en posicion %d.\n", i);
			fclose(f);
			return 0;
		}
	}
	
	fclose(f);
	return 1;
}






int eliminacion_gaussiana_pivoteo_parcial(int n, double A[n][n], double b[n]) {
	int i, j, k;
	double factor;
	
	/* Mostrar matriz y vector ingresados */
	printf("\nMatriz A ingresada:\n");
	for (int ii = 0; ii < n; ii++) {
		for (int jj = 0; jj < n; jj++) {
			printf("%10.4lf ", A[ii][jj]);
		}
		printf("\n");
	}
	
	printf("\nVector b ingresado:\n");
	for (int ii = 0; ii < n; ii++) {
		printf("%10.4lf\n", b[ii]);
	}
	
	
	
	/* OPCIONAL: Norma */
	{
		int mostrar_norma;
		printf("\nDesea mostrar la norma de A? (1=Si, 0=No): ");
		if (scanf("%d", &mostrar_norma) != 1) {
			printf("Entrada invalida.\n");
			return 0;
		}
		
		if (mostrar_norma == 1) {
			double suma2 = 0.0;
			for (i = 0; i < n; i++) {
				for (j = 0; j < n; j++) {
					suma2 += A[i][j] * A[i][j];
				}
			}
			double normaF = sqrt(suma2);
			printf("\nNorma de A: %10.6lf\n", normaF);
		}
	}
	
	
	/* Eliminacion hacia adelante con pivoteo parcial */						//Metodo pricnipal
	for (i = 0; i <= n - 2; i++) {
		
		int p = i;
		
		if (fabs(A[i][i]) < 1e-2) {												//Si es un valor cerca de cero busca el valor mas grande de la columna y interaciba la fila
			
			for (int l = i + 1; l < n; l++) {
				if (fabs(A[l][i]) > fabs(A[p][i])) {
					p = l;
				}
			}
			if (p != i) {
				for (int m = i; m < n; m++) {
					double aux = A[p][m];
					A[p][m] = A[i][m];
					A[i][m] = aux;
				}
				double auxb = b[p];
				b[p] = b[i];
				b[i] = auxb;
			}
		}
		
		for (j = i + 1; j <= n - 1; j++) {
			factor = A[j][i] / A[i][i];
			
			for (k = i; k <= n - 1; k++) {
				A[j][k] = A[j][k] - factor * A[i][k];
			}
			b[j] = b[j] - factor * b[i];
		}
	}
	
	printf("\nMatriz aumentada luego de la eliminacion (pivoteo parcial):\n");
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			printf("%10.4lf ", A[i][j]);
		}
		printf("| %10.4lf\n", b[i]);
	}
	
	/* Determinante */                                                          //Calculo del determinante
	double producto_diagonal = 1.0;
	for (i = 0; i < n; i++) {
		producto_diagonal = producto_diagonal * A[i][i];
	}
	printf("\nDeterminante: %10.6lf\n", producto_diagonal);
	
	//																			Si el determinante es cero no tiene solucion unica 
	if (fabs(producto_diagonal) < 1e-12) {
		printf("\nDeterminante ~ 0: el sistema no tiene solucion unica.\n");
		return 0;
	}
	
	
	/* Sustitucion hacia atras */
	double x[n];
	x[n - 1] = b[n - 1] / A[n - 1][n - 1];
	
	for (i = n - 2; i >= 0; i--) {
		double suma = b[i];
		for (j = i + 1; j < n; j++) {
			suma = suma - A[i][j] * x[j];
		}
		x[i] = suma / A[i][i];
	}
	
	printf("\nSolucion del sistema:\n");
	for (i = 0; i < n; i++) {
		printf("x[%d] = %10.6lf\n", i, x[i]);
	}
	
	return 1;
}

