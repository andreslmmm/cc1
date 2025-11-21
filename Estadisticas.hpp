#ifndef ESTADISTICAS_HPP
#define ESTADISTICAS_HPP

#include <vector>
#include <cmath>
#include <algorithm>

class Estadisticas {
public:
    // Calcular promedio - O(n)
    static double calcularPromedio(const std::vector<double>& datos) {
        if (datos.empty()) return 0.0;
        double suma = 0.0;
        for (double valor : datos) {
            suma += valor;
        }
        return suma / datos.size();
    }

    // Calcular desviación estándar - O(n)
    static double calcularDesviacion(const std::vector<double>& datos) {
        if (datos.size() < 2) return 0.0;
        double promedio = calcularPromedio(datos);
        double sumaDesviaciones = 0.0;
        for (double valor : datos) {
            sumaDesviaciones += (valor - promedio) * (valor - promedio);
        }
        return sqrt(sumaDesviaciones / (datos.size() - 1));
    }

    // Encontrar mínimo - O(n)
    static double encontrarMinimo(const std::vector<double>& datos) {
        if (datos.empty()) return 0.0;
        return *std::min_element(datos.begin(), datos.end());
    }

    // Encontrar máximo - O(n)
    static double encontrarMaximo(const std::vector<double>& datos) {
        if (datos.empty()) return 0.0;
        return *std::max_element(datos.begin(), datos.end());
    }

    // Calcular promedio móvil - O(n)
    static std::vector<double> promedioMovil(const std::vector<double>& datos, int ventana) {
        std::vector<double> resultado;
        if (datos.size() < ventana) return resultado;

        double suma = 0.0;
        // Calcular suma de primera ventana
        for (int i = 0; i < ventana; i++) {
            suma += datos[i];
        }
        resultado.push_back(suma / ventana);

        // Ventana deslizante
        for (size_t i = ventana; i < datos.size(); i++) {
            suma = suma - datos[i - ventana] + datos[i];
            resultado.push_back(suma / ventana);
        }

        return resultado;
    }

    // Detectar picos (valores que superan umbral) - O(n)
    static std::vector<int> detectarPicos(const std::vector<double>& datos, double umbral) {
        std::vector<int> indices;
        for (size_t i = 0; i < datos.size(); i++) {
            if (datos[i] > umbral) {
                indices.push_back(i);
            }
        }
        return indices;
    }

    // Detectar anomalías (valores fuera de ±2σ) - O(n)
    static std::vector<int> detectarAnomalias(const std::vector<double>& datos) {
        std::vector<int> indices;
        if (datos.size() < 2) return indices;

        double promedio = calcularPromedio(datos);
        double desviacion = calcularDesviacion(datos);
        double limiteInf = promedio - 2 * desviacion;
        double limiteSup = promedio + 2 * desviacion;

        for (size_t i = 0; i < datos.size(); i++) {
            if (datos[i] < limiteInf || datos[i] > limiteSup) {
                indices.push_back(i);
            }
        }
        return indices;
    }

    // Calcular regresión lineal simple (pendiente) - O(n)
    static double calcularTendencia(const std::vector<double>& datos) {
        if (datos.size() < 2) return 0.0;

        int n = datos.size();
        double sumaX = 0, sumaY = 0, sumaXY = 0, sumaX2 = 0;

        for (int i = 0; i < n; i++) {
            sumaX += i;
            sumaY += datos[i];
            sumaXY += i * datos[i];
            sumaX2 += i * i;
        }

        // Pendiente = (n*ΣXY - ΣX*ΣY) / (n*ΣX² - (ΣX)²)
        double pendiente = (n * sumaXY - sumaX * sumaY) / (n * sumaX2 - sumaX * sumaX);
        return pendiente;
    }

    // QuickSort para ordenamiento - O(n log n) promedio
    static void quickSort(std::vector<double>& arr, int inicio, int fin) {
        if (inicio < fin) {
            int pivote = particionar(arr, inicio, fin);
            quickSort(arr, inicio, pivote - 1);
            quickSort(arr, pivote + 1, fin);
        }
    }

private:
    static int particionar(std::vector<double>& arr, int inicio, int fin) {
        double pivote = arr[fin];
        int i = inicio - 1;

        for (int j = inicio; j < fin; j++) {
            if (arr[j] < pivote) {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[fin]);
        return i + 1;
    }
};

#endif