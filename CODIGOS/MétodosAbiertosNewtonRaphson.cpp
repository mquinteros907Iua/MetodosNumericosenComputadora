#include <stdio.h>
#include <math.h>

// Definir la funcion f(x)
double f(double x) {
	return cos(x) - x;
}

// Definir la derivada f'(x)
double df(double x) {
	return -sin(x) - 1;
}

// Newton-Raphson con error absoluto
void newtonAbsoluto(double x0, double tol) {
	int maxIter = 10000;
	int iter = 0;
	double x1, error;
	
	printf("\n[Newton-Raphson con Error Absoluto]\n");
	printf("Iter\t x_n\t\t f(x_n)\t\t Error\n");
	printf("--------------------------------------------------\n");
	
	do {
		double f_x0 = f(x0);
		double df_x0 = df(x0);
		
		if (fabs(df_x0) < 1e-12) {
			printf("[Finalizacion] Derivada nula en x = %lf\n", x0);
			return;
		} else if (fabs(df_x0) < 1e-4) {
			printf("[Warning] Derivada muy pequena en x = %lf\n", x0);
		}
		
		x1 = x0 - f_x0 / df_x0;
		error = fabs(x1 - x0);
		
		printf("%d\t %.6lf\t %.6lf\t %.6lf\n", iter + 1, x0, f_x0, error);
		
		x0 = x1;
		iter++;
		
		if (error < tol || fabs(f(x1)) < tol) {
			printf("\n[Finalizacion] Se alcanzo la tolerancia requerida.\n");
			printf("Raiz aproximada: %.10lf\n", x1);
			printf("Iteraciones realizadas: %d\n", iter);
			printf("Error Absoluto: %.lf",error);
			printf("f(x) = %.10lf (debe estar cercano a 0)\n", f(x1));
			return;
		}
		
	} while (error > tol || iter < 10000);
	
	printf("\n[Finalizacion] Se alcanzo el maximo de iteraciones (%d).\n", maxIter);
	printf("Raiz aproximada: %.10lf\n", x1);
	printf("Iteraciones realizadas: %d\n", iter);
	printf("Error Absoluto: %.lf",error);
	printf("f(x) = %.10lf (debe estar cercano a 0)\n", f(x1));
}

// Newton-Raphson con error porcentual
void newtonPorcentual(double x0, double tol) {
	int maxIter = 10000;
	int iter = 0;
	double x1, error;
	
	printf("\n[Newton-Raphson con Error Porcentual]\n");
	printf("Iter\t x_n\t\t f(x_n)\t\t Error(%%)\n");
	printf("--------------------------------------------------\n");
	
	do {
		double f_x0 = f(x0);
		double df_x0 = df(x0);
		
		if (fabs(df_x0) < 1e-12) {
			printf("[Finalizacion] Derivada nula en x = %lf\n", x0);
			return;
		} else if (fabs(df_x0) < 1e-4) {
			printf("[Warning] Derivada muy pequena en x = %lf\n", x0);
		}
		
		x1 = x0 - f_x0 / df_x0;
		error = fabs((x1 - x0) / x1) * 100.0;
		
		printf("%d\t %.6lf\t %.6lf\t %.6lf\n", iter + 1, x0, f_x0, error);
		
		x0 = x1;
		iter++;
		
		if (error < tol || fabs(f(x1)) < tol) {
			printf("\n[Finalizacion] Se alcanzo la tolerancia requerida.\n");
			printf("Raiz aproximada: %.10lf\n", x1);
			printf("Iteraciones realizadas: %d\n", iter);
			printf("Error Absoluto: %.lf",error);
			printf("f(x) = %.10lf (debe estar cercano a 0)\n", f(x1));
			return;
		}
		
	} while (error > tol || iter < 10000);
	
	printf("\n[Finalizacion] Se alcanzo el maximo de iteraciones (%d).\n", maxIter);
	printf("Raiz aproximada: %.10lf\n", x1);
	printf("Iteraciones realizadas: %d\n", iter);
	printf("Error Absoluto: %.lf",error);
	printf("f(x) = %.10lf (debe estar cercano a 0)\n", f(x1));
}

int main() {
	double x0, tol;
	int opcion;
	
	printf("Metodo de Newton-Raphson\n");
	printf("Ingrese el valor inicial x0: ");
	scanf("%lf", &x0);
	
	printf("\nSeleccione el tipo de error:\n");
	printf("1. Error Absoluto\n");
	printf("2. Error Porcentual\n");
	printf("Opcion: ");
	scanf("%d", &opcion);
	
	printf("Ingrese tolerancia: ");
	scanf("%lf", &tol);
	
	if (opcion == 1) {
		newtonAbsoluto(x0, tol);
	} else if (opcion == 2) {
		newtonPorcentual(x0, tol);
	} else {
		printf("Opcion invalida.\n");
	}
	
	return 0;
}
