/* =======================================================================
secante.c
- Método de la Secante con dos criterios: error absoluto / porcentual
======================================================================= */

#include <stdio.h>
#include <math.h>

/* f(x): función cuya raiz queremos encontrar */								//Modificar funcion
double Funcion(double x) {
	
	return x*x*x - 7.0*x + 1.0;													//No se usa la derivada
}
/* ============================================================= */





/* --------- Prototipos --------- */
int secante_error_absoluto(double tolerancia, double x0_inicial, double x1_inicial);
int secante_error_porcentual(double tolerancia, double x0_inicial, double x1_inicial);







/* ======================= MAIN ======================= */
int main(void) {
	int opcion;
	
	do {
		printf("\n========== MENU (Secante) ==========\n");
		printf("1) Ejecutar (error ABSOLUTO)\n");								//De base se usa el absoluto
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
			double tol, x0, x1;
			printf("Ingrese tolerancia: ");
			scanf("%lf", &tol);
			printf("Ingrese x0 (primer aproximacion): ");
			scanf("%lf", &x0);
			printf("Ingrese x1 (segunda aproximacion): ");
			scanf("%lf", &x1);
			
			if (opcion == 1) secante_error_absoluto(tol, x0, x1);
			else             secante_error_porcentual(tol, x0, x1);
			break;
		}
		
		default:
			printf("Opcion invalida.\n");
		}
	} while (1);
}






/* ================= IMPLEMENTACIONES ================= */

/* Secante con error absoluto: |x_n - x_{n-1}| */
int secante_error_absoluto(double tolerancia, double x0, double x1) {			//Normalmente usamos esta
	double x_prev = x0;
	double x_actual  = x1;
	double x_nuevo, err;
	int    it = 0;
	int it_maximas = 10000;
	
	printf("\nIter |        x_{n-1}     |        x_n         |        x_{n+1}     |   |?x|\n");
	printf("-----+---------------------+--------------------+---------------------+-----------------\n");
	
	do {
		it++;
		
		
		if(it >= it_maximas){
			
			printf("Se alcanzo el numero maximo de iteraciones\n Intente probar con otros valores de x\n");
			return 0;
		}
		
		double f0 = Funcion(x_prev);
		double f1 = Funcion(x_actual);
		
		if (f1 == f0) {
			printf("Denominador cero (f(x_n) = f(x_{n-1})). No se puede continuar.\n");
			return 0;
		}
		
		x_nuevo = x_actual - f1 * (x_actual - x_prev) / (f1 - f0);
		
		err   = fabs(x_nuevo - x_actual);
		
		printf("%4d | %19.12lf | %18.12lf | %19.12lf | %13.6e\n", it, x_prev, x_actual, x_nuevo, err);
		
		x_prev = x_actual;
		x_actual  = x_nuevo;
		
	} while (err > tolerancia);
	
	printf("\nAproximacion: x* = %.12lf\n", x_actual);							//CUIDADO puden cambiar los decimales
	printf("f(x*): %.12e\n", Funcion(x_actual));
	printf("Error absoluto final: %.6e\n", err);
	printf("Iteraciones: %d\n", it);
	return 1;
}


/* Secante con error porcentual: |x_n - x_{n-1}|/|x_n| * 100 */
int secante_error_porcentual(double tolerancia, double x0, double x1) {
	double x_prev = x0;
	double x_actual  = x1;
	double x_nuevo, errp;
	int    it = 0;
	
	printf("\nIter |        x_{n-1}     |        x_n         |        x_{n+1}     |  err %%\n");
	printf("-----+---------------------+--------------------+---------------------+-----------------\n");
	
	do {
		it++;
		
		
		if(it >= it_maximas){
			
			printf("Se alcanzo el numero maximo de iteraciones\n Intente probar con otros valores de x\n");
			return 0;
		}
		
		
		double f0 = Funcion(x_prev);
		double f1 = Funcion(x_actual);
		
		
		if (f1 == f0) {
			printf("Denominador cero (f(x_n) = f(x_{n-1})). No se puede continuar.\n");
			return 0;
		}
		
		x_nuevo = x_actual - f1 * (x_actual - x_prev) / (f1 - f0);
		
		
		
		if (fabs(x_nuevo) > 1e-15) {
			errp = fabs((x_nuevo - x_actual) / x_nuevo) * 100.0;
		} else {
			/* evita dividir por un valor ~0 */
			errp = fabs(x_nuevo - x_actual) * 100.0;
		}
		
		
		printf("%4d | %19.12lf | %18.12lf | %19.12lf | %11.6e\n", it, x_prev, x_actual, x_nuevo, errp);
		
		x_prev = x_actual;
		x_actual  = x_nuevo;
		
	} while (errp > tolerancia);
	
	printf("\nAproximacion: x* = %.12lf\n", x_actual);							////CUIDADO puden cambiar los decimales
	printf("f(x*): %.12e\n", Funcion(x_actual));
	printf("Error porcentual final: %.6e\n", errp);
	printf("Iteraciones: %d\n", it);
	return 1;
}
