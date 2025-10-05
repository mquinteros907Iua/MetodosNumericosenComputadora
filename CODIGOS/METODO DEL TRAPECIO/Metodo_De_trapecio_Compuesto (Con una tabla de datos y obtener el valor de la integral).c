#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* === ATENCION: spline_eval(x) es un marcador de posicion.
   Reemplazar esta funcion por la que devuelve S(x) desde el codigo de spline. ===  (Al final del codigo)*/
double spline_eval(double x);

/* === ATENCION: si cambia el formato de datos.txt, ajustar leer_tabla_desde_archivo(). === */
int  leer_tabla_desde_archivo(const char *nombre, double **x, double **y, int *puntos);
int  leer_tabla_manual(double **x, double **y, int *puntos);
void ordenar_por_x(double *x, double *y, int puntos);
void imprimir_tabla(const double *x, const double *y, int puntos);
int  x_estrictamente_crecientes(const double *x, int puntos);

void generar_equidistantes(double a, double b, int n, double *X);
void evaluar_funcion_en_tabla(const double *X, double *Y, int puntos);

double trapecio_compuesto_tabla(const double *X, const double *Y, int puntos, int mostrar_traza,  double *error_abs_final_iter, double *error_pct_final_iter);
double error_teorico_trapecio(const double *X, const double *Y, int puntos);

int main(void) {
    while (1) {
        printf("\n========== MENU (Trapecio Compuesto con funcion de SPLINE) ==========\n");
        printf("1) Leer tabla desde archivo \"datos.txt\"\n");
        printf("2) Cargar tabla manualmente\n");
        printf("0) Salir\n");
        printf("Opcion: ");
        int opcion;
        if (scanf("%d", &opcion) != 1) return 0;

        if (opcion == 0) {
            printf("Fin.\n");
            return 0;
        }
        if (opcion != 1 && opcion != 2) {
            printf("Opcion invalida.\n");
            continue;
        }

        double *x_tabla = NULL;
        double *y_tabla = NULL;
        int puntos_tabla = 0;

        if (opcion == 1) {
            if (!leer_tabla_desde_archivo("datos.txt", &x_tabla, &y_tabla, &puntos_tabla)) {
                continue;
            }
        } else {
            if (!leer_tabla_manual(&x_tabla, &y_tabla, &puntos_tabla)) {
                continue;
            }
        }

        if (puntos_tabla < 2) {
            printf("ADVERTENCIA: se necesitan al menos 2 puntos.\n");
            free(x_tabla);
            free(y_tabla);
            continue;
        }

        ordenar_por_x(x_tabla, y_tabla, puntos_tabla);
        printf("---------------------------------------------------------------\n");
        printf("Tabla cargada (ordenada por x):\n");
        imprimir_tabla(x_tabla, y_tabla, puntos_tabla);

        if (!x_estrictamente_crecientes(x_tabla, puntos_tabla)) {
            printf("No se puede continuar: los x deben ser estrictamente crecientes.\n");
            free(x_tabla); free(y_tabla);
            continue;
        }

        double a = x_tabla[0];
        double b = x_tabla[puntos_tabla - 1];

        int n;
        printf("Ingrese n (cantidad de subintervalos, n >= 1): ");
        if (scanf("%d", &n) != 1 || n < 1) {
            printf("Valor de n invalido.\n");
            free(x_tabla); free(y_tabla);
            continue;
        }

        int puntos = n + 1;
        double *X = (double *)malloc(puntos * sizeof(double));
        double *Y = (double *)malloc(puntos * sizeof(double));
        if (X == NULL || Y == NULL) {
            printf("Error: sin memoria para la malla equiespaciada.\n");
            free(x_tabla); free(y_tabla);
            if (X) free(X);
            if (Y) free(Y);
            continue;
        }

        generar_equidistantes(a, b, n, X);
        evaluar_funcion_en_tabla(X, Y, puntos);

        int mostrar_traza = 0;
        printf("Mostrar traza de iteraciones? 1=si, 0=no: ");
        if (scanf("%d", &mostrar_traza) != 1) {
            free(x_tabla); free(y_tabla); free(X); free(Y);
            return 0;
        }

        double error_abs_final = 0.0;
        double error_pct_final = 0.0;
        double integral_aprox = trapecio_compuesto_tabla(X, Y, puntos, mostrar_traza, &error_abs_final, &error_pct_final);

        double error_teorico = error_teorico_trapecio(X, Y, puntos);

        double h = (b - a) / (double)n;
        printf("---------------------------------------------------------------\n");
        printf("Resultado (Trapecio compuesto con spline_eval):\n");
        printf("  a = %.12lf\n", a);
        printf("  b = %.12lf\n", b);
        printf("  n = %d\n", n);
        printf("  h = %.12lf\n", h);
        printf("  Integral aproximada = %.12lf\n", integral_aprox);
        printf("  Error del metodo (E) = %.12lf\n", error_teorico);

        printf("---------------------------------------------------------------\n");
        printf("Errores basados en iteraciones (respecto a la anterior):\n");
        printf("  Error absoluto final (iter)    = %.12lf\n", error_abs_final);
        printf("  Error porcentual final (iter)  = %.12lf %%\n", error_pct_final);

        free(x_tabla); free(y_tabla);
        free(X); free(Y);
    }
}

/* === IMPLEMENTACIONES === */

int leer_tabla_desde_archivo(const char *nombre, double **x, double **y, int *puntos) {
    FILE *f = fopen(nombre, "r");
    if (f == NULL) {
        printf("Error: no se pudo abrir \"%s\".\n", nombre);
        printf("Ejemplo de datos.txt:\n");
        printf("6\n");
        printf("0.0 1.0\n");
        printf("1.0 2.0\n");
        printf("2.0 5.0\n");
        printf("3.0 10.0\n");
        printf("4.0 17.0\n");
        printf("5.0 26.0\n");
        return 0;
    }

    int cant;
    if (fscanf(f, "%d", &cant) != 1) {
        printf("Error: encabezado invalido (se esperaba cantidad de puntos).\n");
        fclose(f);
        return 0;
    }
    if (cant < 2) {
        printf("Error: se necesitan al menos 2 puntos.\n");
        fclose(f);
        return 0;
    }

    double *tx = (double *)malloc(cant * sizeof(double));
    double *ty = (double *)malloc(cant * sizeof(double));
    if (tx == NULL || ty == NULL) {
        printf("Error: sin memoria para leer la tabla.\n");
        fclose(f);
        if (tx) free(tx);
        if (ty) free(ty);
        return 0;
    }

    for (int i = 0; i < cant; i = i + 1) {
        if (fscanf(f, "%lf %lf", &tx[i], &ty[i]) != 2) {
            printf("Error: formato invalido en la linea %d.\n", i + 2);
            fclose(f);
            free(tx); free(ty);
            return 0;
        }
    }
    fclose(f);

    ordenar_por_x(tx, ty, cant);

    *x = tx;
    *y = ty;
    *puntos = cant;
    return 1;
}

int leer_tabla_manual(double **x, double **y, int *puntos) {
    int cant;
    printf("Ingrese cantidad de puntos (al menos 2): ");
    if (scanf("%d", &cant) != 1) return 0;
    if (cant < 2) {
        printf("Error: se necesitan al menos 2 puntos.\n");
        return 0;
    }

    double *tx = (double *)malloc(cant * sizeof(double));
    double *ty = (double *)malloc(cant * sizeof(double));
    if (tx == NULL || ty == NULL) {
        printf("Error: sin memoria para la tabla.\n");
        if (tx) free(tx);
        if (ty) free(ty);
        return 0;
    }

    for (int i = 0; i < cant; i = i + 1) {
        printf("x[%d] y[%d]: ", i, i);
        if (scanf("%lf %lf", &tx[i], &ty[i]) != 2) {
            printf("Error de lectura.\n");
            free(tx); free(ty);
            return 0;
        }
    }

    ordenar_por_x(tx, ty, cant);

    *x = tx;
    *y = ty;
    *puntos = cant;
    return 1;
}

void ordenar_por_x(double *x, double *y, int puntos) {
    for (int i = 0; i < puntos - 1; i = i + 1) {
        for (int j = 0; j < puntos - 1 - i; j = j + 1) {
            if (x[j] > x[j + 1]) {
                double tx = x[j]; x[j] = x[j + 1]; x[j + 1] = tx;
                double ty = y[j]; y[j] = y[j + 1]; y[j + 1] = ty;
            }
        }
    }
}

void imprimir_tabla(const double *x, const double *y, int puntos) {
    printf("%-10s %-18s %-18s\n", "i", "x[i]", "y[i]");
    for (int i = 0; i < puntos; i = i + 1) {
        printf("%-10d %-18.12lf %-18.12lf\n", i, x[i], y[i]);
    }
}

int x_estrictamente_crecientes(const double *x, int puntos) {
    for (int i = 0; i < puntos - 1; i = i + 1) {
        if (!(x[i] < x[i + 1])) {
            printf("x[%d] = %.12lf y x[%d] = %.12lf no cumplen x[i] < x[i+1].\n",
                   i, x[i], i + 1, x[i + 1]);
            return 0;
        }
    }
    return 1;
}

void generar_equidistantes(double a, double b, int n, double *X) {
    double h = (b - a) / (double)n;
    for (int i = 0; i <= n; i = i + 1) {
        X[i] = a + i * h;
    }
}

void evaluar_funcion_en_tabla(const double *X, double *Y, int puntos) {
    for (int i = 0; i < puntos; i = i + 1) {
        Y[i] = spline_eval(X[i]);
    }
}

double trapecio_compuesto_tabla(const double *X, const double *Y, int puntos, int mostrar_traza,  double *error_abs_final_iter, double *error_pct_final_iter) {
    if (puntos < 2) {
        if (error_abs_final_iter) { *error_abs_final_iter = 0.0; }
        if (error_pct_final_iter) { *error_pct_final_iter = 0.0; }
        return 0.0;
    }

    int n = puntos - 1;
    double a = X[0];
    double b = X[n];

    double suma = Y[0] + Y[n];

    double integral_parcial_anterior = 0.0;
    int hay_integral_anterior = 0;

    if (mostrar_traza == 1) {
        printf("%-12s %-15s %-18s %-18s %-18s %-14s %-14s\n", "iteraciones", "x_i", "y_i", "suma_parcial", "Integral_parcial", "err_abs", "err_pct(%)");
        printf("---------------------------------------------------------------\n");
    }

    for (int i = 1; i <= n - 1; i = i + 1) {
        suma = suma + 2.0 * Y[i];

        double integral_parcial = (b - a) * suma / (2.0 * (double)n);

        if (mostrar_traza == 1) {
            if (hay_integral_anterior == 0) {
                printf("%-12d %-15.12lf %-18.12lf %-18.12lf %-18.12lf %-14s %-14s\n",
                       i, X[i], Y[i], suma, integral_parcial, "-", "-");
            } else {
                double error_abs = integral_parcial - integral_parcial_anterior;
                if (error_abs < 0.0) error_abs = -error_abs;

                double error_pct;
                if (integral_parcial != 0.0) {
                    error_pct = (integral_parcial - integral_parcial_anterior) / integral_parcial;
                    if (error_pct < 0.0) error_pct = -error_pct;
                    error_pct = error_pct * 100.0;
                } else {
                    error_pct = 0.0;
                }

                printf("%-12d %-15.12lf %-18.12lf %-18.12lf %-18.12lf %-14.12lf %-14.12lf\n",
                       i, X[i], Y[i], suma, integral_parcial, error_abs, error_pct);

                if (error_abs_final_iter) { *error_abs_final_iter = error_abs; }
                if (error_pct_final_iter) { *error_pct_final_iter = error_pct; }
            }
        } else {
            if (hay_integral_anterior == 1) {
                double error_abs = integral_parcial - integral_parcial_anterior;
                if (error_abs < 0.0) error_abs = -error_abs;

                double error_pct;
                if (integral_parcial != 0.0) {
                    error_pct = (integral_parcial - integral_parcial_anterior) / integral_parcial;
                    if (error_pct < 0.0) error_pct = -error_pct;
                    error_pct = error_pct * 100.0;
                } else {
                    error_pct = 0.0;
                }

                if (error_abs_final_iter) { *error_abs_final_iter = error_abs; }
                if (error_pct_final_iter) { *error_pct_final_iter = error_pct; }
            }
        }

        integral_parcial_anterior = integral_parcial;
        hay_integral_anterior = 1;
    }

    double integral = (b - a) * suma / (2.0 * (double)n);						//Integral del metodo
    return integral;
}

double error_teorico_trapecio(const double *X, const double *Y, int puntos) {
    if (puntos < 2) {
        return 0.0;
    }
    int n = puntos - 1;
    double a = X[0];
    double b = X[n];
    double h = (b - a) / (double)n;

    double fp_a = 0.0;
    double fp_b = 0.0;

    if (n >= 1) {
        fp_a = (Y[1] - Y[0]) / h;
        fp_b = (Y[n] - Y[n - 1]) / h;
    }

    double E = ((b - a) * (b - a)) / (12.0 * (double)n * (double)n) * (fp_b - fp_a);				//Error del metodo
    return E;
}


/* === ATENCION: reemplazar por la evaluacion real del spline S(x) (por el polinimio). === */
double spline_eval(double x) {
    return x * x + 1.0;
}
