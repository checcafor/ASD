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
#include <fstream>

using namespace std;

class Heap {
    private:
        vector<int> heap;
        int size;

        int parent(int i) {return (i - 1) / 2;}
        int leftchild(int i) {return (i * 2) + 1;}
        int rightchild(int i) {return (i * 2) + 2;}

        void heapify(int i) {
            int left = leftchild(i); int right = rightchild(i);
            int min = i;

            if(left < size and heap[left] < heap[min])
                min = left;
            if(right < size and heap[right] < heap[min])
                min = right;

            if(i != min) {
                swap(heap[i], heap[min]);
                heapify(min);
            }
        }

        void build_heap() {
            for(int i = (size / 2) - 1; i >= 0; i--)
                heapify(i);
        }
    public:
        Heap(vector<int> IN) {
            heap = IN;
            size = IN.size();
            build_heap();
        }

        void decrease_key(int value, int key) {
            int i = -1;
            for (int j = 0; j < size; j++) {
                if (heap[j] == value) {
                    i = j;
                    break;
                }
            }

            if (i == -1 || key > heap[i]) {
                cout << "Errore: Valore non trovato o chiave non valida!" << endl;
                return;
            }

            heap[i] = key;
            while (i > 0 && heap[parent(i)] > heap[i]) {
                swap(heap[i], heap[parent(i)]);
                i = parent(i);
            }
        }

        void heapsort() {
            int originalSize = size;

            while (originalSize > 0) {
                // Scambia il minimo (heap[0]) con l'ultimo elemento
                swap(heap[0], heap[originalSize - 1]);
                originalSize--; // Riduci la dimensione del heap
                heapify(0); // Ripristina la proprietà del MIN HEAP
            }

            reverse(heap.begin(), heap.end());
        }

        void print() {
            for(int el : heap)
                cout << "{" << el << "}\t";
            cout << endl; 
        }
};

int main() {
    vector<int> in;
    ifstream input("HEAP.txt");

    int el;
    while(input >> el)  in.push_back(el);

    input.close();

    Heap h(in);
    h.print();

    cout << "Richiamo decreasekey(53, 30)" << endl;

    h.decrease_key(53, 30);
    h.print();

    cout << "Eseguo Heapsort . . ." << endl;

    h.heapsort();
    h.print();

    return 0;
}