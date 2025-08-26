#include <stdio.h>
#include <math.h>

double newtonRaphson(double A, int n, double tol, int maxIter) {
	double pk = A; // aproximación inicial
	double pk1;
	int k = 0;
	
	do {
		pk1 = ((n - 1.0) * pk + A / pow(pk, n - 1)) / n;
		if (fabs(pk1 - pk) < tol) {
			return pk1;
		}
		pk = pk1;
		k++;
	} while (k < maxIter);
	
	return pk; // retorna la última aproximación
}

int main() {
	double A = 3.0;
	int n;
	printf("Ingrese el valor de n (2 para raiz cuadrada, 3 para cubica, etc.): ");
	scanf("%d", &n);
	
	double tol = 1e-10;
	int maxIter = 1000;
	
	double raiz = newtonRaphson(A, n, tol, maxIter);
	
	printf("La raiz %d-esima de %.2f es aproximadamente: %.10f\n", n, A, raiz);
	
	// Comparación con la función pow de math.h
	printf("Valor real con pow: %.10f\n", pow(A, 1.0/n));
	
	return 0;
}
