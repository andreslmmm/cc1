#ifndef COLA_HPP
#define COLA_HPP

#include "Nodo.hpp"
#include <stdexcept>

template <typename T>
class Cola {
private:
    Nodo<T>* frente;
    Nodo<T>* final;
    int tamano;

public:
    Cola() : frente(nullptr), final(nullptr), tamano(0) {}

    ~Cola() {
        while (!estaVacia()) {
            dequeue();
        }
    }

    // Insertar elemento al final - O(1)
    void enqueue(T dato) {
        Nodo<T>* nuevo = new Nodo<T>(dato);
        if (estaVacia()) {
            frente = final = nuevo;
        } else {
            final->siguiente = nuevo;
            final = nuevo;
        }
        tamano++;
    }

    // Eliminar y retornar elemento del frente - O(1)
    T dequeue() {
        if (estaVacia()) {
            throw std::runtime_error("Cola vacia");
        }
        Nodo<T>* temp = frente;
        T dato = frente->dato;
        frente = frente->siguiente;
        if (frente == nullptr) {
            final = nullptr;
        }
        delete temp;
        tamano--;
        return dato;
    }

    // Ver elemento del frente sin eliminarlo - O(1)
    T front() const {
        if (estaVacia()) {
            throw std::runtime_error("Cola vacia");
        }
        return frente->dato;
    }

    bool estaVacia() const {
        return frente == nullptr;
    }

    int getTamano() const {
        return tamano;
    }

    void limpiar() {
        while (!estaVacia()) {
            dequeue();
        }
    }
};

#endif
