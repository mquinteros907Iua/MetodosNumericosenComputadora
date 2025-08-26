#include <stdio.h>
#include <math.h>

/* ====== Definí acá tu g(x) (x = g(x)) ====== */
double g(double x) {
	/* EJEMPLO: reemplazá esta línea por tu g(x) */
	//1. return pow(x, 5) - 3* pow(x, 3) - 2 * pow(x, 2)+2;  
	//2. return cos(sin(x));
	return pow(x,x-cos(x));
}

/* Derivada numérica de g en x0 */
double derivada_g(double x0) {
	const double h = 0.001;
	return (g(x0 + h) - g(x0)) / h;
}

/* ====== Método de Punto Fijo completo ====== */
void metodo_punto_fijo(void) {
	double x0, tol, raiz, err;
	int max_iter, modo, it;
	
	/* Solicitar datos */
	printf("x0: "); if (scanf("%lf", &x0) != 1) return;
	printf("tolerancia: "); if (scanf("%lf", &tol) != 1) return;
	printf("max_iter: 10000\n"); 
	max_iter=10000;
	printf("modo error (0=abs, 1=rel%%): "); 
	if (scanf("%d", &modo) != 1)
		return;
	
	/* Verificaciones básicas */
	if (tol <= 0.0 || max_iter <= 0 || (modo != 0 && modo != 1)) {
		printf("Error: parametros invalidos.\n");
		return;
	}
	
	/* Chequeo de convergencia inicial */
	double gp = fabs(derivada_g(x0));
	if (gp >= 1.0) {
		printf("El metodo no converge (|g'(x0)| = %.6g >= 1).\n", gp);
		return;
	}
	
	/* Iteraciones */
	double xi = x0, xi1;
	err = tol + 1.0; // Para entrar al bucle
	it = 0;
	
	do {
		it++;
		xi1 = g(xi);
		
		if (modo == 0) {
			err = fabs(xi1 - xi);                  // error absoluto
		} else {
			if (fabs(xi1) < 1e-14) {
				printf("Error relativo indefinido (x_{i+1} ~ 0).\n");
				return;
			}
			err = fabs((xi1 - xi) / xi1) * 100.0;   // error relativo %
		}
		
		xi = xi1;
		
		if (it >= max_iter && err > tol) {
			printf("No convergio en %d iteraciones con tolerancia %g.\n",
				   max_iter, tol);
			return;
		}
	} while (err > tol);
	
	raiz = xi1;
	
	/* Resultados */
	printf("\nConvergio en %d iteraciones.\n", it);
	printf("Raiz aproximada: %.10f\n", raiz);
	if (modo == 0) printf("Error absoluto final: %.6g\n", err);
	else           printf("Error relativo porcentual final: %.6g %%\n", err);
}

/* ====== Main mínimo ====== */
int main(void) {
	metodo_punto_fijo();
	return 0;
}
