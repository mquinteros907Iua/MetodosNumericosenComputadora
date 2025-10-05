/* =======================================================================
Que hace:
Aproxima la integral de f(x) en [a, b] dividiendo el intervalo en n
subintervalos de ancho h = (b - a)/n y sumando el area de trapecios.

Formula:
Suma = f(a) + f(b) + 2 * sum_{i=1}^{n-1} f(a + i*h)
Integral approx = (b - a) * Suma / (2*n)   (equivale a Integral approx (h/2) * Suma)

Error teorico (estimado para trapecio compuesto):
E approx ((b - a)^2 / (12*n^2)) * (f'(b) - f'(a))
donde f'(x) se aproxima numericamente o se usa su derivada analitica si se conoce.
======================================================================= */

#include <stdio.h>
#include <math.h>




/* =================== CONFIGURAR INTEGRAL EXACTA (OPCIONAL) ==================
Si el ejercicio exige comparar con la INTEGRAL EXACTA, active y complete esto:

1) Cambie usar_integral_exacta a 1.
2) Ponga en integral_exacta_el_valor el resultado exacto (hecho a mano) de
la integral de f(x) entre a y b.

Si deja usar_integral_exacta = 0, el programa seguira mostrando los errores
absoluto y porcentual basados en las iteraciones (NO exactos).
============================================================================= */
static int usar_integral_exacta = 0;      											/* 0 = por defecto (no usar exacta), 1 = usar exacta */
static double integral_exacta_el_valor = 0;  										/* coloque aqui el valor exacto cuando usar_integral_exacta = 1 */
/* ========================================================================== */







/* ----------------- DEFINICION DE LA FUNCION A INTEGRAR ----------------- */
/*																				 f(x): editar segun el ejercicio */
double f(double x) {
	return (x*x) + 1;   /* ejemplo */
}

/* 																					IMPORTANTE:
Si el ejercicio provee f'(x) ANALITICA, puede reemplazar esta funcion por	
la derivada real. Ahora se usa una APROXIMACION por diferencia HACIA ADELANTE:
f'(x) ~ (f(x + h) - f(x)) / h, con h = 0.001. */
double derivada_numerica(double x) {
	double h = 0.001;
	double d = (f(x + h) - f(x)) / h;
	return d;
}





/* ------------------------------ PROTOTIPOS ----------------------------- */
double trapecio_compuesto(double a, double b, int n, int mostrar_traza, double *error_abs_final_iter, double *error_pct_final_iter);









/* ================================= MAIN ================================ */
int main(void) {
	int opcion;
	
	while (1) {
		printf("\n========== MENU (Trapecio Compuesto) ==========\n");
		printf("1) Ejecutar trapecio compuesto\n");
		printf("0) Salir\n");
		printf("Opcion: ");
		if (scanf("%d", &opcion) != 1) {
			return 0;
		}
		
		switch (opcion) {
		case 0: {
			printf("Fin.\n");
			return 0;
		}
		
		case 1: {
			double a, b;
			int n;
			int mostrar_traza;
			
			printf("---------------------------------------------------------------\n");
			printf("Metodo: Trapecio compuesto (por funcion)\n");
			printf("---------------------------------------------------------------\n");
			
			printf("Ingrese limite inferior a: ");
			scanf("%lf", &a);
			
			printf("Ingrese limite superior b: ");
			scanf("%lf", &b);
			
			printf("Ingrese numero de subintervalos n (n >= 1): ");
			scanf("%d", &n);
			
			printf("Mostrar traza de iteraciones? 1=si, 0=no: ");
			scanf("%d", &mostrar_traza);
			
			printf("---------------------------------------------------------------\n");
			printf("ADVERTENCIA: los errores absoluto y porcentual impresos se\n");
			printf("calculan SIN usar la integral exacta. Si el ejercicio exige\n");
			printf("comparar con la integral exacta, edite la seccion\n");
			printf("\"CONFIGURAR INTEGRAL EXACTA (OPCIONAL)\" al principio de este\n");
			printf("archivo: ponga usar_integral_exacta = 1 y asigne integral_exacta_el_valor.\n");
			printf("---------------------------------------------------------------\n");
			
			if (n < 1) {
				printf("ADVERTENCIA: n es menor que 1. Debe elegir n >= 1.\n");
				printf("No se ejecuta el metodo.\n");
				printf("---------------------------------------------------------------\n");
				break;
			}
			if (a == b) {
				printf("ADVERTENCIA: a y b son iguales (intervalo de longitud cero).\n");
				printf("La integral exacta en este caso es 0. El metodo devolvera 0.\n");
			}
			if (a > b) {
				printf("AVISO: a es mayor que b. Se intercambian los limites\n");
				printf("para realizar el calculo en [b, a] y luego se ajusta el signo.\n");
			}
			
			double error_abs_final_iter = 0.0;
			double error_pct_final_iter = 0.0;
			double Integral = trapecio_compuesto(a, b, n, mostrar_traza, &error_abs_final_iter, &error_pct_final_iter);
			
			printf("---------------------------------------------------------------\n");
			
			
			/* Error teorico del metodo  */
			/* E approx ((b - a)^2 / (12*n^2)) * (f'(b) - f'(a)) */
			double fp_a = derivada_numerica(a);
			double fp_b = derivada_numerica(b);
			double error_teorico_con_signo = ((b - a) * (b - a)) / (12.0 * (double)n * (double)n) * (fp_b - fp_a); 					/* ERROR DEL METODO */
			
			
			/* ========= RESULTADOS Y  ERRORES  ========= */
			double h = (b - a) / (double)n;
			printf("Resultado (Trapecio compuesto):\n");
			printf("  a = %.12g\n", a);
			printf("  b = %.12g\n", b);
			printf("  n = %d\n", n);
			printf("  h = (b - a) / n = %.12g\n", h);
			printf("  Integral aproximada = %.12g\n", Integral);
			printf("Error del metodo (E) = %.12lf\n", error_teorico_con_signo);
			
			
			printf("---------------------------------------------------------------\n");
			printf("Errores basados en iteraciones (respecto a la anterior):\n");
			printf("  Error absoluto final (iter)    = %.12lf\n", error_abs_final_iter);
			printf("  Error porcentual final (iter)  = %.12lf %%\n", error_pct_final_iter);
			
			/* Error vs integral exacta */
			if (usar_integral_exacta == 1) {
				double error_abs_exacta = fabs(Integral - integral_exacta_el_valor);
				double error_pct_exacta;
				if (integral_exacta_el_valor != 0.0) {
					error_pct_exacta = fabs((Integral - integral_exacta_el_valor) / integral_exacta_el_valor) * 100.0;
				} else {
					error_pct_exacta = 0.0;
				}
				printf("Errores contra INTEGRAL EXACTA (:\n");
				printf("  Integral exacta = %.12g\n", integral_exacta_el_valor);
				printf("  Error absoluto exacto          = %.12lf\n", error_abs_exacta);
				printf("  Error porcentual exacto        = %.12lf %%\n", error_pct_exacta);
			} else {
				printf("Nota: no se configuro integral exacta (usar_integral_exacta = 0).\n");
			}
			
			
			break;
		}
		
		default: {
			printf("Opcion invalida.\n");
			break;
		}
		}
	}
}







/* ======================= IMPLEMENTACION DEL METODO ====================== */
double trapecio_compuesto(double a, double b, int n, int mostrar_traza, double *error_abs_final_iter, double *error_pct_final_iter) {
	
	if (n < 1) {
		if (error_abs_final_iter) { *error_abs_final_iter = 0.0; }
		if (error_pct_final_iter) { *error_pct_final_iter = 0.0; }
		return 0.0;
	}
	
	if (a == b) {
		if (mostrar_traza == 1) {
			printf("---------------------------------------------------------------\n");
			printf("Traza: a == b, no hay puntos internos y la integral vale 0.\n");
			printf("---------------------------------------------------------------\n");
		}
		if (error_abs_final_iter) { *error_abs_final_iter = 0.0; }
		if (error_pct_final_iter) { *error_pct_final_iter = 0.0; }
		return 0.0;
	}
	
	/* Ajuste de limites y signo */
	int signo = 1;
	if (a > b) {
		double t = a; a = b; b = t;
		signo = -1;
	}
	
	double h = (b - a) / (double)n;  											/* Distancia entre 2 puntos equidistantes */
	double suma = f(a) + f(b);       											/* Suma de los extremos */
	
	/* Variables para errores por iteracion */
	double Integral_parcial_anterior = 0.0;
	int hay_integral_anterior = 0;
	
	if (mostrar_traza == 1) {
		printf("f(a) = f(%.10g) = %.10g\n", a, f(a));
		printf("f(b) = f(%.10g) = %.10g\n", b, f(b));
		printf("---------------------------------------------------------------\n");
		printf("%-12s %-15s %-18s %-18s %-18s %-14s %-14s\n", "iteraciones", "x_i", "f(x_i)", "suma_parcial", "Integral_parcial", "err_abs", "err_pct(%)");
		printf("---------------------------------------------------------------\n");
	}
	
	/* Bucle por puntos internos */
	for (int i = 1; i <= n - 1; i = i + 1) {
		
		double x_i = a + i * h;
		double fx_i = f(x_i);
		
		suma = suma + 2.0 * fx_i;
		
		/* Integral parcial con la suma actual */
		double Integral_parcial = ((b - a) * suma) / (2.0 * (double)n);
		
		if (mostrar_traza == 1) {
			
			if (hay_integral_anterior == 0) {
				
				printf("%-12d %-15.8g %-18.8g %-18.8g %-18.8g %-14s %-14s\n", i, x_i, fx_i, suma, Integral_parcial, "-", "-");
				
			} else {
				
				/* ====== CALCULO DE ERRORES ====== */
				double error_abs = fabs(Integral_parcial - Integral_parcial_anterior); 												/* Error Absoluto por cada iteracion */
				
				double error_pct;
				if (Integral_parcial != 0.0) {
					error_pct = fabs((Integral_parcial - Integral_parcial_anterior) / Integral_parcial) * 100.0; 					/* Error porcentual */
				} else {
					error_pct = 0.0;
				}
				/* ========================================== */
				
				printf("%-12d %-15.8g %-18.8g %-18.8g %-18.8g %-14.12lf %-14.12lf\n", i, x_i, fx_i, suma, Integral_parcial, error_abs, error_pct);
				
				if (error_abs_final_iter) {
					*error_abs_final_iter = error_abs;
				}
				if (error_pct_final_iter) {
					*error_pct_final_iter = error_pct;
				}
			}
			
		} else {
			
			if (hay_integral_anterior == 1) {
				
				/* ====== CALCULO DE ERRORES ====== */
				double error_abs = fabs(Integral_parcial - Integral_parcial_anterior); 																	/* Error Absoluto */
				
				double error_pct;
				if (Integral_parcial != 0.0) {
					error_pct = fabs((Integral_parcial - Integral_parcial_anterior) / Integral_parcial) * 100.0; 										/* Error Porcentual */
				} else {
					error_pct = 0.0;
				}
				/* ========================================== */
				
				if (error_abs_final_iter) {
					*error_abs_final_iter = error_abs;
				}
				if (error_pct_final_iter) {
					*error_pct_final_iter = error_pct;
				}
			}
		}
		
		Integral_parcial_anterior = Integral_parcial;
		hay_integral_anterior = 1;
	}
	
	/* Integral final y ajuste de signo */
	double Integral = ((b - a) * suma) / (2.0 * (double)n);
	Integral = Integral * signo;
	
	return Integral;
}
