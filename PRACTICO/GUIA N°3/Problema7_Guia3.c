#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 10

// Funci�n para imprimir matriz aumentada
void imprimirMatriz(double matriz[MAX][MAX+1], int n) {
	for(int i = 0; i < n; i++) {
		for(int j = 0; j <= n; j++)
			printf("%8.3lf ", matriz[i][j]);
		printf("\n");
	}
	printf("\n");
}

// Funci�n para armar la matriz aumentada [A|b]
void crearMatrizAumentada(double A[MAX][MAX], double b[MAX], double matriz[MAX][MAX+1], int n) {
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++)
			matriz[i][j] = A[i][j];
			matriz[i][n] = b[i];
	}
}

// Funci�n para intercambiar dos filas
void intercambiarFilas(double matriz[MAX][MAX+1], int fila1, int fila2, int n) {
	for(int j = 0; j <= n; j++) {
		double temp = matriz[fila1][j];
		matriz[fila1][j] = matriz[fila2][j];
		matriz[fila2][j] = temp;
	}
}

// Funci�n para reducir la matriz aumentada a triangular superior con pivoteo parcial
int triangularSuperiorPivoteo(double matriz[MAX][MAX+1], int n) {
	for(int i = 0; i < n-1; i++) {
		// Pivoteo parcial: buscar el m�ximo en la columna i desde la fila i hacia abajo
		int filaMax = i;
		double max = fabs(matriz[i][i]);
		for(int k = i+1; k < n; k++) {
			if(fabs(matriz[k][i]) > max) {
				max = fabs(matriz[k][i]);
				filaMax = k;
			}
		}
		
		// Intercambiar filas si es necesario
		if(filaMax != i) {
			intercambiarFilas(matriz, i, filaMax, n);
			printf("Se intercambiaron las filas %d y %d por pivoteo parcial:\n", i+1, filaMax+1);
			imprimirMatriz(matriz, n);
		}
		
		if(matriz[i][i] == 0) {
			printf("Error: pivote nulo en fila %d, el sistema puede ser singular.\n", i+1);
			return 0;
		}
		
		// Eliminaci�n hacia adelante
		for(int k = i+1; k < n; k++) {
			double factor = matriz[k][i] / matriz[i][i];
			for(int j = i; j <= n; j++)
				matriz[k][j] -= factor * matriz[i][j];
		}
		
		printf("Matriz despu�s de eliminar columna %d:\n", i+1);
		imprimirMatriz(matriz, n);
	}
	return 1; // �xito
}

// Funci�n para calcular determinante de la matriz triangular superior
double determinante(double matriz[MAX][MAX+1], int n) {
	double det = 1.0;
	for(int i = 0; i < n; i++)
		det *= matriz[i][i];
		return det;
}

// Sustituci�n hacia atr�s para resolver Ax = b
void sustitucionAtras(double matriz[MAX][MAX+1], int n, double x[MAX]) {
	for(int i = n-1; i >= 0; i--) {
		x[i] = matriz[i][n];
		for(int j = i+1; j < n; j++)
			x[i] -= matriz[i][j] * x[j];
			x[i] /= matriz[i][i];
	}
}

int main() {
	int n = 4; // n�mero de inc�gnitas
	double A[MAX][MAX], b[MAX], matriz[MAX][MAX+1], x[MAX];
	
	// Matriz A del problema
	double tempA[4][4] = {
		{80, -50, -30, 0},
	{-50, 100, -10, -25},
		{-30, -10, 65, -20},
	{0, -25, -20, 100}
	};
	
	// Vector b
	double tempb[4] = {240, 0, 0, 0};
	
	// Copiar a A y b
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			A[i][j] = tempA[i][j];
		}
		b[i] = tempb[i];
	}
	
	// Crear matriz aumentada
	crearMatrizAumentada(A, b, matriz, n);
	
	printf("Matriz aumentada inicial [A|b]:\n");
	imprimirMatriz(matriz, n);
	
	// Reducir a triangular superior con pivoteo parcial
	if(!triangularSuperiorPivoteo(matriz, n)) {
		printf("No se puede continuar, sistema singular.\n");
		return 1;
	}
	
	// Verificar determinante
	double det = determinante(matriz, n);
	printf("Determinante de la matriz: %.6lf\n", det);
	if(det == 0) {
		printf("El sistema no tiene soluci�n �nica.\n");
		return 1;
	}
	
	// Sustituci�n hacia atr�s
	sustitucionAtras(matriz, n, x);
	
	// Mostrar soluci�n
	printf("Soluci�n del sistema:\n");
	for(int i = 0; i < n; i++)
		printf("i%d = %.6lf A\n", i+1, x[i]);
	
	return 0;
}
