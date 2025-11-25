#ifndef ACTUADOR_HPP
#define ACTUADOR_HPP

#include <string>
#include <iostream>
#include <algorithm>

// Clase base abstracta para actuadores
class Actuador {
protected:
    std::string id;
    std::string tipo;
    bool activo;
    double intensidad; // 0-100%
    double efectoTemp; // Efecto en temperatura (�C por ciclo)
    double efectoHumedad; // Efecto en humedad (% por ciclo)
    double efectoHumedadRelativa; // Efecto en humedad relativa (% por ciclo)

public:
    Actuador(std::string _id, std::string _tipo, double _efectoTemp = 0.0, double _efectoHumedad = 0.0, double _efectoHumedadRelativa = 0.0)
        : id(_id), tipo(_tipo), activo(false), intensidad(0.0), efectoTemp(_efectoTemp), efectoHumedad(_efectoHumedad), efectoHumedadRelativa(_efectoHumedadRelativa) {}

    virtual ~Actuador() {}

    virtual void activar() { activo = true; }
    virtual void desactivar() { activo = false; intensidad = 0.0; }
    virtual void ajustar(double porcentaje) {
        intensidad = std::max(0.0, std::min(100.0, porcentaje));
        if (intensidad > 0) activo = true;
        else activo = false;
    }

    std::string getEstado() const {
        if (!activo) return "OFF";
        return std::to_string((int)intensidad) + "%";
    }

    bool estaActivo() const { return activo; }
    double getIntensidad() const { return intensidad; }
    double getEfectoTemp() const { return efectoTemp * (intensidad / 100.0); }
    double getEfectoHumedad() const { return efectoHumedad * (intensidad / 100.0); }
    double getEfectoHumedadRelativa() const { return efectoHumedadRelativa * (intensidad / 100.0); }
    std::string getID() const { return id; }
};

// Actuadores espec�ficos
class Ventilador : public Actuador {
public:
    Ventilador(std::string _id) : Actuador(_id, "Ventilador", -0.5, 0.0, 0.0) {} // Enfr�a
};

class Calefactor : public Actuador {
public:
    Calefactor(std::string _id) : Actuador(_id, "Calefactor", 0.5, 0.0, 0.0) {} // Calienta
};

class SistemaRiego : public Actuador {
public:
    SistemaRiego(std::string _id) : Actuador(_id, "Sistema Riego", 0.0, 2.0, 0.0) {} // Aumenta humedad suelo
};

class LuzLED : public Actuador {
public:
    LuzLED(std::string _id) : Actuador(_id, "Luz LED", 0.1, 0.0, 0.0) {} // Ligero calentamiento
};

class Nebulizador : public Actuador {
public:
    Nebulizador(std::string _id) : Actuador(_id, "Nebulizador", 0.0, 0.0, 1.0) {} // Aumenta humedad relativa
};

#endif
