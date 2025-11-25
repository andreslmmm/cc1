#ifndef INVERNADERO_HPP
#define INVERNADERO_HPP

#include "Sensor.hpp"
#include "Actuador.hpp"
#include "Lectura.hpp"
#include "Alarma.hpp"
#include "ArbolDecision.hpp"
#include "Grafo.hpp"
#include "ListaEnlazada.hpp"
#include "ListaDoble.hpp"
#include "Pila.hpp"
#include "Cola.hpp"
#include "HeapPrioridad.hpp"
#include "ArbolAVL.hpp"
#include "Estadisticas.hpp"
#include "SistemaGameplay.hpp"  // Incluir el sistema de gamificación
#include "GestorPartidas.hpp"
#include "ControlActuadores.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>

class Invernadero {
private:
    // Sensores
    SensorTemperatura* sensorTempAmb;
    SensorHumedad* sensorHumRel;
    SensorHumedad* sensorHumSuelo;
    SensorLuz* sensorLuz;
    SensorPH* sensorPH;
    SensorCO2* sensorCO2;
    SensorNivelAgua* sensorAgua;

    // Actuadores
    Ventilador* ventilador;
    Calefactor* calefactor;
    SistemaRiego* riego;
    LuzLED* luzLED;
    Nebulizador* nebulizador;

    // Sistema de control inteligente
    ArbolDecision* arbolControl;
    GrafoEstados* grafoEstados;

    // Estructuras de datos
    ListaDoble<Lectura>* historialLecturas;
    HeapPrioridad<Alarma>* colaAlarmas;
    ArbolAVL<Lectura>* indiceTimestamp;
    Pila<std::string>* pilaConfiguraciones;
    Cola<std::string>* colaComandos;
    ListaEnlazada<Alarma>* logAlarmas;

    // Configuración
    int maxLecturas;
    bool modoAutomatico;
    int ciclosSimulacion;
    std::string modoControl; // "ARBOL" o "GRAFO"

    // Sistemas de gamificación
    SistemaGameplay* sistemaGameplay;
    double calidadPromedio;
    int ciclosExitosos;
    int totalAlarmasEvitadas;

    ControlActuadores* controlActuadores;
    bool ultimoModoManual;

    double factorPenalizacionManual;

public:
    Invernadero() : maxLecturas(1000), modoAutomatico(true), 
                    ciclosSimulacion(0), modoControl("ARBOL"),
                    calidadPromedio(100.0), ciclosExitosos(0), 
                    totalAlarmasEvitadas(0) {
        // Inicializar sensores
        sensorTempAmb = new SensorTemperatura("TEMP", 25.0);
        sensorHumRel = new SensorHumedad("HUM_REL", 70.0, false);
        sensorHumSuelo = new SensorHumedad("HUM_SUELO", 65.0, true);
        sensorLuz = new SensorLuz("LUZ", 45000.0);
        sensorPH = new SensorPH("PH", 6.5);
        sensorCO2 = new SensorCO2("CO2", 450.0);
        sensorAgua = new SensorNivelAgua("AGUA", 500.0);

        // Inicializar actuadores
        ventilador = new Ventilador("VENT_01");
        calefactor = new Calefactor("CALEF_01");
        riego = new SistemaRiego("RIEGO_01");
        luzLED = new LuzLED("LED_01");
        nebulizador = new Nebulizador("NEB_01");

        // Inicializar sistemas de control inteligente
        arbolControl = new ArbolDecision();
        grafoEstados = new GrafoEstados();

        // Inicializar estructuras de datos
        historialLecturas = new ListaDoble<Lectura>();
        colaAlarmas = new HeapPrioridad<Alarma>();
        indiceTimestamp = new ArbolAVL<Lectura>();
        pilaConfiguraciones = new Pila<std::string>();
        colaComandos = new Cola<std::string>();
        logAlarmas = new ListaEnlazada<Alarma>();

        sistemaGameplay = new SistemaGameplay();
        controlActuadores = new ControlActuadores();
        ultimoModoManual = !modoAutomatico;
        factorPenalizacionManual = 1.0;
    }

    ~Invernadero() {
        delete sensorTempAmb;
        delete sensorHumRel;
        delete sensorHumSuelo;
        delete sensorLuz;
        delete sensorPH;
        delete sensorCO2;
        delete sensorAgua;
        delete ventilador;
        delete calefactor;
        delete riego;
        delete luzLED;
        delete nebulizador;
        delete arbolControl;
        delete grafoEstados;
        delete historialLecturas;
        delete colaAlarmas;
        delete indiceTimestamp;
        delete pilaConfiguraciones;
        delete colaComandos;
        delete logAlarmas;
        delete sistemaGameplay;
        delete controlActuadores;
    }

    // Ciclo principal de control con visualización
    void ejecutarCicloControl() {
        ciclosSimulacion++;

        std::cout << "\n+----------------------------------------------------+\n";
        std::cout << "¦         CICLO DE CONTROL #" << std::setw(4) << ciclosSimulacion << "                    ¦\n";
        std::cout << "+----------------------------------------------------+\n";

        // 1. Leer todos los sensores
        std::cout << "\n[1/5]  Leyendo sensores...\n";
        time_t ahora = time(nullptr);
        double tempAmb = sensorTempAmb->leer();
        double humRel = sensorHumRel->leer();
        double humSuelo = sensorHumSuelo->leer();
        double luz = sensorLuz->leer();
        double ph = sensorPH->leer();
        double co2 = sensorCO2->leer();
        double agua = sensorAgua->leer();

        std::cout << "   Temperatura: " << std::fixed << std::setprecision(1) 
                  << tempAmb << "°C\n";
        std::cout << "   Humedad Suelo: " << humSuelo << "%\n";
        std::cout << "   Humedad Relativa: " << humRel << "%\n";

        // 2. Almacenar lecturas
        std::cout << "\n[2/5]  Almacenando datos...\n";
        Lectura lecTemp(ahora, "TEMP", tempAmb, sensorTempAmb->evaluarEstado());
        Lectura lecHumSuelo(ahora, "HUM_SUELO", humSuelo, sensorHumSuelo->evaluarEstado());
        Lectura lecHumRel(ahora, "HUM_REL", humRel, sensorHumRel->evaluarEstado());
        
        historialLecturas->insertarFinal(lecTemp);
        historialLecturas->insertarFinal(lecHumSuelo);
        historialLecturas->insertarFinal(lecHumRel);
        indiceTimestamp->insertar(lecTemp);

        if (historialLecturas->getTamano() > maxLecturas) {
            historialLecturas->eliminarInicio();
        }

        std::cout << "   Lecturas almacenadas: " << historialLecturas->getTamano() << "\n";

        // 3. Verificar alarmas
        std::cout << "\n[3/5]  Verificando alarmas...\n";
        int alarmasAntes = colaAlarmas->getTamano();
        verificarAlarmasConModoControl(tempAmb, humSuelo, humRel, agua);
        int alarmsDespues = colaAlarmas->getTamano();
        
        bool cicloExitoso = (alarmsDespues == 0) && 
                            (tempAmb >= 18 && tempAmb <= 32) &&
                            (humSuelo >= 50 && humSuelo <= 80);
        
        if (cicloExitoso) {
            ciclosExitosos++;
            sistemaGameplay->ganarPuntos(10, "Ciclo exitoso");
            sistemaGameplay->actualizarMision(1);  // MANTENER_TEMPERATURA
            sistemaGameplay->actualizarMision(2);  // ALCANZAR_HUMEDAD
        }
        
        if (alarmsDespues < alarmasAntes) {
            totalAlarmasEvitadas++;
            sistemaGameplay->ganarPuntos(25, "Alarma evitada");
            sistemaGameplay->actualizarMision(4);  // SIN_ALARMAS
        }

        if (colaAlarmas->getTamano() > 0) {
            std::cout << "    " << colaAlarmas->getTamano() << " alarma(s) activa(s)\n";
        } else {
            std::cout << "   Sin alarmas\n";
        }

        // 4. Control automático con ÁRBOL o GRAFO
        if (modoAutomatico) {
            std::cout << "\n[4/5]  Ejecutando control automático [" << modoControl << "]...\n";
            
            // Preparar mapa de sensores
            std::map<std::string, double> sensores;
            sensores["TEMP"] = tempAmb;
            sensores["HUM_SUELO"] = humSuelo;
            sensores["HUM_REL"] = humRel;
            sensores["LUZ"] = luz;
            sensores["PH"] = ph;
            sensores["CO2"] = co2;
            sensores["AGUA"] = agua;

            if (modoControl == "ARBOL") {
                // Control basado en árbol de decisión
                std::cout << "\n   ÁRBOL DE DECISIÓN:\n";
                auto acciones = arbolControl->decidir(sensores);
                arbolControl->mostrarProcesoDecision();
                
                // Aplicar acciones
                for (const auto& accion : acciones) {
                    aplicarAccion(accion.actuador, accion.intensidad);
                }
            } else if (modoControl == "GRAFO") {
                // Control basado en grafo de estados
                std::cout << "\n   GRAFO DE ESTADOS:\n";
                std::string estadoAnterior = grafoEstados->getEstadoActual();
                std::string nuevoEstado = grafoEstados->evaluarTransiciones(sensores);
                
                if (estadoAnterior != nuevoEstado) {
                    std::cout << "   Transición: " << estadoAnterior 
                             << "  " << nuevoEstado << "\n";
                }
                
                grafoEstados->mostrarEstadoActual();
                
                // Aplicar configuración del estado
                auto config = grafoEstados->getConfiguracionActual();
                for (const auto& par : config) {
                    aplicarAccion(par.first, par.second);
                }
            }
        } else {
            std::cout << "\n[4/5]   Modo manual (sin control automático)\n";
        }

        // 5. Aplicar efectos de actuadores sobre sensores
        std::cout << "\n[5/5]  Aplicando efectos físicos...\n";
        if (ventilador->estaActivo()) {
            sensorTempAmb->aplicarControlCalor(ventilador->getEfectoTemp());
            std::cout << "    Ventilador enfriando\n";
        }
        if (calefactor->estaActivo()) {
            sensorTempAmb->aplicarControlCalor(calefactor->getEfectoTemp());
            std::cout << "   Calefactor calentando\n";
        }
        if (riego->estaActivo()) {
            sensorHumSuelo->aplicarRiego(riego->getEfectoHumedad());
            std::cout << "   Sistema de riego activo\n";
        }
        if (nebulizador->estaActivo()) {
            sensorHumRel->aplicarRiego(nebulizador->getEfectoHumedadRelativa());
            std::cout << "    Nebulizador aumentando humedad\n";
        }

        std::cout << "\n Ciclo completado\n";
    }

    // Aplicar acción de control a actuador
    void aplicarAccion(std::string actuador, double intensidad) {
        if (actuador == "VENTILADOR") {
            ventilador->ajustar(intensidad);
        } else if (actuador == "CALEFACTOR") {
            calefactor->ajustar(intensidad);
        } else if (actuador == "RIEGO") {
            riego->ajustar(intensidad);
        } else if (actuador == "LUZ_LED") {
            luzLED->ajustar(intensidad);
        } else if (actuador == "NEBULIZADOR") {
            if (intensidad > 0) nebulizador->ajustar(intensidad);
            else nebulizador->desactivar();
        }
    }

    // Verificar condiciones y generar alarmas
    void verificarAlarmasConModoControl(double temp, double humSuelo, double humRel, double agua) {
        double multiplicadorManual = modoAutomatico  ?1.0 : 1.5; // 50% más sensible en manual
        
        if (temp > (40.0 / multiplicadorManual) || temp < (5.0 / multiplicadorManual)) {
            Alarma alarma(1, "CRITICA", 
                "Temperatura critica: " + std::to_string((int)temp) + "C", 
                "TEMP", temp);
            colaAlarmas->insertar(alarma);
            logAlarmas->insertarFinal(alarma);
            controlActuadores->registrarAlarma(!modoAutomatico);
        }

        if (humSuelo < (30.0 / multiplicadorManual)) {
            Alarma alarma(2, "ALTA", 
                "Humedad del suelo critica: " + std::to_string((int)humSuelo) + "%",
                "HUM_SUELO", humSuelo);
            colaAlarmas->insertar(alarma);
            logAlarmas->insertarFinal(alarma);
            controlActuadores->registrarAlarma(!modoAutomatico);
        }

        if (agua < 50.0) {
            Alarma alarma(1, "CRITICA",
                "Nivel de agua critico: " + std::to_string((int)agua) + "L",
                "AGUA", agua);
            colaAlarmas->insertar(alarma);
            logAlarmas->insertarFinal(alarma);
            controlActuadores->registrarAlarma(!modoAutomatico);
        }
        
        if (!modoAutomatico && (humRel < 40 || humRel > 90)) {
            Alarma alarma(3, "MEDIA", 
                "Humedad relativa fuera de rango: " + std::to_string((int)humRel) + "%",
                "HUM_REL", humRel);
            colaAlarmas->insertar(alarma);
            logAlarmas->insertarFinal(alarma);
            controlActuadores->registrarAlarma(true);
        }
    }

    // Mostrar estado actual del sistema
    void mostrarEstado() {
        std::cout << "\n";
        std::cout << "+----------------------------------------------------+\n";
        std::cout << "¦       ESTADO DEL INVERNADERO INTELIGENTE          ¦\n";
        std::cout << "+----------------------------------------------------+\n";
        std::cout << "\n+--- SENSORES --------------------------------------+\n";
        std::cout << std::fixed << std::setprecision(1);
        std::cout << "¦ Temperatura Amb: " << std::setw(6) << sensorTempAmb->getValorActual() << " °C\n";
        std::cout << "¦ Humedad Suelo:   " << std::setw(6) << sensorHumSuelo->getValorActual() << " %\n";
        std::cout << "¦ Humedad Relativa:" << std::setw(6) << sensorHumRel->getValorActual() << " %\n";
        std::cout << "¦ Intensidad Luz:  " << std::setw(6) << sensorLuz->getValorActual() << " Lux\n";
        std::cout << "¦ pH Suelo:        " << std::setw(6) << sensorPH->getValorActual() << "\n";
        std::cout << "¦ CO2:             " << std::setw(6) << sensorCO2->getValorActual() << " ppm\n";
        std::cout << "¦ Nivel Agua:      " << std::setw(6) << sensorAgua->getValorActual() << " L\n";
        std::cout << "+---------------------------------------------------+\n";

        std::cout << "\n+--- ACTUADORES ------------------------------------+\n";
        std::cout << "¦ Ventilador:    " << std::setw(15) << ventilador->getEstado() << "\n";
        std::cout << "¦ Calefactor:    " << std::setw(15) << calefactor->getEstado() << "\n";
        std::cout << "¦ Luz LED:       " << std::setw(15) << luzLED->getEstado() << "\n";
        std::cout << "¦ Nebulizador:   " << std::setw(15) << nebulizador->getEstado() << "\n";
        std::cout << "+---------------------------------------------------+\n";

        std::cout << "\n+--- CONTROL INTELIGENTE ---------------------------+\n";
        std::cout << "¦ Modo: " << ( modoAutomatico ? "AUTOMATICO" : "MANUAL") << "\n";
        std::cout << "¦ Sistema: " << modoControl << "\n";
        if (modoControl == "GRAFO") {
            std::cout << "¦ Estado: " << grafoEstados->getEstadoActual() << "\n";
        }
        std::cout << "+---------------------------------------------------+\n";

        std::cout << "\n+--- ALARMAS ACTIVAS (" << colaAlarmas->getTamano() << ") ---------------------------+\n";
        if (!colaAlarmas->estaVacio()) {
            HeapPrioridad<Alarma> tempHeap = *colaAlarmas;
            int mostradas = 0;
            while (!tempHeap.estaVacio() && mostradas < 3) {
                Alarma a = tempHeap.extraerMin();
                std::cout << "¦ [" << a.getNivelPrioridad() << "] " 
                          << a.mensaje.substr(0, 40) << "\n";
                mostradas++;
            }
        } else {
            std::cout << "¦ No hay alarmas activas                            ¦\n";
        }
        std::cout << "+---------------------------------------------------+\n";

        std::cout << "\nCiclo: " << ciclosSimulacion 
                  << " | Lecturas: " << historialLecturas->getTamano() << "\n";
    }

    // Métodos para visualizar sistemas de control
    void mostrarArbolDecision() {
        arbolControl->mostrarArbol();
    }

    void mostrarGrafoEstados() {
        grafoEstados->mostrarGrafo();
    }

    // Cambiar modo de control
    void setModoControl(std::string modo) {
        if (modo == "ARBOL" || modo == "GRAFO") {
            modoControl = modo;
            pilaConfiguraciones->push("Modo control: " + modo);
            std::cout << "\n Modo de control cambiado a: " << modo << "\n";
        }
    }

    std::string getModoControl() const { return modoControl; }

    // Otros getters y setters
    void setModoAutomatico(bool modo) { modoAutomatico = modo; }
    bool getModoAutomatico() const { return modoAutomatico; }
    int getCiclosSimulacion() const { return ciclosSimulacion; }
    int getTamanoHistorial() const { return historialLecturas->getTamano(); }
    int getAlturaAVL() const { return indiceTimestamp->getAltura(); }
    int getNumAlarmas() const { return colaAlarmas->getTamano(); }

    void procesarAlarma() {
        if (!colaAlarmas->estaVacio()) {
            Alarma a = colaAlarmas->extraerMin();
            std::cout << "\n[ALARMA PROCESADA] " << a.getNivelPrioridad() 
                      << ": " << a.mensaje << "\n";
        }
    }

    // Control manual simplificado
    void ajustarVentilador(double intensidad) {
        ventilador->ajustar(intensidad);
        if (!modoAutomatico) {
            controlActuadores->registrarCambioManual();
        }
    }

    void ajustarCalefactor(double intensidad) {
        calefactor->ajustar(intensidad);
        if (!modoAutomatico) {
            controlActuadores->registrarCambioManual();
        }
    }

    void ajustarRiego(double intensidad) {
        riego->ajustar(intensidad);
        if (!modoAutomatico) {
            controlActuadores->registrarCambioManual();
        }
    }

    void ajustarLuzLED(double intensidad) {
        luzLED->ajustar(intensidad);
        if (!modoAutomatico) {
            controlActuadores->registrarCambioManual();
        }
    }

    void ajustarNebulizador(double intensidad) {
        nebulizador->ajustar(intensidad);
        if (!modoAutomatico) {
            controlActuadores->registrarCambioManual();
        }
    }

    void rellenarTanqueAgua(double litros) {
        sensorAgua->rellenar(litros);
        std::cout << "\n Tanque rellenado con " << litros << " litros.\n";
        std::cout << "  Nivel actual: " << sensorAgua->getValorActual() << " L\n";
    }

    // Análisis estadístico simplificado
    void mostrarEstadisticas() {
        if (historialLecturas->getTamano() < 10) {
            std::cout << "\nDatos insuficientes (mínimo 10 lecturas)\n";
            return;
        }

        std::vector<double> datosTemp;
        std::vector<Lectura> lecturas = historialLecturas->obtenerTodos();
        for (const Lectura& lec : lecturas) {
            if (lec.sensorID == "TEMP") {
                datosTemp.push_back(lec.valor);
            }
        }

        if (datosTemp.empty()) return;

        std::cout << "\n+----------------------------------------------------+\n";
        std::cout << "¦        ANÁLISIS ESTADÍSTICO - TEMPERATURA         ¦\n";
        std::cout << "+----------------------------------------------------+\n";
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "\nMuestras: " << datosTemp.size() << "\n";
        std::cout << "Promedio: " << Estadisticas::calcularPromedio(datosTemp) << " °C\n";
        std::cout << "Desviación: " << Estadisticas::calcularDesviacion(datosTemp) << " °C\n";
        std::cout << "Mínimo: " << Estadisticas::encontrarMinimo(datosTemp) << " °C\n";
        std::cout << "Máximo: " << Estadisticas::encontrarMaximo(datosTemp) << " °C\n";
    }

    // Mostrar estadísticas avanzadas
    void mostrarEstadisticasAvanzadas() {
        if (historialLecturas->getTamano() < 10) {
            std::cout << "\nDatos insuficientes (mínimo 10 lecturas)\n";
            return;
        }

        std::vector<double> datosTemp, datosHumSuelo, datosHumRel;
        std::vector<Lectura> lecturas = historialLecturas->obtenerTodos();
        
        for (const Lectura& lec : lecturas) {
            if (lec.sensorID == "TEMP") datosTemp.push_back(lec.valor);
            else if (lec.sensorID == "HUM_SUELO") datosHumSuelo.push_back(lec.valor);
            else if (lec.sensorID == "HUM_REL") datosHumRel.push_back(lec.valor);
        }

        std::cout << "\n+====================================================+\n";
        std::cout << "|         ESTADISTICAS AVANZADAS DEL SISTEMA         |\n";
        std::cout << "+====================================================+\n\n";

        std::cout << "+---- TEMPERATURA ------------------------------+\n";
        std::cout << "  Promedio:     " << std::fixed << std::setprecision(1)
                  << Estadisticas::calcularPromedio(datosTemp) << " °C\n";
        std::cout << "  Desv. Estándar: " << Estadisticas::calcularDesviacion(datosTemp) << " °C\n";
        std::cout << "  Rango:        " << Estadisticas::calcularRango(datosTemp) << " °C\n";
        std::cout << "  Mediana:      " << Estadisticas::calcularMediana(datosTemp) << " °C\n";
        std::cout << "+-------------------------------------------+\n";

        std::cout << "\n+---- HUMEDAD SUELO ----------------------------+\n";
        std::cout << "  Promedio:     " << Estadisticas::calcularPromedio(datosHumSuelo) << " %\n";
        std::cout << "  Desv. Estándar: " << Estadisticas::calcularDesviacion(datosHumSuelo) << " %\n";
        std::cout << "  Rango:        " << Estadisticas::calcularRango(datosHumSuelo) << " %\n";
        std::cout << "  Mediana:      " << Estadisticas::calcularMediana(datosHumSuelo) << " %\n";
        std::cout << "+-------------------------------------------+\n";

        std::cout << "\n+---- GENERAL ---------------------------------+\n";
        std::cout << "  Ciclos exitosos:   " << ciclosExitosos << "/" << ciclosSimulacion << "\n";
        std::cout << "  Alarmas evitadas:  " << totalAlarmasEvitadas << "\n";
        std::cout << "  Calidad promedio:  " << (ciclosExitosos * 100 / std::max(1, ciclosSimulacion)) << "%\n";
        std::cout << "+-------------------------------------------+\n";
    }

    // Mostrar tablero de juego
    void mostrarTableroJuego() {
        sistemaGameplay->mostrarTablero();
    }

    SistemaGameplay* getGameplay() { return sistemaGameplay; }

    DatosPartida exportarDatosPartida(const std::string& nombre, const std::string& desc = "") {
        DatosPartida datos;
        datos.nombrePartida = nombre;
        datos.ciclos = ciclosSimulacion;
        datos.puntuacion = sistemaGameplay->getPuntuacion();
        datos.nivel = sistemaGameplay->getNivel();
        datos.experiencia = sistemaGameplay->getExperiencia();
        datos.temperatura = sensorTempAmb->getValorActual();
        datos.humedad_suelo = sensorHumSuelo->getValorActual();
        datos.humedad_relativa = sensorHumRel->getValorActual();
        datos.nivel_agua = sensorAgua->getValorActual();
        datos.alarmas_activas = colaAlarmas->getTamano();
        datos.ciclos_exitosos = ciclosExitosos;
        datos.alarmas_evitadas = totalAlarmasEvitadas;
        datos.modo_control = modoControl;
        datos.modo_automatico = modoAutomatico;
        datos.fecha_guardado = time(nullptr);
        datos.descripcion = desc;
        
        return datos;
    }

    void importarDatosPartida(const DatosPartida& datos) {
        // Restaurar estado del sistema desde partida guardada
        ciclosSimulacion = datos.ciclos;
        modoControl = datos.modo_control;
        modoAutomatico = datos.modo_automatico;
        ciclosExitosos = datos.ciclos_exitosos;
        totalAlarmasEvitadas = datos.alarmas_evitadas;
        
        // Restaurar valores de sensores
        sensorTempAmb->aplicarControlCalor(datos.temperatura - sensorTempAmb->getValorActual());
        sensorHumSuelo->aplicarRiego(datos.humedad_suelo - sensorHumSuelo->getValorActual());
        sensorHumRel->aplicarRiego(datos.humedad_relativa - sensorHumRel->getValorActual());
        sensorAgua->rellenar(datos.nivel_agua - sensorAgua->getValorActual());
        
        // Restaurar estado de gameplay
        // Nota: Para restaurar completamente el gameplay, necesitaríamos más datos
        std::cout << "\n✓ Estado del invernadero restaurado.\n";
    }

    void mostrarComparativaModos() {
        controlActuadores->mostrarComparativa();
    }
};

#endif
