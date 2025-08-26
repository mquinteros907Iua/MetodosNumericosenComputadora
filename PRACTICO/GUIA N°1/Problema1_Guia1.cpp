#include <iostream>
#include <math.h>
#include <stdlib.h>

using namespace std;

// Definición de la función f(x)
double f(double x) {
	return -2 + (7 * x) - (5 * pow(x, 2)) + (6 * pow(x, 3));
}


// Método de Bisección
void biseccion(double a, double b, double tolerancia) {
	if (f(a) * f(b) > 0) {
		cout << "Error: no hay cambio de signo en el intervalo [" << a << ", " << b << "]. No hay raíz en el intervalo" << endl;
		return;
	}
	
	double errorAbsoluto, cViejo, c;
	cViejo = a;
	
	do {
		c = (a + b) / 2;
		
		if (f(a) * f(c) > 0) {
			a = c;
		} else if (f(a) * f(c) < 0) {
			b = c;
		} else {
			printf("La raíz es: %f\n", c);
			exit(0);
		}
		
		errorAbsoluto = fabs(c - cViejo);
		cViejo = c;
		
	} while (errorAbsoluto > tolerancia);

	printf("\n[Método Bisección]\n");
	printf("La raíz es: %.5f\n", c);
	printf("Error absoluto: %.5f\n", errorAbsoluto);
}

int main() {
	double a, b, tolerancia;
	
	// Cargar los datos
	a=0; //x_l=0
	b=1; //x_u=1
	tolerancia = 1e-4; //E = 1x10^-4
	
	// Llamar a los métodos
	biseccion(a, b, tolerancia);

	return 0;
}

