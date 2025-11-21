        #ifndef LISTA_DOBLE_HPP
        #define LISTA_DOBLE_HPP

        #include "Nodo.hpp"
        #include <stdexcept>
        #include <vector>

        template <typename T>
        class ListaDoble {
        private:
            NodoDoble<T>* cabeza;
            NodoDoble<T>* cola;
            int tamano;

        public:
            ListaDoble() : cabeza(nullptr), cola(nullptr), tamano(0) {}

            ~ListaDoble() {
                limpiar();
            }

            // Insertar al inicio - O(1)
            void insertarInicio(T dato) {
                NodoDoble<T>* nuevo = new NodoDoble<T>(dato);
                if (estaVacia()) {
                    cabeza = cola = nuevo;
                } else {
                    nuevo->siguiente = cabeza;
                    cabeza->anterior = nuevo;
                    cabeza = nuevo;
                }
                tamano++;
            }

            // Insertar al final - O(1)
            void insertarFinal(T dato) {
                NodoDoble<T>* nuevo = new NodoDoble<T>(dato);
                if (estaVacia()) {
                    cabeza = cola = nuevo;
                } else {
                    cola->siguiente = nuevo;
                    nuevo->anterior = cola;
                    cola = nuevo;
                }
                tamano++;
            }

            // Eliminar primer elemento - O(1)
            T eliminarInicio() {
                if (estaVacia()) {
                    throw std::runtime_error("Lista vacía");
                }
                NodoDoble<T>* temp = cabeza;
                T dato = cabeza->dato;
                cabeza = cabeza->siguiente;
                if (cabeza != nullptr) {
                    cabeza->anterior = nullptr;
                } else {
                    cola = nullptr;
                }
                delete temp;
                tamano--;
                return dato;
            }

            // Eliminar último elemento - O(1)
            T eliminarFinal() {
                if (estaVacia()) {
                    throw std::runtime_error("Lista vacía");
                }
                NodoDoble<T>* temp = cola;
                T dato = cola->dato;
                cola = cola->anterior;
                if (cola != nullptr) {
                    cola->siguiente = nullptr;
                } else {
                    cabeza = nullptr;
                }
                delete temp;
                tamano--;
                return dato;
            }

            // Obtener elemento en posición i - O(n)
            T obtener(int indice) const {
                if (indice < 0 || indice >= tamano) {
                    throw std::out_of_range("Índice fuera de rango");
                }
                NodoDoble<T>* actual;
                // Optimización: recorrer desde el extremo más cercano
                if (indice < tamano / 2) {
                    actual = cabeza;
                    for (int i = 0; i < indice; i++) {
                        actual = actual->siguiente;
                    }
                } else {
                    actual = cola;
                    for (int i = tamano - 1; i > indice; i--) {
                        actual = actual->anterior;
                    }
                }
                return actual->dato;
            }

            // Buscar elemento - O(n)
            bool buscar(T dato) const {
                NodoDoble<T>* actual = cabeza;
                while (actual != nullptr) {
                    if (actual->dato == dato) {
                        return true;
                    }
                    actual = actual->siguiente;
                }
                return false;
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

            // Recorrido hacia adelante
            template <typename Func>
            void recorrerAdelante(Func funcion) {
                NodoDoble<T>* actual = cabeza;
                while (actual != nullptr) {
                    funcion(actual->dato);
                    actual = actual->siguiente;
                }
            }

            // Recorrido hacia atrás
            template <typename Func>
            void recorrerAtras(Func funcion) {
                NodoDoble<T>* actual = cola;
                while (actual != nullptr) {
                    funcion(actual->dato);
                    actual = actual->anterior;
                }
            }

            // Obtener todos los elementos como vector (para facilitar acceso)
            std::vector<T> obtenerTodos() const {
                std::vector<T> resultado;
                NodoDoble<T>* actual = cabeza;
                while (actual != nullptr) {
                    resultado.push_back(actual->dato);
                    actual = actual->siguiente;
                }
                return resultado;
            }
        };

        #endif