/* ================================================================
DIFERENCIACION NUMERICA (adelante / atras / centrada)
Derivadas de orden 1..4
Version: 1 = Menos Precisa , 2 = Mas precisa
================================================================ */

#include <stdio.h>
#include <math.h>

/* =================== CONFIGURACION DEL EJERCICIO ===================

ATENCION: MODIFICAR f(x) PARA CADA EJERCICIO.
SI SE DESEA MOSTRAR ERROR EXACTO PARA LA PRIMERA DERIVADA,
ACTIVAR usar_derivada_exacta = 1 Y EDITAR df1_exacta(x).

==================================================================== */

/* --- Funcion del ejercicio (EDITAR) --- */
double f(double x) {
	/*                                                                          CAMBIAR SEGUN EL EJERCICIO */
	return x*x*x;
}

/* --- Derivada exacta de ORDEN 1 --- */
int usar_derivada_exacta = 0;  /* 0 = no comparar; 1 = comparar */
double df1_exacta(double x) {
	/*                                                                          CAMBIAR SI ACTIVAS LA COMPARACION */
	return (6*x*x)-1;
}

/* ------------------ PROTOTIPOS ------------------ */
void calcular_x_equidistanciados(double a, double b, int n, double x[]);

/* Adelante (forward) */
void adelante_orden1(int version, double h, int n, const double x[], double y[]);
void adelante_orden2(int version, double h, int n, const double x[], double y[]);
void adelante_orden3(int version, double h, int n, const double x[], double y[]);
void adelante_orden4(int version, double h, int n, const double x[], double y[]);

/* Atras (backward) */
void atras_orden1(int version, double h, int n, const double x[], double y[]);
void atras_orden2(int version, double h, int n, const double x[], double y[]);
void atras_orden3(int version, double h, int n, const double x[], double y[]);
void atras_orden4(int version, double h, int n, const double x[], double y[]);

/* Centrada */
void centrada_orden1(int version, double h, int n, const double x[], double y[]);
void centrada_orden2(int version, double h, int n, const double x[], double y[]);
void centrada_orden3(int version, double h, int n, const double x[], double y[]);
void centrada_orden4(int version, double h, int n, const double x[], double y[]);

int calcular_diferencias(int esquema, int orden, int version, double a, double b, int n);

/* =============================== MAIN =============================== */
int main(void) {
	while (1) {
		int opcion;
		printf("\n===== DIFERENCIACION NUMERICA =====\n");
		printf("1) Calcular\n");
		printf("0) Salir\n");
		printf("Opcion: ");
		if (scanf("%d", &opcion) != 1) return 0;
		if (opcion == 0) {
			printf("Fin.\n");
			return 0;
		}
		if (opcion != 1) {
			printf("Opcion invalida.\n");
			continue;
		}
		
		double a, b;
		int n;
		int esquema;  /* 1 adelante, 2 atras, 3 centrada */
		int orden;    /* 1..4 */
		int version;  /* 1 Menos Precisa, 2 Mas precisa */
		
		if (usar_derivada_exacta)
			printf("*** COMPARACION ACTIVADA: df1_exacta(x) DEBE COINCIDIR CON EL EJERCICIO ***\n");
		
		printf("Ingrese el intervalo\n");
		printf("\nIngrese a: ");
		if (scanf("%lf", &a) != 1) return 0;
		printf("Ingrese b: ");
		if (scanf("%lf", &b) != 1) return 0;
		printf("Ingrese n (subintervalos, n >= 1): ");
		if (scanf("%d", &n) != 1) return 0;
		
		printf("Esquema (1=adelante, 2=atras, 3=centrada): ");
		if (scanf("%d", &esquema) != 1) return 0;
		printf("Orden de derivada (1..4): ");
		if (scanf("%d", &orden) != 1) return 0;
		printf("Version (1=Menos Precisa, 2=Mas precisa): ");
		if (scanf("%d", &version) != 1) return 0;
		
		if (a == b) {
			printf("Advertencia: a y b son iguales. h = 0 y no se puede calcular.\n");
			continue;
		}
		if (n < 1) {
			printf("n debe ser >= 1.\n");
			continue;
		}
		if (esquema < 1 || esquema > 3) {
			printf("Esquema invalido.\n");
			continue;
		}
		if (orden < 1 || orden > 4) {
			printf("Orden invalido.\n");
			continue;
		}
		if (version != 1 && version != 2) {
			printf("Version invalida.\n");
			continue;
		}
		
		calcular_diferencias(esquema, orden, version, a, b, n);
	}
}

/* ================== IMPLEMENTACIONES  ================== */

void calcular_x_equidistanciados(double a, double b, int n, double x[]) {
	double h = (b - a) / (double)n;
	for (int i = 0; i <= n; i = i + 1) x[i] = a + i * h;
}

/* ---------------- Adelante ---------------- */
void adelante_orden1(int version, double h, int n, const double x[], double y[]) {
	if (version == 1) {
		for (int i = 0; i <= n - 1; i = i + 1)
			y[i] = (f(x[i+1]) - f(x[i])) / h;
	} else {
		for (int i = 0; i <= n - 2; i = i + 1)
			y[i] = (-f(x[i+2]) + 4.0*f(x[i+1]) - 3.0*f(x[i])) / (2.0*h);
	}
}
void adelante_orden2(int version, double h, int n, const double x[], double y[]) {
	double h2 = h*h;
	if (version == 1) {
		for (int i = 0; i <= n - 2; i = i + 1)
			y[i] = (f(x[i+2]) - 2.0*f(x[i+1]) + f(x[i])) / h2;
	} else {
		for (int i = 0; i <= n - 3; i = i + 1)
			y[i] = (-f(x[i+3]) + 4.0*f(x[i+2]) - 5.0*f(x[i+1]) + 2.0*f(x[i])) / h2;
	}
}
void adelante_orden3(int version, double h, int n, const double x[], double y[]) {
	double h3 = h*h*h;
	if (version == 1) {
		for (int i = 0; i <= n - 3; i = i + 1)
			y[i] = (f(x[i+3]) - 3.0*f(x[i+2]) + 3.0*f(x[i+1]) - f(x[i])) / h3;
	} else {
		for (int i = 0; i <= n - 4; i = i + 1)
			y[i] = (-3.0*f(x[i+4]) + 14.0*f(x[i+3]) - 24.0*f(x[i+2]) +
					18.0*f(x[i+1]) - 5.0*f(x[i])) / (2.0*h3);
	}
}
void adelante_orden4(int version, double h, int n, const double x[], double y[]) {
	double h4 = h*h*h*h;
	if (version == 1) {
		for (int i = 0; i <= n - 4; i = i + 1)
			y[i] = (f(x[i+4]) - 4.0*f(x[i+3]) + 6.0*f(x[i+2]) - 4.0*f(x[i+1]) + f(x[i])) / h4;
	} else {
		for (int i = 0; i <= n - 5; i = i + 1)
			y[i] = (-2.0*f(x[i+5]) + 11.0*f(x[i+4]) - 24.0*f(x[i+3]) + 26.0*f(x[i+2])
					- 14.0*f(x[i+1]) + 3.0*f(x[i])) / h4;
	}
}

/* ---------------- Atras ---------------- */
void atras_orden1(int version, double h, int n, const double x[], double y[]) {
	if (version == 1) {
		for (int i = 1; i <= n; i = i + 1)
			y[i] = (f(x[i]) - f(x[i-1])) / h;
	} else {
		for (int i = 2; i <= n; i = i + 1)
			y[i] = (3.0*f(x[i]) - 4.0*f(x[i-1]) + f(x[i-2])) / (2.0*h);
	}
}
void atras_orden2(int version, double h, int n, const double x[], double y[]) {
	double h2 = h*h;
	if (version == 1) {
		for (int i = 2; i <= n; i = i + 1)
			y[i] = (f(x[i]) - 2.0*f(x[i-1]) + f(x[i-2])) / h2;
	} else {
		for (int i = 3; i <= n; i = i + 1)
			y[i] = (2.0*f(x[i]) - 5.0*f(x[i-1]) + 4.0*f(x[i-2]) - f(x[i-3])) / h2;
	}
}
void atras_orden3(int version, double h, int n, const double x[], double y[]) {
	double h3 = h*h*h;
	if (version == 1) {
		for (int i = 4; i <= n; i = i + 1)
			y[i] = (f(x[i]) - 3.0*f(x[i-1]) + 3.0*f(x[i-2]) - f(x[i-3])) / h3;
	} else {
		for (int i = 4; i <= n; i = i + 1)
			y[i] = (5.0*f(x[i]) - 18.0*f(x[i-1]) + 24.0*f(x[i-2]) -
					14.0*f(x[i-3]) + 3.0*f(x[i-4])) / (2.0*h3);
	}
}
void atras_orden4(int version, double h, int n, const double x[], double y[]) {
	double h4 = h*h*h*h;
	if (version == 1) {
		for (int i = 4; i <= n; i = i + 1)
			y[i] = (f(x[i]) - 4.0*f(x[i-1]) + 6.0*f(x[i-2]) - 4.0*f(x[i-3]) + f(x[i-4])) / h4;
	} else {
		for (int i = 5; i <= n; i = i + 1)
			y[i] = (3.0*f(x[i]) - 14.0*f(x[i-1]) + 26.0*f(x[i-2]) - 24.0*f(x[i-3]) +
					11.0*f(x[i-4]) - 2.0*f(x[i-5])) / h4;
	}
}

/* ---------------- Centrada ---------------- */
void centrada_orden1(int version, double h, int n, const double x[], double y[]) {
	if (version == 1) {
		for (int i = 1; i <= n - 1; i = i + 1)
			y[i] = (f(x[i+1]) - f(x[i-1])) / (2.0*h);
	} else {
		for (int i = 2; i <= n - 2; i = i + 1)
			y[i] = (-f(x[i+2]) + 8.0*f(x[i+1]) - 8.0*f(x[i-1]) + f(x[i-2])) / (12.0*h);
	}
}
void centrada_orden2(int version, double h, int n, const double x[], double y[]) {
	double h2 = h*h;
	if (version == 1) {
		for (int i = 1; i <= n - 1; i = i + 1)
			y[i] = (f(x[i+1]) - 2.0*f(x[i]) + f(x[i-1])) / h2;
	} else {
		for (int i = 2; i <= n - 2; i = i + 1)
			y[i] = (-f(x[i+2]) + 16.0*f(x[i+1]) - 30.0*f(x[i]) +
					16.0*f(x[i-1]) - f(x[i-2])) / (12.0*h2);
	}
}
void centrada_orden3(int version, double h, int n, const double x[], double y[]) {
	double h3 = h*h*h;
	if (version == 1) {
		for (int i = 2; i <= n - 2; i = i + 1)
			y[i] = (f(x[i+2]) - 2.0*f(x[i+1]) + 2.0*f(x[i-1]) - f(x[i-2])) / (2.0*h3);
	} else {
		for (int i = 3; i <= n - 3; i = i + 1)
			y[i] = (-f(x[i+3]) + 8.0*f(x[i+2]) - 13.0*f(x[i+1]) + 13.0*f(x[i-1])
					- 8.0*f(x[i-2]) + f(x[i-3])) / (8.0*h3);
	}
}
void centrada_orden4(int version, double h, int n, const double x[], double y[]) {
	double h4 = h*h*h*h;
	if (version == 1) {
		for (int i = 2; i <= n - 2; i = i + 1)
			y[i] = (f(x[i+2]) - 4.0*f(x[i+1]) + 6.0*f(x[i]) - 4.0*f(x[i-1]) + f(x[i-2])) / h4;
	} else {
		for (int i = 3; i <= n - 3; i = i + 1)
			y[i] = (-f(x[i+3]) + 12.0*f(x[i+2]) - 39.0*f(x[i+1]) + 56.0*f(x[i])
					- 39.0*f(x[i-1]) + 12.0*f(x[i-2]) - f(x[i-3])) / (6.0*h4);
	}
}

int calcular_diferencias(int esquema, int orden, int version,
						 double a, double b, int n) {
	const int MAXP = 2050;
	int puntos = n + 1;
	if (puntos > MAXP) {
		printf("Demasiados puntos para el arreglo interno.\n");
		return 0;
	}
	
	double h = (b - a) / (double)n;
	double x[MAXP];
	double y[MAXP];
	
	for (int i = 0; i < puntos; i = i + 1) y[i] = 0.0;
	calcular_x_equidistanciados(a, b, n, x);
	
	if (esquema == 1) {
		if      (orden == 1) adelante_orden1(version, h, n, x, y);
		else if (orden == 2) adelante_orden2(version, h, n, x, y);
		else if (orden == 3) adelante_orden3(version, h, n, x, y);
		else                 adelante_orden4(version, h, n, x, y);
	} else if (esquema == 2) {
		if      (orden == 1) atras_orden1(version, h, n, x, y);
		else if (orden == 2) atras_orden2(version, h, n, x, y);
		else if (orden == 3) atras_orden3(version, h, n, x, y);
		else                 atras_orden4(version, h, n, x, y);
	} else {
		if      (orden == 1) centrada_orden1(version, h, n, x, y);
		else if (orden == 2) centrada_orden2(version, h, n, x, y);
		else if (orden == 3) centrada_orden3(version, h, n, x, y);
		else                 centrada_orden4(version, h, n, x, y);
	}
	
	int i_min = 0, i_max = -1;
	if (esquema == 1) {                /* adelante */
		if      (orden == 1) { i_min = 0; i_max = (version==1)? n-1 : n-2; }
		else if (orden == 2) { i_min = 0; i_max = (version==1)? n-2 : n-3; }
		else if (orden == 3) { i_min = 0; i_max = (version==1)? n-3 : n-4; }
		else                 { i_min = 0; i_max = (version==1)? n-4 : n-5; }
	} else if (esquema == 2) {         /* atras */
		if      (orden == 1) { i_min = (version==1)? 1 : 2; i_max = n; }
		else if (orden == 2) { i_min = (version==1)? 2 : 3; i_max = n; }
		else if (orden == 3) { i_min = (version==1)? 3 : 4; i_max = n; }
		else                 { i_min = (version==1)? 4 : 5; i_max = n; }
	} else {                           /* centrada */
		if      (orden == 1) { i_min = (version==1)? 1 : 2; i_max = (version==1)? n-1 : n-2; }
		else if (orden == 2) { i_min = (version==1)? 1 : 2; i_max = (version==1)? n-1 : n-2; }
		else if (orden == 3) { i_min = (version==1)? 2 : 3; i_max = (version==1)? n-2 : n-3; }
		else                 { i_min = (version==1)? 2 : 3; i_max = (version==1)? n-2 : n-3; }
	}
	
	if (i_max < i_min) {
		printf("No hay puntos suficientes para esa combinacion (esquema/orden/version).\n");
		return 0;
	}
	
	printf("\n---------------------------------------------------------------\n");
	printf("a = %.12lf, b = %.12lf, n = %d, h = %.12lf\n", a, b, n, h);
	if (esquema == 1) printf("Esquema: adelante\n");
	else if (esquema == 2) printf("Esquema: atras\n");
	else printf("Esquema: centrada\n");
	printf("Orden derivada: %d, Version: %d\n", orden, version);
	printf("---------------------------------------------------------------\n");
	printf("Derivadas en los puntos x\n");
	
	/* === Archivo de salida para GNUPLOT: x[i]  y[i] === */
	FILE *out = fopen("datos.txt", "w");
	if (!out) {
		printf("No se pudo abrir 'datos.txt' para escribir.\n");
	}
	
	for (int i = i_min; i <= i_max; i = i + 1) {
		if (usar_derivada_exacta && orden == 1) {
			double ex = df1_exacta(x[i]);
			double ea = fabs(y[i] - ex);
			printf(" x=%.12lf  aprox=%.12lf  exacta=%.12lf  err_abs=%.12lf\n",
				   x[i], y[i], ex, ea);
		} else {
			printf("i=%d  x=%.12lf  aprox=%.12lf\n", i, x[i], y[i]);
		}
		if (out) fprintf(out, "%.12lf %.12lf\n", x[i], y[i]);
	}
	
	if (out) {
		fclose(out);
		printf("\nSe guardaron %d pares en 'datos.txt' (formato: x fp).\n", (i_max - i_min + 1));
	}
	
	return 1;
}
