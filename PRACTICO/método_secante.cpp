#include <stdio.h>
#include <math.h>

// Definimos la función f(x) que queremos encontrar su raíz
double f(double x) {
	return x * x - 2;  // Ejemplo: f(x) = x^2 - 2  (raíz: sqrt(2))
}

// Método de la secante
double secante(double x0, double x1, double tol, int max_iter) {
	double x2;
	int iter = 0;
	
	while (iter < max_iter) {
		double f0 = f(x0);
		double f1 = f(x1);
		
		if (fabs(f1 - f0) < 1e-12) {  
			printf("Error: división por valor muy pequeño.\n");
			return x1;
		}
		
		// Fórmula de la secante
		x2 = x1 - f1 * (x1 - x0) / (f1 - f0);
		
		printf("Iter %d: x = %.10f, f(x) = %.10f\n", iter + 1, x2, f(x2));
		
		// Condición de parada
		if (fabs(x2 - x1) < tol || fabs(f(x2)) < tol) {
			return x2;
		}
		
		// Actualizamos valores
		x0 = x1;
		x1 = x2;
		iter++;
	}
	
	printf("Se alcanzó el máximo de iteraciones.\n");
	return x2;
}

int main() {
	double x0, x1, raiz;
	int max_iter;
	double tol;
	
	printf("Método de la Secante\n");
	printf("Ingrese el primer valor inicial (x0): ");
	scanf("%lf", &x0);
	printf("Ingrese el segundo valor inicial (x1): ");
	scanf("%lf", &x1);
	printf("Ingrese la tolerancia: ");
	scanf("%lf", &tol);
	printf("Ingrese el número máximo de iteraciones: ");
	scanf("%d", &max_iter);
	
	raiz = secante(x0, x1, tol, max_iter);
	
	printf("\nLa raíz aproximada es: %.10f\n", raiz);
	
	return 0;
}
