#ifndef SISTEMA_GAMEPLAY_HPP
#define SISTEMA_GAMEPLAY_HPP

#include "Mision.hpp"
#include <vector>
#include <iostream>
#include <iomanip>

class SistemaGameplay {
private:
    int puntuacion;
    int nivel;
    int experiencia;
    int experieniaParaNivel;
    std::vector<Mision> misiones;
    std::vector<std::string> logros;

public:
    SistemaGameplay() : puntuacion(0), nivel(1), experiencia(0), experieniaParaNivel(1000) {
        crearMisiones();
    }

    void crearMisiones() {
        misiones.push_back(Mision(1, "Iniciante", 
            "Mantener la temperatura entre 20-30°C durante 10 ciclos",
            MANTENER_TEMPERATURA, 10, 100));
        
        misiones.push_back(Mision(2, "Equilibrio Hidrico",
            "Lograr humedad del suelo entre 60-75% durante 15 ciclos",
            ALCANZAR_HUMEDAD, 15, 150));
        
        misiones.push_back(Mision(3, "Resistencia",
            "Sobrevivir 50 ciclos sin que el sistema colapse",
            SOBREVIVIR_CICLOS, 50, 200));
        
        misiones.push_back(Mision(4, "Precision",
            "Completar 10 ciclos sin activar alarmas críticas",
            SIN_ALARMAS, 10, 250, 0.9));
        
        misiones.push_back(Mision(5, "Riego Experto",
            "Mantener sistema de riego optimizado por 20 ciclos",
            RIEGO_OPTIMO, 20, 175));
        
        misiones.push_back(Mision(6, "Científico",
            "Recolectar 100 lecturas de sensores correctamente",
            RECOLECTAR_DATOS, 100, 300));
    }

    void ganarPuntos(int cantidad, const std::string& razon) {
        puntuacion += cantidad;
        experiencia += cantidad / 10;
        
        std::cout << "\n+-- PUNTOS GANADOS --+\n";
        std::cout << "| " << cantidad << " pts: " << razon << "\n";
        std::cout << "+--------------------+\n";
        
        verificarSubidaNivel();
    }

    void verificarSubidaNivel() {
        if (experiencia >= experieniaParaNivel) {
            experiencia -= experieniaParaNivel;
            nivel++;
            experieniaParaNivel = nivel * 1000;
            std::cout << "\n*** SUBISTE DE NIVEL: " << nivel << " ***\n\n";
        }
    }

    void actualizarMision(int idMision, int incremento = 1) {
        for (auto& m : misiones) {
            if (m.id == idMision && !m.completada) {
                m.progreso += incremento;
                if (m.verificarComplecion()) {
                    std::cout << "\n*** MISION COMPLETADA: " << m.nombre << " ***\n";
                    ganarPuntos(m.recompensaPuntos, m.nombre);
                }
            }
        }
    }

    void mostrarTablero() {
        std::cout << "\n+==================================================+\n";
        std::cout << "|            TABLERO DE JUGADOR                    |\n";
        std::cout << "+==================================================+\n\n";
        
        std::cout << "  Nivel:    " << nivel << "\n";
        std::cout << "  Puntos:   " << puntuacion << "\n";
        std::cout << "  Exp:      " << experiencia << " / " << experieniaParaNivel << "\n";
        
        // Barra de experiencia
        int barraExp = (experiencia * 20) / experieniaParaNivel;
        std::cout << "  [";
        for (int i = 0; i < 20; ++i) {
            std::cout << (i < barraExp ? "=" : "-");
        }
        std::cout << "]\n\n";
        
        std::cout << "+---- MISIONES ACTIVAS --------------------------+\n";
        int misionesCompletas = 0;
        for (const auto& m : misiones) {
            if (m.completada) {
                misionesCompletas++;
                std::cout << "  [✓] " << m.nombre << " - COMPLETADA\n";
            } else {
                int barras = (m.getProgreso() * 15);
                std::cout << "  [ ] " << m.nombre << "\n";
                std::cout << "      [";
                for (int i = 0; i < 15; ++i) {
                    std::cout << (i < barras ? "#" : "-");
                }
                std::cout << "] " << (int)(m.getProgreso() * 100) << "%\n";
            }
        }
        std::cout << "\n  Misiones completadas: " << misionesCompletas << "/" << misiones.size() << "\n";
        std::cout << "+================================================+\n";
    }

    int getPuntuacion() const { return puntuacion; }
    int getNivel() const { return nivel; }
    int getExperiencia() const { return experiencia; }
    std::vector<Mision>& getMisiones() { return misiones; }
};

#endif
