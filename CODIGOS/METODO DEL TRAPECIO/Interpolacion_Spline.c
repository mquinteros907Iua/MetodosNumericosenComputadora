/* =========================================================================
Spline_Cubico.c
- Interpolación por spline cúbico (a_k x^3 + b_k x^2 + c_k x + d_k por tramo)
- Construcción del sistema A * coeficientes = b según tu pseudocódigo:
* 2n filas: paso por extremos de cada tramo
* (n-1) filas: continuidad de 1ra derivada
* (n-1) filas: continuidad de 2da derivada (dividida entre 2 -> 3a x + b)
* 2 filas finales: condiciones naturales (2da derivada en x0 y xn igual a 0)
- Utilidades: leer archivo/teclado, imprimir tabla, verificar x crecientes,
localizar subintervalo de X_COPETE, evaluar spline.
- Resolución: Gauss con pivoteo parcial .
======================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_PUNTOS   1000
#define EPS_PIVOTE   1e-12

/* ------------------- Prototipos  ------------------- */
int  leer_puntos_desde_archivo(const char *nombre_archivo, double x[], double y[], int max_puntos);
void leer_puntos_desde_teclado(int *cantidad, double x[], double y[]);
void imprimir_puntos(int cantidad, const double x[], const double y[]);
int  x_estrictamente_crecientes(int cantidad, const double x[]);
int  buscar_subintervalo(int cantidad, const double x[], double X_COPETE);


/* --------------- Prototipos ----------------- */
double **crear_matriz(int n);
void   liberar_matriz(double **A, int n);

void construir_sistema_spline_cubico_natural(int cantidad_puntos,const double x[], const double y[], double **A, double b[]);
int  gauss_pivoteo_parcial(int n, double **A, double b[], double x[]);

double evaluar_spline(int cantidad_puntos, const double x[], const double coef[], double X_COPETE);




/* --------------------------- MAIN ------------------------------ */
int main(void)
{
	int cantidad_puntos = 0;
	double x[MAX_PUNTOS], y[MAX_PUNTOS];
	
	/* ---- Carga de datos ---- */
	int opcion_carga = 0;
	printf("Cargar puntos (x y por linea)\n");
	printf("  1) Manual por teclado\n");
	printf("  2) Desde archivo\n");
	printf("Opcion: ");
	if (scanf("%d", &opcion_carga) != 1) {
		printf("Entrada invalida.\n");
		return 0;
	}
	
	switch (opcion_carga) {
	case 1:
		leer_puntos_desde_teclado(&cantidad_puntos, x, y);
		break;
	case 2: {
		char nombre[256];
		printf("Nombre del archivo: ");
		if (scanf("%255s", nombre) != 1) {
			printf("Entrada invalida.\n");
			return 0;
		}
		int leidos = leer_puntos_desde_archivo(nombre, x, y, MAX_PUNTOS);
		if (leidos < 2) {
			printf("No hay suficientes puntos en el archivo.\n");
			return 0;
		}
		cantidad_puntos = leidos;
		break;
	}
	default:
		printf("Opcion inexistente.\n");
		return 0;
	}
	
	/* Mostrar puntos*/
	imprimir_puntos(cantidad_puntos, x, y);
	if (!x_estrictamente_crecientes(cantidad_puntos, x)) {
		printf("\nNo se puede continuar: los x deben ser estrictamente crecientes.\n");
		return 0;
	}
	
	/* Variables para spline */
	int n_tramos = cantidad_puntos - 1;        /* n */
	int dimension = 4 * n_tramos;              /* tamaño del sistema */
	double **A = NULL;
	double *b  = NULL;
	double *coeficientes = NULL;               
	
	/* Menú de operaciones */
	int opcion = -1;
	int spline_listo = 0;
	
	while (1) {
		printf("\n--------- MENU ---------\n");
		printf("1) Ubicar X_COPETE en su subintervalo\n");
		printf("2) Calcular spline cubico natural (con Gauss) y mostrar coeficientes\n");		//Crear y mostrar el polinomio
		printf("3) Evaluar spline en X_COPETE (requiere opcion 2)\n");
		printf("0) Salir\n");
		printf("Opcion: ");
		if (scanf("%d", &opcion) != 1) {
			printf("Entrada invalida.\n");
			return 0;
		}
		if (opcion == 0) break;
		
		switch (opcion) {
			
			/* ---- Localizar subintervalo ---- */
		case 1: {
			double X_COPETE = 0.0;
			printf("Ingrese X_COPETE: ");
			if (scanf("%lf", &X_COPETE) != 1) {
				printf("Entrada invalida.\n");
				break;
			}
			int i = buscar_subintervalo(cantidad_puntos, x, X_COPETE);
			if (i == -1) {
				if (X_COPETE < x[0])
					printf("X_COPETE esta a la izquierda de x[0].\n");
				else
					printf("X_COPETE esta a la derecha de x[%d].\n", cantidad_puntos-1);
			} else {
				printf("X_COPETE pertenece al subintervalo [x[%d], x[%d]] = [%.10lf, %.10lf]\n", i, i+1, x[i], x[i+1]);
			}
		} break;
		
		
		/* ---- Construir sistema + Gauss + mostrar P_k(x) ---- */					//Crear y mostar el polinomio
		case 2: {
			/* reservar memoria del sistema  y crea las matrices*/
			A = crear_matriz(dimension);
			b = (double *)calloc(dimension, sizeof(double));
			coeficientes = (double *)calloc(dimension, sizeof(double));
			if (!A || !b || !coeficientes) {
				printf("No hay memoria suficiente.\n");
				return 0;
			}
			
			
			/* armar A y b */													//Este metodo crea y hace los calculos para el polinomio 						
			construir_sistema_spline_cubico_natural(cantidad_puntos, x, y, A, b);
			
			/* resolver */
			int ok = gauss_pivoteo_parcial(dimension, A, b, coeficientes);
			if (!ok) {
				printf("No se pudo resolver el sistema (pivote ~ 0 o inestable).\n");
				break;
			}
			
			/* mostrar coeficientes por tramo */
			printf("\n--- Coeficientes del spline (por tramo) ---\n");
			for (int k = 0; k < n_tramos; k++) {
				double a = coeficientes[4*k + 0];
				double b1 = coeficientes[4*k + 1];
				double c = coeficientes[4*k + 2];
				double d = coeficientes[4*k + 3];
				printf("Tramo %d  [%.10lf, %.10lf]:  P_%d(x) = " "%.10lf x^3  +  %.10lf x^2  +  %.10lf x  +  %.10lf\n", k, x[k], x[k+1], k, a, b1, c, d);
			}
			
			spline_listo = 1;
		} break;
		
		
		/* ---- Evaluar spline en X_COPETE ---- */
		case 3: {
			if (!spline_listo) {
				printf("Primero calcule el spline (opcion 2).\n");
				break;
			}
			double X_COPETE = 0.0;
			printf("Ingrese X_COPETE a evaluar: ");
			if (scanf("%lf", &X_COPETE) != 1) {
				printf("Entrada invalida.\n");
				break;
			}
			int i = buscar_subintervalo(cantidad_puntos, x, X_COPETE);
			if (i == -1) {
				printf("X_COPETE fuera de [x0, xn]; no se evalua spline.\n");
				break;
			}
			double y_val = evaluar_spline(cantidad_puntos, x, coeficientes, X_COPETE);
			printf("X_COPETE en [x[%d], x[%d]] -> S(X_COPETE) = %.12lf\n", i, i+1, y_val);
		} break;
		
		default:
			printf("Opcion inexistente.\n");
		}
	}
	

	if (A) liberar_matriz(A, dimension);
	free(b);
	free(coeficientes);
	
	return 0;
}





/* ================= Implementaciones utilitarias ================= */

int leer_puntos_desde_archivo(const char *nombre_archivo, double x[], double y[], int max_puntos)
{
	FILE *fp = fopen(nombre_archivo, "r");
	if (!fp) {
		printf("No se puede abrir el archivo: %s\n", nombre_archivo);
		return 0;
	}
	int i = 0;
	while (i < max_puntos && fscanf(fp, "%lf %lf", &x[i], &y[i]) == 2) {
		i++;
	}
	fclose(fp);
	return i;
}

void leer_puntos_desde_teclado(int *cantidad, double x[], double y[])
{
	int n = 0;
	printf("Cantidad de puntos: ");
	if (scanf("%d", &n) != 1 || n < 2 || n > MAX_PUNTOS) {
		printf("Cantidad invalida.\n");
		exit(0);
	}
	printf("\n--- Carga de puntos (x_i, y_i) ---\n");
	for (int i = 0; i < n; i++) {
		printf("x[%d] = ", i);  scanf("%lf", &x[i]);
		printf("y[%d] = ", i);  scanf("%lf", &y[i]);
	}
	*cantidad = n;
}

void imprimir_puntos(int cantidad, const double x[], const double y[])
{
	printf("\n--- Puntos cargados ---\n");
	printf("  i |         x[i]         |         y[i]\n");
	printf("-----------------------------------------------\n");
	for (int i = 0; i < cantidad; i++) {
		printf("%3d | %18.10lf | %18.10lf\n", i, x[i], y[i]);
	}
}

int x_estrictamente_crecientes(int cantidad, const double x[])
{
	for (int i = 0; i < cantidad - 1; i++) {
		if (!(x[i] < x[i+1])) {
			printf("No se puede continuar: x[%d]=%.10lf y x[%d]=%.10lf no cumplen x[i] < x[i+1].\n",
				   i, x[i], i+1, x[i+1]);
			return 0;
		}
	}
	return 1;
}


/* Devuelve i tal que x[i] <= X_COPETE <= x[i+1], o -1 si esta fuera */
int buscar_subintervalo(int cantidad, const double x[], double X_COPETE)
{
	if (X_COPETE < x[0] || X_COPETE > x[cantidad-1]) 
		return -1;
	if (X_COPETE == x[cantidad-1]) 
		return -1; /* no hay tramo a la derecha */
	
	for (int i = 0; i < cantidad - 1; i++) {
		if (X_COPETE >= x[i] && X_COPETE <= x[i+1]) 
			return i;
	}
	return -1;
}




/* ================== Memoria para matriz cuadrada ================= */

double **crear_matriz(int n)
{
	double **A = (double **)malloc(n * sizeof(double *));
	if (!A) return NULL;
	A[0] = (double *)calloc(n * n, sizeof(double));
	if (!A[0]) { free(A); return NULL; }
	for (int i = 1; i < n; i++) A[i] = A[0] + i * n;
	return A;
}

void liberar_matriz(double **A, int n)
{
	(void)n;
	if (!A) return;
	free(A[0]);
	free(A);
}


void construir_sistema_spline_cubico_natural(int cantidad_puntos, const double x[], const double y[], double **A, double b[])
{
	int n = cantidad_puntos - 1;   /* cantidad de tramos */
	int filas_total = 4 * n;
	
	/* Limpio por las dudas */
	for (int i = 0; i < filas_total; i++) {
		b[i] = 0.0;
		for (int j = 0; j < 4 * n; j++) A[i][j] = 0.0;
	}
	
	/* (1) Primeras 2n filas: paso por extremos */
	for (int k = 0; k < n; k++) {
		int base = 4 * k;
		
		/* fila 2k -> x_k */
		for (int j = 0; j < 4; j++) {
			A[2 * k][base + j]     = pow(x[k],   3 - j);
			A[2 * k + 1][base + j] = pow(x[k+1], 3 - j);
		}
		b[2 * k]     = y[k];
		b[2 * k + 1] = y[k + 1];
	}
	
	/* (2) (n-1) filas: continuidad de S' en x_{k+1}
	Estructura con 3 fors, pero activando solo la fila i=2n+k */
	for (int i = 2 * n; i <= 3 * n - 2; i++) {          /* filas */
		for (int k = 0; k <= n - 2; k++) {              /* tramos */
			if (i == 2 * n + k) {                       /* <-- clave */
				double xx = x[k + 1];
				int base_k  = 4 * k;
				int base_k1 = 4 * (k + 1);
				
				for (int j = 0; j <= 2; j++) {          /* j = 0,1,2 */
					/* +S'_k(xx) */
					A[i][base_k  + j] = (3.0 - j) * pow(xx, 2 - j);
					/* -S'_{k+1}(xx) */
					A[i][base_k1 + j] = -(3.0 - j) * pow(xx, 2 - j);
				}
				b[i] = 0.0;
			}
		}
	}
	
	/* (3) (n-1) filas: continuidad de S'' en x_{k+1}
	(3 a_k x + b_k) - (3 a_{k+1} x + b_{k+1}) = 0
	SOLO 2 fors, como dejaste */
	for (int i = 3 * n - 1; i <= 4 * n - 3; i++) {   /* filas */
		for (int k = 0; k <= n - 2; k++) {          /* tramos */
			if (k == i - (3 * n - 1)) {
				double xx = x[k + 1];
				int base_k  = 4 * k;
				int base_k1 = 4 * (k + 1);
				
				A[i][base_k + 0]  =  3.0 * xx;  /* a_k   */
				A[i][base_k + 1]  =  1.0;       /* b_k   */
				A[i][base_k1 + 0] = -3.0 * xx;  /* a_k+1 */
				A[i][base_k1 + 1] = -1.0;       /* b_k+1 */
				b[i] = 0.0;
			}
		}
	}
	
	/* (4) Últimas 2 filas (natural): S''(x0)=0 y S''(xn)=0
	Usamos filas 4n-2 y 4n-1 para no pisar el bloque (3). */
	{
		int i0 = 4 * n - 2; /* fila para x0 */
		int in = 4 * n - 1; /* fila para xn */
		
		/* S''_0(x0) = 6 a0 x0 + 2 b0 = 0 */
		A[i0][0] = 6.0 * x[0];  /* a0 */
		A[i0][1] = 2.0;         /* b0 */
		b[i0]    = 0.0;
		
		/* S''_{n-1}(xn) = 6 a_{n-1} xn + 2 b_{n-1} = 0 */
		int base_last = 4 * (n - 1);
		A[in][base_last + 0] = 6.0 * x[n];  /* a_{n-1} */
		A[in][base_last + 1] = 2.0;         /* b_{n-1} */
		b[in]                = 0.0;
	}
}

/* =================== Gauss con pivoteo parcial ================== */
int gauss_pivoteo_parcial(int n, double **A, double b[], double x[])
{
	/* Eliminación hacia adelante con pivoteo parcial por columna */
	for (int k = 0; k < n - 1; k++) {
		/* buscar pivote */
		int p = k;
		double max = fabs(A[k][k]);
		for (int i = k + 1; i < n; i++) {
			double v = fabs(A[i][k]);
			if (v > max) { max = v; p = i; }
		}
		if (max < EPS_PIVOTE) {
			printf("No se puede continuar: pivote A[%d][%d] ~ 0.\n", k, k);
			return 0;
		}
		/* swap filas si corresponde */
		if (p != k) {
			double *tmpRow = A[p]; A[p] = A[k]; A[k] = tmpRow;
			double tmpb = b[p]; b[p] = b[k]; b[k] = tmpb;
		}
		
		/* anular debajo */
		for (int i = k + 1; i < n; i++) {
			double m = A[i][k] / A[k][k];
			if (m == 0.0) continue;
			for (int j = k; j < n; j++) A[i][j] -= m * A[k][j];
			b[i] -= m * b[k];
		}
	}
	
	if (fabs(A[n-1][n-1]) < EPS_PIVOTE) {
		printf("No se puede continuar: pivote A[%d][%d] ~ 0.\n", n-1, n-1);
		return 0;
	}
	
	/* Sustitución hacia atrás */
	for (int i = n - 1; i >= 0; i--) {
		double suma = b[i];
		for (int j = i + 1; j < n; j++) suma -= A[i][j] * x[j];
		x[i] = suma / A[i][i];
	}
	return 1;
}

/* ================ Evaluación del spline en X_COPETE ================ */
double evaluar_spline(int cantidad_puntos, const double x[], const double coef[], double X_COPETE)
{
	int n = cantidad_puntos - 1;
	int i = buscar_subintervalo(cantidad_puntos, x, X_COPETE);
	if (i < 0) {
		/* fuera de rango: a elección, aquí devolvemos NaN */
		return NAN;
	}
	double a = coef[4*i + 0];
	double b1 = coef[4*i + 1];
	double c = coef[4*i + 2];
	double d = coef[4*i + 3];
	return ((a*X_COPETE + b1)*X_COPETE + c)*X_COPETE + d;  /* Horner */
}
