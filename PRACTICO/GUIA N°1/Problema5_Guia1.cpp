#include <iostream>
#include <math.h>
#include <stdlib.h>

using namespace std;

// Definición de la función f(x)
double n(double x){
	return (log(x) - (1.0 - 1.0/x)) / (log(x) + 1.5 * (1.0 - 1.0/x));
}
	
double f(double x) {
	return n(x) - 0.30;
}

// Método de Bisección
void biseccion(double a, double b, double tolerancia) {
	double errorAbsoluto, errorPorcentual, cViejo, c;
	cViejo = a;
	
	if (f(a) * f(b) > 0) {
		cout << "Error: no hay cambio de signo en el intervalo [" << a << ", " << b << "]. No hay raíz en el intervalo" << endl;
		return;
	}
	
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
	printf("La raíz es: %.7f\n", c);
	printf("Error absoluto: %.7f\n", errorAbsoluto);
}

int main() {
	double a, b, tolerancia;
	
	// Cargar los datos
	cout << "Ingrese el extremo izquierdo (a): ";
	cin >> a;
	
	cout << "Ingrese el extremo derecho (b): ";
	cin >> b;
	
	tolerancia = 1e-6; 
	biseccion(a, b, tolerancia);
	
	return 0;
}
