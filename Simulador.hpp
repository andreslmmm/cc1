#ifndef SIMULADOR_HPP
#define SIMULADOR_HPP

#include "Invernadero.hpp"
#include <thread>
#include <chrono>

// Clase para simulación acelerada
class Simulador {
private:
    Invernadero* invernadero;
    int ciclosPorSegundo;

public:
    Simulador(Invernadero* inv, int cps = 10) : invernadero(inv), ciclosPorSegundo(cps) {}

    void ejecutarSimulacion(int numCiclos) {
        for (int i = 0; i < numCiclos; ++i) {
            invernadero->ejecutarCicloControl();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 / ciclosPorSegundo));
        }
    }
};

#endif