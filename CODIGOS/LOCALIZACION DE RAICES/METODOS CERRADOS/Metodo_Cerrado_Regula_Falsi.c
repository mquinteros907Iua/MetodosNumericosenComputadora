/* =========================================================================
regla_falsa.c
- Método de Regla Falsa para localizar raíces de f(x)
- Dos criterios de parada: error absoluto y error porcentual
========================================================================= */

#include <stdio.h>
#include <math.h>

/* --------- Declaración de la función f(x) ---------
//																				Editár esta función para cambiar el problema. */
double Funcion(double x) {

	return x*x*x - 7.0*x + 1.0;
}



/* --------- Prototipos --------- */
int regla_falsa_error_absoluto(double tolerancia, double extremo_izq, double extremo_der);
int regla_falsa_error_porcentual(double tolerancia, double extremo_izq, double extremo_der);

/* Chequeo de cambio de signo en [a,b] */
static int hay_cambio_de_signo(double a, double b) {
	return (Funcion(a) * Funcion(b) <= 0.0);
}




/* ======================= MAIN ======================= */
int main(void) {
	int opcion_menu;
	
	do {
		printf("\n========== MENU (Regla Falsa) ==========\n");
		printf("1) Ejecutar regla falsa (error ABSOLUTO)\n");
		printf("2) Ejecutar regla falsa (error PORCENTUAL)\n");
		printf("0) Salir\n");
		printf("Opcion: ");
		if (scanf("%d", &opcion_menu) != 1) return 0;
		
		switch (opcion_menu) {
		case 0:
			printf("Fin.\n");
			return 0;
			
		case 1:
		case 2: {
			double tolerancia, a, b;
			
			printf("Ingrese la tolerancia: ");
			scanf("%lf", &tolerancia);
			
			printf("Ingrese extremo izquierdo (a): ");
			scanf("%lf", &a);
			
			printf("Ingrese extremo derecho (b): ");
			scanf("%lf", &b);
			
			
			// 																	Verifica que alguno de los 2 valores evaludos sea negaitvo (condicion necesaria) si no (los 2 son positovos o los 2 negativos) finaliza el programa
			if (!hay_cambio_de_signo(a, b)) {
				printf("\nNo hay cambio de signo en [a,b]. " "Elija otro intervalo.\n");
				break;
			}
			
			
			if (opcion_menu == 1)
				regla_falsa_error_absoluto(tolerancia, a, b);
			else
				regla_falsa_error_porcentual(tolerancia, a, b);
			
			break;
		}
		
		default:
			printf("Opcion invalida.\n");
		}
	} while (1);
	
	return 0;
}





/* ================== IMPLEMENTACIONES ================== */

/* c = a*f(b) - b*f(a) / ( f(b) - f(a) ) */ 									//Ecuacion de la recta entre 2 puntos 
static inline double interseccion_recta(double a, double b) {
	double fa = Funcion(a), fb = Funcion(b);
	return (a*fb - b*fa) / (fb - fa);
}

/* ---- Regla Falsa con error absoluto estimado ----
error_abs = |c_n - c_{n-1}|                              */
int regla_falsa_error_absoluto(double tolerancia, double a, double b) {
	double c_anterior = a;  
	double c_actual   = interseccion_recta(a, b);								//Asigna la recta de la funciona anerior
	double error_abs  = fabs(c_actual - c_anterior);
	int    iter       = 0;
	
	printf("\nIter |        a        |        b        |        c        |   f(c)\n");
	printf("-----+-----------------+-----------------+-----------------+-----------------\n");
	
	do {
		iter++;
		c_actual = interseccion_recta(a, b);
		
		printf("%4d | %15.8lf | %15.8lf | %15.8lf | %15.8lf\n", iter, a, b, c_actual, Funcion(c_actual));
		
		
		// 																		La raiz es exacta
		if (Funcion(c_actual) == 0.0) {
			error_abs = 0.0;
			break;
		}
		
		/* Selección del subintervalo con cambio de signo */
		if (Funcion(a) * Funcion(c_actual) < 0.0)
			b = c_actual;
		else					//F(a) * F(c) > 0
			a = c_actual;
		
		error_abs  = fabs(c_actual - c_anterior);
		c_anterior = c_actual;
		
	} while (error_abs > tolerancia);
	
	printf("\nRaiz aproximada: %.12lf\n", c_actual);							//Cuidado (Cambiar decimales)
	printf("Error absoluto:  %.12e\n", error_abs);
	printf("Iteraciones:     %d\n", iter);
	return 1;
}

/* ---- Regla Falsa con error porcentual estimado ----
error_% = |c_n - c_{n-1}| / |c_n| * 100                 */
int regla_falsa_error_porcentual(double tolerancia, double a, double b) {
	double c_anterior = a;
	double c_actual   = interseccion_recta(a, b);								//Ecuacion de la recta entre 2 puntos 
	double error_pct;
	int    iter       = 0;
	
	printf("\nIter |        a        |        b        |        c        |   f(c)\n");
	printf("-----+-----------------+-----------------+-----------------+-----------------\n");
	
	do {
		iter++;
		c_actual = interseccion_recta(a, b);
		
		printf("%4d | %15.8lf | %15.8lf | %15.8lf | %15.8lf\n", iter, a, b, c_actual, Funcion(c_actual));
		
		
		// 																		La raiz es exacta
		if (Funcion(c_actual) == 0.0) {
			error_pct = 0.0;
			break;
		}
	
		if (Funcion(a) * Funcion(c_actual) < 0.0)
			b = c_actual;
		else				//F(a) * F(c) > 0
			a = c_actual;
		
		//																		Evita la division por cero 0.14 ceros
		if (fabs(c_actual) > 1e-15)
			error_pct = fabs((c_actual - c_anterior) / c_actual) * 100.0;
		else
			error_pct = fabs(c_actual - c_anterior) * 100.0; /* Evita división por ~0 */
		
		c_anterior = c_actual;
		
	} while (error_pct > tolerancia);
	
	printf("\nRaiz aproximada: %.12lf\n", c_actual);							//Cuidado (Cambiar decimales)
	printf("Error %%:         %.12e\n", error_pct);
	printf("Iteraciones:     %d\n", iter);
	return 1;
}
