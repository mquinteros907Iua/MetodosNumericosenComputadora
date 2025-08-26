#include <stdio.h>
#include <math.h>

#define TOL 1e-9
#define MAX_ITER 1000

// Función f(x) = x(10 - 2x)(16 - 2x) - 100
double f(double x) {
	return x * (10 - 2*x) * (16 - 2*x) - 100;
}

// Derivada f'(x)
double f_deriv(double x) {
	return (10 - 2*x)*(16 - 2*x) + x*(-2)*(16 - 2*x) + x*(10 - 2*x)*(-2);
}

int main() {
	double x0, x1;
	int iter = 0;
	
	printf("Ingrese valor inicial: ");
	scanf("%lf", &x0);
	
	do {
		double fx = f(x0);
		double fpx = f_deriv(x0);
		
		if (fabs(fpx) < 1e-12) {
			printf("Derivada cercana a cero. Método falla.\n");
			return 1;
		}
		
		x1 = x0 - fx / fpx;
		
		if (fabs(x1 - x0) < TOL) {
			printf("Solución encontrada: x = %.9lf cm\n", x1);
			printf("Iteraciones: %d\n", iter+1);
			return 0;
		}
		
		x0 = x1;
		iter++;
		
	} while (iter < MAX_ITER);
	
	printf("No se encontró solución en %d iteraciones.\n", MAX_ITER);
	return 0;
}
