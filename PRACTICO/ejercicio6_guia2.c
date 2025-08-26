#include <stdio.h>
#include <math.h>

#define TOL 1e-9
#define MAX_ITER 1000

double r = 10.0;       // radio en cm
double rho_m = 0.638;  // densidad madera
double rho_agua = 1.0; // densidad agua

double f(double d) {
	double Ve = (4.0/3.0) * M_PI * pow(r, 3);
	double Vs = (M_PI * pow(d, 2) * (3*r - d)) / 3.0;
	return rho_m * Ve - rho_agua * Vs;
}

double f_deriv(double d) {
	// Derivada de Vs respecto a d
	double dVs = (M_PI / 3.0) * (2*d*(3*r - d) - pow(d, 2));
	return -dVs; // porque f(d) = rho_m*Ve - Vs
}

int main() {
	double d0, d1;
	int iter = 0;
	
	printf("Ingrese valor inicial para d: ");
	scanf("%lf", &d0);
	
	do {
		double fd = f(d0);
		double fpd = f_deriv(d0);
		
		if (fabs(fpd) < 1e-12) {
			printf("Derivada cercana a cero. Método falla.\n");
			return 1;
		}
		
		d1 = d0 - fd / fpd;
		
		if (fabs(d1 - d0) < TOL) {
			printf("Solución encontrada: d = %.9lf cm\n", d1);
			printf("Iteraciones: %d\n", iter+1);
			return 0;
		}
		
		d0 = d1;
		iter++;
		
	} while (iter < MAX_ITER);
	
	printf("No se encontró solución en %d iteraciones.\n", MAX_ITER);
	return 0;
}
