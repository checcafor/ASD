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

        int parent(int i) {return (i - 1) / 2;}
        int leftchild(int i) {return (i * 2) + 1;}
        int rightchild(int i) {return (i * 2) + 2;}

        void minheapify (int i) {
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

        void maxheapify (int i) {
            int left = leftchild(i);
            int right = rightchild(i);
            int max = i;

            if(left < size and heap[left] > heap[max])
                max = left;
            
            if(right < size and heap[right] > heap[max])
                max = right;
            
            if(i != max) {
                swap(heap[i], heap[max]);
                maxheapify(max);
            }
        }

        void buildheap() {
            for(int i = (size / 2) - 1; i >= 0; i--) {
                (type == MIN) ? minheapify(i) : maxheapify(i);
            }
        }

        void decrease_key(int i, int key) {
            if(i > size - 1 or heap[i] < key)
                return;
            heap[i] = key;

            while(i > 0 and heap[parent(i)] > heap[i]) {
                swap(heap[i], heap[parent(i)]);
                i = parent(i);
            }
        }

        void increase_key(int i, int key) {
            if(i > size - 1 or heap[i] > key) 
                return;

            heap[i] = key;
            while(i > 0 and heap[parent(i)] < heap[i]) {
                swap(heap[i], heap[parent(i)]);
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

        void insert_min(int key) {
            heap.push_back(__INT_MAX__);
            size++;
            decrease_key(size - 1, key);
        }

        void insert_max(int key) {
            heap.push_back(-(__INT_MAX__));
            size++;
            increase_key(size - 1, key);
        }
};

int main() {

    return 0;
}