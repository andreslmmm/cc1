#ifndef NODO_HPP
#define NODO_HPP

// Nodo genérico para lista enlazada simple
template <typename T>
class Nodo {
public:
    T dato;
    Nodo<T>* siguiente;

    Nodo(T valor) : dato(valor), siguiente(nullptr) {}
};

// Nodo para lista doblemente enlazada
template <typename T>
class NodoDoble {
public:
    T dato;
    NodoDoble<T>* siguiente;
    NodoDoble<T>* anterior;

    NodoDoble(T valor) : dato(valor), siguiente(nullptr), anterior(nullptr) {}
};

// Nodo para árbol AVL
template <typename T>
class NodoAVL {
public:
    T dato;
    NodoAVL<T>* izquierdo;
    NodoAVL<T>* derecho;
    int altura;

    NodoAVL(T valor) : dato(valor), izquierdo(nullptr), derecho(nullptr), altura(1) {}
};

#endif