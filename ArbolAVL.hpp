#ifndef ARBOL_AVL_HPP
#define ARBOL_AVL_HPP

#include "Nodo.hpp"
#include <algorithm>
#include <vector>
#include <stdexcept>

template <typename T>
class ArbolAVL {
private:
    NodoAVL<T>* raiz;
    int tamano;

    // Obtener altura del nodo - O(1)
int altura(NodoAVL<T>* nodo) const {
        return nodo ? nodo->altura : 0;
    }

    // Obtener factor de balance - O(1)
    int factorBalance(NodoAVL<T>* nodo) {
        return nodo ? altura(nodo->izquierdo) - altura(nodo->derecho) : 0;
    }

    // Actualizar altura del nodo - O(1)
    void actualizarAltura(NodoAVL<T>* nodo) {
        if (nodo) {
            nodo->altura = 1 + std::max(altura(nodo->izquierdo), altura(nodo->derecho));
        }
    }

    // Rotación simple a la derecha - O(1)
    NodoAVL<T>* rotarDerecha(NodoAVL<T>* y) {
        NodoAVL<T>* x = y->izquierdo;
        NodoAVL<T>* T2 = x->derecho;

        x->derecho = y;
        y->izquierdo = T2;

        actualizarAltura(y);
        actualizarAltura(x);

        return x;
    }

    // Rotación simple a la izquierda - O(1)
    NodoAVL<T>* rotarIzquierda(NodoAVL<T>* x) {
        NodoAVL<T>* y = x->derecho;
        NodoAVL<T>* T2 = y->izquierdo;

        y->izquierdo = x;
        x->derecho = T2;

        actualizarAltura(x);
        actualizarAltura(y);

        return y;
    }

    // Balancear nodo después de inserción/eliminación - O(1)
    NodoAVL<T>* balancear(NodoAVL<T>* nodo) {
        actualizarAltura(nodo);
        int balance = factorBalance(nodo);

        // Caso Izquierda-Izquierda
        if (balance > 1 && factorBalance(nodo->izquierdo) >= 0) {
            return rotarDerecha(nodo);
        }

        // Caso Derecha-Derecha
        if (balance < -1 && factorBalance(nodo->derecho) <= 0) {
            return rotarIzquierda(nodo);
        }

        // Caso Izquierda-Derecha
        if (balance > 1 && factorBalance(nodo->izquierdo) < 0) {
            nodo->izquierdo = rotarIzquierda(nodo->izquierdo);
            return rotarDerecha(nodo);
        }

        // Caso Derecha-Izquierda
        if (balance < -1 && factorBalance(nodo->derecho) > 0) {
            nodo->derecho = rotarDerecha(nodo->derecho);
            return rotarIzquierda(nodo);
        }

        return nodo;
    }

    // Insertar recursivo - O(log n)
    NodoAVL<T>* insertarRec(NodoAVL<T>* nodo, T dato) {
        if (!nodo) {
            tamano++;
            return new NodoAVL<T>(dato);
        }

        if (dato < nodo->dato) {
            nodo->izquierdo = insertarRec(nodo->izquierdo, dato);
        } else if (dato > nodo->dato) {
            nodo->derecho = insertarRec(nodo->derecho, dato);
        } else {
            return nodo; // No insertar duplicados
        }

        return balancear(nodo);
    }

    // Buscar recursivo - O(log n)
    bool buscarRec(NodoAVL<T>* nodo, T dato) const {
        if (!nodo) return false;
        if (dato == nodo->dato) return true;
        if (dato < nodo->dato) return buscarRec(nodo->izquierdo, dato);
        return buscarRec(nodo->derecho, dato);
    }

    // Recorrido inorden recursivo - O(n)
    void inordenRec(NodoAVL<T>* nodo, std::vector<T>& resultado) const {
        if (nodo) {
            inordenRec(nodo->izquierdo, resultado);
            resultado.push_back(nodo->dato);
            inordenRec(nodo->derecho, resultado);
        }
    }

    // Buscar en rango - O(log n + k) donde k = elementos en rango
    void buscarRangoRec(NodoAVL<T>* nodo, T min, T max, std::vector<T>& resultado) const {
        if (!nodo) return;

        if (min < nodo->dato) {
            buscarRangoRec(nodo->izquierdo, min, max, resultado);
        }

        if (min <= nodo->dato && nodo->dato <= max) {
            resultado.push_back(nodo->dato);
        }

        if (nodo->dato < max) {
            buscarRangoRec(nodo->derecho, min, max, resultado);
        }
    }

    // Liberar memoria recursivo
    void liberarRec(NodoAVL<T>* nodo) {
        if (nodo) {
            liberarRec(nodo->izquierdo);
            liberarRec(nodo->derecho);
            delete nodo;
        }
    }

public:
    ArbolAVL() : raiz(nullptr), tamano(0) {}

    ~ArbolAVL() {
        liberarRec(raiz);
    }

    // Insertar elemento - O(log n)
    void insertar(T dato) {
        raiz = insertarRec(raiz, dato);
    }

    // Buscar elemento - O(log n)
    bool buscar(T dato) const {
        return buscarRec(raiz, dato);
    }

    // Obtener elementos en orden - O(n)
    std::vector<T> inorden() const {
        std::vector<T> resultado;
        inordenRec(raiz, resultado);
        return resultado;
    }

    // Buscar elementos en rango [min, max] - O(log n + k)
    std::vector<T> buscarRango(T min, T max) const {
        std::vector<T> resultado;
        buscarRangoRec(raiz, min, max, resultado);
        return resultado;
    }

    bool estaVacio() const {
        return raiz == nullptr;
    }

    int getTamano() const {
        return tamano;
    }

    int getAltura() const {
        return altura(raiz);
    }
};

#endif