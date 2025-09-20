#include <stdio.h>
#include <math.h>

double f(double x) {
	return 3 + sin(x) - exp(x);
}

int main() {
	double xmin, xmax, x, y;
	int n, i;
	FILE *fp;
	
	printf("Ingrese el valor minimo de x: ");
	scanf("%lf", &xmin);
	printf("Ingrese el valor maximo de x: ");
	scanf("%lf", &xmax);
	printf("Ingrese la cantidad de puntos: ");
	scanf("%d", &n);
	
	fp = fopen("datos.txt", "w");
	if (fp == NULL) {
		printf("Error al abrir el archivo.\n");
		return 1;
	}
	
	double paso = (xmax - xmin) / (n - 1);
	for (i = 0; i < n; i++) {
		x = xmin + i * paso;
		y = f(x);
		fprintf(fp, "%.2lf %.2lf\n", x, y);
	}
	
	fclose(fp);
	printf("Datos guardados en datos.txt\n");
	
	return 0;
}
