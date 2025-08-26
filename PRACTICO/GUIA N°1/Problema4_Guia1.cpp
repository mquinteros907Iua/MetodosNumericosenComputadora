#include <iostream>
#include <math.h>
#include <stdlib.h>

using namespace std;

// Definición de la función f(x)
double f(double a) {
	return pow(a,10) - 1;
}


// Método de Bisección
void biseccion(double a, double b, double tolerancia) {
	double errorAbsoluto=0.0, cViejo, c;
	int i = 0;
	cViejo = a;
	
	if (f(a) * f(b) > 0) {
		cout << "Error: no hay cambio de signo en el intervalo [" << a << ", " << b << "]." << endl;
		return;
	}
	
	do {
		c = (a + b) / 2;
		i++;
		if (f(a) * f(c) > 0) {
			a = c;
		} else if (f(a) * f(c) < 0) {
			b = c;
		} else {
			break;
		}
		
		errorAbsoluto = fabs(c - cViejo);
		cViejo = c;
		
	} while (errorAbsoluto > tolerancia);
	
	printf("\n[Método Bisección]\n");
	printf("Raíz aproximada: %.6f\n", c);
	printf("Error absoluto: %.6f\n", errorAbsoluto);
	printf("Iteraciones: %d\n", i);
}

int main() {
	double a, b, tolerancia;
	
	// Cargar los datos raices(x=-1 , x=1)
	cout << "Ingrese el extremo izquierdo (a): ";
	cin >> a;
	
	cout << "Ingrese el extremo derecho (b): ";
	cin >> b;
	
	tolerancia = 1e-5; //E = 1x10^-5
	printf("Tolerancia: %.5f\n", tolerancia);
	
	// Llamar a los métodos
	biseccion(a, b, tolerancia);
	
	return 0;
}
