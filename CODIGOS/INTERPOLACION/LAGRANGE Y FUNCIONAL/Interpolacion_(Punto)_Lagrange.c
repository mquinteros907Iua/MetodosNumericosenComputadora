/* =========================================================================
LagrangePunto.c
- Interpolación de Lagrange (Para Ecnontrar un punto)
- Carga de puntos: archivo o teclado

========================================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_PUNTOS 1000

/* ========= DEFINIR LA FUNCION f(x) AQUI ========= */
double f(double x)
{
	
	return (x + 2.0) / x;														//Modificar Funcion
}
/* =============================================== */







/* --------- Prototipos --------- */
int leer_puntos_archivo(const char *nombre_archivo, double valores_x[], double valores_y[], int max_puntos);

void mostrar_tabla_puntos(int grado_polinomio, const double valores_x[], const double valores_y[]);

void lagrange_punto_error_absoluto(int grado_polinomio, const double valores_x[], const double valores_y[]);










/* ------------------------------ MAIN ------------------------------- */
int main(void)
{
	int grado_polinomio = 0;        
	int cantidad_puntos = 0;        /* cantidad de puntos = n + 1 */ //Siendo n la cantidad de ecauciones
	double valores_x[MAX_PUNTOS];
	double valores_y[MAX_PUNTOS];
	
	/* --- Selección del modo de carga --- */
	int modo_carga = 0;
	
	printf("Cargar puntos:\n");
	printf("  1) Manual (teclado)\n");
	printf("  2) Desde archivo (formato: 'x y' por linea)\n");
	printf("Opcion: ");
	
	if (scanf("%d", &modo_carga) != 1)
	{
		printf("Entrada invalida.\n");
		return 0;
	}
	
	if (modo_carga == 2)
	{
		char nombre_archivo[256];
		
		printf("Nombre del archivo: ");
		if (scanf("%255s", nombre_archivo) != 1)
		{
			printf("Entrada invalida.\n");
			return 0;
		}
		
		cantidad_puntos = leer_puntos_archivo(nombre_archivo, valores_x, valores_y, MAX_PUNTOS);
		
		if (cantidad_puntos <= 1)
		{
			printf("No hay suficientes puntos en el archivo.\n");
			return 0;
		}
		
		grado_polinomio = cantidad_puntos - 1;
		
		printf("\nSe leyeron %d puntos (grado n = %d)\n", cantidad_puntos, grado_polinomio);
	}
	else
	{
		printf("Ingrese n (tendra n+1 puntos): ");
		if (scanf("%d", &grado_polinomio) != 1)
		{
			printf("Entrada invalida.\n");
			return 0;
		}
		
		if (grado_polinomio < 1)
		{
			printf("n invalido: debe ser >= 1.\n");
			return 0;
		}
		
		cantidad_puntos = grado_polinomio + 1;
		
		printf("\n--- Carga de puntos (x_i, y_i) ---\n");
		for (int i = 0; i < cantidad_puntos; i++)
		{
			printf("x[%d] = ", i);
			scanf("%lf", &valores_x[i]);
			
			printf("y[%d] = ", i);
			scanf("%lf", &valores_y[i]);
		}
	}
	
	/* Mostrar datos y evaluar */
	mostrar_tabla_puntos(grado_polinomio, valores_x, valores_y);
	lagrange_punto_error_absoluto(grado_polinomio, valores_x, valores_y);
	
	return 0;
}







/* ================ Interpolacion de Lagrange + error absoluto contra f(x^). ================*/
void lagrange_punto_error_absoluto(int grado_polinomio, const double valores_x[], const double valores_y[])
{
	double X_copete = 0.0;														//Valor de x ingresado que queremos saber su valor en y
	
	printf("\nIngrese el valor de X (valor donde desea interpolar): ");
	if (scanf("%lf", &X_copete) != 1)
	{
		printf("Entrada invalida.\n");
		return;
	}
	
	/* P(x^) = sum_{k=0..n} y[k] * Lk(x^) */
	double resultado_final = 0.0;
	
	printf("\n k |           Ck(x_copete)            |        y[k]*Ck(x_copete)\n");
	printf("-----------------------------------------------------------\n");
	
	for (int k = 0; k <= grado_polinomio; k++)									//Calculo del termino "n" (osea cadaa vuelta estoy calculoando un termino de la suma del polinomio)
	{
		double Calculo_Ck = 1.0;												//Variable donde se almacena cada fracion que vamos multiplciando
		
		for (int i = 0; i <= grado_polinomio; i++)								//Esto va generando las diferentes fraciones que se deben multiplicar 
		{
			if (i != k)															//Esto hace que la priemra interaccion no se realice
			{
				double numerador = X_copete - valores_x[i];						//Parte de arriba de la fraccion
				double denominador = valores_x[k] - valores_x[i];				//Parte de abajo de la fraccion
				Calculo_Ck *= (numerador / denominador);				        //Calculo de cada fracion que vamos multiplicando y almacenando
			}
		}
		
		double aporte = valores_y[k] * Calculo_Ck;								//Aca precisamente se calcula el termino del polinomio (osea el Yn * Cn)
		resultado_final += aporte;
		
		printf("%2d | %24.12lf | %24.12lf\n", k, Calculo_Ck, aporte);
	}
	
	printf("-----------------------------------------------------------\n");
	printf("P_n(%.8lf) = %.12lf\n", X_copete, resultado_final);
	
	/* Error absoluto automático: |aprox - exacto| */
	double y_exacto = f(X_copete);
	double error_absoluto = fabs(resultado_final - y_exacto);
	
	printf("f(%.8lf)    = %.12lf  (exacto)\n", resultado_final, y_exacto);
	printf("Error absoluto = %.12lf\n", error_absoluto);
}













/* ---------------- Implementaciones auxiliares ---------------- */

/* Lee pares (x y) de un archivo de texto. Devuelve la cantidad leida. */
int leer_puntos_archivo(const char *nombre_archivo, double valores_x[], double valores_y[], int max_puntos)
{
	FILE *archivo = fopen(nombre_archivo, "r");
	if (archivo == NULL)
	{
		printf("No se puede abrir el archivo.\n");
		exit(1);
	}
	
	int i = 0;
	while (i < max_puntos &&
		   fscanf(archivo, "%lf %lf", &valores_x[i], &valores_y[i]) == 2)
	{
		i++;
	}
	
	fclose(archivo);
	return i;
}


/* Muestra una tabla prolija de los puntos cargados. */
void mostrar_tabla_puntos(int grado_polinomio, const double valores_x[], const double valores_y[])
{
	printf("\n--- Puntos cargados ---\n");
	printf(" i |        x[i]        |        y[i]\n");
	printf("-------------------------------------------\n");
	
	for (int i = 0; i <= grado_polinomio; i++)
	{
		printf("%2d | %16.8lf | %16.8lf\n",
			   i, valores_x[i], valores_y[i]);
	}
}



