#ifndef ARBOL_DECISION_HPP
#define ARBOL_DECISION_HPP

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <cmath>
#include <iomanip>

// Estructura para representar una acción de control
struct AccionControl {
    std::string actuador;
    double intensidad;
    std::string razon;
    
    AccionControl(std::string act, double inten, std::string raz)
        : actuador(act), intensidad(inten), razon(raz) {}
};

// Nodo del árbol de decisión
class NodoDecision {
public:
    std::string etiqueta;
    std::string condicion;
    std::vector<AccionControl> acciones;
    
    NodoDecision* izquierdo;  // Rama SI
    NodoDecision* derecho;    // Rama NO
    
    bool esHoja;
    int nivel;
    
    NodoDecision(std::string etiq, std::string cond = "", int niv = 0)
        : etiqueta(etiq), condicion(cond), izquierdo(nullptr), 
          derecho(nullptr), esHoja(false), nivel(niv) {}
    
    ~NodoDecision() {
        delete izquierdo;
        delete derecho;
    }
    
    void agregarAccion(std::string actuador, double intensidad, std::string razon) {
        acciones.push_back(AccionControl(actuador, intensidad, razon));
        esHoja = true;
    }
};

// Árbol de decisión para control del invernadero
class ArbolDecision {
private:
    NodoDecision* raiz;
    std::vector<std::string> caminoDecision;
    std::vector<AccionControl> accionesFinales;
    std::map<std::string, double> valoresSensores;
    
    // Evaluar condición
    bool evaluarCondicion(std::string condicion, std::map<std::string, double>& sensores) {
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
    
    // Recorrer árbol
    void recorrerArbol(NodoDecision* nodo, std::map<std::string, double>& sensores) {
        if (!nodo) return;
        
        if (nodo == raiz) {
            caminoDecision.push_back("RAIZ|" + nodo->condicion);
        } else if (!nodo->esHoja) {
            caminoDecision.push_back("DECISION|" + nodo->etiqueta + "|" + nodo->condicion);
        } else {
            caminoDecision.push_back("ACCION|" + nodo->etiqueta);
        }
        
        if (nodo->esHoja) {
            for (const auto& accion : nodo->acciones) {
                accionesFinales.push_back(accion);
            }
            return;
        }
        
        bool resultado = evaluarCondicion(nodo->condicion, sensores);
        
        // Extraer sensor y valor
        std::string sensorNombre = nodo->condicion.substr(0, nodo->condicion.find_first_of("<>="));
        sensorNombre.erase(0, sensorNombre.find_first_not_of(" \t"));
        sensorNombre.erase(sensorNombre.find_last_not_of(" \t") + 1);
        double valorSensor = sensores[sensorNombre];
        
        if (resultado && nodo->izquierdo) {
            caminoDecision.push_back("SI|" + sensorNombre + "|" + std::to_string((int)valorSensor) + "|" + nodo->condicion);
            recorrerArbol(nodo->izquierdo, sensores);
        } else if (!resultado && nodo->derecho) {
            caminoDecision.push_back("NO|" + sensorNombre + "|" + std::to_string((int)valorSensor) + "|" + nodo->condicion);
            recorrerArbol(nodo->derecho, sensores);
        }
    }

public:
    ArbolDecision() {
        construirArbol();
    }
    
    ~ArbolDecision() {
        delete raiz;
    }
    
    // Construir árbol lógico
    void construirArbol() {
        raiz = new NodoDecision("RAIZ", "TEMP>35", 0);
        
        // SI: Temp > 35
        NodoDecision* enfriar = new NodoDecision("ENFRIAR MAXIMO", "", 1);
        enfriar->agregarAccion("VENTILADOR", 100.0, "Temperatura critica alta");
        enfriar->agregarAccion("CALEFACTOR", 0.0, "Apagar calefaccion");
        enfriar->agregarAccion("NEBULIZADOR", 100.0, "Enfriamiento evaporativo");
        raiz->izquierdo = enfriar;
        
        // NO: Temp <= 35, evaluar temp baja
        NodoDecision* evalTempBaja = new NodoDecision("Eval Temp Baja", "TEMP<15", 1);
        raiz->derecho = evalTempBaja;
        
            // SI: Temp < 15
            NodoDecision* calentar = new NodoDecision("CALENTAR", "", 2);
            calentar->agregarAccion("CALEFACTOR", 90.0, "Temperatura critica baja");
            calentar->agregarAccion("VENTILADOR", 0.0, "Apagar ventilacion");
            evalTempBaja->izquierdo = calentar;
            
            // NO: Temp OK, evaluar humedad suelo
            NodoDecision* evalHumSuelo = new NodoDecision("Eval Hum Suelo", "HUM_SUELO<40", 2);
            evalTempBaja->derecho = evalHumSuelo;
            
                // SI: Suelo seco
                NodoDecision* regar = new NodoDecision("REGAR INTENSO", "", 3);
                regar->agregarAccion("RIEGO", 100.0, "Suelo muy seco");
                regar->agregarAccion("NEBULIZADOR", 50.0, "Aumentar humedad");
                regar->agregarAccion("VENTILADOR", 20.0, "Circulacion leve");
                evalHumSuelo->izquierdo = regar;
                
                // NO: Suelo OK, evaluar humedad relativa
                NodoDecision* evalHumRel = new NodoDecision("Eval Hum Relativa", "HUM_REL<60", 3);
                evalHumSuelo->derecho = evalHumRel;
                
                    // SI: Ambiente seco
                    NodoDecision* nebulizar = new NodoDecision("NEBULIZAR", "", 4);
                    nebulizar->agregarAccion("NEBULIZADOR", 80.0, "Ambiente seco");
                    nebulizar->agregarAccion("VENTILADOR", 15.0, "Circulacion");
                    nebulizar->agregarAccion("RIEGO", 30.0, "Mantener suelo");
                    evalHumRel->izquierdo = nebulizar;
                    
                    // NO: Todo OK
                    NodoDecision* mantener = new NodoDecision("MANTENER OPTIMO", "", 4);
                    mantener->agregarAccion("VENTILADOR", 25.0, "Circulacion");
                    mantener->agregarAccion("RIEGO", 30.0, "Mantener humedad");
                    mantener->agregarAccion("LUZ_LED", 60.0, "Iluminacion");
                    evalHumRel->derecho = mantener;
    }
    
    // Tomar decisiones
    std::vector<AccionControl> decidir(std::map<std::string, double> sensores) {
        caminoDecision.clear();
        accionesFinales.clear();
        valoresSensores = sensores;
        
        recorrerArbol(raiz, sensores);
        
        return accionesFinales;
    }
    
    // Obtener camino de decisión
    std::vector<std::string> getCaminoDecision() const {
        return caminoDecision;
    }
    
    // VISUALIZAR ÁRBOL COMPLETO
    void mostrarArbol() {
        std::cout << "\n";
        std::cout << "+===============================================================+\n";
        std::cout << "|          ARBOL DE DECISION DEL INVERNADERO                    |\n";
        std::cout << "+===============================================================+\n\n";
        
        std::cout << "ESTRUCTURA DEL ARBOL:\n\n";
        
        std::cout << "                        [RAIZ]\n";
        std::cout << "                   (Temp > 35C?)\n";
        std::cout << "                    /         \\\n";
        std::cout << "                  SI           NO\n";
        std::cout << "                 /               \\\n";
        std::cout << "          [ENFRIAR MAX]      (Temp < 15C?)\n";
        std::cout << "           Vent:100%           /         \\\n";
        std::cout << "           Neb:100%          SI           NO\n";
        std::cout << "                            /               \\\n";
        std::cout << "                      [CALENTAR]      (HumSuelo < 40%?)\n";
        std::cout << "                       Calef:90%        /           \\\n";
        std::cout << "                                      SI             NO\n";
        std::cout << "                                     /                 \\\n";
        std::cout << "                              [REGAR]            (HumRel < 60%?)\n";
        std::cout << "                              Riego:100%           /          \\\n";
        std::cout << "                              Neb:50%            SI            NO\n";
        std::cout << "                                                /                \\\n";
        std::cout << "                                         [NEBULIZAR]        [MANTENER]\n";
        std::cout << "                                         Neb:80%            Vent:25%\n";
        std::cout << "                                         Vent:15%           Riego:30%\n";
        std::cout << "                                                            Luz:60%\n\n";
        
        std::cout << "+===============================================================+\n";
        std::cout << "|                TABLA DE DECISIONES                            |\n";
        std::cout << "+===============================================================+\n\n";
        std::cout << "  Nivel 1: Temp > 35C?  --> SI: ENFRIAR MAXIMO\n";
        std::cout << "                        --> NO: Continuar\n\n";
        std::cout << "  Nivel 2: Temp < 15C?  --> SI: CALENTAR\n";
        std::cout << "                        --> NO: Continuar\n\n";
        std::cout << "  Nivel 3: HumSuelo < 40%? --> SI: REGAR INTENSO\n";
        std::cout << "                           --> NO: Continuar\n\n";
        std::cout << "  Nivel 4: HumRel < 60%? --> SI: NEBULIZAR\n";
        std::cout << "                         --> NO: MANTENER OPTIMO\n\n";
        
        std::cout << "+===============================================================+\n";
        std::cout << "|                DESCRIPCION DE ACCIONES                        |\n";
        std::cout << "+===============================================================+\n\n";
        
        std::cout << "1. ENFRIAR MAXIMO:\n";
        std::cout << "   - Ventilador al 100 por ciento\n";
        std::cout << "   - Nebulizador al 100 por ciento\n";
        std::cout << "   - Apagar calefaccion\n\n";
        
        std::cout << "2. CALENTAR:\n";
        std::cout << "   - Calefactor al 90 por ciento\n";
        std::cout << "   - Apagar ventilacion\n\n";
        
        std::cout << "3. REGAR INTENSO:\n";
        std::cout << "   - Riego al 100 por ciento\n";
        std::cout << "   - Nebulizador al 50 por ciento\n";
        std::cout << "   - Ventilador al 20 por ciento\n\n";
        
        std::cout << "4. NEBULIZAR:\n";
        std::cout << "   - Nebulizador al 80 por ciento\n";
        std::cout << "   - Ventilador al 15 por ciento\n";
        std::cout << "   - Riego al 30 por ciento\n\n";
        
        std::cout << "5. MANTENER OPTIMO:\n";
        std::cout << "   - Ventilador al 25 por ciento\n";
        std::cout << "   - Riego al 30 por ciento\n";
        std::cout << "   - Luz LED al 60 por ciento\n\n";
        
        std::cout << "+===============================================================+\n";
        std::cout << "|  ESTADISTICAS: Profundidad=4, Nodos=9, Hojas=5               |\n";
        std::cout << "+===============================================================+\n\n";
    }
    
    // VISUALIZAR PROCESO DE DECISIÓN
    void mostrarProcesoDecision() {
        std::cout << "\n";
        std::cout << "+===============================================================+\n";
        std::cout << "|              CAMINO DE DECISION TOMADO                        |\n";
        std::cout << "+===============================================================+\n\n";
        
        std::cout << "VALORES ACTUALES:\n";
        std::cout << "+-- Temperatura:    " << std::fixed << std::setprecision(1) 
                  << valoresSensores["TEMP"] << " grados C\n";
        std::cout << "+-- Humedad Suelo:  " << valoresSensores["HUM_SUELO"] << " por ciento\n";
        std::cout << "+-- Humedad Relat:  " << valoresSensores["HUM_REL"] << " por ciento\n\n";
        
        std::cout << "RECORRIDO:\n\n";
        
        // Parsear y mostrar camino
        for (size_t i = 0; i < caminoDecision.size(); ++i) {
            std::string linea = caminoDecision[i];
            std::vector<std::string> partes;
            
            size_t pos = 0;
            while ((pos = linea.find('|')) != std::string::npos) {
                partes.push_back(linea.substr(0, pos));
                linea.erase(0, pos + 1);
            }
            partes.push_back(linea);
            
            if (partes[0] == "RAIZ") {
                std::cout << "              +----------------------------------+\n";
                std::cout << "              |  RAIZ: " << partes[1] << "          |\n";
                std::cout << "              +----------------------------------+\n";
                std::cout << "                           |\n";
                
            } else if (partes[0] == "SI") {
                std::cout << "                           +--SI (" << partes[1] 
                         << "=" << partes[2] << " cumple " << partes[3] << ")\n";
                std::cout << "                           |\n";
                
            } else if (partes[0] == "NO") {
                std::cout << "                           +--NO (" << partes[1] 
                         << "=" << partes[2] << " NO cumple " << partes[3] << ")\n";
                std::cout << "                           |\n";
                
            } else if (partes[0] == "DECISION") {
                std::cout << "              +----------------------------------+\n";
                std::cout << "              |  Evaluar: " << partes[2] << "     |\n";
                std::cout << "              +----------------------------------+\n";
                std::cout << "                           |\n";
                
            } else if (partes[0] == "ACCION") {
                std::cout << "              +----------------------------------+\n";
                std::cout << "              |  ACCION: " << partes[1] << "      |\n";
                std::cout << "              +----------------------------------+\n";
            }
        }
        
        std::cout << "\n";
        std::cout << "+===============================================================+\n";
        std::cout << "|                 ACCIONES A EJECUTAR                           |\n";
        std::cout << "+===============================================================+\n\n";
        
        for (const auto& accion : accionesFinales) {
            int barras = (int)accion.intensidad / 10;
            std::string barra = "[";
            for (int i = 0; i < 10; ++i) {
                barra += (i < barras) ? "#" : "-";
            }
            barra += "]";
            
            std::cout << "  * " << std::left << std::setw(15) << accion.actuador 
                     << " " << barra << " " 
                     << std::right << std::setw(3) << (int)accion.intensidad << " por ciento\n";
            std::cout << "    Razon: " << accion.razon << "\n\n";
        }
        
        std::cout << "+===============================================================+\n";
        std::cout << "|  SI = Condicion cumplida (rama izquierda)                     |\n";
        std::cout << "|  NO = Condicion NO cumplida (rama derecha)                    |\n";
        std::cout << "+===============================================================+\n";
    }
};

#endif
