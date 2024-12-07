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

class MinHeap {
    private:
        vector<int> heap;
        int size;

        int parent(int i) {return (i - 1) / 2;}
        int leftchild(int i) {return (i * 2) + 1;}
        int rightchild(int i) {return (i * 2) + 2;}

        void decrease_key(int i, int key) {
            if(i > size - 1 or heap[i] < key)
                return;
            
            heap[i] = key;
            while(i > 0 and heap[parent(i)] < heap[i]) {
                swap(heap[parent(i)], heap[i]);
                i = parent(i);
            }
        }

        void min_heapify(int i) {
            int left = leftchild(i);
            int right = rightchild(i);
            int min = i;

            if(left < size and heap[left] < heap[min])
                left = min;
            
            if(right < size and heap[right] < heap[min])
                right = min;

            if(i != min) {
                swap(heap[i], heap[min]);
                min_heapify(min);
            }
        }

        void build_heap() {
            for(int i = (size / 2) - 1; i >= 0; i--)
                min_heapify(i);
        }
    public:
        MinHeap(vector<int> in) {
            heap = in;
            size = in.size();
            build_heap();
        }

        void insert(int val) {
            heap.push_back(__INT_MAX__);
            size++;
            decrease_key(size - 1, val);
        }
};

int main() {

    return 0;
}