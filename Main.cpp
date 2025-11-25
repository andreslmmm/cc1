#include "Invernadero.hpp"
#include "Simulador.hpp"
#include "GestorPartidas.hpp"
#include <iostream>
#include <limits>
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
#include <windows.h>

#define RESET       "\033[0m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define CYAN        "\033[36m"
#define MAGENTA     "\033[35m"
#define BOLD        "\033[1m"
#define GRAY        "\033[90m"

void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausar() {
    std::cout << GRAY << "\nPresione Enter para continuar..." << RESET;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void mostrarBannerInicio() {
    std::cout << CYAN << BOLD;
    std::cout << R"(
╔═══════════════════════════════════════════════════════════════════╗
║                                                                   ║
║          🌱 SIMULADOR INTELIGENTE DE INVERNADERO 🌱               ║
║                                                                   ║
║   Control Avanzado  | Árbol de Decisión | Grafo Estados           ║
║                                                                   ║
║              Gestiona tu invernadero virtual 2025                 ║
║                                                                   ║
╚═══════════════════════════════════════════════════════════════════╝
)" << RESET;
}

void mostrarMenuPrincipal() {
    std::cout << BOLD << YELLOW;
    std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                     MENU PRINCIPAL                         ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n" << RESET;

    std::cout << GREEN;
    std::cout << "\n[ 🔍 MONITOREO Y CONTROL ]\n";
    std::cout << "  1. Ver estado actual del invernadero\n";
    std::cout << "  2. Ejecutar un ciclo de control\n";
    std::cout << "  3. Ejecutar simulación continua\n";

    std::cout << CYAN;
    std::cout << "\n[ 🧠 SISTEMAS INTELIGENTES ]\n";
    std::cout << "  4. Visualizar Árbol de Decisión\n";
    std::cout << "  5. Visualizar Grafo de Estados\n";
    std::cout << "  6. Cambiar modo de control (Árbol/Grafo)\n";
    std::cout << "  7. Activar/Desactivar modo automático\n";

    std::cout << MAGENTA;
    std::cout << "\n[ 🎮 CONTROL MANUAL ]\n";
    std::cout << "  8. Control manual de actuadores\n";
    std::cout << "  9. Rellenar tanque de agua\n";

    std::cout << BLUE;
    std::cout << "\n[ 📊 ESTADISTICAS Y ANÁLISIS ]\n";
    std::cout << " 10. Ver y procesar alarmas\n";
    std::cout << " 11. Estadísticas simples\n";
    std::cout << " 12. Estadísticas avanzadas\n";
    std::cout << " 13. Tablero de juego (misiones y puntos)\n";
    std::cout << " 14. Generar reporte completo\n";

    std::cout << YELLOW;
    std::cout << "\n[ 💾 PARTIDAS ]\n";
    std::cout << " 15. Gestionar partidas (guardar/cargar)\n";

    std::cout << RED;
    std::cout << "\n[ 📈 COMPARATIVA MODOS ]\n";
    std::cout << " 16. Ver comparativa: Manual vs Automático\n";

    std::cout << RED;
    std::cout << "\n  0. Salir del sistema\n";

    std::cout << RESET << "\nSeleccione una opción: ";
}

void submenuControlSensores(Invernadero& inv) {
    limpiarPantalla();
    std::cout << MAGENTA << BOLD << "\n=== CONTROL MANUAL DE SENSORES ===\n" << RESET;
    std::cout << "Ingrese intensidad (0-100) para cada actuador:\n\n";

    double vent, calef, riego, luz, neb;

    std::cout << "  Ventilador: "; std::cin >> vent;
    std::cout << "  Calefactor: "; std::cin >> calef;
    std::cout << "  Sistema de Riego: "; std::cin >> riego;
    std::cout << "  Luz LED: "; std::cin >> luz;
    std::cout << "  Nebulizador: "; std::cin >> neb;

    inv.setModoAutomatico(false);
    inv.ajustarVentilador(vent);
    inv.ajustarCalefactor(calef);
    inv.ajustarRiego(riego);
    inv.ajustarLuzLED(luz);

    std::cout << GREEN << "\n✓ Configuración manual aplicada\n" << RESET;
    pausar();
}

void submenuControlSensoresAvanzado(Invernadero& inv) {
    int opcion;
    do {
        limpiarPantalla();
        std::cout << MAGENTA << BOLD << "\n=== CONTROL MANUAL DE ACTUADORES ===\n" << RESET;
        std::cout << GREEN << "⚠️  MODO MANUAL: Mayor dificultad, más alarmas, menos puntos\n\n" << RESET;
        
        std::cout << "1. Ventilador (Enfría 0.5°C por ciclo)\n";
        std::cout << "2. Calefactor (Calienta 0.5°C por ciclo)\n";
        std::cout << "3. Sistema de Riego (Aumenta humedad suelo 2% por ciclo)\n";
        std::cout << "4. Luz LED (Ligero calentamiento 0.1°C)\n";
        std::cout << "5. Nebulizador (Aumenta humedad relativa 1% por ciclo)\n";
        std::cout << "6. Control automático (Aplicar a todos)\n";
        std::cout << "7. Apagar todo\n";
        std::cout << "8. Ver estado actual\n";
        std::cout << "0. Volver\n\n";
        
        std::cout << "Seleccione actuador: ";
        std::cin >> opcion;
        std::cin.ignore();
        
        if (opcion >= 1 && opcion <= 5) {
            std::cout << "Intensidad (0-100%): ";
            double intensidad;
            std::cin >> intensidad;
            std::cin.ignore();
            
            switch (opcion) {
                case 1:
                    inv.ajustarVentilador(intensidad);
                    std::cout << BLUE << "✓ Ventilador ajustado a " << intensidad << "%\n" << RESET;
                    break;
                case 2:
                    inv.ajustarCalefactor(intensidad);
                    std::cout << BLUE << "✓ Calefactor ajustado a " << intensidad << "%\n" << RESET;
                    break;
                case 3:
                    inv.ajustarRiego(intensidad);
                    std::cout << BLUE << "✓ Sistema de riego ajustado a " << intensidad << "%\n" << RESET;
                    break;
                case 4:
                    inv.ajustarLuzLED(intensidad);
                    std::cout << BLUE << "✓ Luz LED ajustada a " << intensidad << "%\n" << RESET;
                    break;
                case 5:
                    inv.ajustarNebulizador(intensidad);
                    std::cout << BLUE << "✓ Nebulizador ajustado a " << intensidad << "%\n" << RESET;
                    break;
            }
            
            inv.mostrarEstado();
            pausar();
        } else if (opcion == 6) {
            inv.setModoAutomatico(true);
            std::cout << GREEN << "\n✓ Modo automático activado (Dificultad: Baja)\n" << RESET;
            pausar();
        } else if (opcion == 7) {
            inv.ajustarVentilador(0);
            inv.ajustarCalefactor(0);
            inv.ajustarRiego(0);
            inv.ajustarLuzLED(0);
            inv.ajustarNebulizador(0);
            std::cout << RED << "\n✓ Todos los actuadores apagados\n" << RESET;
            pausar();
        } else if (opcion == 8) {
            limpiarPantalla();
            inv.mostrarEstado();
            pausar();
        }
    } while (opcion != 0);
}

void submenuAnalisis(Invernadero& inv) {
    int opcion;
    do {
        limpiarPantalla();
        std::cout << BLUE << BOLD << "\n=== SUBMENU ANÁLISIS ===\n" << RESET;
        std::cout << "1. Estadísticas simples\n";
        std::cout << "2. Estadísticas avanzadas\n";
        std::cout << "3. Ver y procesar alarmas\n";
        std::cout << "0. Volver\n";
        std::cout << "Opción: ";
        std::cin >> opcion;

        switch (opcion) {
            case 1:
                limpiarPantalla();
                inv.mostrarEstadisticas();
                pausar();
                break;
            case 2:
                limpiarPantalla();
                inv.mostrarEstadisticasAvanzadas();
                pausar();
                break;
            case 3:
                limpiarPantalla();
                inv.procesarAlarma();
                pausar();
                break;
        }
    } while (opcion != 0);
}

void demostrarSistemaIA(Invernadero& inv) {
    limpiarPantalla();
    std::cout << YELLOW << BOLD;
    std::cout << "\n=== DEMOSTRACION: SISTEMAS IA ===\n\n" << RESET;

    inv.setModoControl("ARBOL");
    inv.setModoAutomatico(true);

    std::cout << GREEN << "Ejecutando 5 ciclos con control inteligente...\n" << RESET;
    pausar();

    for (int i = 0; i < 5; ++i) {
        limpiarPantalla();
        inv.ejecutarCicloControl();
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    }

    pausar();
}

void submenuGuardarCargar(Invernadero& inv, GestorPartidas& gestor) {
    int opcion;
    do {
        limpiarPantalla();
        std::cout << CYAN << BOLD << "\n=== GESTIÓN DE PARTIDAS ===\n" << RESET;
        std::cout << "1. Guardar partida actual\n";
        std::cout << "2. Cargar partida\n";
        std::cout << "3. Ver partidas guardadas\n";
        std::cout << "4. Eliminar partida\n";
        std::cout << "0. Volver al menú principal\n";
        std::cout << "Opción: ";
        std::cin >> opcion;
        std::cin.ignore();

        switch (opcion) {
            case 1: {
                limpiarPantalla();
                std::cout << "Nombre de la partida: ";
                std::string nombre;
                std::getline(std::cin, nombre);
                
                std::cout << "Descripción (opcional): ";
                std::string desc;
                std::getline(std::cin, desc);
                
                if (!nombre.empty()) {
                    DatosPartida datos = inv.exportarDatosPartida(nombre, desc);
                    gestor.guardarPartida(datos);
                }
                pausar();
                break;
            }
            case 2: {
                limpiarPantalla();
                gestor.mostrarPartidas();
                
                if (!gestor.getPartidas().empty()) {
                    std::cout << "Nombre de partida a cargar (sin extensión): ";
                    std::string nombre;
                    std::getline(std::cin, nombre);
                    
                    DatosPartida datos;
                    if (gestor.cargarPartida(nombre, datos)) {
                        inv.importarDatosPartida(datos);
                    }
                }
                pausar();
                break;
            }
            case 3: {
                limpiarPantalla();
                gestor.mostrarPartidas();
                pausar();
                break;
            }
            case 4: {
                limpiarPantalla();
                gestor.mostrarPartidas();
                
                if (!gestor.getPartidas().empty()) {
                    std::cout << "Nombre de partida a eliminar: ";
                    std::string nombre;
                    std::getline(std::cin, nombre);
                    gestor.eliminarPartida(nombre);
                }
                pausar();
                break;
            }
        }
    } while (opcion != 0);
}

int main() {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    Invernadero invernadero;
    GestorPartidas gestor;

    int opcion;
    
    mostrarBannerInicio();
    pausar();
    
    do {
        limpiarPantalla();
        mostrarMenuPrincipal();
        std::cin >> opcion;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << RED << "\nOpción inválida.\n" << RESET;
            pausar();
            continue;
        }

        switch (opcion) {
            case 1: {
                limpiarPantalla();
                invernadero.mostrarEstado();
                pausar();
                break;
            }
            case 2: {
                limpiarPantalla();
                invernadero.ejecutarCicloControl();
                pausar();
                break;
            }
            case 3: {
                int ciclos;
                limpiarPantalla();
                std::cout << "¿Cuántos ciclos deseas simular? ";
                std::cin >> ciclos;
                
                pausar();
                for (int i = 0; i < ciclos; ++i) {
                    limpiarPantalla();
                    invernadero.ejecutarCicloControl();
                    std::this_thread::sleep_for(std::chrono::milliseconds(800));
                }
                pausar();
                break;
            }
            case 4: {
                limpiarPantalla();
                std::cout << CYAN << BOLD << "\n=== ÁRBOL DE DECISIÓN ===\n" << RESET;
                invernadero.mostrarArbolDecision();
                pausar();
                break;
            }
            case 5: {
                limpiarPantalla();
                std::cout << CYAN << BOLD << "\n=== GRAFO DE ESTADOS ===\n" << RESET;
                invernadero.mostrarGrafoEstados();
                pausar();
                break;
            }
            case 6: {
                limpiarPantalla();
                std::cout << "Modo actual: " << invernadero.getModoControl() << "\n\n";
                std::cout << "1. ÁRBOL de Decisión\n2. GRAFO de Estados\nSeleccione: ";
                int m;
                std::cin >> m;

                if (m == 1) invernadero.setModoControl("ARBOL");
                else if (m == 2) invernadero.setModoControl("GRAFO");
                else std::cout << RED << "\nModo inválido.\n" << RESET;

                pausar();
                break;
            }
            case 7: {
                invernadero.setModoAutomatico(!invernadero.getModoAutomatico());
                limpiarPantalla();
                std::cout << GREEN << "Modo cambiado a: " 
                         << (invernadero.getModoAutomatico() ? "AUTOMÁTICO" : "MANUAL")
                         << "\n" << RESET;
                pausar();
                break;
            }
            case 8:
                submenuControlSensoresAvanzado(invernadero);
                break;
            case 9: {
                double litros;
                limpiarPantalla();
                std::cout << "¿Cuántos litros agregar? ";
                std::cin >> litros;
                invernadero.rellenarTanqueAgua(litros);
                pausar();
                break;
            }
            case 10:
                submenuAnalisis(invernadero);
                break;
            case 11: {
                limpiarPantalla();
                invernadero.mostrarEstadisticas();
                pausar();
                break;
            }
            case 12: {
                limpiarPantalla();
                invernadero.mostrarEstadisticasAvanzadas();
                pausar();
                break;
            }
            case 13: {
                limpiarPantalla();
                invernadero.mostrarTableroJuego();
                pausar();
                break;
            }
            case 14: {
                limpiarPantalla();
                std::cout << BOLD << "=== REPORTE COMPLETO ===\n" << RESET;
                invernadero.mostrarEstado();
                invernadero.mostrarEstadisticasAvanzadas();
                invernadero.mostrarTableroJuego();
                pausar();
                break;
            }
            case 15:
                submenuGuardarCargar(invernadero, gestor);
                break;
            case 16: {
                limpiarPantalla();
                invernadero.mostrarComparativaModos();
                pausar();
                break;
            }
            case 0:
                limpiarPantalla();
                std::cout << CYAN << "\nGracias por jugar. ¡Hasta pronto!\n" << RESET;
                break;

            default:
                std::cout << RED << "\nOpción no válida.\n" << RESET;
                pausar();
        }

    } while (opcion != 0);

    return 0;
}
