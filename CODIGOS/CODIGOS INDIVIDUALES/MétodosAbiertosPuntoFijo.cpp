#include <stdio.h>
#include <math.h>

#define MAX_ITER 10000

/* g(x) definida por el ejercicio */
double g(double x) {
	return pow(x,2)+x-4;
}

/* Derivada numérica aproximada de g(x) */
double derivada_g(double x0) {
	const double h = 0.001;
	return (g(x0 + h) - g(x0)) / h;
}

/* Punto fijo con error absoluto */
void puntoFijoAbs(double x0, double tol) {
	double xi = x0, xi1, err;
	int it = 0;
	
	/* Verificación convergencia inicial */
	double gp = fabs(derivada_g(x0));
	if (gp >= 1.0) {
		printf("Advertencia: el metodo podria NO converger (|g'(x0)| = %.6f >= 1)\n", gp);
	}
	
	do {
		xi1 = g(xi);
		err = fabs(xi1 - xi);   // error absoluto
		xi = xi1;
		it++;
		
		if (it >= MAX_ITER && err > tol) {
			printf("No convergio en %d iteraciones con tolerancia %g.\n", MAX_ITER, tol);
			return;
		}
		
	} while (err < tol);
	
	printf("\nConvergio en %d iteraciones.\n", it);
	printf("Raiz aproximada: %.10f\n", xi1);
	printf("Error absoluto final: %.6g\n", err);
}

/* Punto fijo con error porcentual */
void puntoFijoRel(double x0, double tol) {
	double xi = x0, xi1, err;
	int it = 0;
	
	/* Verificación convergencia inicial */
	double gp = fabs(derivada_g(x0));
	if (gp >= 1.0) {
		printf("Advertencia: el metodo podria NO converger (|g'(x0)| = %.6f >= 1)\n", gp);
	}
	
	do {
		xi1 = g(xi);
		if (fabs(xi1) < 1e-14) {
			printf("Error relativo indefinido (x_{i+1} ~ 0).\n");
			return;
		}
		err = fabs((xi1 - xi) / xi1) * 100.0;  // error relativo %
		xi = xi1;
		it++;
		
		if (it >= MAX_ITER && err > tol) {
			printf("No convergio en %d iteraciones con tolerancia %g.\n", MAX_ITER, tol);
			return;
		}
		
	} while (err < tol);
	
	printf("\nConvergio en %d iteraciones.\n", it);
	printf("Raiz aproximada: %.10f\n", xi1);
	printf("Error relativo porcentual final: %.6g %%\n", err);
}

int main() {
	double x0, tol;
	int modo;
	
	printf("Metodo de Punto Fijo\n");
	printf("Ingrese x0: ");
	if (scanf("%lf", &x0) != 1) return 0;
	
	printf("Ingrese tolerancia: ");
	if (scanf("%lf", &tol) != 1) return 0;
	
	printf("Seleccione tipo de error (0 = absoluto, 1 = relativo %%): ");
	if (scanf("%d", &modo) != 1) return 0;
	
	if (modo == 0) {
		puntoFijoAbs(x0, tol);
	} else if (modo == 1) {
		puntoFijoRel(x0, tol);
	} else {
		printf("Error: modo de error invalido. Debe ser 0 o 1.\n");
	}
	
	return 0;
}
