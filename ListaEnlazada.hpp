#ifndef LISTA_ENLAZADA_HPP
#define LISTA_ENLAZADA_HPP

#include "Nodo.hpp"
#include <stdexcept>

template <typename T>
class ListaEnlazada {
private:
    Nodo<T>* cabeza;
    Nodo<T>* cola;
    int tamano;

public:
    ListaEnlazada() : cabeza(nullptr), cola(nullptr), tamano(0) {}

    ~ListaEnlazada() {
        limpiar();
    }

    // Insertar al inicio - O(1)
    void insertarInicio(T dato) {
        Nodo<T>* nuevo = new Nodo<T>(dato);
        if (estaVacia()) {
            cabeza = cola = nuevo;
        } else {
            nuevo->siguiente = cabeza;
            cabeza = nuevo;
        }
        tamano++;
    }

    // Insertar al final - O(1) con puntero cola
    void insertarFinal(T dato) {
        Nodo<T>* nuevo = new Nodo<T>(dato);
        if (estaVacia()) {
            cabeza = cola = nuevo;
        } else {
            cola->siguiente = nuevo;
            cola = nuevo;
        }
        tamano++;
    }

    // Buscar elemento - O(n)
    bool buscar(T dato) const {
        Nodo<T>* actual = cabeza;
        while (actual != nullptr) {
            if (actual->dato == dato) {
                return true;
            }
            actual = actual->siguiente;
        }
        return false;
    }

    // Eliminar primer elemento - O(1)
    T eliminarInicio() {
        if (estaVacia()) {
            throw std::runtime_error("Lista vacia");
        }
        Nodo<T>* temp = cabeza;
        T dato = cabeza->dato;
        cabeza = cabeza->siguiente;
        if (cabeza == nullptr) {
            cola = nullptr;
        }
        delete temp;
        tamano--;
        return dato;
    }

    // Obtener elemento en posici�n i - O(n)
    T obtener(int indice) const {
        if (indice < 0 || indice >= tamano) {
            throw std::out_of_range("Indice fuera de rango");
        }
        Nodo<T>* actual = cabeza;
        for (int i = 0; i < indice; i++) {
            actual = actual->siguiente;
        }
        return actual->dato;
    }

    bool estaVacia() const {
        return cabeza == nullptr;
    }

    int getTamano() const {
        return tamano;
    }

    void limpiar() {
        while (!estaVacia()) {
            eliminarInicio();
        }
    }

    // Recorrido para procesamiento
    template <typename Func>
    void recorrer(Func funcion) {
        Nodo<T>* actual = cabeza;
        while (actual != nullptr) {
            funcion(actual->dato);
            actual = actual->siguiente;
        }
    }
};

#endif
