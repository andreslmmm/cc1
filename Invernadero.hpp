#ifndef INVERNADERO_HPP
#define INVERNADERO_HPP

#include "Sensor.hpp"
#include "Actuador.hpp"
#include "Lectura.hpp"
#include "Alarma.hpp"
#include "ControladorPID.hpp"
#include "ListaEnlazada.hpp"
#include "ListaDoble.hpp"
#include "Pila.hpp"
#include "Cola.hpp"
#include "HeapPrioridad.hpp"
#include "ArbolAVL.hpp"
#include "Estadisticas.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

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

    // Controladores PID
    ControladorPID* pidTemperatura;
    ControladorPID* pidHumedadSuelo;
    ControladorPID* pidHumedadRelativa;

    // Estructuras de datos
    ListaDoble<Lectura>* historialLecturas;  // Series temporales
    HeapPrioridad<Alarma>* colaAlarmas;      // Cola de prioridad
    ArbolAVL<Lectura>* indiceTimestamp;      // Índice por timestamp
    Pila<std::string>* pilaConfiguraciones;  // Historial de cambios
    Cola<std::string>* colaComandos;         // Comandos pendientes
    ListaEnlazada<Alarma>* logAlarmas;       // Log completo de alarmas

    // Configuración
    int maxLecturas;
    bool modoAutomatico;
    int ciclosSimulacion;

public:
    Invernadero() : maxLecturas(1000), modoAutomatico(true), ciclosSimulacion(0) {
        // Inicializar sensores
        sensorTempAmb = new SensorTemperatura("TEMP_AMB", 25.0);
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

        // Inicializar controladores PID
        pidTemperatura = new ControladorPID("PID_TEMP", 2.0, 0.5, 1.0);
        pidTemperatura->setSetpoint(25.0);
        pidTemperatura->setLimitesSalida(-100, 100);

        pidHumedadSuelo = new ControladorPID("PID_HUM_SUELO", 1.5, 0.3, 0.5);
        pidHumedadSuelo->setSetpoint(70.0);
        pidHumedadSuelo->setLimitesSalida(-100, 100);

        pidHumedadRelativa = new ControladorPID("PID_HUM_REL", 1.0, 0.2, 0.3);
        pidHumedadRelativa->setSetpoint(72.0);
        pidHumedadRelativa->setLimitesSalida(-100, 100);

        // Inicializar estructuras de datos
        historialLecturas = new ListaDoble<Lectura>();
        colaAlarmas = new HeapPrioridad<Alarma>();
        indiceTimestamp = new ArbolAVL<Lectura>();
        pilaConfiguraciones = new Pila<std::string>();
        colaComandos = new Cola<std::string>();
        logAlarmas = new ListaEnlazada<Alarma>();
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
        delete pidTemperatura;
        delete pidHumedadSuelo;
        delete pidHumedadRelativa;
        delete historialLecturas;
        delete colaAlarmas;
        delete indiceTimestamp;
        delete pilaConfiguraciones;
        delete colaComandos;
        delete logAlarmas;
    }

    // Ciclo principal de control - O(1) por ciclo
    void ejecutarCicloControl() {
        ciclosSimulacion++;

        // 1. Leer todos los sensores - O(s) donde s = número de sensores
        time_t ahora = time(nullptr);
        double tempAmb = sensorTempAmb->leer();
        double humRel = sensorHumRel->leer();
        double humSuelo = sensorHumSuelo->leer();
        double luz = sensorLuz->leer();
        double ph = sensorPH->leer();
        double co2 = sensorCO2->leer();
        double agua = sensorAgua->leer();

        // 2. Almacenar lecturas - O(1) cada una
        Lectura lecTemp(ahora, "TEMP_AMB", tempAmb, sensorTempAmb->evaluarEstado());
        Lectura lecHumSuelo(ahora, "HUM_SUELO", humSuelo, sensorHumSuelo->evaluarEstado());
        Lectura lecHumRel(ahora, "HUM_REL", humRel, sensorHumRel->evaluarEstado());
        Lectura lecLuz(ahora, "LUZ", luz, sensorLuz->evaluarEstado());
        Lectura lecPH(ahora, "PH", ph, sensorPH->evaluarEstado());
        Lectura lecCO2(ahora, "CO2", co2, sensorCO2->evaluarEstado());
        Lectura lecAgua(ahora, "AGUA", agua, sensorAgua->evaluarEstado());
        

        
        historialLecturas->insertarFinal(lecTemp);
        historialLecturas->insertarFinal(lecHumSuelo);
        historialLecturas->insertarFinal(lecHumRel);
        historialLecturas->insertarFinal(lecLuz);
        historialLecturas->insertarFinal(lecPH);
        historialLecturas->insertarFinal(lecCO2);
        historialLecturas->insertarFinal(lecAgua);
        

        // Insertar en AVL para búsqueda rápida - O(log n)
        indiceTimestamp->insertar(lecTemp);

        // Limitar tamaño del historial
        if (historialLecturas->getTamano() > maxLecturas) {
            historialLecturas->eliminarInicio();
        }

        // 3. Verificar alarmas - O(1) por sensor
        verificarAlarmas(tempAmb, humSuelo, humRel, agua);

        // 4. Control automático con PID
        if (modoAutomatico) {
            // Control de temperatura
            double salidaPIDTemp = pidTemperatura->calcular(tempAmb);
            if (salidaPIDTemp > 0) {
                // Calentar
                calefactor->ajustar(salidaPIDTemp);
                ventilador->ajustar(0);
            } else {
                // Enfriar
                ventilador->ajustar(-salidaPIDTemp);
                calefactor->ajustar(0);
            }

            // Control de humedad del suelo
            double salidaPIDHumSuelo = pidHumedadSuelo->calcular(humSuelo);
            if (salidaPIDHumSuelo < 0) {
                // Necesita riego (error negativo)
                riego->ajustar(-salidaPIDHumSuelo);
            } else {
                riego->ajustar(0);
            }

            // Control de humedad relativa
            double salidaPIDHumRel = pidHumedadRelativa->calcular(humRel);
            if (salidaPIDHumRel < 0) {
                nebulizador->activar();
            } else {
                nebulizador->desactivar();
            }
        }

        // 5. Aplicar efectos de actuadores sobre sensores
        if (ventilador->estaActivo()) {
            sensorTempAmb->aplicarControlCalor(ventilador->getEfectoTemp());
        }
        if (calefactor->estaActivo()) {
            sensorTempAmb->aplicarControlCalor(calefactor->getEfectoTemp());
        }
        if (riego->estaActivo()) {
            sensorHumSuelo->aplicarRiego(riego->getEfectoHumedad());
        }
        if (nebulizador->estaActivo()) {
            sensorHumRel->aplicarRiego(nebulizador->getEfectoHumedadRelativa());
        }
    }

    // Verificar condiciones y generar alarmas - O(1)
    void verificarAlarmas(double temp, double humSuelo, double humRel, double agua) {
        // Temperatura crítica
        if (temp > 40.0 || temp < 5.0) {
            Alarma alarma(1, "CRITICA", 
                "Temperatura critica: " + std::to_string(temp) + "C", 
                "TEMP_AMB", temp);
            colaAlarmas->insertar(alarma);
            logAlarmas->insertarFinal(alarma);
        } else if (temp > 35.0 || temp < 10.0) {
            Alarma alarma(2, "ALTA", 
                "Temperatura fuera de rango optimo: " + std::to_string(temp) + "C",
                "TEMP_AMB", temp);
            colaAlarmas->insertar(alarma);
            logAlarmas->insertarFinal(alarma);
        }

        // Humedad del suelo baja
        if (humSuelo < 30.0) {
            Alarma alarma(2, "ALTA", 
                "Humedad del suelo critica: " + std::to_string(humSuelo) + "%",
                "HUM_SUELO", humSuelo);
            colaAlarmas->insertar(alarma);
            logAlarmas->insertarFinal(alarma);
        } else if (humSuelo < 50.0) {
            Alarma alarma(3, "MEDIA",
                "Humedad del suelo baja: " + std::to_string(humSuelo) + "%",
                "HUM_SUELO", humSuelo);
            colaAlarmas->insertar(alarma);
            logAlarmas->insertarFinal(alarma);
        }

        // Nivel de agua bajo
        if (agua < 50.0) {
            Alarma alarma(1, "CRITICA",
                "Nivel de agua critico: " + std::to_string(agua) + "L",
                "AGUA", agua);
            colaAlarmas->insertar(alarma);
            logAlarmas->insertarFinal(alarma);
        } else if (agua < 200.0) {
            Alarma alarma(2, "ALTA",
                "Nivel de agua bajo: " + std::to_string(agua) + "L",
                "AGUA", agua);
            colaAlarmas->insertar(alarma);
            logAlarmas->insertarFinal(alarma);
        }
    }

    // Mostrar estado actual del sistema
    void mostrarEstado() {
        std::cout << "\n";
        std::cout << "╔════════════════════════════════════════════════════╗\n";
        std::cout << "║       ESTADO DEL INVERNADERO INTELIGENTE          ║\n";
        std::cout << "╚════════════════════════════════════════════════════╝\n";
        std::cout << "\n┌─── SENSORES ──────────────────────────────────────┐\n";
        std::cout << std::fixed << std::setprecision(1);
        std::cout << "│ Temperatura Amb: " << std::setw(6) << sensorTempAmb->getValorActual() 
                  << " °C  │ Setpoint: " << std::setw(6) << pidTemperatura->getSetpoint() << " °C\n";
        std::cout << "│ Humedad Suelo:   " << std::setw(6) << sensorHumSuelo->getValorActual() 
                  << " %   │ Setpoint: " << std::setw(6) << pidHumedadSuelo->getSetpoint() << " %\n";
        std::cout << "│ Humedad Relativa:" << std::setw(6) << sensorHumRel->getValorActual() 
                  << " %   │ Setpoint: " << std::setw(6) << pidHumedadRelativa->getSetpoint() << " %\n";
        std::cout << "│ Intensidad Luz:  " << std::setw(6) << sensorLuz->getValorActual() 
                  << " Lux │ pH Suelo: " << std::setw(6) << sensorPH->getValorActual() << "\n";
        std::cout << "│ CO2:             " << std::setw(6) << sensorCO2->getValorActual() 
                  << " ppm │ Nivel Agua: " << std::setw(6) << sensorAgua->getValorActual() << " L\n";
        std::cout << "└───────────────────────────────────────────────────┘\n";

        std::cout << "\n┌─── ACTUADORES ────────────────────────────────────┐\n";
        std::cout << "│ Ventilador:    " << std::setw(15) << ventilador->getEstado() << "\n";
        std::cout << "│ Calefactor:    " << std::setw(15) << calefactor->getEstado() << "\n";
        std::cout << "│ Sistema Riego: " << std::setw(15) << riego->getEstado() << "\n";
        std::cout << "│ Luz LED:       " << std::setw(15) << luzLED->getEstado() << "\n";
        std::cout << "│ Nebulizador:   " << std::setw(15) << nebulizador->getEstado() << "\n";
        std::cout << "└───────────────────────────────────────────────────┘\n";

        std::cout << "\n┌─── ALARMAS ACTIVAS (" << colaAlarmas->getTamano() << ") ───────────────────────────┐\n";
        if (!colaAlarmas->estaVacio()) {
            // Mostrar solo las 3 alarmas más urgentes
            HeapPrioridad<Alarma> tempHeap = *colaAlarmas;
            int mostradas = 0;
            while (!tempHeap.estaVacio() && mostradas < 3) {
                Alarma a = tempHeap.extraerMin();
                std::cout << "│ [" << a.getNivelPrioridad() << "] " 
                          << a.mensaje.substr(0, 40) << "\n";
                mostradas++;
            }
        } else {
            std::cout << "│ No hay alarmas activas                            │\n";
        }
        std::cout << "└───────────────────────────────────────────────────┘\n";

        std::cout << "\nCiclo: " << ciclosSimulacion 
                  << " | Modo: " << (modoAutomatico ? "AUTOMATICO" : "MANUAL")
                  << " | Lecturas almacenadas: " << historialLecturas->getTamano() << "\n";
    }

    // Análisis estadístico del historial
    void mostrarEstadisticas() {
        if (historialLecturas->getTamano() < 10) {
            std::cout << "\nDatos insuficientes para analisis (minimo 10 lecturas)\n";
            return;
        }

        // Extraer datos de temperatura
        std::vector<double> datosTemp;
        std::vector<Lectura> lecturas = historialLecturas->obtenerTodos();
        for (const Lectura& lec : lecturas) {
            if (lec.sensorID == "TEMP_AMB") {
                datosTemp.push_back(lec.valor);
            }
        }

        if (datosTemp.empty()) return;

        std::cout << "\n╔════════════════════════════════════════════════════╗\n";
        std::cout << "║           ANALISIS ESTADISTICO - TEMPERATURA       ║\n";
        std::cout << "╚════════════════════════════════════════════════════╝\n";
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "\nMuestras analizadas: " << datosTemp.size() << "\n";
        std::cout << "Promedio:            " << Estadisticas::calcularPromedio(datosTemp) << " °C\n";
        std::cout << "Desviacion estandar: " << Estadisticas::calcularDesviacion(datosTemp) << " °C\n";
        std::cout << "Minimo:              " << Estadisticas::encontrarMinimo(datosTemp) << " °C\n";
        std::cout << "Maximo:              " << Estadisticas::encontrarMaximo(datosTemp) << " °C\n";
        std::cout << "Tendencia:           " << Estadisticas::calcularTendencia(datosTemp) << " °C/ciclo\n";

        // Detección de anomalías
        std::vector<int> anomalias = Estadisticas::detectarAnomalias(datosTemp);
        std::cout << "\nAnomalias detectadas: " << anomalias.size() 
                  << " (" << (100.0 * anomalias.size() / datosTemp.size()) << "%)\n";

        // Promedio móvil
        if (datosTemp.size() >= 10) {
            std::vector<double> promedioMov = Estadisticas::promedioMovil(datosTemp, 10);
            std::cout << "\nPromedio movil (ventana=10): ";
            std::cout << promedioMov.back() << " °C (ultimo valor)\n";
        }
    }

    // Getters y setters
    void setModoAutomatico(bool modo) { modoAutomatico = modo; }
    bool getModoAutomatico() const { return modoAutomatico; }

    void setSetpointTemperatura(double sp) {
        pidTemperatura->setSetpoint(sp);
        pilaConfiguraciones->push("Setpoint Temp: " + std::to_string(sp));
    }

    void setSetpointHumedadSuelo(double sp) {
        pidHumedadSuelo->setSetpoint(sp);
        pilaConfiguraciones->push("Setpoint Hum Suelo: " + std::to_string(sp));
    }

    void ajustarVentilador(double porcentaje) {
        ventilador->ajustar(porcentaje);
    }

    void ajustarCalefactor(double porcentaje) {
        calefactor->ajustar(porcentaje);
    }

    void ajustarRiego(double porcentaje) {
        riego->ajustar(porcentaje);
    }

    void ajustarLuzLED(double porcentaje) {
        luzLED->ajustar(porcentaje);
    }

    void activarNebulizador() {
        nebulizador->activar();
    }

    void desactivarNebulizador() {
        nebulizador->desactivar();
    }

    int getNumAlarmas() const {
        return colaAlarmas->getTamano();
    }

    void procesarAlarma() {
        if (!colaAlarmas->estaVacio()) {
            Alarma a = colaAlarmas->extraerMin();
            std::cout << "\n[ALARMA PROCESADA] " << a.getNivelPrioridad() 
                      << ": " << a.mensaje << "\n";
        }
    }

    void rellenarTanqueAgua(double litros) {
        sensorAgua->rellenar(litros);
        std::cout << "\nTanque rellenado con " << litros << " litros.\n";
        std::cout << "Nivel actual: " << sensorAgua->getValorActual() << " L\n";
    }

    ControladorPID* getPIDTemperatura() { return pidTemperatura; }
    ControladorPID* getPIDHumedadSuelo() { return pidHumedadSuelo; }
    ControladorPID* getPIDHumedadRelativa() { return pidHumedadRelativa; }

    int getCiclosSimulacion() const { return ciclosSimulacion; }
    int getTamanoHistorial() const { return historialLecturas->getTamano(); }
    int getAlturaAVL() const { return indiceTimestamp->getAltura(); }
};

#endif