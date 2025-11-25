#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <string>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <iostream>
#include <iomanip>
#include <cmath>

// Representa un estado del invernadero
struct EstadoInvernadero {
    std::string nombre;
    std::string descripcion;
    std::map<std::string, double> configuracionActuadores;
    
    EstadoInvernadero(std::string n, std::string desc) 
        : nombre(n), descripcion(desc) {}
};

// Arista del grafo con condici�n de transici�n
struct Transicion {
    std::string estadoDestino;
    std::string condicion;
    int prioridad;
    
    Transicion(std::string dest, std::string cond, int prior = 1)
        : estadoDestino(dest), condicion(cond), prioridad(prior) {}
};

// Grafo de estados para control del invernadero
class GrafoEstados {
private:
    std::map<std::string, EstadoInvernadero*> estados;
    std::map<std::string, std::vector<Transicion>> transiciones;
    std::string estadoActual;
    std::vector<std::string> historialEstados;
    
    bool evaluarCondicionTransicion(std::string condicion, 
                                    std::map<std::string, double>& sensores) {
        // Similar a ArbolDecision, pero m�s espec�fico para transiciones
        size_t posOperador = condicion.find_first_of("<>=");
        if (posOperador == std::string::npos) return false;
        
        std::string sensor = condicion.substr(0, posOperador);
        sensor.erase(0, sensor.find_first_not_of(" \t"));
        sensor.erase(sensor.find_last_not_of(" \t") + 1);
        
        char operador = condicion[posOperador];
        double umbral = std::stod(condicion.substr(posOperador + 1));
        
        if (sensores.find(sensor) == sensores.end()) return false;
        
        double valorSensor = sensores[sensor];
        
        switch(operador) {
            case '>': return valorSensor > umbral;
            case '<': return valorSensor < umbral;
            case '=': return std::abs(valorSensor - umbral) < 0.1;
            default: return false;
        }
    }

public:
    GrafoEstados() {
        estadoActual = "NORMAL";
        construirGrafo();
    }
    
    ~GrafoEstados() {
        for (auto& par : estados) {
            delete par.second;
        }
    }
    
    void construirGrafo() {
        // Estado 1: Normal (operaci�n est�ndar)
        EstadoInvernadero* normal = new EstadoInvernadero(
            "NORMAL", "Condiciones �ptimas del invernadero");
        normal->configuracionActuadores["VENTILADOR"] = 20.0;
        normal->configuracionActuadores["CALEFACTOR"] = 0.0;
        normal->configuracionActuadores["RIEGO"] = 30.0;
        normal->configuracionActuadores["LUZ_LED"] = 50.0;
        normal->configuracionActuadores["NEBULIZADOR"] = 0.0;
        estados["NORMAL"] = normal;
        
        // Estado 2: Calor extremo
        EstadoInvernadero* calorExtremo = new EstadoInvernadero(
            "CALOR_EXTREMO", "Temperatura peligrosamente alta");
        calorExtremo->configuracionActuadores["VENTILADOR"] = 100.0;
        calorExtremo->configuracionActuadores["CALEFACTOR"] = 0.0;
        calorExtremo->configuracionActuadores["RIEGO"] = 80.0;
        calorExtremo->configuracionActuadores["LUZ_LED"] = 0.0;
        calorExtremo->configuracionActuadores["NEBULIZADOR"] = 100.0;
        estados["CALOR_EXTREMO"] = calorExtremo;
        
        // Estado 3: Fr�o extremo
        EstadoInvernadero* frioExtremo = new EstadoInvernadero(
            "FRIO_EXTREMO", "Temperatura peligrosamente baja");
        frioExtremo->configuracionActuadores["VENTILADOR"] = 0.0;
        frioExtremo->configuracionActuadores["CALEFACTOR"] = 100.0;
        frioExtremo->configuracionActuadores["RIEGO"] = 0.0;
        frioExtremo->configuracionActuadores["LUZ_LED"] = 80.0;
        frioExtremo->configuracionActuadores["NEBULIZADOR"] = 0.0;
        estados["FRIO_EXTREMO"] = frioExtremo;
        
        // Estado 4: Sequ�a
        EstadoInvernadero* sequia = new EstadoInvernadero(
            "SEQUIA", "Humedad del suelo cr�ticamente baja");
        sequia->configuracionActuadores["VENTILADOR"] = 10.0;
        sequia->configuracionActuadores["CALEFACTOR"] = 0.0;
        sequia->configuracionActuadores["RIEGO"] = 100.0;
        sequia->configuracionActuadores["LUZ_LED"] = 40.0;
        sequia->configuracionActuadores["NEBULIZADOR"] = 80.0;
        estados["SEQUIA"] = sequia;
        
        // Estado 5: Humedad excesiva
        EstadoInvernadero* humedad = new EstadoInvernadero(
            "HUMEDAD_ALTA", "Humedad excesiva - riesgo de hongos");
        humedad->configuracionActuadores["VENTILADOR"] = 80.0;
        humedad->configuracionActuadores["CALEFACTOR"] = 30.0;
        humedad->configuracionActuadores["RIEGO"] = 0.0;
        humedad->configuracionActuadores["LUZ_LED"] = 60.0;
        humedad->configuracionActuadores["NEBULIZADOR"] = 0.0;
        estados["HUMEDAD_ALTA"] = humedad;
        
        // Estado 6: Recuperaci�n
        EstadoInvernadero* recuperacion = new EstadoInvernadero(
            "RECUPERACION", "Retornando a condiciones normales");
        recuperacion->configuracionActuadores["VENTILADOR"] = 30.0;
        recuperacion->configuracionActuadores["CALEFACTOR"] = 10.0;
        recuperacion->configuracionActuadores["RIEGO"] = 40.0;
        recuperacion->configuracionActuadores["LUZ_LED"] = 50.0;
        recuperacion->configuracionActuadores["NEBULIZADOR"] = 20.0;
        estados["RECUPERACION"] = recuperacion;
        
        // Definir transiciones
        // Desde NORMAL
        transiciones["NORMAL"].push_back(Transicion("CALOR_EXTREMO", "TEMP>35", 1));
        transiciones["NORMAL"].push_back(Transicion("FRIO_EXTREMO", "TEMP<15", 1));
        transiciones["NORMAL"].push_back(Transicion("SEQUIA", "HUM_SUELO<40", 2));
        transiciones["NORMAL"].push_back(Transicion("HUMEDAD_ALTA", "HUM_REL>85", 2));
        
        // Desde CALOR_EXTREMO
        transiciones["CALOR_EXTREMO"].push_back(Transicion("RECUPERACION", "TEMP<32", 1));
        transiciones["CALOR_EXTREMO"].push_back(Transicion("SEQUIA", "HUM_SUELO<30", 2));
        
        // Desde FRIO_EXTREMO
        transiciones["FRIO_EXTREMO"].push_back(Transicion("RECUPERACION", "TEMP>18", 1));
        
        // Desde SEQUIA
        transiciones["SEQUIA"].push_back(Transicion("RECUPERACION", "HUM_SUELO>55", 1));
        transiciones["SEQUIA"].push_back(Transicion("CALOR_EXTREMO", "TEMP>35", 2));
        
        // Desde HUMEDAD_ALTA
        transiciones["HUMEDAD_ALTA"].push_back(Transicion("RECUPERACION", "HUM_REL<75", 1));
        
        // Desde RECUPERACION
        transiciones["RECUPERACION"].push_back(Transicion("NORMAL", "TEMP>20", 1));
    }
    
    // Evaluar y cambiar de estado si es necesario
    std::string evaluarTransiciones(std::map<std::string, double>& sensores) {
        std::string nuevoEstado = estadoActual;
        int maxPrioridad = 999;
        
        // Buscar transiciones desde estado actual
        if (transiciones.find(estadoActual) != transiciones.end()) {
            for (const auto& trans : transiciones[estadoActual]) {
                if (evaluarCondicionTransicion(trans.condicion, sensores)) {
                    if (trans.prioridad < maxPrioridad) {
                        nuevoEstado = trans.estadoDestino;
                        maxPrioridad = trans.prioridad;
                    }
                }
            }
        }
        
        // Si hay cambio de estado, registrarlo
        if (nuevoEstado != estadoActual) {
            historialEstados.push_back(estadoActual);
            if (historialEstados.size() > 50) {
                historialEstados.erase(historialEstados.begin());
            }
            estadoActual = nuevoEstado;
        }
        
        return estadoActual;
    }
    
    // Obtener configuraci�n del estado actual
    std::map<std::string, double> getConfiguracionActual() {
        if (estados.find(estadoActual) != estados.end()) {
            return estados[estadoActual]->configuracionActuadores;
        }
        return std::map<std::string, double>();
    }
    
    std::string getEstadoActual() const {
        return estadoActual;
    }
    
    std::string getDescripcionEstado() const {
        if (estados.find(estadoActual) != estados.end()) {
            return estados.at(estadoActual)->descripcion;
        }
        return "Desconocido";
    }
    
    // Visualizar grafo de estados
    void mostrarGrafo() {
        std::cout << "\n+----------------------------------------------------+\n";
        std::cout << "�            GRAFO DE ESTADOS DEL SISTEMA           �\n";
        std::cout << "+----------------------------------------------------+\n\n";
        
        for (const auto& par : estados) {
            std::string nombre = par.first;
            bool esActual = (nombre == estadoActual);
            
            std::cout << (esActual ? " " : "  ");
            std::cout << "[" << nombre << "] ";
            std::cout << par.second->descripcion << "\n";
            
            // Mostrar transiciones salientes
            if (transiciones.find(nombre) != transiciones.end()) {
                for (const auto& trans : transiciones[nombre]) {
                    std::cout << "    +- " << trans.estadoDestino 
                             << " si [" << trans.condicion << "]\n";
                }
            }
            std::cout << "\n";
        }
    }
    
    // Mostrar estado actual con detalle visual
    void mostrarEstadoActual() {
        std::cout << "\n+----------------------------------------------------+\n";
        std::cout << "�              ESTADO ACTUAL DEL SISTEMA            �\n";
        std::cout << "+----------------------------------------------------+\n";
        
        std::cout << "\n  Estado: " << estadoActual << "\n";
        std::cout << "  Descripci�n: " << getDescripcionEstado() << "\n\n";
        
        std::cout << "+--- CONFIGURACI�N DE ACTUADORES -------------------+\n";
        auto config = getConfiguracionActual();
        for (const auto& par : config) {
            std::cout << "� " << std::left << std::setw(15) << par.first 
                     << ": " << std::right << std::setw(6) << (int)par.second << "%  ";
            
            // Barra visual
            int barras = static_cast<int>(par.second / 10);
            std::cout << "[";
            for (int i = 0; i < 10; ++i) {
                std::cout << (i < barras ? "" : "");
            }
            std::cout << "]\n";
        }
        std::cout << "+---------------------------------------------------+\n";
        
        // Historial reciente
        if (!historialEstados.empty()) {
            std::cout << "\nHistorial reciente: ";
            int mostrar = std::min(5, (int)historialEstados.size());
            for (int i = historialEstados.size() - mostrar; i < (int)historialEstados.size(); ++i) {
                std::cout << historialEstados[i];
                if (i < (int)historialEstados.size() - 1) std::cout << "  ";
            }
            std::cout << "  " << estadoActual << "\n";
        }
    }
    
    // Buscar camino m�s corto entre dos estados (BFS)
    std::vector<std::string> encontrarCamino(std::string origen, std::string destino) {
        std::queue<std::vector<std::string>> cola;
        std::set<std::string> visitados;
        
        cola.push({origen});
        visitados.insert(origen);
        
        while (!cola.empty()) {
            std::vector<std::string> camino = cola.front();
            cola.pop();
            
            std::string actual = camino.back();
            
            if (actual == destino) {
                return camino;
            }
            
            if (transiciones.find(actual) != transiciones.end()) {
                for (const auto& trans : transiciones[actual]) {
                    if (visitados.find(trans.estadoDestino) == visitados.end()) {
                        std::vector<std::string> nuevoCamino = camino;
                        nuevoCamino.push_back(trans.estadoDestino);
                        cola.push(nuevoCamino);
                        visitados.insert(trans.estadoDestino);
                    }
                }
            }
        }
        
        return std::vector<std::string>(); // No hay camino
    }
};

#endif
