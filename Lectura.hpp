#ifndef LECTURA_HPP
#define LECTURA_HPP

#include <string>
#include <ctime>

// Clase para almacenar una lectura de sensor
class Lectura {
public:
    time_t timestamp;
    std::string sensorID;
    double valor;
    std::string estado; // "normal", "alerta", "critico"

    Lectura() : timestamp(0), sensorID(""), valor(0.0), estado("normal") {}

    Lectura(time_t t, std::string id, double v, std::string est = "normal") 
        : timestamp(t), sensorID(id), valor(v), estado(est) {}

    bool operator<(const Lectura& otra) const {
        return timestamp < otra.timestamp;
    }

    bool operator>(const Lectura& otra) const {
        return timestamp > otra.timestamp;
    }

    bool operator==(const Lectura& otra) const {
        return timestamp == otra.timestamp && sensorID == otra.sensorID;
    }
};

#endif
