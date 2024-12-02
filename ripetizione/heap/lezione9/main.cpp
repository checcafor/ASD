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

class min_heap {
    private:
        vector<int> heap;
        int size;

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

            if(min != i) {
                swap(heap[i], heap[min]);
                minheapify(min);
            }
        }

        void build_minheap() {
            for(int i = (size / 2) - 1; i >= 0; i--)
                minheapify(i);
        }
    public:
        min_heap(vector<int> input) {
            heap = input;
            size = input.size();
            build_minheap();
        }

        void increase_key(int i, int key) {
            if(i > size - 1 or heap[i] < key)
                return;

            heap[i] = key;
            while(i > 0 and heap[parent(i)] > heap[i]) {
                swap(heap[parent(i)], heap[i]);
                i = parent(i);
            }
        }

        void insert(int key) {
            heap.push_back(__INT_MAX__);
            size++;
            increase_key(size - 1, key);
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

            if(size > 0) minheapify(i);
        }

        /* implementare algoritmo ricorsivo che dato un min heap ed un valore x calcoli la somma dei valori dei nodi minori di x */
        int sum(int i, int x, int &result) {
            if(i >= size || heap[i] >= x) 
                return 0;
            result += heap[i];

            sum(leftchild(i), x, result);
            sum(rightchild(i), x, result);

            return result;
        }

        int sumOfNodesLessThanX(int x) {
            int result = 0;
            sum(0, x, result);
            return result;
        }
};

int main() {
    vector<int> elements = {1, 3, 6, 5, 9, 8};
    min_heap heap(elements);

    int x = 7;
    cout << "Somma dei nodi minori di " << x << ": " << heap.sumOfNodesLessThanX(x) << endl;

    return 0;
}