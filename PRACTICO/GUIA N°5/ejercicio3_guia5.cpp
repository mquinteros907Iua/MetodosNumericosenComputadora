#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX 100

// Función para contar filas en el archivo
int contarFilas(const char *nombreArchivo) {
	FILE *fp = fopen(nombreArchivo, "r");
	if (fp == NULL) {
		printf("No se puede abrir el archivo.\n");
		exit(1);
	}
	
	int filas = 0;
	char c;
	while ((c = fgetc(fp)) != EOF) {
		if (c == '\n') filas++;
	}
	
	fclose(fp);
	return filas;
}

// Función para leer datos en vectores x[] y y[]
void leerDatos(const char *nombreArchivo, double x[], double y[], int filas) {
	FILE *fp = fopen(nombreArchivo, "r");
	if (fp == NULL) {
		printf("No se puede abrir el archivo.\n");
		exit(1);
	}
	
	for (int i = 0; i < filas; i++) {
		fscanf(fp, "%lf %lf", &x[i], &y[i]);
	}
	
	fclose(fp);
}

// Función para interpolar con Lagrange
double lagrange(double x[], double y[], int n, double xp) {
	double suma = 0.0;
	
	for (int k = 0; k < n; k++) {
		double prod = 1.0;
		for (int i = 0; i < n; i++) {
			if (i != k) {
				prod *= (xp - x[i]) / (x[k] - x[i]);
			}
		}
		suma += y[k] * prod;
	}
	
	return suma;
}

int main() {
	const char *nombreArchivo = "ejercicio3.txt";
	double x[MAX], y[MAX];
	int filas;
	
	// Contar cuántos puntos hay
	filas = contarFilas(nombreArchivo);
	printf("Cantidad de puntos = %d\n", filas);
	
	// Leer datos en vectores x[] y y[]
	leerDatos(nombreArchivo, x, y, filas);
	
	// Mostrar datos leídos
	printf("Datos leídos:\n");
	for (int i = 0; i < filas; i++) {
		printf("x[%d] = %lf, y[%d] = %lf\n", i, x[i], i, y[i]);
	}
	
	// Pedir un valor donde interpolar
	double xp;
	printf("\nIngrese el valor de x para interpolar: ");
	scanf("%lf", &xp);
	
	// Calcular interpolación
	double yp = lagrange(x, y, filas, xp);
	printf("\nP(%.3lf) = %.6lf\n", xp, yp);
	
	return 0;
}
