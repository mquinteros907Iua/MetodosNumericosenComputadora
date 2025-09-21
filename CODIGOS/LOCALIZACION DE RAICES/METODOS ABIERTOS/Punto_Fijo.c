/* =======================================================================
punto_fijo.c
- Método de Punto Fijo g(x) con dos criterios: error absoluto / porcentual
======================================================================= */

#include <stdio.h>
#include <math.h>


/* -----------------FUNCIONES---------------------------------*/
/* g(x): función de iteración  (elegila según el problema) */
double g(double x) {
	
	return cos(x);																//Modificar funcion
}
/* gp(x): derivada numérica de g */												//Alternativa, puede ser que haya que cambiar la formula por la derivada real de la funcion g
double gp(double x) {
	const double h = 1e-3;
	return (g(x + h) - g(x)) / h;
}
/* ================================================================ */




/* --------- Prototipos --------- */
int punto_fijo_error_absoluto(double tolerancia, double x_inicial);
int punto_fijo_error_porcentual(double tolerancia, double x_inicial);






/* ======================= MAIN ======================= */
int main(void) {
	int opcion;
	
	do {
		printf("\n========== MENU (Punto Fijo) ==========\n");
		printf("1) Ejecutar (error ABSOLUTO)\n");
		printf("2) Ejecutar (error PORCENTUAL)\n");
		printf("0) Salir\n");
		printf("Opcion: ");
		if (scanf("%d", &opcion) != 1) return 0;
		
		switch (opcion) {
		case 0:
			printf("Fin.\n");
			return 0;
			
		case 1:
		case 2: {
			double tol, x0;
			printf("Ingrese tolerancia: ");
			scanf("%lf", &tol);
			printf("Ingrese x_inicial: ");
			scanf("%lf", &x0);
			
			if (opcion == 1) punto_fijo_error_absoluto(tol, x0);
			else             punto_fijo_error_porcentual(tol, x0);
			break;
		}
		
		default:
			printf("Opcion invalida.\n");
		}
	} while (1);
}






/* ================= IMPLEMENTACIONES ================= */

/* Punto Fijo con error absoluto: |x_n - x_{n-1}| */
int punto_fijo_error_absoluto(double tolerancia, double x_inicial) {
	double x_prev = x_inicial;
	double x_actual  = g(x_prev);
	double err    = fabs(x_actual - x_prev);
	int    it     = 0;
	
	printf("\nIter |         x_n        |        g(x_n)       |    |?x|\n");
	printf("-----+--------------------+---------------------+-----------------\n");
	
	do {
		it++;
		
		/* Chequeo de convergencia local: |g'(x)| < 1 */
		double derivada_aprox = fabs(gp(x_prev));
		if (derivada_aprox >= 1.0) {
			printf("El metodo no converge localmente: |g'(x)| >= 1 en x = %.12lf\n", x_prev);
			printf("|g'(x)| = %.12lf\n", derivada_aprox);
			printf("Iteraciones realizadas: %d\n", it);
			return 0;
		}
		
		x_actual = g(x_prev);
		err   = fabs(x_actual - x_prev);
		
		printf("%4d | %18.12lf | %19.12lf | %13.6e\n", it, x_prev, x_actual, err);
		
		x_prev = x_actual;
		
	} while (err > tolerancia);
	
	printf("\nAproximacion: x* = %.12lf\n", x_actual);
	printf("Error absoluto final: %.6e\n", err);
	printf("Iteraciones: %d\n", it);
	return 1;
}

/* Punto Fijo con error porcentual: |x_n - x_{n-1}|/|x_n| * 100 */
int punto_fijo_error_porcentual(double tolerancia, double x_inicial)
{
	
	double x_anterior = x_inicial;    
	double x_actual;                  
	double error_porcentual;          
	int    iteraciones = 0;
	
	
	x_actual = g(x_anterior);
	
	
	if (fabs(x_actual) > 1e-15) {
		error_porcentual = fabs((x_actual - x_anterior) / x_actual) * 100.0;
	} else {
		/* Si x_actual es ~0, evitamos división por ~0 y usamos el numerador */
		error_porcentual = fabs(x_actual - x_anterior) * 100.0;
	}
	
	/* Iteración principal */
	while (error_porcentual > tolerancia) {
		iteraciones++;
		
		/* Chequeo de convergencia local: |g'(x)| < 1 en el punto actual */
		double derivada_aprox = fabs(gp(x_anterior));
		if (derivada_aprox >= 1.0) {
			printf("El metodo no converge localmente: |g'(x)| >= 1 en x = %.12lf\n", x_anterior);
			printf("|g'(x)| = %.12lf\n", derivada_aprox);
			printf("Iteraciones realizadas: %d\n", iteraciones);
			return 0;
		}
		
		x_actual   = g(x_anterior);
		
		/* Recalcular error porcentual, sin ternarios */
		if (fabs(x_actual) > 1e-15) {
			error_porcentual = fabs((x_actual - x_anterior) / x_actual) * 100.0;
		} else {
			error_porcentual = fabs(x_actual - x_anterior) * 100.0;
		}
		
		printf("%4d | %18.12lf | %19.12lf | %13.6e\n", iteraciones, x_anterior, x_actual, error_porcentual);
		
		x_anterior = x_actual;
	}

	printf("La raiz aproximada (punto fijo) es %.12lf\n", x_actual);
	printf("Error porcentual = %.12lf %%\n", error_porcentual);
	printf("Iteraciones: %d\n", iteraciones);
	return 1;
}
