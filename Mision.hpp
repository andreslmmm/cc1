#ifndef MISION_HPP
#define MISION_HPP

#include <string>
#include <vector>

enum TipoMision {
    MANTENER_TEMPERATURA,
    ALCANZAR_HUMEDAD,
    SOBREVIVIR_CICLOS,
    SIN_ALARMAS,
    RIEGO_OPTIMO,
    RECOLECTAR_DATOS
};

struct Mision {
    int id;
    std::string nombre;
    std::string descripcion;
    TipoMision tipo;
    int objetivoValor;
    int progreso;
    bool completada;
    int recompensaPuntos;
    double umbralExito; // Porcentaje de éxito requerido

    Mision(int _id, std::string _nombre, std::string _desc, TipoMision _tipo, 
           int _objetivo, int _puntos, double _umbral = 0.8)
        : id(_id), nombre(_nombre), descripcion(_desc), tipo(_tipo), 
          objetivoValor(_objetivo), progreso(0), completada(false), 
          recompensaPuntos(_puntos), umbralExito(_umbral) {}

    double getProgreso() const {
        if (objetivoValor <= 0) return 0.0;
        return (double)progreso / objetivoValor;
    }

    bool verificarComplecion() {
        if (getProgreso() >= umbralExito) {
            completada = true;
            return true;
        }
        return false;
    }
};

#endif
