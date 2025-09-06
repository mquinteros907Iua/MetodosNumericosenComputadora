import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import lagrange

# Leer puntos desde archivo txt
data = np.loadtxt("ejercicio3.txt")  # lee dos columnas
x_nodes = data[:,0]   # primera columna
y_nodes = data[:,1]   # segunda columna

# Polinomio interpolador de Lagrange
P = lagrange(x_nodes, y_nodes)

# Rango para graficar (entre el mínimo y máximo de x_nodes)
x_vals = np.linspace(min(x_nodes), max(x_nodes), 500)
P_vals = P(x_vals)

# --------- Gráfico de interpolación ----------
plt.figure(figsize=(8,6))
plt.plot(x_vals, P_vals, label="Polinomio de Lagrange", color="red")
plt.scatter(x_nodes, y_nodes, color="blue", zorder=5, label="Puntos del archivo")
plt.legend()
plt.title("Interpolación de Lagrange con puntos del archivo")
plt.xlabel("x")
plt.ylabel("y")
plt.grid(True)
plt.show()
