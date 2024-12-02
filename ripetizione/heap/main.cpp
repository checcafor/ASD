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

class MaxHeap {
    private:
        vector<int> heap;
        int size;

        int parent(int i) {return (i - 1) / 2;}
        int leftchild(int i) {return (i * 2) + 1;}
        int rightchild(int i) {return (i * 2) + 2;}

        void maxheapify(int i) {
            int left = leftchild(i);
            int right = rightchild(i);
            int max = i;

            if(left < size and heap[left] > heap[max]) 
                max = left;

            if(right < size and heap[right] > heap[max])
                max = right;

            if(max != i) {
                swap(heap[i], heap[max]);
                maxheapify(max);
            }
        }

        void build_maxheap() {
            for(int i = (size / 2) - 1; i >= 0; i--)
                maxheapify(i);
        }
    public:
        MaxHeap(vector<int> input) {
            heap = input;
            size = input.size();
            build_maxheap();
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

        void insert(int key) {
            heap.push_back(-(__INT_MAX__));
            size++;
            decrease_key(size - 1, key);
        }

        void del(int num) {
            int i;
            for(i = 0; i < size; i++)
                if(heap[i] == num)
                    break;

            if(i == size) return;

            swap(heap[i], heap[i - 1]);
            heap.pop_back();
            size--;

            if(size > 0) maxheapify(i);
        }
};

int main() {

    return 0;
}