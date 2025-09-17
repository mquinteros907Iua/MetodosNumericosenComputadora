/* =========================================================================
Regresion_Minimos.c
- Ajuste polinomial por mínimos cuadrados (según el pseudocódigo dado).
- CUATRO funciones principales:
1) leer_puntos_desde_archivo(...)
2) gauss_pivoteo_parcial(...)
3) ajustar_polinomio_minimos_cuadrados(...)  // arma A y b y llama a Gauss
4) coeficiente_correlacion(...)              // calcula Sr, St y r
- Variables en ESPAÑOL y comentarios claros.
- El main solo organiza el flujo y usa switch para la carga.
====================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ---------- Límites y constantes ---------- */
#define MAX_PUNTOS   1000
#define MAX_GRADO      20
#define MAX_ORDEN    (MAX_GRADO + 1)
#define EPS          1e-12




/* ---------- Prototipos ---------- */
/* 1) Entrada de datos */
int  leer_puntos_desde_archivo(const char *nombre_archivo, double valores_x[], double valores_y[], int max_puntos);
void leer_puntos_desde_teclado(int *cantidad_puntos, double valores_x[], double valores_y[]);

/* 2) Eliminación gaussiana con pivoteo parcial */
int  gauss_pivoteo_parcial(int orden, double A[][MAX_ORDEN], double b[], double solucion[]);

/* 3) Ajuste por mínimos cuadrados: arma A y b y llama a Gauss */
int  ajustar_polinomio_minimos_cuadrados(int cantidad_puntos, int grado, const double valores_x[], const double valores_y[], double coeficientes[]);

/* 4) Métrica de calidad: Sr, St y r (todo aquí) */
double coeficiente_correlacion(int cantidad_puntos, int grado, const double valores_x[], const double valores_y[], const double coeficientes[],  double *Sr_out, double *St_out);

/* Utilidades (mostrar/validar/evaluar) */
void   mostrar_puntos(int cantidad_puntos, const double valores_x[], const double valores_y[]);
void   mostrar_polinomio(int grado, const double coeficientes[]);
double evaluar_polinomio(int grado, const double coeficientes[], double x);
int    verificar_puntos_suficientes(int cantidad_puntos, int grado);







/* =============================== MAIN =============================== */
int main(void)
{
	int cantidad_puntos = 0;
	int grado = 0;
	double valores_x[MAX_PUNTOS], valores_y[MAX_PUNTOS];
	double coeficientes[MAX_ORDEN];
	
	/* --------- Carga de datos (switch como pediste) --------- */
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
	case 1: 																	//Metodo de cargas puntos a mano (No modificar)
		leer_puntos_desde_teclado(&cantidad_puntos, valores_x, valores_y);
		break;
		
	case 2: {																	//Metodo que carga puntos desde un archivo (No modificar)
		char nombre[256]; 
		printf("Nombre del archivo: ");
		if (scanf("%255s", nombre) != 1) {
			printf("Entrada invalida.\n");
			return 0;
		}
		int leidos = leer_puntos_desde_archivo(nombre, valores_x, valores_y, MAX_PUNTOS);
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
	
	
	
	/* Mostrar para verificar la carga */
	mostrar_puntos(cantidad_puntos, valores_x, valores_y);
	
	/* --------- Elegir grado --------- */
	printf("\nIngrese el grado del polinomio p (p >= 1, p <= %d): ", MAX_GRADO);
	if (scanf("%d", &grado) != 1 || grado < 1 || grado > MAX_GRADO) {
		printf("Grado invalido.\n");
		return 0;
	}
	
	/* --------- Verificar que la cantidad de putnos sea adecuada para el grado del polinomio --------- */
	if (!verificar_puntos_suficientes(cantidad_puntos, grado)) {
		/* Mensaje ya mostrado adentro */
		return 0;
	}
	
	/* --------- Ajustar (arma A y b y resuelve por Gauss) --------- */
	if (!ajustar_polinomio_minimos_cuadrados(cantidad_puntos, grado, valores_x, valores_y, coeficientes)) {
		printf("No se pudo obtener el polinomio (sistema inestable o pivote ~ 0).\n");
		return 0;
	}
	
	
	/* --------- Mostrar el polinomio resultado --------- */
	mostrar_polinomio(grado, coeficientes);
	
	
	/* --------- Calidad del ajuste: Sr, St, r --------- */
	double Sr = 0.0, St = 0.0;
	double r = coeficiente_correlacion(cantidad_puntos, grado, valores_x, valores_y, coeficientes, &Sr, &St);
	
	/* Resumen final */
	printf("\n--- Resumen ---\n");
	printf("Sr = %.12lf\n", Sr);
	printf("St = %.12lf\n", St);
	printf("r  = %.12lf\n", r);
	
	return 0;
}





/* ====================== Implementaciones ====================== */

/* ---------------- 1) Lectura de archivo ---------------- */
int leer_puntos_desde_archivo(const char *nombre_archivo, double valores_x[], double valores_y[], int max_puntos)
{
	FILE *fp = fopen(nombre_archivo, "r");
	if (!fp) {
		printf("No se puede abrir el archivo: %s\n", nombre_archivo);
		return 0;
	}
	
	int i = 0;
	char linea[512];
	while (i < max_puntos && fgets(linea, sizeof(linea), fp) != NULL) {
		/* Saltear comentarios o líneas vacías */
		if (linea[0] == '#' || linea[0] == '\n' || linea[0] == '\r') continue;
		
		double vx, vy;
		if (sscanf(linea, "%lf %lf", &vx, &vy) == 2) {
			valores_x[i] = vx;
			valores_y[i] = vy;
			i++;
		}
		/* Si la línea no tiene 2 doubles, la ignoro. */
	}
	
	fclose(fp);
	return i;
}

void leer_puntos_desde_teclado(int *cantidad_puntos, double valores_x[], double valores_y[])
{
	int cant = 0;
	printf("Cantidad de puntos: ");
	if (scanf("%d", &cant) != 1 || cant < 2 || cant > MAX_PUNTOS) {
		printf("Cantidad invalida.\n");
		exit(0);
	}
	
	printf("\n--- Carga de puntos ---\n");
	for (int i = 0; i < cant; i++) {
		printf("x[%d] = ", i);
		scanf("%lf", &valores_x[i]);
		printf("y[%d] = ", i);
		scanf("%lf", &valores_y[i]);
	}
	*cantidad_puntos = cant;
}

/* --------------- Utilidades simples --------------- */
void mostrar_puntos(int cantidad_puntos, const double valores_x[], const double valores_y[])
{
	printf("\n--- Puntos cargados ---\n");
	printf("  i |         x[i]         |         y[i]\n");
	printf("-----------------------------------------------\n");
	for (int i = 0; i < cantidad_puntos; i++) {
		printf("%3d | %18.10lf | %18.10lf\n", i, valores_x[i], valores_y[i]);
	}
}

int verificar_puntos_suficientes(int cantidad_puntos, int grado)
{
	if (cantidad_puntos < grado + 1) {
		printf("\nNo hay datos suficientes: para grado p=%d se requieren al menos %d puntos.\n",
			   grado, grado + 1);
		return 0;
	}
	return 1;
}

void mostrar_polinomio(int grado, const double coeficientes[])
{																		
	printf("\nCoeficientes a_k:\n");											//Muestra las incodgnitas (ax, bx, cx)
	for (int k = 0; k <= grado; k++) {
		printf("a[%d] = %.12lf\n", k, coeficientes[k]);
	}
	
	printf("\nP_%d(x) = ", grado);												//Muestra como queda el polinomio resultante (resutlado)
	for (int k = 0; k <= grado; k++) {
		if (k == 0) 
			printf("(%.12lf)", coeficientes[k]);
		else       
			printf(" + (%.12lf)*x^%d", coeficientes[k], k);
	}
	printf("\n");
}

double evaluar_polinomio(int grado, const double coeficientes[], double x)
{
	double suma = 0.0;
	double potencia = 1.0;  /* x^0 */
	for (int k = 0; k <= grado; k++) {
		suma += coeficientes[k] * potencia;
		potencia *= x;       /* genera x^(k+1) */
	}
	return suma;
}

/* --------------- 2) Gauss con pivoteo parcial --------------- */
int gauss_pivoteo_parcial(int orden, double A[][MAX_ORDEN], double b[], double solucion[]){

	/* Eliminación hacia adelante */
	for (int k = 0; k < orden - 1; k++) {
		/* Buscar pivote (máximo absoluto en la columna k) */
		int fila_pivote = k;
		double maxabs = fabs(A[k][k]);
		for (int i = k + 1; i < orden; i++) {
			double val = fabs(A[i][k]);
			if (val > maxabs) { maxabs = val; fila_pivote = i; }
		}
		
		if (maxabs < EPS) {
			printf("No se puede continuar: pivote A[%d][%d] ~ 0\n", fila_pivote, k);
			return 0;
		}
		
		/* Intercambio de filas si hace falta */
		if (fila_pivote != k) {
			for (int j = k; j < orden; j++) {
				double tmp = A[k][j];
				A[k][j] = A[fila_pivote][j];
				A[fila_pivote][j] = tmp;
			}
			double tmpb = b[k]; b[k] = b[fila_pivote]; b[fila_pivote] = tmpb;
		}
		
		/* Anular por debajo del pivote */
		for (int i = k + 1; i < orden; i++) {
			double m = A[i][k] / A[k][k];
			A[i][k] = 0.0;
			for (int j = k + 1; j < orden; j++) {
				A[i][j] -= m * A[k][j];
			}
			b[i] -= m * b[k];
		}
	}
	
	if (fabs(A[orden-1][orden-1]) < EPS) {
		printf("No se puede continuar: pivote A[%d][%d] ~ 0\n", orden-1, orden-1);
		return 0;
	}
	
	/* Sustitución hacia atrás */
	for (int i = orden - 1; i >= 0; i--) {
		double suma = b[i];
		for (int j = i + 1; j < orden; j++) suma -= A[i][j] * solucion[j];
		solucion[i] = suma / A[i][i];
	}
	
	return 1;
}


/* --------------- 3) Ajuste: arma A y b y llama a Gauss --------------- */
/* Pseudocódigo seguido literalmente:
Para cada l = 0..p:
b[l] = sum_{i=0..n} pow(x[i], l) * y[i]
Para cada m = 0..p:
A[l][m] = sum_{i=0..n} pow(x[i], l+m)
*/
int ajustar_polinomio_minimos_cuadrados(int cantidad_puntos, int grado, const double valores_x[], const double valores_y[], double coeficientes[]) { 

	const int orden = grado + 1;
	double A[MAX_ORDEN][MAX_ORDEN];
	double b[MAX_ORDEN];
	
	
	//Completa la matriz A (Con sus respectivos calculos en cada slot) al igaul que B
	for (int l = 0; l <= grado; l++){		//Recorre las filas 
		/* Calcular b[l] */
		double suma_b = 0.0;
		for (int i = 0; i < cantidad_puntos; i++) { 				//Calcula los valores de la matriz B (fila por fila)
			suma_b += pow(valores_x[i], l) * valores_y[i];
		}
		b[l] = suma_b;
		
		//Calcula los valores de la matriz A (fila por fila) (cada slot tieen un formula)
		for (int m = 0; m <= grado; m++) {
			double suma_A = 0.0;
			for (int i = 0; i < cantidad_puntos; i++) {
				suma_A += pow(valores_x[i], l + m);
			}
			A[l][m] = suma_A;
			
			//		
		}
	}
	
	/* Resolver A * coeficientes = b */ //nos da los valores de los coefiencte (incognitas)
	if (!gauss_pivoteo_parcial(orden, A, b, coeficientes)) {
		return 0;
	}
	return 1;
}



/* --------------- 4) Coeficiiente de CORRELACION (Calculo interno de St, Sr)--------------- */
double coeficiente_correlacion(int cantidad_puntos, int grado, const double valores_x[], const double valores_y[], const double coeficientes[], double *Sr_out, double *St_out)
{	
	//Calculo del Sr
	
	/* Sr = S (P(xi) - yi)^2 */
	double Sr = 0.0;
	for (int i = 0; i < cantidad_puntos; i++) {
		double yi_aprox = evaluar_polinomio(grado, coeficientes, valores_x[i]);
		double e = yi_aprox - valores_y[i];
		Sr += e * e;
	}
	
	//Calculo del St
	
	/* y_copete y St */
	double suma_y = 0.0;
	for (int i = 0; i < cantidad_puntos; i++) suma_y += valores_y[i];
	double y_copete = suma_y / ((double)cantidad_puntos + 1);
	
	double St = 0.0;
	for (int i = 0; i < cantidad_puntos; i++) {
		double d = y_copete - valores_y[i];
		St += d * d;
	}
	
	
	//Calculo final de r (Coefiente de Correlacion)
	
	/* r = sqrt( (St - Sr) / St ) , con control de St=0 */
	double r = 0.0;
	if (St > 0.0) {
		r = sqrt((St - Sr) / St);
		if (r < 0.0) r = 0.0;
		if (r > 1.0) r = 1.0;
	} else {
		r = 0.0;
	}
	
	/* Mostrar resultados aquí (como pediste) */
	printf("\n--- Calidad del ajuste ---\n");
	printf("Sr = %.12lf  (suma de residuos al cuadrado)\n", Sr);
	printf("St = %.12lf  (dispersion total respecto al promedio)\n", St);
	printf("r  = %.12lf  (coeficiente de correlacion: 0 malo, 1 excelente)\n", r);
	
	if (Sr_out) *Sr_out = Sr;
	if (St_out) *St_out = St;
	return r;
}
