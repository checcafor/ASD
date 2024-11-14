#include <iostream>
#include <vector>
#include <limits>

enum Type {MIN, MAX};
#define BIGMAX __INT_MAX__
#define BIGMIN -__INT_MAX__

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
            int left = leftchild(i), right = rightchild(i);
            int min = i;

            if(left < size and heap[left] < heap[i])
                min = left;
            if(right < size and heap[right] < heap[i])
                min = right;

            if(min != i) {
                swap(heap[i], heap[min]);
                minheapify(min);
            }
        }

        void maxheapify(int i) {
            int left = leftchild(i), right = rightchild(i);
            int max = i; 

            if(left < size and heap[left] > heap [i])
                max  = left;
            if(right < size and heap[right] > heap[i])
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

        void decreasekey(int i, int key) {
            if(i > size - 1 or key > heap[i])
                return;
            
            heap[i] = key;
            while(i > 0 and heap[parent(i)] > heap[i]) {
                swap(heap[i], heap[parent(i)]);
                i = parent(i);
            }
        }

        void increasekey(int i, int key) {
            if(i > size - 1 or key < heap[i])
                return;
            
            heap[i] = key;
            while(i > 0 and heap[parent(i)] < heap[i]) {
                swap(heap[i], heap[parent(i)]);
                i = parent(i);
            }
        }
    public:
        Heap(vector<int> input, Type type = MIN) {
            heap = input;
            size = input.size();
            this->type = type;
            buildheap();
        }

        void insert(int key) {
            (type == MIN) ? (heap.push_back(BIGMAX)) : (heap.push_back(BIGMIN));
            size++;
            (type == MIN) ? decreasekey(size - 1, key) : increasekey(size - 1, key);
        }

        int extract_max() {
            if(type == MIN)
                return -1;
            int max = heap[0];
            swap(heap[0], heap[size - 1]);
            size--;

            maxheapify(0);
            return max;
        }

        int extract_min() {
            if(type == MAX)
                return -1;
            int min = heap[0];
            swap(heap[0], heap[size - 1]);
            size--;

            minheapify(0);
            return min;
        }

        void del(int num) {
            int i;
            for(i = 0; i < size; i++) 
                if(heap[i] == num) {
                    cout << "Elemento " << num << " trovato ed eliminato!" << endl;
                    swap(heap[i], heap[size - 1]);
                    break;
                }
            
            heap.pop_back();
            size--;
            if(size > 0)
                (type == MIN) ? minheapify(i) : maxheapify(i);
        }

        void print() {
            (type == MIN) ? cout << "\nstampa minheap ..." << endl : cout << "\nstampa maxheap ..." << endl;
            for(int el : heap) {
                cout << " {" << el << "} \t";
            }

            cout << endl;
        }
};

int main() {
    vector<int> input = {10, 3, 12, -1, 45, 8, 12, 78};

    Heap minheap(input);
    minheap.print();

    minheap.del(3);

    Heap maxheap(input, MAX);
    maxheap.print();

    return 0;
}