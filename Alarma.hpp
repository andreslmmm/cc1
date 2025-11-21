#ifndef ALARMA_HPP
#define ALARMA_HPP

#include <string>
#include <ctime>

// Clase para representar alarmas del sistema
class Alarma {
public:
    int prioridad; // 1=Crítica, 2=Alta, 3=Media, 4=Baja
    time_t timestamp;
    std::string tipo;
    std::string mensaje;
    std::string sensorID;
    double valor;
    bool resuelta;

    Alarma() : prioridad(4), timestamp(0), tipo(""), mensaje(""), 
               sensorID(""), valor(0.0), resuelta(false) {}

    Alarma(int p, std::string t, std::string msg, std::string sid = "", double v = 0.0)
        : prioridad(p), tipo(t), mensaje(msg), sensorID(sid), 
          valor(v), resuelta(false) {
        timestamp = time(nullptr);
    }

    // Operadores para comparación en heap (menor prioridad = mayor urgencia)
    bool operator<(const Alarma& otra) const {
        if (prioridad != otra.prioridad)
            return prioridad > otra.prioridad; // Invertido para min-heap
        return timestamp > otra.timestamp;
    }

    bool operator>(const Alarma& otra) const {
        if (prioridad != otra.prioridad)
            return prioridad < otra.prioridad;
        return timestamp < otra.timestamp;
    }

    std::string getNivelPrioridad() const {
        switch(prioridad) {
            case 1: return "CRITICA";
            case 2: return "ALTA";
            case 3: return "MEDIA";
            case 4: return "BAJA";
            default: return "DESCONOCIDA";
        }
    }

    void resolver() {
        resuelta = true;
    }
};

#endif