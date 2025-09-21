/* =======================================================================
newton_raphson.c
- Método de Newton–Raphson con dos criterios: error absoluto / porcentual
======================================================================= */

#include <stdio.h>
#include <math.h>


/* f(x): función cuyo cero queremos encontrar */								//Editar Funcion
double Funcion(double x) {
	
	return x*x*x - 7.0*x + 1.0;
}
/* Derivada numérica de f */													//Puede ser que pidan la derivda a mano
double Derivada(double x) {
	const double h = 1e-4;
	return (Funcion(x + h) - Funcion(x)) / h;
}






/* --------- Prototipos --------- */
int newton_raphson_error_absoluto(double tolerancia, double x_inicial);
int newton_raphson_error_porcentual(double tolerancia, double x_inicial);







/* ======================= MAIN ======================= */
int main(void) {
	int opcion = 0;
	
	do {
		printf("\n========== MENU (Newton_Raphson) ==========\n");
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
			
			if (opcion == 1) newton_raphson_error_absoluto(tol, x0);
			else             newton_raphson_error_porcentual(tol, x0);
			break;
		}
		
		default:
			printf("Opcion invalida.\n");
		}
	} while (1);
}





/* ================= IMPLEMENTACIONES ================= */

/* Newton con error absoluto: |x_n - x_{n-1}| */
int newton_raphson_error_absoluto(double tolerancia, double x_inicial) {
	double x_prev = x_inicial;
	double x_actual;
	double err;
	int    it = 0;
	int it_maximas = 10000;
	
	printf("\nIter |         x_n        |        f(x_n)       |       f'(x_n)      |   |?x|\n");
	printf("-----+--------------------+---------------------+---------------------+-----------------\n");
	
	do {
		it++;
		
		
		if(it >= it_maximas){
			
			printf("Se alcanzo el numero maximo de 10.000 iteraciones, derivada en el denominador muy pequeña\n Intente probar con otros valores de X\n");
			return 0;
		}
		
		//																		Verifica si la derivada en el punto se acerca a cero (pueden pedirnos cambiar la tolerancia)
		double d = Derivada(x_prev);
		if (fabs(d) < 1e-4) {
			printf("Derivada ~ 0 en x = %.12lf. No se puede continuar.\n", x_prev);
			return 0;
		}
		
		//Calculo principal
		x_actual = x_prev - Funcion(x_prev) / d;
		
		
		err   = fabs(x_actual - x_prev);
		
		printf("%4d | %18.12lf | %19.12lf | %19.12lf | %13.6e\n", it, x_prev, Funcion(x_prev), d, err);
		
		x_prev = x_actual;
		
	} while (err > tolerancia);
	
	printf("\nAproximacion: x (la raiz) = %.12lf\n", x_actual);							//CUIDADO puede ser que haya que cambiar los descimales
	printf("f(x*): %.12e\n", Funcion(x_actual));
	printf("Error absoluto final: %.6e\n", err);
	printf("Iteraciones: %d\n", it);
	return 1;
}




/* Newton con error porcentual: |x_n - x_{n-1}|/|x_n| * 100 */
int newton_raphson_error_porcentual(double tolerancia, double x_inicial) {
	double x_prev = x_inicial;
	double x_actual;
	double errp;
	int    it = 0;
	int it_maximas = 10000;
	
	printf("\nIter |         x_n        |        f(x_n)       |       f'(x_n)      |  err %%\n");
	printf("-----+--------------------+---------------------+---------------------+-----------------\n");
	
	do {
		it++;
		
		if(it >= it_maximas){
			
			printf("Se alcanzo el numero maximo de 10.000 iteraciones, derivada en el denominador muy pequeña\n Intente probar con otros valores de X\n");
				return 0;
		}
		
		//																		Verifica si la derivada en el punto se acerca a cero (pueden pedirnos cambiar la tolerancia)
		double d = Derivada(x_prev);
		if (fabs(d) < 1e-10) {
			printf("Derivada ~ 0 en x = %.12lf. No se puede continuar.\n", x_prev);
			return 0;
		}
		
		
		//Calculo Principal
		x_actual = x_prev - Funcion(x_prev) / d;
		
		/* Cálculo del error porcentual */
		if (fabs(x_actual) > 1e-15) {
			errp = fabs((x_actual - x_prev) / x_actual) * 100.0;
		} else {
			/* Evita división por ~0 cuando x_actual es muy pequeño */
			errp = fabs(x_actual - x_prev) * 100.0;
		}
		
		
		printf("%4d | %18.12lf | %19.12lf | %19.12lf | %11.6e\n",  it, x_prev, Funcion(x_prev), d, errp);
		
		x_prev = x_actual;
		
	} while (errp > tolerancia);
	
	printf("\nAproximacion: x* = %.12lf\n", x_actual);							//CUIDADO puede ser que haya que cambiar los descimales
	printf("f(x*): %.12e\n", Funcion(x_actual));
	printf("Error porcentual final: %.6e\n", errp);
	printf("Iteraciones: %d\n", it);
	return 1;
}
