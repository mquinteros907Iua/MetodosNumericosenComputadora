#include <iostream>
#include <math.h>
#include <stdlib.h>

using namespace std;

// Definici�n de la funci�n f(x)
double f(double x) {
	return 5 * x - 3;
}

// M�todo de Falsa Posici�n (Regula-Falsi)
void falsaPosicion(double a, double b, double tolerancia) {
	if (f(a) * f(b) > 0) {
		cout << "Error: no hay cambio de signo en el intervalo [" << a << ", " << b << "]. No hay ra�z en el intervalo" << endl;
		return;
	}
	
	double errorAbsoluto, errorPorcentual, cViejo, c;
	cViejo = a;
	
	do {
		c = (a * f(b) - b * f(a)) / (f(b) - f(a));
		
		if (f(a) * f(c) > 0) {
			a = c;
		} else if (f(a) * f(c) < 0) {
			b = c;
		} else {
			printf("La ra�z es: %f\n", c);
			exit(0);
		}
		
		errorAbsoluto = fabs(c - cViejo);
		errorPorcentual = (fabs(errorAbsoluto / c)) * 100.0;
		cViejo = c;
		
	} while (errorAbsoluto > tolerancia);
	//}while(errorPorcentual > tolerancia)
	
	printf("\n[M�todo Falsa Posici�n]\n");
	printf("La ra�z es: %f\n", c);
	printf("Error absoluto: %f\n", errorAbsoluto);
	printf("Error porcentual: %f%%\n", errorPorcentual);
}

// M�todo de Bisecci�n
void biseccion(double a, double b, double tolerancia) {
	if (f(a) * f(b) > 0) {
		cout << "Error: no hay cambio de signo en el intervalo [" << a << ", " << b << "]. No hay ra�z en el intervalo" << endl;
		return;
	}
	
	double errorAbsoluto, errorPorcentual, cViejo, c;
	cViejo = a;
	
	do {
		c = (a + b) / 2;
		
		if (f(a) * f(c) > 0) {
			a = c;
		} else if (f(a) * f(c) < 0) {
			b = c;
		} else {
			printf("La ra�z es: %f\n", c);
			exit(0);
		}
		
		errorAbsoluto = fabs(c - cViejo);
		errorPorcentual = (fabs(errorAbsoluto / c)) * 100.0;
		cViejo = c;
		
	} while (errorAbsoluto > tolerancia);
	//}while(errorPorcentual > tolerancia)
	
	printf("\n[M�todo Bisecci�n]\n");
	printf("La ra�z es: %f\n", c);
	printf("Error absoluto: %f\n", errorAbsoluto);
	printf("Error porcentual: %f%%\n", errorPorcentual);
}

int main() {
	double a, b, tolerancia;
	
	// Cargar los datos
	cout << "Ingrese el extremo izquierdo (a): ";
	cin >> a;
	
	cout << "Ingrese el extremo derecho (b): ";
	cin >> b;
	
	cout << "Ingrese la tolerancia (por ejemplo 0.0001): ";
	cin >> tolerancia;
	
	// Llamar a los m�todos
	biseccion(a, b, tolerancia);
	falsaPosicion(a, b, tolerancia);
	
	return 0;
}

