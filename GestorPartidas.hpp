#ifndef GESTOR_PARTIDAS_HPP
#define GESTOR_PARTIDAS_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <ctime>

struct DatosPartida {
    std::string nombrePartida;
    int ciclos;
    int puntuacion;
    int nivel;
    int experiencia;
    double temperatura;
    double humedad_suelo;
    double humedad_relativa;
    double nivel_agua;
    int alarmas_activas;
    int ciclos_exitosos;
    int alarmas_evitadas;
    std::string modo_control;
    bool modo_automatico;
    time_t fecha_guardado;
    std::string descripcion;

    DatosPartida() : ciclos(0), puntuacion(0), nivel(1), experiencia(0),
                     temperatura(25), humedad_suelo(65), humedad_relativa(70),
                     nivel_agua(500), alarmas_activas(0), ciclos_exitosos(0),
                     alarmas_evitadas(0), modo_control("ARBOL"), 
                     modo_automatico(true), fecha_guardado(0) {}

    std::string obtenerFecha() const {
        struct tm* timeinfo = localtime(&fecha_guardado);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
        return std::string(buffer);
    }
};

class GestorPartidas {
private:
    std::string directorioPartidas;
    std::vector<DatosPartida> partidasRecientes;

public:
    GestorPartidas(std::string directorio = "partidas/") 
        : directorioPartidas(directorio) {
        crearDirectorio();
        cargarListaPartidas();
    }

    void crearDirectorio() {
#ifdef _WIN32
        system("mkdir partidas 2>nul");
#else
        system("mkdir -p partidas");
#endif
    }

    bool guardarPartida(const DatosPartida& datos) {
        std::string nombreArchivo = directorioPartidas + datos.nombrePartida + ".inv";
        std::ofstream archivo(nombreArchivo, std::ios::binary);

        if (!archivo.is_open()) {
            std::cout << "Error: No se pudo crear el archivo de partida.\n";
            return false;
        }

        // Guardar cada dato separado por líneas
        archivo << "NOMBRE:" << datos.nombrePartida << "\n";
        archivo << "CICLOS:" << datos.ciclos << "\n";
        archivo << "PUNTUACION:" << datos.puntuacion << "\n";
        archivo << "NIVEL:" << datos.nivel << "\n";
        archivo << "EXPERIENCIA:" << datos.experiencia << "\n";
        archivo << "TEMPERATURA:" << datos.temperatura << "\n";
        archivo << "HUMEDAD_SUELO:" << datos.humedad_suelo << "\n";
        archivo << "HUMEDAD_RELATIVA:" << datos.humedad_relativa << "\n";
        archivo << "NIVEL_AGUA:" << datos.nivel_agua << "\n";
        archivo << "ALARMAS:" << datos.alarmas_activas << "\n";
        archivo << "CICLOS_EXITOSOS:" << datos.ciclos_exitosos << "\n";
        archivo << "ALARMAS_EVITADAS:" << datos.alarmas_evitadas << "\n";
        archivo << "MODO_CONTROL:" << datos.modo_control << "\n";
        archivo << "MODO_AUTOMATICO:" << (datos.modo_automatico ? "1" : "0") << "\n";
        archivo << "FECHA:" << datos.fecha_guardado << "\n";
        archivo << "DESCRIPCION:" << datos.descripcion << "\n";
        archivo << "FIN_PARTIDA\n";

        archivo.close();
        
        std::cout << "\n✓ Partida '" << datos.nombrePartida << "' guardada exitosamente.\n";
        return true;
    }

    bool cargarPartida(const std::string& nombrePartida, DatosPartida& datosOut) {
        std::string nombreArchivo = directorioPartidas + nombrePartida + ".inv";
        std::ifstream archivo(nombreArchivo);

        if (!archivo.is_open()) {
            std::cout << "Error: No se encontró la partida '" << nombrePartida << "'.\n";
            return false;
        }

        std::string linea;
        while (std::getline(archivo, linea)) {
            if (linea.empty() || linea == "FIN_PARTIDA") continue;

            size_t posicion = linea.find(':');
            if (posicion == std::string::npos) continue;

            std::string clave = linea.substr(0, posicion);
            std::string valor = linea.substr(posicion + 1);

            if (clave == "NOMBRE") datosOut.nombrePartida = valor;
            else if (clave == "CICLOS") datosOut.ciclos = std::stoi(valor);
            else if (clave == "PUNTUACION") datosOut.puntuacion = std::stoi(valor);
            else if (clave == "NIVEL") datosOut.nivel = std::stoi(valor);
            else if (clave == "EXPERIENCIA") datosOut.experiencia = std::stoi(valor);
            else if (clave == "TEMPERATURA") datosOut.temperatura = std::stod(valor);
            else if (clave == "HUMEDAD_SUELO") datosOut.humedad_suelo = std::stod(valor);
            else if (clave == "HUMEDAD_RELATIVA") datosOut.humedad_relativa = std::stod(valor);
            else if (clave == "NIVEL_AGUA") datosOut.nivel_agua = std::stod(valor);
            else if (clave == "ALARMAS") datosOut.alarmas_activas = std::stoi(valor);
            else if (clave == "CICLOS_EXITOSOS") datosOut.ciclos_exitosos = std::stoi(valor);
            else if (clave == "ALARMAS_EVITADAS") datosOut.alarmas_evitadas = std::stoi(valor);
            else if (clave == "MODO_CONTROL") datosOut.modo_control = valor;
            else if (clave == "MODO_AUTOMATICO") datosOut.modo_automatico = (valor == "1");
            else if (clave == "FECHA") datosOut.fecha_guardado = std::stol(valor);
            else if (clave == "DESCRIPCION") datosOut.descripcion = valor;
        }

        archivo.close();
        std::cout << "\n✓ Partida '" << nombrePartida << "' cargada exitosamente.\n";
        return true;
    }

    void cargarListaPartidas() {
        partidasRecientes.clear();

#ifdef _WIN32
        system("dir /B partidas\\*.inv > partidas_temp.txt 2>nul");
#else
        system("ls partidas/*.inv > partidas_temp.txt 2>/dev/null");
#endif

        std::ifstream archivo("partidas_temp.txt");
        std::string linea;
        
        while (std::getline(archivo, linea)) {
            if (linea.empty()) continue;
            
            // Eliminar extensión .inv
            size_t pos = linea.find(".inv");
            if (pos != std::string::npos) {
                std::string nombreSinExtension = linea.substr(0, pos);
                DatosPartida datos;
                if (cargarPartida(nombreSinExtension, datos)) {
                    partidasRecientes.push_back(datos);
                }
            }
        }
        
        archivo.close();
        system("del partidas_temp.txt 2>nul || rm partidas_temp.txt 2>/dev/null");
    }

    void mostrarPartidas() {
        if (partidasRecientes.empty()) {
            std::cout << "\nNo hay partidas guardadas.\n";
            return;
        }

        std::cout << "\n+==================================================+\n";
        std::cout << "|              PARTIDAS GUARDADAS                  |\n";
        std::cout << "+==================================================+\n\n";

        for (size_t i = 0; i < partidasRecientes.size(); ++i) {
            const DatosPartida& p = partidasRecientes[i];
            std::cout << "  [" << (i + 1) << "] " << p.nombrePartida << "\n";
            std::cout << "      Nivel: " << p.nivel << " | Puntos: " << p.puntuacion 
                     << " | Ciclos: " << p.ciclos << "\n";
            std::cout << "      Guardado: " << p.obtenerFecha() << "\n\n";
        }
    }

    DatosPartida* obtenerPartida(const std::string& nombre) {
        for (auto& p : partidasRecientes) {
            if (p.nombrePartida == nombre) {
                return &p;
            }
        }
        return nullptr;
    }

    bool eliminarPartida(const std::string& nombrePartida) {
        std::string nombreArchivo = directorioPartidas + nombrePartida + ".inv";
        
        if (remove(nombreArchivo.c_str()) == 0) {
            std::cout << "\n✓ Partida '" << nombrePartida << "' eliminada.\n";
            cargarListaPartidas();
            return true;
        }
        
        std::cout << "\nError: No se pudo eliminar la partida.\n";
        return false;
    }

    std::vector<DatosPartida>& getPartidas() {
        return partidasRecientes;
    }
};

#endif
