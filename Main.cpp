/*#include "Invernadero.hpp"
#include "Simulador.hpp"
#include <iostream>
#include <limits>
#include <ctime>

void mostrarMenu() {
    std::cout << "\n╔══════════════════════════════════════════════════════╗\n";
    std::cout << "║ SISTEMA DE CONTROL DE INVERNADERO INTELIGENTE      ║\n";
    std::cout << "╚══════════════════════════════════════════════════════╝\n";
    std::cout << "ESTADO ACTUAL: Temperatura: " << Simular estado  "24.5°C | Setpoint: 25.0°C | Error: -0.5°C\n";
    std::cout << "Humedad Suelo: 65% | Setpoint: 70% | Riego: OFF\n";
    std::cout << "Luz: 45,000 Lux | CO₂: 450 ppm | Ventilador: 30%\n";
    std::cout << "ALARMAS ACTIVAS (2): [MEDIA] Humedad suelo -5% bajo setpoint [BAJA] Nivel agua tanque: 15%\n";
    std::cout << "\nMENÚ:\n";
    std::cout << "1. Lecturas de sensores en tiempo real\n";
    std::cout << "2. Configurar setpoints de control\n";
    std::cout << "3. Ajustar parámetros PID (Kp, Ki, Kd)\n";
    std::cout << "4. Estado de actuadores\n";
    std::cout << "5. Control manual de actuadores\n";
    std::cout << "6. Gestionar alarmas\n";
    std::cout << "7. Programar eventos (riego, iluminación)\n";
    std::cout << "8. Historial de lecturas (navegación temporal)\n";
    std::cout << "9. Búsqueda por rango de fechas\n";
    std::cout << "10. Estadísticas y análisis de tendencias\n";
    std::cout << "11. Detección de anomalías\n";
    std::cout << "12. Generar reporte completo\n";
    std::cout << "13. Simulación acelerada (demo)\n";
    std::cout << "14. Análisis de complejidad y rendimiento\n";
    std::cout << "15. Deshacer última acción\n";
    std::cout << "16. Salir\n";
    std::cout << "Seleccione una opción: ";
}

int main() {
    Invernadero invernadero;
    Simulador simulador(&invernadero);
    int opcion;

    do {
        invernadero.mostrarEstado(); // Mostrar estado actual
        mostrarMenu();
        std::cin >> opcion;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Opción inválida. Intente de nuevo.\n";
            continue;
        }

        switch (opcion) {
            case 1: {
                std::cout << "\nLecturas en tiempo real:\n";
                invernadero.ejecutarCicloControl();
                break;
            }
            case 2: {
                double temp, humSuelo, humRel;
                std::cout << "Nuevo setpoint temperatura: ";
                std::cin >> temp;
                std::cout << "Nuevo setpoint humedad suelo: ";
                std::cin >> humSuelo;
                std::cout << "Nuevo setpoint humedad relativa: ";
                std::cin >> humRel;
                invernadero.setSetpointTemperatura(temp);
                invernadero.setSetpointHumedadSuelo(humSuelo);
                // Humedad relativa no tiene setter directo, asumir ajuste manual
                break;
            }
            case 3: {
                double kp, ki, kd;
                std::cout << "PID Temperatura - Kp, Ki, Kd: ";
                std::cin >> kp >> ki >> kd;
                invernadero.getPIDTemperatura()->setParametros(kp, ki, kd);
                break;
            }
            case 4: {
                std::cout << "\nEstado de actuadores:\n";
                // Mostrar estados (implementado en mostrarEstado)
                break;
            }
            case 5: {
                double vent, calef, riego, luz;
                std::cout << "Ajustar Ventilador (0-100): ";
                std::cin >> vent;
                invernadero.ajustarVentilador(vent);
                std::cout << "Ajustar Calefactor (0-100): ";
                std::cin >> calef;
                invernadero.ajustarCalefactor(calef);
                std::cout << "Ajustar Riego (0-100): ";
                std::cin >> riego;
                invernadero.ajustarRiego(riego);
                std::cout << "Ajustar Luz LED (0-100): ";
                std::cin >> luz;
                invernadero.ajustarLuzLED(luz);
                break;
            }
            case 6: {
                if (invernadero.getNumAlarmas() > 0) {
                    invernadero.procesarAlarma();
                } else {
                    std::cout << "No hay alarmas activas.\n";
                }
                break;
            }
            case 7: {
                // Programar eventos (simplificado: activar riego o luz)
                int evento;
                std::cout << "1. Activar riego | 2. Activar luz: ";
                std::cin >> evento;
                if (evento == 1) invernadero.ajustarRiego(100);
                else if (evento == 2) invernadero.ajustarLuzLED(100);
                break;
            }
            case 8: {
                // Historial (simplificado: mostrar últimas lecturas)
                std::cout << "Historial (últimas lecturas):\n";
                // Usar historialLecturas para mostrar
                break;
            }
            case 9: {
                time_t inicio, fin;
                std::cout << "Rango de timestamps (inicio fin): ";
                std::cin >> inicio >> fin;
                // Buscar en AVL (simplificado)
                break;
            }
            case 10: {
                invernadero.mostrarEstadisticas();
                break;
            }
            case 11: {
                // Detección de anomalías (usar Estadisticas)
                std::vector<double> datos = {24.5, 25.0, 30.0}; // Ejemplo
                auto anomalias = Estadisticas::detectarAnomalias(datos);
                std::cout << "Anomalías en índices: ";
                for (int idx : anomalias) std::cout << idx << " ";
                std::cout << "\n";
                break;
            }
            case 12: {
                std::cout << "Reporte completo:\n";
                invernadero.mostrarEstado();
                invernadero.mostrarEstadisticas();
                break;
            }
            case 13: {
                int ciclos;
                std::cout << "Número de ciclos para simulación: ";
                std::cin >> ciclos;
                simulador.ejecutarSimulacion(ciclos);
                break;
            }
            case 14: {
                std::cout << "Análisis de complejidad:\n";
                std::cout << "Ciclos simulados: " << invernadero.getCiclosSimulacion() << "\n";
                std::cout << "Tamaño historial: " << invernadero.getTamanoHistorial() << " (O(n))\n";
                std::cout << "Altura AVL: " << invernadero.getAlturaAVL() << " (O(log n))\n";
                break;
            }
            case 15: {
                // Deshacer (usar pila de configuraciones)
                std::cout << "Última acción deshecha (simulado).\n";
                break;
            }
            case 16: {
                std::cout << "Saliendo...\n";
                break;
            }
            default: {
                std::cout << "Opción inválida.\n";
            }
        }
    } while (opcion != 16);

    return 0;
}*/
#include "Invernadero.hpp"
#include "Simulador.hpp"
#include <iostream>
#include <limits>
#include <thread>
#include <chrono>

void mostrarMenu() {
    std::cout << "\n╔══════════════════════════════════════════════════════╗\n";
    std::cout << "║    🌱 SISTEMA DE CONTROL DE INVERNADERO INTELIGENTE   ║\n";
    std::cout << "╚══════════════════════════════════════════════════════╝\n\n";
    std::cout << "1. Ver estado actual del invernadero\n";
    std::cout << "2. Ejecutar ciclo de simulación (1 vez)\n";
    std::cout << "3. Ejecutar simulación continua (automática)\n";
    std::cout << "4. Cambiar modo de control (manual/automático)\n";
    std::cout << "5. Ajustar setpoints (temperatura, humedad suelo)\n";
    std::cout << "6. Control manual de actuadores\n";
    std::cout << "7. Ver y procesar alarmas\n";
    std::cout << "8. Mostrar estadísticas generales\n";
    std::cout << "9. Generar reporte completo del sistema\n";
    std::cout << "10. Exportar historial a archivo CSV\n";
    std::cout << "0. Salir del sistema\n";
    std::cout << "────────────────────────────────────────────────────────\n";
    std::cout << "Seleccione una opción: ";
}

int main() {
    Invernadero invernadero;
    Simulador simulador(&invernadero);
    int opcion;

    do {
        mostrarMenu();
        std::cin >> opcion;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\n⚠️  Opción inválida. Intente de nuevo.\n";
            continue;
        }

        switch (opcion) {
            case 1: {
                std::cout << "\n🌡️  Estado actual del invernadero:\n";
                invernadero.mostrarEstado();
                break;
            }
            case 2: {
                std::cout << "\n🔄 Ejecutando ciclo de simulación...\n";
                invernadero.ejecutarCicloControl();
                std::cout << "✅ Ciclo completado.\n";
                break;
            }
            case 3: {
                int ciclos;
                std::cout << "Ingrese el número de ciclos a simular: ";
                std::cin >> ciclos;
                std::cout << "⏳ Iniciando simulación automática...\n";
                for (int i = 0; i < ciclos; ++i) {
                    invernadero.ejecutarCicloControl();
                    invernadero.mostrarEstado();
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                }
                std::cout << "✅ Simulación completada.\n";
                break;
            }
            case 4: {
                bool nuevoModo = !invernadero.getModoAutomatico();
                invernadero.setModoAutomatico(nuevoModo);
                std::cout << (nuevoModo ? "🟢 Modo automático activado.\n" : "🟡 Modo manual activado.\n");
                break;
            }
            case 5: {
                double temp, hum;
                std::cout << "Ingrese nuevo setpoint de temperatura (°C): ";
                std::cin >> temp;
                std::cout << "Ingrese nuevo setpoint de humedad del suelo (%): ";
                std::cin >> hum;
                invernadero.setSetpointTemperatura(temp);
                invernadero.setSetpointHumedadSuelo(hum);
                std::cout << "✅ Setpoints actualizados.\n";
                break;
            }
            case 6: {
                double vent, calef, riego, luz;
                std::cout << "⚙️  Control manual de actuadores:\n";
                std::cout << "Ventilador (0-100): "; std::cin >> vent;
                std::cout << "Calefactor (0-100): "; std::cin >> calef;
                std::cout << "Riego (0-100): "; std::cin >> riego;
                std::cout << "Luz LED (0-100): "; std::cin >> luz;
                invernadero.ajustarVentilador(vent);
                invernadero.ajustarCalefactor(calef);
                invernadero.ajustarRiego(riego);
                invernadero.ajustarLuzLED(luz);
                std::cout << "✅ Ajustes aplicados.\n";
                break;
            }
            case 7: {
                if (invernadero.getNumAlarmas() > 0) {
                    std::cout << "\n🚨 Alarmas activas detectadas.\n";
                    invernadero.procesarAlarma();
                } else {
                    std::cout << "✅ No hay alarmas activas.\n";
                }
                break;
            }
            case 8: {
                std::cout << "\n📊 Estadísticas generales del sistema:\n";
                invernadero.mostrarEstadisticas();
                break;
            }
            case 9: {
                std::cout << "\n🧾 Reporte completo del invernadero:\n";
                invernadero.mostrarEstado();
                invernadero.mostrarEstadisticas();
                break;
            }
            case 10: {
                std::string archivo;
                std::cout << "Ingrese nombre del archivo CSV (sin extensión): ";
                std::cin >> archivo;
                //invernadero.exportarHistorialCSV(archivo + ".csv");
                break;
            }
            case 0: {
                std::cout << "\n👋 Saliendo del sistema...\n";
                break;
            }
            default:
                std::cout << "\n⚠️  Opción no válida. Intente de nuevo.\n";
        }

    } while (opcion != 0);

    return 0;
}
