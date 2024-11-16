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

class Heap {
    private:
        vector<int> heap;
        int size, k;

        int parent(int i) {return (i - 1) / 2;}
        int child(int i, int j) {return i * k + j + 1;} // j-esimo figlio

        void minheapify(int i) {
            int smallest = i;

            for(int j = 0; j < k; j++) {
                int c = child(i, j);
                if(c < size and heap[c] < heap[smallest])
                    smallest = c;
            }

            if(smallest != i) {
                swap(heap[smallest], heap[i]);
                minheapify(smallest);
            }
        }

        void buildheap() {
            for(int i = (size / 2) - 1; i >= 0; i--)
                minheapify(i);
        }
    public:
        Heap(vector<int> input, int k) {
            heap = input;
            this->k = k;
            size = input.size();

            buildheap();
        }
};

int main() {

    return 0;
}