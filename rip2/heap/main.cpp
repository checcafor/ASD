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
        int size; Type type;

        int parent(int i) {return (i - 1) / 2;}
        int leftchild(int i) {return (i * 2) + 1;}
        int rightchild(int i) {return (i * 2) + 2;}

        void minheapify(int i) {
            int left = leftchild(i);
            int right = rightchild(i);
            int min = i;

            if(left < size and heap[left] < heap[min])
                min = left;

            if(right < size and heap[right] < heap[min])
                min = right;

            if(i != min) {
                swap(heap[i], heap[min]);
                minheapify(min);
            }
        }

        void build_heap() {
            for(int i = (size / 2) - 1; i >= 0; i--)    
                minheapify(i);
        }
    public:
        Heap(vector<int> input, Type t = MIN) {
            heap = input;
            size = input.size();
            type = t;
            build_heap();
        }

        void decrease_key(int i, int key) {
            if(i > size - 1 or heap[i] < key)
                return;
            
            heap[i] = key;
            while(i > 0 and heap[parent(i)] > heap[i]) {
                swap(heap[parent(i)], heap[i]);
                i = parent(i);
            }
        }

        void increase_key(int i, int key) {
            if(i > size - 1 or heap[i] > key)
                return;
            
            heap[i] = key;
            while(i > 0 and heap[parent(i)] > heap[i]) {
                swap(heap[parent(i)], heap[i]);
                i = parent(i);
            }
        }

        void insert(int key) {
            (type == MIN) ? heap.push_back(__INT_MAX__) : heap.push_back(-(__INT_MAX__));
            size++;
            (type == MIN) ? decrease_key(size - 1, key) : increase_key(size - 1, key);
        }

        void heap_sort() {
            int original_size = size;
            for(int i = size - 1; i > 0; i--) {
                swap(heap[0], heap[i]);
                size--;
                minheapify(0);
            }

            size = original_size;
            reverse(heap.begin(), heap.end());
        }
};

int main() {

    return 0;
}