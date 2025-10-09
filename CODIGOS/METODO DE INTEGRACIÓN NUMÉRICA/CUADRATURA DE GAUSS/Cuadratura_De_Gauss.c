/* =======================================================================
Cuadratura de Gauss-Legendre (2 a 6 puntos)
- Integracion numerica de f(x) en [a, b].
- Muestra traza de cada contribucion y la cantidad de iteraciones.
======================================================================= */

#include <stdio.h>
#include <math.h>



/* ----------------- Definir f(x) segun ejercicio ----------------- */
double f(double x) {
	/* 																			cambiar Funncion segun el problema */
	return x*x*x+2*x*x+x+1;
}




/* --------------------------- Prototipos -------------------------- */
int cargar_nodos_y_pesos(int m, double x[], double c[]);
double gauss_legendre_integrar(double a, double b, int m, int mostrar_traza);
void imprimir_advertencias_rango_puntos(int m);




/* ============================== MAIN ============================= */
int main(void) {
	int opcion;
	
	while (1) {
		printf("\n========== MENU (Cuadratura de Gauss-Legendre) ==========\n");
		printf("1) Ejecutar Gauss-Legendre (2 a 6 puntos)\n");
		printf("0) Salir\n");
		printf("Opcion: ");
		if (scanf("%d", &opcion) != 1) return 0;
		
		if (opcion == 0) {
			printf("Fin.\n");
			return 0;
		} else if (opcion == 1) {
			double a, b;
			int cant_puntos;
			int mostrar_traza;
			
			printf("---------------------------------------------------------------\n");
			printf("Metodo: Gauss-Legendre\n");
			printf("---------------------------------------------------------------\n");
			
			printf("Ingrese limite inferior a: ");
			scanf("%lf", &a);
			printf("Ingrese limite superior b: ");
			scanf("%lf", &b);
			
			printf("Ingrese cantidad de puntos (entre 2 y 6): ");
			scanf("%d", &cant_puntos);
			
			if (cant_puntos < 2 || cant_puntos > 6) {
				printf("Advertencia: la cantidad de puntos debe estar entre 2 y 6.\n");
				printf("Valores validos: 2, 3, 4, 5 o 6.\n");
			}
			
			printf("Mostrar traza de iteraciones? 1=si, 0=no: ");
			scanf("%d", &mostrar_traza);
			
			if (cant_puntos < 2 || cant_puntos > 6) {
				printf("No se ejecuta: cantidad de puntos fuera de rango [2..6].\n");
				continue;
			}
			if (a == b) {
				printf("Aviso: a y b son iguales. La integral exacta es 0.\n");
			}
			
			/*signo */
			int signo = 1;
			if (a > b) {
				double t = a; a = b; b = t;
				signo = -1;
				printf("Aviso: a > b, se intercambian los limites y se ajusta el signo.\n");
			}
			
			double I = gauss_legendre_integrar(a, b, cant_puntos, mostrar_traza);
			I = I * (double)signo;
			
			printf("---------------------------------------------------------------\n");
			printf("Resultado (Gauss-Legendre):\n");
			printf("  a = %.12lf\n", a);
			printf("  b = %.12lf\n", b);
			printf("  puntos = %d\n", cant_puntos);
			printf("  Integral aproximada = %.12lf\n", I);
			printf("---------------------------------------------------------------\n");
		} else {
			printf("Opcion invalida.\n");
		}
	}
}





/* ===================== Implementaciones ========================= */
int cargar_nodos_y_pesos(int cant_puntos, double x[], double c[]) {					//Valores de la tabla
	if (cant_puntos == 2) {
		x[0] = -0.577350269;  x[1] =  0.577350269;
		c[0] =  1.0000000;    c[1] =  1.0000000;
		return 1;
	}
	if (cant_puntos == 3) {
		x[0] = -0.774596669;  x[1] =  0;             x[2] =  0.774596669;
		c[0] =  0.5555556;    c[1] =  0.8888889;     c[2] =  0.5555556;
		return 1;
	}
	if (cant_puntos == 4) {
		x[0] = -0.861136312;  x[1] = -0.339981044; x[2] =  0.339981044;  x[3] =  0.861136312;
		c[0] =  0.3478548;    c[1] =  0.6521452; c[2] =  0.6521452;    c[3] =  0.3478548;
		return 1;
	}
	if (cant_puntos == 5) {
		x[0] = -0.906179846;  x[1] = -0.538469310;  x[2] =  0; x[3] =  0.538469310;  x[4] =  0.906179846;
		c[0] =  0.2369269;    c[1] =  0.4786287;    c[2] =  0.5688889; c[3] =  0.4786287;    c[4] =  0.2369269;
		return 1;
	}
	if (cant_puntos == 6) {
		x[0] = -0.932469514;  x[1] = -0.661209386;  x[2] = -0.238619186; x[3] =  0.238619186;  x[4] =  0.661209386;  x[5] =  0.932469514;
		c[0] =  0.1713245;    c[1] =  0.3607616;    c[2] =  0.4679139; c[3] =  0.4679139;    c[4] =  0.3607616;    c[5] =  0.1713245;
		return 1;
	}
	return 0;
}





double gauss_legendre_integrar(double a, double b, int cant_puntos, int mostrar_traza) {	//Funcion principal
	double x[6], c[6];
	int ok = cargar_nodos_y_pesos(cant_puntos, x, c);
	if (!ok) return 0.0;
	
	double factor = (b - a) / 2.0;
	double centro = (b + a) / 2.0;
	
	double suma_aportes = 0.0;
	
	if (mostrar_traza == 1) {
		printf("%-12s %-15s %-15s %-15s %-15s %-18s %-18s\n", "iteracion", "x_k", "t_k", "f(t_k)", "c_k", "aporte c_k*f(t_k)", "suma_parcial");
		printf("-----------------------------------------------------------------------------------------------\n");
	}
	
	for (int k = 0; k < cant_puntos; k = k + 1) {								//Lo que va adentro de la funcion
		double xk = x[k];
		double ck = c[k];
		double tk = factor * xk + centro;
		double ftk = f(tk);														//Funcion evaluada en lo de adentro
		double aporte = ck * ftk;												//Funcion multiplicada por su Cn
		
		suma_aportes = suma_aportes + aporte;
		
		double integral_parcial = factor * suma_aportes;						//Integral de cada iteracion (no es el resultado final)
		
		if (mostrar_traza == 1) {
			printf("%-12d %-15.12lf %-15.12lf %-15.12lf %-15.12lf %-18.12lf %-18.12lf\n",  k + 1, xk, tk, ftk, ck, aporte, integral_parcial);
		}
	}
	
	return factor * suma_aportes;												//Resultado final (multiplicacion final)
}


