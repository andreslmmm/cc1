#ifndef SENSOR_HPP
#define SENSOR_HPP

#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <random>

// Clase base abstracta para sensores
class Sensor {
protected:
    std::string id;
    std::string tipo;
    std::string unidad;
    double valorActual;
    double rangoMin;
    double rangoMax;
    double umbralAlerta;
    double umbralCritico;
    double tasaEvaporacion;  // para simular evaporación más realista
    double tasaConsumo;      // para consumo de agua/nutrientes

public:
    Sensor(std::string _id, std::string _tipo, std::string _unidad, 
           double _min, double _max, double _alerta, double _critico)
        : id(_id), tipo(_tipo), unidad(_unidad), valorActual(0),
          rangoMin(_min), rangoMax(_max), umbralAlerta(_alerta), 
          umbralCritico(_critico), tasaEvaporacion(0.0), tasaConsumo(0.0) {}

    virtual ~Sensor() {}

    // Método virtual puro para leer sensor (simulado)
    virtual double leer() = 0;

    std::string getID() const { return id; }
    std::string getTipo() const { return tipo; }
    std::string getUnidad() const { return unidad; }
    double getValorActual() const { return valorActual; }
    double getRangoMin() const { return rangoMin; }
    double getRangoMax() const { return rangoMax; }

    std::string evaluarEstado() {
        if (valorActual >= umbralCritico || valorActual <= (rangoMin + (rangoMax - rangoMin) * 0.05))
            return "critico";
        else if (valorActual >= umbralAlerta)
            return "alerta";
        return "normal";
    }
};

// Sensor de temperatura mejorado
class SensorTemperatura : public Sensor {
private:
    double deriva;
    double cicloAmbiente;  // para simular día/noche

public:
    SensorTemperatura(std::string _id, double tempInicial = 25.0)
        : Sensor(_id, "Temperatura", "°C", 0.0, 50.0, 35.0, 40.0), 
          deriva(0), cicloAmbiente(0) {
        valorActual = tempInicial;
        srand(time(nullptr));
    }

    double leer() override {
        time_t ahora = time(nullptr);
        struct tm* tiempo = localtime(&ahora);
        int hora = tiempo->tm_hour;
        
        // Ciclo día/noche: más calor durante el día
        double cicloTemp = 5.0 * sin((hora - 6) * 3.14159 / 12.0);
        if (hora < 6 || hora > 18) cicloTemp = -3.0;
        
        double variacion = ((rand() % 100) - 50) / 100.0;
        deriva += ((rand() % 100) - 50) / 1000.0;
        
        valorActual += (cicloTemp * 0.01) + variacion + (deriva * 0.5);

        if (valorActual < rangoMin) valorActual = rangoMin;
        if (valorActual > rangoMax) valorActual = rangoMax;

        return valorActual;
    }

    std::string getID() const { return id; }
    std::string getTipo() const { return tipo; }
    std::string getUnidad() const { return unidad; }
    double getValorActual() const { return valorActual; }
    double getRangoMin() const { return rangoMin; }
    double getRangoMax() const { return rangoMax; }

    std::string evaluarEstado() {
        if (valorActual >= umbralCritico || valorActual <= (rangoMin + (rangoMax - rangoMin) * 0.05))
            return "critico";
        else if (valorActual >= umbralAlerta)
            return "alerta";
        return "normal";
    }

    void aplicarControlCalor(double ajuste) {
        valorActual += ajuste;
        if (valorActual < rangoMin) valorActual = rangoMin;
        if (valorActual > rangoMax) valorActual = rangoMax;
    }
};

// Sensor de humedad mejorado
class SensorHumedad : public Sensor {
private:
    double tasaEvaporacionBase;
    bool esSuelo;

public:
    SensorHumedad(std::string _id, double humInicial = 70.0, bool _esSuelo = false)
        : Sensor(_id, _esSuelo ? "Humedad Suelo" : "Humedad Relativa", "%", 
                 0.0, 100.0, 85.0, 95.0), tasaEvaporacionBase(0.15), esSuelo(_esSuelo) {
        valorActual = humInicial;
    }

    double leer() override {
        double factor = esSuelo ? 0.08 : 0.12;
        double variacion = ((rand() % 100) - 50) / 200.0;
        valorActual -= (tasaEvaporacionBase * factor) + variacion;

        if (valorActual < rangoMin) valorActual = rangoMin;
        if (valorActual > rangoMax) valorActual = rangoMax;

        return valorActual;
    }

    std::string getID() const { return id; }
    std::string getTipo() const { return tipo; }
    std::string getUnidad() const { return unidad; }
    double getValorActual() const { return valorActual; }
    double getRangoMin() const { return rangoMin; }
    double getRangoMax() const { return rangoMax; }

    std::string evaluarEstado() {
        if (valorActual >= umbralCritico || valorActual <= (rangoMin + (rangoMax - rangoMin) * 0.05))
            return "critico";
        else if (valorActual >= umbralAlerta)
            return "alerta";
        return "normal";
    }

    void aplicarRiego(double cantidad) {
        valorActual += cantidad;
        if (valorActual > rangoMax) valorActual = rangoMax;
    }
};

// Sensor de luz mejorado
class SensorLuz : public Sensor {
public:
    SensorLuz(std::string _id, double luzInicial = 45000.0)
        : Sensor(_id, "Luz", "Lux", 0.0, 100000.0, 80000.0, 95000.0) {
        valorActual = luzInicial;
    }

    double leer() override {
        time_t ahora = time(nullptr);
        struct tm* tiempo = localtime(&ahora);
        int hora = tiempo->tm_hour;

        if (hora >= 6 && hora <= 18) {
            int cicloHora = hora - 6;
            // Máximo al mediodía (hora 12)
            double factorLuz = sin((cicloHora * 3.14159) / 12.0);
            valorActual = 20000 + (factorLuz * 50000) + (rand() % 5000);
        } else {
            valorActual = rand() % 2000;  // Luz nocturna mínima
        }

        return valorActual;
    }

    std::string getID() const { return id; }
    std::string getTipo() const { return tipo; }
    std::string getUnidad() const { return unidad; }
    double getValorActual() const { return valorActual; }
    double getRangoMin() const { return rangoMin; }
    double getRangoMax() const { return rangoMax; }

    std::string evaluarEstado() {
        if (valorActual >= umbralCritico || valorActual <= (rangoMin + (rangoMax - rangoMin) * 0.05))
            return "critico";
        else if (valorActual >= umbralAlerta)
            return "alerta";
        return "normal";
    }
};

// Sensor de pH
class SensorPH : public Sensor {
public:
    SensorPH(std::string _id, double phInicial = 6.5)
        : Sensor(_id, "pH", "pH", 0.0, 14.0, 8.0, 9.0) {
        valorActual = phInicial;
    }

    double leer() override {
        // Simulación: cambios muy lentos
        double variacion = ((rand() % 100) - 50) / 500.0;
        valorActual += variacion;

        if (valorActual < rangoMin) valorActual = rangoMin;
        if (valorActual > rangoMax) valorActual = rangoMax;

        return valorActual;
    }

    std::string getID() const { return id; }
    std::string getTipo() const { return tipo; }
    std::string getUnidad() const { return unidad; }
    double getValorActual() const { return valorActual; }
    double getRangoMin() const { return rangoMin; }
    double getRangoMax() const { return rangoMax; }

    std::string evaluarEstado() {
        if (valorActual >= umbralCritico || valorActual <= (rangoMin + (rangoMax - rangoMin) * 0.05))
            return "critico";
        else if (valorActual >= umbralAlerta)
            return "alerta";
        return "normal";
    }
};

// Sensor de CO2
class SensorCO2 : public Sensor {
public:
    SensorCO2(std::string _id, double co2Inicial = 450.0)
        : Sensor(_id, "CO2", "ppm", 0.0, 2000.0, 1200.0, 1800.0) {
        valorActual = co2Inicial;
    }

    double leer() override {
        double variacion = ((rand() % 100) - 50) / 10.0;
        valorActual += variacion;

        if (valorActual < rangoMin) valorActual = rangoMin;
        if (valorActual > rangoMax) valorActual = rangoMax;

        return valorActual;
    }

    std::string getID() const { return id; }
    std::string getTipo() const { return tipo; }
    std::string getUnidad() const { return unidad; }
    double getValorActual() const { return valorActual; }
    double getRangoMin() const { return rangoMin; }
    double getRangoMax() const { return rangoMax; }

    std::string evaluarEstado() {
        if (valorActual >= umbralCritico || valorActual <= (rangoMin + (rangoMax - rangoMin) * 0.05))
            return "critico";
        else if (valorActual >= umbralAlerta)
            return "alerta";
        return "normal";
    }
};

// Sensor de nivel de agua
class SensorNivelAgua : public Sensor {
private:
    double consumo;

public:
    SensorNivelAgua(std::string _id, double nivelInicial = 500.0)
        : Sensor(_id, "Nivel Agua", "L", 0.0, 1000.0, 200.0, 50.0), consumo(0.5) {
        valorActual = nivelInicial;
    }

    double leer() override {
        // Simulación: consumo gradual
        valorActual -= consumo;
        if (valorActual < rangoMin) valorActual = rangoMin;

        return valorActual;
    }

    void rellenar(double cantidad) {
        valorActual += cantidad;
        if (valorActual > rangoMax) valorActual = rangoMax;
    }

    std::string getID() const { return id; }
    std::string getTipo() const { return tipo; }
    std::string getUnidad() const { return unidad; }
    double getValorActual() const { return valorActual; }
    double getRangoMin() const { return rangoMin; }
    double getRangoMax() const { return rangoMax; }

    std::string evaluarEstado() {
        if (valorActual >= umbralCritico || valorActual <= (rangoMin + (rangoMax - rangoMin) * 0.05))
            return "critico";
        else if (valorActual >= umbralAlerta)
            return "alerta";
        return "normal";
    }
};

#endif
