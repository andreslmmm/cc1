#ifndef LISTA_CIRCULAR_HPP
#define LISTA_CIRCULAR_HPP

#include "Nodo.hpp"
#include <stdexcept>
#include <vector>

template <typename T>
class ListaCircular {
private:
    NodoDoble<T>* cabeza;
    int tamano;

public:
    ListaCircular() : cabeza(nullptr), tamano(0) {}
    ~ListaCircular() { limpiar(); }

    // Insertar al final - O(1)
    void insertar(T dato) {
        NodoDoble<T>* nuevo = new NodoDoble<T>(dato);
        if (estaVacia()) {
            cabeza = nuevo;
            cabeza->siguiente = cabeza;
            cabeza->anterior = cabeza;
        } else {
            nuevo->siguiente = cabeza;
            nuevo->anterior = cabeza->anterior;
            cabeza->anterior->siguiente = nuevo;
            cabeza->anterior = nuevo;
        }
        tamano++;
    }

    // Eliminar elemento - O(n) (busca y elimina)
    void eliminar(T dato) {
        if (estaVacia()) return;
        NodoDoble<T>* actual = cabeza;
        do {
            if (actual->dato == dato) {
                if (tamano == 1) {
                    delete cabeza;
                    cabeza = nullptr;
                } else {
                    actual->anterior->siguiente = actual->siguiente;
                    actual->siguiente->anterior = actual->anterior;
                    if (actual == cabeza) cabeza = actual->siguiente;
                    delete actual;
                }
                tamano--;
                return;
            }
            actual = actual->siguiente;
        } while (actual != cabeza);
    }

    // Buscar - O(n)
    bool buscar(T dato) const {
        if (estaVacia()) return false;
        NodoDoble<T>* actual = cabeza;
        do {
            if (actual->dato == dato) return true;
            actual = actual->siguiente;
        } while (actual != cabeza);
        return false;
    }

    bool estaVacia() const { return cabeza == nullptr; }
    int getTamano() const { return tamano; }

    void limpiar() {
        if (estaVacia()) return;
        NodoDoble<T>* actual = cabeza;
        do {
            NodoDoble<T>* temp = actual;
            actual = actual->siguiente;
            delete temp;
        } while (actual != cabeza);
        cabeza = nullptr;
        tamano = 0;
    }

    // Obtener todos como vector
    std::vector<T> obtenerTodos() const {
        std::vector<T> resultado;
        if (estaVacia()) return resultado;
        NodoDoble<T>* actual = cabeza;
        do {
            resultado.push_back(actual->dato);
            actual = actual->siguiente;
        } while (actual != cabeza);
        return resultado;
    }
};

#endif