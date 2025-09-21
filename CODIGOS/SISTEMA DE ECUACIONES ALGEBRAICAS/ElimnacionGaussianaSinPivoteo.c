#include <stdio.h>
#include <math.h>



/* ---------------------- Prototipos ---------------------- */
int eliminacion_gaussiana_sin_pivoteo(int n, double A[n][n], double b[n]);
int leer_n_desde_archivo(const char *nombre_archivo, int *n_out);
int leer_sistema_desde_archivo(const char *nombre_archivo, int n, double A[n][n], double b[n]);




/* =============================== MAIN =============================== */
int main(void) {
	int modo, n, i, j;
	
	printf("\n=== Eliminacion Gaussiana SIN pivoteo ===\n");
	printf("Seleccione modo de carga de datos:\n");
	printf("1) Ingresar por teclado\n");
	printf("2) Leer desde archivo 'datos.txt'\n");
	printf("Opcion: ");
	if (scanf("%d", &modo) != 1) {
		printf("Error de entrada.\n");
		return 0;
	}
	
	if (modo == 1) {
		/* ---- Carga por teclado ---- */
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
		
		eliminacion_gaussiana_sin_pivoteo(n, A, b);
	}
	else if (modo == 2) {
		/* ---- Carga desde archivo ---- */
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
		
		eliminacion_gaussiana_sin_pivoteo(n, A, b);
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
	
	/* Leer A: n filas, cada una con n numeros */
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (fscanf(f, "%lf", &A[i][j]) != 1) {
				printf("Error leyendo A en fila %d, columna %d.\n", i, j);
				fclose(f);
				return 0;
			}
		}
	}
	
	/* Leer b: n numeros en una linea (o varias, da igual) */
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







int eliminacion_gaussiana_sin_pivoteo(int n, double A[n][n], double b[n]) {
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
	
	
	
	
	/* OPCIONAL: Norma de A */
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
	
	
	/* Eliminacion hacia adelante SIN pivoteo */								//Diagonalizacion
	for (i = 0; i <= n - 2; i++) {
		
		/* Chequeo de pivote nulo/casi nulo (no se permuta(sin pivoteo)) */
		if (fabs(A[i][i]) < 1e-2) {
			printf("\nError: pivote A[%d][%d] = %.3e es demasiado pequeno. " "El metodo sin pivoteo no puede continuar.\n", i, i, A[i][i]);
			return 0;
		}
		
		for (j = i + 1; j <= n - 1; j++) {
			factor = A[j][i] / A[i][i];
			
			for (k = i; k <= n - 1; k++) {
				A[j][k] -= factor * A[i][k];
			}
			b[j] -= factor * b[i];
		}
	}
	
	
	
	/*																			Muestra Matriz aumentada luego de la eliminacion */
	printf("\nMatriz aumentada luego de la eliminacion (SIN pivoteo):\n");
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			printf("%10.4lf ", A[i][j]);
		}
		printf("| %10.4lf\n", b[i]);
	}
	
	/* Determinante (producto de la diagonal) */   								//Muestra el determinante
	double producto_diagonal = 1.0;
	for (i = 0; i < n; i++) {
		producto_diagonal *= A[i][i];
	}
	printf("\nDeterminante: %10.6lf\n", producto_diagonal);
	
	
	//																			verificacion de el determinante
	if (fabs(producto_diagonal) < 1e-12) {
		printf("\nDeterminante ~ 0: el sistema no tiene solucion unica (o es mal condicionado).\n");
		return 0;
	}
	
	
	/* Sustitucion hacia atras */
	double x[n];
	
	//																			Verifica que el ultimo elemento de la matriz no sea cero porque sino no podemos despejar nada 0=0
	if (fabs(A[n - 1][n - 1]) < 1e-14) {										
		printf("\nError: pivote final A[%d][%d] ~ 0 (%.3e). No se puede dividir.\n", n - 1, n - 1, A[n - 1][n - 1]);
		return 0;
	}
	
	//																			Despeja la primera incognita del ultimo valor d ela matriz
	x[n - 1] = b[n - 1] / A[n - 1][n - 1];
	
	for (i = n - 2; i >= 0; i--) {
		double suma = b[i];
		for (j = i + 1; j < n; j++) {
			suma -= A[i][j] * x[j];
		}
		if (fabs(A[i][i]) < 1e-14) {											//Verifica si el valor es cero porque sino no podemos despejar
			printf("\nError: pivote A[%d][%d] ~ 0 (%.3e) en sustitucion hacia atras.\n", i, i, A[i][i]);
			return 0;
		}
		x[i] = suma / A[i][i];
	}
	
	/* Solucion */
	printf("\nSolucion del sistema (Gauss sin pivoteo):\n");
	for (i = 0; i < n; i++) {
		printf("x[%d] = %10.6lf\n", i, x[i]);
	}
	
	return 1;
}
