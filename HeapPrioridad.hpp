#ifndef HEAP_PRIORIDAD_HPP
#define HEAP_PRIORIDAD_HPP

#include <vector>
#include <stdexcept>

// Min-Heap para cola de prioridad (menor valor = mayor prioridad)
template <typename T>
class HeapPrioridad {
private:
    std::vector<T> heap;

    // Obtener �ndice del padre - O(1)
    int padre(int i) { return (i - 1) / 2; }

    // Obtener �ndice del hijo izquierdo - O(1)
    int hijoIzq(int i) { return 2 * i + 1; }

    // Obtener �ndice del hijo derecho - O(1)
    int hijoDer(int i) { return 2 * i + 2; }

    // HeapifyUp: restaurar propiedad de heap hacia arriba - O(log n)
    void heapifyUp(int i) {
        while (i > 0 && heap[i] < heap[padre(i)]) {
            std::swap(heap[i], heap[padre(i)]);
            i = padre(i);
        }
    }

    // HeapifyDown: restaurar propiedad de heap hacia abajo - O(log n)
    void heapifyDown(int i) {
        int menor = i;
        int izq = hijoIzq(i);
        int der = hijoDer(i);

        if (izq < (int)heap.size() && heap[izq] < heap[menor]) {
            menor = izq;
        }
        if (der < (int)heap.size() && heap[der] < heap[menor]) {
            menor = der;
        }

        if (menor != i) {
            std::swap(heap[i], heap[menor]);
            heapifyDown(menor);
        }
    }

public:
    HeapPrioridad() {}

    // Insertar elemento - O(log n)
    void insertar(T elemento) {
        heap.push_back(elemento);
        heapifyUp(heap.size() - 1);
    }

    // Extraer m�nimo (mayor prioridad) - O(log n)
    T extraerMin() {
        if (estaVacio()) {
            throw std::runtime_error("Heap vacio");
        }

        T minimo = heap[0];
        heap[0] = heap.back();
        heap.pop_back();

        if (!estaVacio()) {
            heapifyDown(0);
        }

        return minimo;
    }

    // Ver m�nimo sin extraer - O(1)
    T verMin() const {
        if (estaVacio()) {
            throw std::runtime_error("Heap vacio");
        }
        return heap[0];
    }

    bool estaVacio() const {
        return heap.empty();
    }

    int getTamano() const {
        return heap.size();
    }

    void limpiar() {
        heap.clear();
    }

    // Construir heap desde vector - O(n)
    void construirHeap(std::vector<T>& elementos) {
        heap = elementos;
        for (int i = heap.size() / 2 - 1; i >= 0; i--) {
            heapifyDown(i);
        }
    }
};

#endif
