/* =========================================================================
PolinomioFuncional.c
- Construye el polinomio interpolador en forma funcional:
P_n(x) = a0 + a1*x + ... + an*x^n
- Arma una "matriz de potencias de x": A[i][j] = (x_i)^j
- Resuelve A*a = b con Eliminacion Gaussiana + Pivoteo Parcial
- Permite evaluar P_n en un X_copete (opcional)
- Carga de puntos desde archivo o manual

Compilar (Zinjal / GCC):
gcc -std=c11 -Wall -Wextra PolinomioFuncional.c -o funcional -lm
========================================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_PUNTOS 1000
#define EPS 1e-12

/* --------------------- Prototipos de funciones --------------------- */
int leer_puntos_archivo(const char *nombre_archivo,
						double valores_x[], double valores_y[],
						int max_puntos);

void mostrar_tabla_puntos(int grado_polinomio,
						  const double valores_x[], const double valores_y[]);

int gauss_pivoteo_parcial(int orden,
						  double matriz_A[orden][orden],
							  double vector_b[orden],
							  double solucion[orden]);

void mostrar_matriz(int filas, int columnas,
					double M[filas][columnas],
						const char *titulo);

void mostrar_vector(int n, const double v[], const char *titulo);

/* ------------------------------- MAIN ------------------------------- */
int main(void)
{
	int grado_polinomio = 0;                 /* n */
	int cantidad_puntos = 0;                 /* n + 1 */
	double valores_x[MAX_PUNTOS];
	double valores_y[MAX_PUNTOS];
	
	/* --------- Seleccion del modo de carga --------- */
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
		
		cantidad_puntos = leer_puntos_archivo(nombre_archivo,
											  valores_x, valores_y,
											  MAX_PUNTOS);
		
		if (cantidad_puntos <= 1)
		{
			printf("No hay suficientes puntos en el archivo.\n");
			return 0;
		}
		
		grado_polinomio = cantidad_puntos - 1;
		
		printf("\nSe leyeron %d puntos (grado n = %d)\n",
			   cantidad_puntos, grado_polinomio);
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
	
	/* --------- Verificar visualmente los puntos cargados --------- */
	mostrar_tabla_puntos(grado_polinomio, valores_x, valores_y);
	
	/* --------- 1) Armar la "matriz de potencias de x" y el vector b ---------
	A[i][j] = (x_i)^j   con j = 0..n
	b[i]    = y_i
	*/
	int orden = grado_polinomio + 1;      /* tamaño del sistema */
	double matriz_A[orden][orden];
	double vector_b[orden];
	
	for (int i = 0; i < orden; i++)
	{
		vector_b[i] = valores_y[i];
		
		for (int j = 0; j < orden; j++)
		{
			matriz_A[i][j] = pow(valores_x[i], j);   /* (x_i)^j */
		}
	}
	
	printf("\n--- Sistema A * a = b (matriz de potencias de x) ---\n");
	mostrar_matriz(orden, orden, matriz_A, "Matriz A:");
	mostrar_vector(orden, vector_b, "\nVector b (y_i):");
	
	/* --------- 2) Resolver por Eliminacion Gaussiana con pivoteo parcial --------- */
	double coeficientes[orden];   /* a0..an */
	int exito = gauss_pivoteo_parcial(orden, matriz_A, vector_b, coeficientes);
	
	if (!exito)
	{
		printf("\nNo se pudo resolver el sistema: pivote ~ 0 o inestable.\n");
		return 0;
	}
	
	/* --------- 3) Mostrar coeficientes y polinomio --------- */
	printf("\n--- Coeficientes del polinomio P_n(x) ---\n");
	for (int j = 0; j < orden; j++)
	{
		printf("a[%d] = %16.10lf\n", j, coeficientes[j]);
	}
	
	printf("\nP_n(x) = ");
	for (int j = 0; j < orden; j++)
	{
		if (j == 0)
		{
			printf("(%.10lf)", coeficientes[j]);
		}
		else
		{
			printf(" + (%.10lf) * x^%d", coeficientes[j], j);
		}
	}
	printf("\n");
	
	/* --------- 4) Evaluacion opcional del polinomio en X_copete --------- */
	int desea_evaluar = 0;
	
	printf("\nDesea evaluar P_n en un X_copete ? (1=si, 0=no): ");
	if (scanf("%d", &desea_evaluar) == 1 && desea_evaluar == 1)
	{
		double X_copete = 0.0;
		double resultado_final = 0.0;
		
		printf("Ingrese X_copete: ");
		if (scanf("%lf", &X_copete) == 1)
		{
			for (int j = 0; j < orden; j++)
			{
				resultado_final += coeficientes[j] * pow(X_copete, j);
			}
			
			printf("P_n(%.10lf) = %.12lf\n", X_copete, resultado_final);
		}
	}
	
	return 0;
}

/* =================== Implementaciones auxiliares =================== */

/* Lee pares (x y) de un archivo de texto. Devuelve la cantidad leida. */
int leer_puntos_archivo(const char *nombre_archivo,
						double valores_x[], double valores_y[],
						int max_puntos)
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
void mostrar_tabla_puntos(int grado_polinomio,
						  const double valores_x[], const double valores_y[])
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

/* Eliminacion Gaussiana con pivoteo parcial: resuelve A * solucion = b
Retorna 1 si OK, 0 si falla por pivote ~ 0 */
int gauss_pivoteo_parcial(int orden,
						  double matriz_A[orden][orden],
							  double vector_b[orden],
							  double solucion[orden])
{
	/* ---- Eliminacion hacia adelante con pivoteo ---- */
	for (int k = 0; k < orden - 1; k++)
	{
		/* Seleccionar el mayor valor absoluto en la columna k (desde fila k) */
		int fila_pivote = k;
		double max_abs = fabs(matriz_A[k][k]);
		
		for (int i = k + 1; i < orden; i++)
		{
			double valor = fabs(matriz_A[i][k]);
			if (valor > max_abs)
			{
				max_abs = valor;
				fila_pivote = i;
			}
		}
		
		if (max_abs < EPS)
		{
			printf("No se puede continuar: pivote A[%d][%d] ~ 0\n",
				   fila_pivote, k);
			return 0;
		}
		
		/* Intercambiar filas si el pivote no esta en k */
		if (fila_pivote != k)
		{
			for (int j = k; j < orden; j++)
			{
				double tmp = matriz_A[k][j];
				matriz_A[k][j] = matriz_A[fila_pivote][j];
				matriz_A[fila_pivote][j] = tmp;
			}
			
			double tmpb = vector_b[k];
			vector_b[k] = vector_b[fila_pivote];
			vector_b[fila_pivote] = tmpb;
		}
		
		/* Anular por debajo del pivote */
		for (int i = k + 1; i < orden; i++)
		{
			double m = matriz_A[i][k] / matriz_A[k][k];
			matriz_A[i][k] = 0.0;
			
			for (int j = k + 1; j < orden; j++)
			{
				matriz_A[i][j] -= m * matriz_A[k][j];
			}
			
			vector_b[i] -= m * vector_b[k];
		}
	}
	
	if (fabs(matriz_A[orden - 1][orden - 1]) < EPS)
	{
		printf("No se puede continuar: pivote A[%d][%d] ~ 0\n",
			   orden - 1, orden - 1);
		return 0;
	}
	
	/* ---- Sustitucion hacia atras ---- */
	for (int i = orden - 1; i >= 0; i--)
	{
		double suma = vector_b[i];
		
		for (int j = i + 1; j < orden; j++)
		{
			suma -= matriz_A[i][j] * solucion[j];
		}
		
		solucion[i] = suma / matriz_A[i][i];
	}
	
	return 1;
}

/* Impresion de matriz con titulo. */
void mostrar_matriz(int filas, int columnas,
					double M[filas][columnas],
						const char *titulo)
{
	if (titulo != NULL)
	{
		printf("%s\n", titulo);
	}
	
	for (int i = 0; i < filas; i++)
	{
		for (int j = 0; j < columnas; j++)
		{
			printf("%14.8lf ", M[i][j]);
		}
		printf("\n");
	}
}

/* Impresion de vector con titulo. */
void mostrar_vector(int n, const double v[], const char *titulo)
{
	if (titulo != NULL)
	{
		printf("%s\n", titulo);
	}
	
	for (int i = 0; i < n; i++)
	{
		printf("[%2d] = %16.8lf\n", i, v[i]);
	}
}
