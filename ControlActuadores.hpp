#ifndef CONTROLACTUADORES_HPP
#define CONTROLACTUADORES_HPP

#include <string>
#include <map>
#include <iostream>
#include <iomanip>

// Sistema mejorado de control de actuadores con penalizaciones en modo manual
class ControlActuadores {
private:
    // Historial de desempeño
    struct DesempenioModo {
        int totalCiclos;
        int ciclosExitosos;
        int alarmasGeneradas;
        double calidadPromedio;
        int cambiosActuadores;
        
        DesempenioModo() : totalCiclos(0), ciclosExitosos(0), 
                          alarmasGeneradas(0), calidadPromedio(100.0), 
                          cambiosActuadores(0) {}
    };
    
    DesempenioModo modoManualStats;
    DesempenioModo modoAutomaticoStats;
    
    bool modoManualActivo;
    
public:
    ControlActuadores() : modoManualActivo(false) {}
    
    // Registrar cambio en modo manual
    void registrarCambioManual() {
        modoManualStats.cambiosActuadores++;
    }
    
    // Calcular penalización por cambios frecuentes en modo manual
    double calcularPenalizacionManual() {
        // Penalización si hay cambios muy frecuentes
        if (modoManualStats.cambiosActuadores > 50) {
            return 0.7; // 30% de penalización
        } else if (modoManualStats.cambiosActuadores > 30) {
            return 0.85; // 15% de penalización
        } else if (modoManualStats.cambiosActuadores > 15) {
            return 0.95; // 5% de penalización
        }
        return 1.0;
    }
    
    // Registrar ciclo exitoso o fallido
    void registrarCiclo(bool exitoso, bool esManual) {
        if (esManual) {
            modoManualStats.totalCiclos++;
            if (exitoso) modoManualStats.ciclosExitosos++;
        } else {
            modoAutomaticoStats.totalCiclos++;
            if (exitoso) modoAutomaticoStats.ciclosExitosos++;
        }
    }
    
    // Registrar alarma generada
    void registrarAlarma(bool esManual) {
        if (esManual) {
            modoManualStats.alarmasGeneradas++;
        } else {
            modoAutomaticoStats.alarmasGeneradas++;
        }
    }
    
    // Obtener tasa de éxito del modo
    double obtenerTasaExito(bool esManual) const {
        const auto& stats = esManual ? modoManualStats : modoAutomaticoStats;
        if (stats.totalCiclos == 0) return 0.0;
        return (double)stats.ciclosExitosos / stats.totalCiclos * 100.0;
    }
    
    // Obtener promedio de alarmas por ciclo
    double obtenerAlarmaPorCiclo(bool esManual) const {
        const auto& stats = esManual ? modoManualStats : modoAutomaticoStats;
        if (stats.totalCiclos == 0) return 0.0;
        return (double)stats.alarmasGeneradas / stats.totalCiclos;
    }
    
    // Mostrar comparativa de modos
    void mostrarComparativa() {
        std::cout << "\n";
        std::cout << "╔═══════════════════════════════════════════════════════════╗\n";
        std::cout << "║           COMPARATIVA: MANUAL vs AUTOMÁTICO              ║\n";
        std::cout << "╚═══════════════════════════════════════════════════════════╝\n";
        
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "\n┌─── MODO MANUAL ────────────────────────────────────────┐\n";
        std::cout << "│ Ciclos totales:        " << std::setw(6) << modoManualStats.totalCiclos << "\n";
        std::cout << "│ Ciclos exitosos:       " << std::setw(6) << modoManualStats.ciclosExitosos << "\n";
        std::cout << "│ Tasa de éxito:         " << std::setw(6) << obtenerTasaExito(true) << "%\n";
        std::cout << "│ Alarmas totales:       " << std::setw(6) << modoManualStats.alarmasGeneradas << "\n";
        std::cout << "│ Alarmas por ciclo:     " << std::setw(6) << obtenerAlarmaPorCiclo(true) << "\n";
        std::cout << "│ Cambios de actuadores: " << std::setw(6) << modoManualStats.cambiosActuadores << "\n";
        std::cout << "│ Dificultad:           ⭐⭐⭐⭐⭐ (Máxima)\n";
        std::cout << "└────────────────────────────────────────────────────────────┘\n";
        
        std::cout << "\n┌─── MODO AUTOMÁTICO ───────────────────────────────────┐\n";
        std::cout << "│ Ciclos totales:        " << std::setw(6) << modoAutomaticoStats.totalCiclos << "\n";
        std::cout << "│ Ciclos exitosos:       " << std::setw(6) << modoAutomaticoStats.ciclosExitosos << "\n";
        std::cout << "│ Tasa de éxito:         " << std::setw(6) << obtenerTasaExito(false) << "%\n";
        std::cout << "│ Alarmas totales:       " << std::setw(6) << modoAutomaticoStats.alarmasGeneradas << "\n";
        std::cout << "│ Alarmas por ciclo:     " << std::setw(6) << obtenerAlarmaPorCiclo(false) << "\n";
        std::cout << "│ Cambios de actuadores: " << std::setw(6) << modoAutomaticoStats.cambiosActuadores << "\n";
        std::cout << "│ Dificultad:           ⭐⭐ (Fácil - IA optimizada)\n";
        std::cout << "└────────────────────────────────────────────────────────────┘\n";
        
        double ventaja = obtenerTasaExito(false) - obtenerTasaExito(true);
        std::cout << "\n▶ Ventaja del modo automático: +" << ventaja << "%\n";
    }
    
    // Getters para estadísticas
    const DesempenioModo& getEstadisticasManual() const { return modoManualStats; }
    const DesempenioModo& getEstadisticasAutomatico() const { return modoAutomaticoStats; }
};

#endif
