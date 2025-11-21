#ifndef CONTROLADOR_PID_HPP
#define CONTROLADOR_PID_HPP

#include <string>

// Controlador PID discreto para regulación automática
class ControladorPID {
private:
    std::string nombre;
    double Kp; // Ganancia proporcional
    double Ki; // Ganancia integral
    double Kd; // Ganancia derivativa

    double setpoint;       // Valor deseado
    double errorAnterior;  // Error del ciclo anterior
    double integral;       // Acumulador del error integral

    double salidaMin;      // Límite inferior de salida
    double salidaMax;      // Límite superior de salida

    bool activo;

public:
    ControladorPID(std::string _nombre, double _Kp = 1.0, double _Ki = 0.1, double _Kd = 0.05)
        : nombre(_nombre), Kp(_Kp), Ki(_Ki), Kd(_Kd), 
          setpoint(0), errorAnterior(0), integral(0),
          salidaMin(-100), salidaMax(100), activo(true) {}

    // Calcular salida del controlador PID - O(1)
    double calcular(double medicion, double deltaT = 1.0) {
        if (!activo) return 0.0;

        double error = setpoint - medicion;
        double proporcional = Kp * error;

        integral += error * deltaT;
        if (integral > 100) integral = 100;
        if (integral < -100) integral = -100;
        double integralTerm = Ki * integral;

        double derivativo = Kd * (error - errorAnterior) / deltaT;

        double salida = proporcional + integralTerm + derivativo;

        if (salida > salidaMax) salida = salidaMax;
        if (salida < salidaMin) salida = salidaMin;

        errorAnterior = error;
        return salida;
    }

    void resetear() {
        errorAnterior = 0;
        integral = 0;
    }

    // Configuración
    void setSetpoint(double valor) { setpoint = valor; }
    void setParametros(double _Kp, double _Ki, double _Kd) {
        Kp = _Kp;
        Ki = _Ki;
        Kd = _Kd;
    }
    void setLimitesSalida(double min, double max) {
        salidaMin = min;
        salidaMax = max;
    }

    // Getters
    double getSetpoint() const { return setpoint; }
    double getKp() const { return Kp; }
    double getKi() const { return Ki; }
    double getKd() const { return Kd; }
    double getErrorAnterior() const { return errorAnterior; }
    double getIntegral() const { return integral; }
    std::string getNombre() const { return nombre; }

    void activar() { activo = true; }
    void desactivar() { 
        activo = false;
        resetear();
    }
    bool estaActivo() const { return activo; }
};

#endif