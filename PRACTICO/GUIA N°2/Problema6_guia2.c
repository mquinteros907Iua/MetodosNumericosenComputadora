#include <stdio.h>
#include <math.h>

/*
PROBLEMA: hallar la profundidad d (desde el punto mas bajo de la esfera)
que queda sumergida, para una esfera de radio r y densidad rho (agua = 1 g/cm^3).

Derivación resumen:
V_total = (4/3) * pi * r^3
V_sub(d) = (pi * d^2 * (3r - d)) / 3      (volumen de casquete de altura d)
Queremos V_sub(d) = f * V_total  con f = rho (fraccion sumergida)

Definimos:
g(d) = V_sub(d) - f * V_total
Entonces buscamos g(d) = 0.

derivada:
g'(d) = d/dd [ (pi/3) (3r d^2 - d^3) ] 
= pi * (2 r d - d^2)
*/

#define TOL 1e-7        /* tolerancia residual para g(d) y para cambio en d */
#define DERIV_MIN 1e-4  /* umbral para derivada (no dividir si es mas pequeña) */
#define MAX_ITER 1000

/* volumen casquete de altura d, radio r */
double V_sub(double d, double r) {
	return M_PI * d * d * (3.0 * r - d) / 3.0;
}

/* g(d) = V_sub(d) - f * V_total */
double g(double d, double r, double f) {
	double Vtot = 4.0/3.0 * M_PI * r * r * r;
	return V_sub(d, r) - f * Vtot;
}

/* g'(d) = pi * (2 r d - d^2) */
double dg(double d, double r) {
	return M_PI * (2.0 * r * d - d * d);
}

/* Newton-Raphson para g(d)=0 */
double newton_depth(double r, double rho, double d0) {
	double d = d0;
	for (int k = 0; k < MAX_ITER; ++k) {
		double gd = g(d, r, rho);
		double gpd = dg(d, r);
		
		/* comprobacion derivada */
		if (fabs(gpd) < DERIV_MIN) {
			printf("Advertencia: derivada muy pequeña (|g'| = %.3e) en iter %d\n", gpd, k);
			break; /* salimos para evitar division numericamente peligrosa */
		}
		
		double d1 = d - gd / gpd;         /* paso de Newton */
		double gd1 = g(d1, r, rho);       /* evaluamos g en el candidato */
		
		/* condiciones de aceptacion */
		if (fabs(d1 - d) < TOL && fabs(gd1) < TOL) {
			printf("Convergencia Newton en %d iteraciones.\n", k+1);
			printf("Residuo g(d) = %.6e\n", gd1);
			return d1;
		}
		
		/* si el residuo ya es suficientemente pequeño lo aceptamos */
		if (fabs(gd1) < TOL) {
			printf("Convergencia por residuo en iter %d.\n", k+1);
			printf("Residuo g(d) = %.6e\n", gd1);
			return d1;
		}
		
		d = d1;
	}
	
	/* si llegamos aqui, no se alcanzo convergencia estricta */
	double final_res = g(d, r, rho);
	printf("Fin sin convergencia completa. Ultima aproximacion d = %.12f\n", d);
	printf("g(d) = %.6e\n", final_res);
	return d;
}

int main() {
	double r = 10.0;         /* cm */
	double rho = 0.638;      /* gr/cm^3 (densidad madera) */
	double inicial = 10.0;   /* guess inicial (por ejemplo r) */
	
	printf("Radio r = %.3f cm, densidad rho = %.6f\n", r, rho);
	
	double d = newton_depth(r, rho, inicial);
	
	double Vtot = 4.0/3.0 * M_PI * r * r * r;
	double Vsub = V_sub(d, r);
	printf("\nResultado:\n");
	printf("Profundidad sumergida d = %.9f cm\n", d);
	printf("Volumen total = %.6f cm^3\n", Vtot);
	printf("Volumen sumergido = %.6f cm^3\n", Vsub);
	printf("Fraction sumergida = %.6f (%.2f%%)\n", Vsub / Vtot, 100.0 * Vsub / Vtot);
	
	return 0;
}
