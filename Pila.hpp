#ifndef PILA_HPP
#define PILA_HPP

#include "Nodo.hpp"
#include <stdexcept>

template <typename T>
class Pila {
private:
    Nodo<T>* tope;
    int tamano;

public:
    Pila() : tope(nullptr), tamano(0) {}

    ~Pila() {
        while (!estaVacia()) {
            pop();
        }
    }

    // Insertar elemento en el tope - O(1)
    void push(T dato) {
        Nodo<T>* nuevo = new Nodo<T>(dato);
        nuevo->siguiente = tope;
        tope = nuevo;
        tamano++;
    }

    // Eliminar y retornar elemento del tope - O(1)
    T pop() {
        if (estaVacia()) {
            throw std::runtime_error("Pila vacia");
        }
        Nodo<T>* temp = tope;
        T dato = tope->dato;
        tope = tope->siguiente;
        delete temp;
        tamano--;
        return dato;
    }

    // Ver elemento del tope sin eliminarlo - O(1)
    T top() const {
        if (estaVacia()) {
            throw std::runtime_error("Pila vacia");
        }
        return tope->dato;
    }

    bool estaVacia() const {
        return tope == nullptr;
    }

    int getTamano() const {
        return tamano;
    }

    void limpiar() {
        while (!estaVacia()) {
            pop();
        }
    }
};

#endif
