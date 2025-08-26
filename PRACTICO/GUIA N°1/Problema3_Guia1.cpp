#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <cmath>

using namespace std;

// Definición de la función f(x)
double f(double m) {
	double g = 9.81;
	double c = 14.0;
	double t = 7.0;
	double v = 35.0;
	return ((g * m / c) * (1 - exp(- (c * t) / m)) ) - v;
}

// Método de Falsa Posición (Regula-Falsi)
void falsaPosicion(double a, double b, double tolerancia) {
	double errorAbsoluto=0.0, cViejo, c;
	int i = 0;
	cViejo = a;
	
	if (f(a) * f(b) > 0) {
		cout << "Error: no hay cambio de signo en el intervalo [" << a << ", " << b << "]." << endl;
		return;
	}
	
	do {
		c = (a * f(b) - b * f(a)) / (f(b) - f(a));
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
	
	printf("\n[Método Falsa Posición]\n");
	printf("Raíz aproximada: %.6f\n", c);
	printf("Error absoluto: %.6f\n", errorAbsoluto);
	printf("Iteraciones: %d\n", i);
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
	falsaPosicion(a, b, tolerancia);
	
	return 0;
}
