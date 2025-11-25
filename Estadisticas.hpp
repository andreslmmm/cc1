#ifndef ESTADISTICAS_HPP
#define ESTADISTICAS_HPP

#include <vector>
#include <cmath>
#include <algorithm>

class Estadisticas {
public:
    static double calcularPromedio(const std::vector<double>& datos) {
        if (datos.empty()) return 0.0;
        double suma = 0.0;
        for (double d : datos) suma += d;
        return suma / datos.size();
    }

    static double calcularDesviacion(const std::vector<double>& datos) {
        if (datos.size() < 2) return 0.0;
        double promedio = calcularPromedio(datos);
        double sumaCuadrados = 0.0;
        for (double d : datos) {
            double diferencia = d - promedio;
            sumaCuadrados += diferencia * diferencia;
        }
        return std::sqrt(sumaCuadrados / (datos.size() - 1));
    }

    static double encontrarMinimo(const std::vector<double>& datos) {
        if (datos.empty()) return 0.0;
        return *std::min_element(datos.begin(), datos.end());
    }

    static double encontrarMaximo(const std::vector<double>& datos) {
        if (datos.empty()) return 0.0;
        return *std::max_element(datos.begin(), datos.end());
    }

    static double calcularRango(const std::vector<double>& datos) {
        if (datos.empty()) return 0.0;
        return encontrarMaximo(datos) - encontrarMinimo(datos);
    }

    static double calcularMediana(std::vector<double> datos) {
        if (datos.empty()) return 0.0;
        std::sort(datos.begin(), datos.end());
        if (datos.size() % 2 == 1) {
            return datos[datos.size() / 2];
        }
        return (datos[datos.size() / 2 - 1] + datos[datos.size() / 2]) / 2.0;
    }
};

#endif
