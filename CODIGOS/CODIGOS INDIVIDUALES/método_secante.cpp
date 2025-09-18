#include <stdio.h>
#include <math.h>

// Definimos la función f(x) que queremos encontrar su raíz
double f(double x) {
	return x * x - 2;  // Ejemplo: f(x) = x^2 - 2  (raíz: sqrt(2))
}

// Método de la secante con error porcentual
void secantePorcentual(double x0Inicial, double x1Inicial, double tol, int max_iter) {
	double x0, x1, x2, errorPorcentual;
	int iter = 0;
	
	x0 = x0Inicial;
	x1 = x1Inicial;
	
	do {
		double f0 = f(x0);
		double f1 = f(x1);
		
		if (fabs(f1 - f0) < 1e-12) {  
			printf("\n[Finalización] División por valor muy pequeño, no se puede continuar.\n");
			return;
		}
		
		// Fórmula de la secante
		x2 = x1 - f1 * (x1 - x0) / (f1 - f0);
		errorPorcentual = (fabs(x2 - x1) / fabs(x2)) * 100.0;
		
		printf("Iter %d: x = %.10f, f(x) = %.10f, Error = %.6f%%\n", iter + 1, x2, f(x2), errorPorcentual);
		
		if (errorPorcentual < tol || fabs(f(x2)) < tol) {
			printf("\n[Finalización] Se alcanzó la tolerancia requerida.\n");
			printf("Raíz aproximada: %.10f\n", x2);
			printf("Iteraciones realizadas: %d\n", iter + 1);
			printf("f(x) = %.10f\n", f(x2));
			printf("Error Porcentual: %.6f%%\n", errorPorcentual);
			return;
		}
		
		x0 = x1;
		x1 = x2;
		iter++;
		
	} while (errorPorcentual > tol || iter < max_iter);
	
	printf("\n[Finalización] Se alcanzó el máximo de iteraciones.\n");
	printf("Raíz aproximada: %.10f\n", x2);
	printf("Iteraciones realizadas: %d\n", iter);
	printf("f(x) = %.10f\n", f(x2));
	printf("Error Porcentual: %.6f%%\n", errorPorcentual);
}

// Método de la secante con error absoluto
void secanteAbsoluto(double x0Inicial, double x1Inicial, double tol, int max_iter) {
	double x0, x1, x2, errorAbs;
	int iter = 0;
	
	x0 = x0Inicial;
	x1 = x1Inicial;
	
	do {
		double f0 = f(x0);
		double f1 = f(x1);
		
		if (fabs(f1 - f0) < 1e-12) {  
			printf("\n[Finalización] División por valor muy pequeño, no se puede continuar.\n");
			return;
		}
		
		// Fórmula de la secante
		x2 = x1 - f1 * (x1 - x0) / (f1 - f0);
		errorAbs = fabs(x2 - x1);
		
		printf("Iter %d: x = %.10f, f(x) = %.10f, Error = %.10f\n", iter + 1, x2, f(x2), errorAbs);
		
		if (errorAbs < tol || fabs(f(x2)) < tol) {
			printf("\n[Finalización] Se alcanzó la tolerancia requerida.\n");
			printf("Raíz aproximada: %.10f\n", x2);
			printf("Iteraciones realizadas: %d\n", iter + 1);
			printf("f(x) = %.10f\n", f(x2));
			printf("Error Absoluto: %.10f\n", errorAbs);
			return;
		}
		
		x0 = x1;
		x1 = x2;
		iter++;
		
	} while (errorAbs > tol || iter < max_iter);
	
	printf("\n[Finalización] Se alcanzó el máximo de iteraciones.\n");
	printf("Raíz aproximada: %.10f\n", x2);
	printf("Iteraciones realizadas: %d\n", iter);
	printf("f(x) = %.10f\n", f(x2));
	printf("Error Absoluto: %.10f\n", errorAbs);
}

int main() {
	double x0, x1, tol;
	int max_iter, opcion;
	
	printf("Método de la Secante\n");
	printf("Ingrese el primer valor inicial (x0): ");
	scanf("%lf", &x0);
	printf("Ingrese el segundo valor inicial (x1): ");
	scanf("%lf", &x1);
	printf("Ingrese el número máximo de iteraciones: ");
	scanf("%d", &max_iter);
	
	printf("\nSeleccione el tipo de error:\n");
	printf("1. Error Absoluto\n");
	printf("2. Error Porcentual\n");
	printf("Opción: ");
	scanf("%d", &opcion);
	
	if (opcion == 1) {
		printf("Ingrese la tolerancia (número): ");
		scanf("%lf", &tol);
		secanteAbsoluto(x0, x1, tol, max_iter);
	} else if (opcion == 2) {
		printf("Ingrese la tolerancia en porcentaje: ");
		scanf("%lf", &tol);
		secantePorcentual(x0, x1, tol, max_iter);
	} else {
		printf("Opción inválida.\n");
		return 1;
	}
	
	return 0;
}
