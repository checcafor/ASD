/*
 * --------------------------------------------------------------------------------
 * Codice sorgente di proprietà riservata.
 * Tutti i diritti riservati © Checca.
 *
 * Questo codice è confidenziale e destinato esclusivamente all'uso autorizzato.
 * È vietata la riproduzione, la distribuzione o la divulgazione di qualsiasi
 * parte di questo codice senza il consenso esplicito di Checca.
 *
 * Autore: Checca
 * --------------------------------------------------------------------------------
 */

#include <iostream>
#include <vector>

using namespace std;

enum Type {MIN, MAX};

class Heap {
private:
    vector<int> heap;
    int size;
    Type type;

    int parent(int i) { return (i - 1) / 2; }
    int leftchild(int i) { return (i * 2) + 1; }
    int rightchild(int i) { return (i * 2) + 2; }

    void heapify(int i) {
        int left = leftchild(i);
        int right = rightchild(i);
        int extreme = i;

        if (type == MIN) {
            if (left < size && heap[left] < heap[extreme])
                extreme = left;
            if (right < size && heap[right] < heap[extreme])
                extreme = right;
        } else {
            if (left < size && heap[left] > heap[extreme])
                extreme = left;
            if (right < size && heap[right] > heap[extreme])
                extreme = right;
        }

        if (i != extreme) {
            swap(heap[i], heap[extreme]);
            heapify(extreme);
        }
    }

    void buildheap() {
        for (int i = size / 2 - 1; i >= 0; i--)
            heapify(i);
    }

    void decrease_key(int i, int key) {
        if(i > size - 1 or heap[i] < key)
            return;
        
        heap[i] = key;

        while(i > 0 and heap[parent(i)] < heap[i]) {
            swap(heap[parent(i)], heap[i]);
            i = parent(i);
        } 
    }
public:
    Heap(vector<int> input, Type t = MIN) {
        heap = input;
        size = input.size();
        type = t;
        buildheap();
    }

    void heap_sort() {
        int original_size = size;
        for (int i = size - 1; i > 0; i--) {
            swap(heap[0], heap[i]);
            size--;
            heapify(0);
        }
        size = original_size;  

        reverse(heap.begin(), heap.end());
    }

    void insert(int key) {
        heap.push_back(__INT_MAX__);
        size++;
        decrease_key(size - 1, key);
    }

    void printHeap() {
        for (int i = 0; i < size; i++)
            cout << heap[i] << " ";
        cout << endl;
    }
};

int main() {
    vector<int> input = {4, 10, 3, 5, 1};
    Heap h(input, MAX);  
    cout << "Heap iniziale: ";
    h.printHeap();

    h.heap_sort();
    cout << "Heap ordinato (ordine crescente): ";
    h.printHeap();

    return 0;
}
