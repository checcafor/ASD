#include <iostream>
#include <vector>
#include <limits>

enum Type {MIN, MAX};

using namespace std;

class Heap {
    private:
        vector<int> heap;
        int size; 
        Type type;

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

        void buildheap() {
            for(int i = (size / 2) - 1; i >= 0; i--)
                (type == MIN) ? minheapify(i) : maxheapify(i);
        }
    public:
        Heap(vector<int> IN, Type t = MIN) {
            heap = IN;
            size = IN.size();
            type = t;

            buildheap();
        }

        void decreasekey(int i, int key) {
            if(i >= size or /*i < 0 or*/ key > heap[i])
                return;
            
            heap[i] = key;
            while(i > 0 and heap[parent(i)] > heap[i]) {
                swap(heap[i], heap[parent(i)]);
                i = parent(i);
            }
        }

        void increasekey(int i, int key) {
            if(i >= size or /*i < 0 or*/ key < heap[i])
                return;
            
            heap[i] = key;
            while(i > 0 && heap[parent(i)] < heap[i]) {
                swap(heap[i], heap[parent(i)]);
                i = parent(i);
            }
        }

        void insert(int key) {
            (type == MIN) ? heap.push_back(__INT_MAX__) : heap.push_back(std::numeric_limits<int>::min());
            size++;
            (type == MIN) ? decreasekey(size - 1, key) : increasekey(size - 1, key);
        }

        int extractmin() {
            if(type == MAX || size == 0) 
                return std::numeric_limits<int>::max();
            int min = heap[0];
            heap[0] = heap[size - 1];
            heap.pop_back();
            size--;

            minheapify(0);
            return min;
        }

        int extractmax() {
            if(type == MIN || size == 0) 
                return std::numeric_limits<int>::min();
            int max = heap[0];
            heap[0] = heap[size - 1];
            heap.pop_back();
            size--;

            maxheapify(0);
            return max;
        }

        void printheap() {
            for(int i = 0; i < size; i++) {
                cout << heap[i] << " ";
            }
            cout << endl;
        }
};

int main() {
    vector<int> data = {3, 1, 6, 5, 2, 4};
    Heap minHeap(data, MIN);
    
    cout << "Min-Heap iniziale: ";
    minHeap.printheap();

    minHeap.insert(0);
    cout << "Min-Heap dopo inserimento di 0: ";
    minHeap.printheap();

    cout << "Estrazione del minimo: " << minHeap.extractmin() << endl;
    minHeap.printheap();

    Heap maxHeap(data, MAX);
    
    cout << "Max-Heap iniziale: ";
    maxHeap.printheap();

    maxHeap.insert(7);
    cout << "Max-Heap dopo inserimento di 7: ";
    maxHeap.printheap();

    cout << "Estrazione del massimo: " << maxHeap.extractmax() << endl;
    maxHeap.printheap();

    return 0;
}
