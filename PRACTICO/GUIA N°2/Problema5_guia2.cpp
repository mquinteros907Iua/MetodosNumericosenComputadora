#include <stdio.h>
#include <math.h>

/*
PROBLEMA: hallar la catenaria que pasa por los puntos (±a , b).

Ecuación de la catenaria (con vértice en el origen):
y = C cosh(x/C) - C

Condición para que pase por (a , b):
b = C cosh(a/C) - C

Definimos:
f(C) = C cosh(a/C) - C - b

Su derivada:
f'(C) = cosh(a/C) - (a/C) sinh(a/C) - 1
*/

#define TOL 1e-6      // tolerancia para convergencia
#define DERIV_MIN 1e-4 // mínimo valor aceptable para |f'(C)|
#define MAX_ITER 10000   // máximo de iteraciones

// Función f(C)
double f(double C, double a, double b) {
	return C * cosh(a / C) - C - b;
}

// Derivada f'(C)
double df(double C, double a) {
	return cosh(a / C) - (a / C) * sinh(a / C) - 1;
}

// Método de Newton-Raphson
double newton(double a, double b, double x0) {
	double x = x0;
	for (int i = 0; i < MAX_ITER; i++) {
		double fx = f(x, a, b);
		double dfx = df(x, a);
		
		// Verificación: derivada no demasiado pequeña
		if (fabs(dfx) < DERIV_MIN) {
			printf("La derivada es demasiado pequeña en la iteracion %d\n", i);
			break;
		}
		
		double x1 = x - fx / dfx;
		
		if (fabs(x1 - x) < TOL) {
			// Verificamos que f(x1) ˜ 0
			double error_final = f(x1, a, b);
			if (fabs(error_final) < TOL) {
				printf("Convergencia alcanzada en %d iteraciones.\n", i+1);
				printf("f(C) en la solucion = %.6e\n", error_final);
				return x1;
			} else {
				printf("No se alcanzo suficiente precision: f(C)=%.6e\n", error_final);
				return x1;
			}
		}
		
		x = x1;
	}
	
	printf("No convergio en %d iteraciones.\n", MAX_ITER);
	return x;
}

int main() {
	double a = 12, b = 5.0;
	
	// Valor inicial (cercano a "a")
	double C = newton(a, b, 10.0);
	
	printf("La catenaria que pasa por (±%.0lf , %.0lf) es:\n", a, b);
	printf("y = %.6lf * cosh(x / %.6lf) - %.6lf\n", C, C, C);
	
	return 0;
}
