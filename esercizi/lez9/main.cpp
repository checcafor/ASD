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
        int size;

        int parent(int i) {return (i - 1) / 2;}
        int leftchild(int i) {return (i * 2) + 1;}
        int rightchild(int i) {return (i * 2) + 2;}

        void heapify(int i) {
            int left = leftchild(i);
            int right = rightchild(i);
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

        void builheap() {
            for(int i = (size / 2) - 1; i >= 0; i--) 
                heapify(i);
        }
    public:
        Heap(vector<int> IN) {
            heap = IN;
            size = heap.size();

            cout << "prima della build\t";
            cout << (check_isheap(IN) ? "Array è un min heap\n" : "Array NON è un min heap\n");

            builheap();

            cout << "dopo la build\t";
            cout << (check_isheap(heap) ? "Array è un min heap\n" : "Array NON è un min heap\n");
        }

        bool check_isheap(vector<int> vet) {
            int size = vet.size();

            for(int i = 0; i <= (size / 2) - 1; i++) {
                int left = leftchild(i);
                int right = rightchild(i);

                if(left < size and vet[left] < vet[i])
                    return false;
                if(right < size and vet[right] < vet[i])
                    return false;
            }

            return true;
        }
};

int main() {
    vector<int> array1 = {10, 15, 14, 12, 40, 25};

    Heap h(array1);

    return 0;
}