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
#include <fstream>

using namespace std;

class Node {
    public:
        int key;
        Node *parent, *left, *right;

        Node(int key) : key(key) {parent = left = right = nullptr;}
};

class ABR {
    private:
        Node* root;

        Node* min (Node* node) {
            Node* tmp = node;
            while(tmp->left != nullptr) tmp = tmp->left;
            return tmp;
        }

        Node* max (Node* node) {
            Node* tmp = node;
            while(tmp->right != nullptr) tmp = tmp->right;
            return tmp;
        }

        Node* predecessor(Node* node) {
            if(node->left != nullptr)
                return max(node->left);
            Node* parent = node->parent;
            while(parent != nullptr and node == parent->left) {
                node = parent;
                parent = parent->parent;
            }
            return parent;
        }

        Node* successor(Node* node) {
            if(node->right != nullptr)
                return min(node->right);
            Node* parent = node->parent;
            while(parent != nullptr and node == parent->right) {
                node = parent;
                parent = parent->parent;
            }

            return parent;
        }

        void transplant(Node* u, Node* v) {
            Node* parent = u->parent;
            if(parent == nullptr)
                root = v;
            else if(parent->left == u)
                parent->left = v;
            else parent->right = v;

            if(v != nullptr)
                v->parent = parent;
        }

        void deletion(Node* z) {
            if(z->right == nullptr)
                transplant(z, z->left);
            else if(z->left == nullptr)
                transplant(z, z->right);
            else {
                Node* y = min(z->right);
                if(y != z->right) {
                    transplant(y, y->right);
                    y->right = z->right;
                    y->right->parent = y;
                }

                transplant(z, y);
                y->left = z->left;
                y->left->parent = y;
            }
        }
    public:
        ABR() : root(nullptr) {
            // ifstream in("ABR.txt");
            // if(!in) return;

            // int key;
            // while(in >> key) insert(key);

            // in.close();
        }

        void insert(int key) {
            Node* z = new Node(key);
            Node* x = root; Node* xp = nullptr;

            while(x != nullptr) {
                xp = x;
                x = (x->key > key) ? x->left : x->right;
            }

            z->parent = xp;
            if(xp == nullptr)
                root = z;
            else if(xp->key > key)
                xp->left = z;
            else
                xp->right = z;
        }

        Node* search(Node* node, int key) {
            if(node == nullptr or node->key == key)
                return node;

            return (node->key > key) ? search(node->left, key) : search(node->right, key);
        }

        bool del(int key) {
            Node* z = search(root, key);
            if(z == nullptr) return false;
            deletion(z);
            delete(z);
            return true;
        }

        int getMin() {
            if(root == nullptr)
                throw runtime_error("Tree is empty");
            return min(root)->key;
        }

        int getMax() {
            if(root == nullptr)
                throw runtime_error("Tree is empty");
            return max(root)->key;
        }
};

/*
    progettare una classe PriorityQueue che implementi una coda di priorità basata su binary search tree
*/

class PriorityQueue {
    private:
        ABR tree;
    public:
        void push(int value) {
            tree.insert(value);
        }

        int popMin() {
            int min = tree.getMin();
            tree.del(min);
            return min;
        }

        int popMax() {
            int max = tree.getMax();
            tree.del(max);
            return max;
        } 


};

int main () {
    PriorityQueue pq;
    pq.push(10);
    pq.push(20);
    pq.push(5);
    pq.push(15);

    cout << "Min: " << pq.popMin() << endl; // 5
    cout << "Max: " << pq.popMax() << endl; // 20
    cout << "Min: " << pq.popMin() << endl; // 10
    cout << "Max: " << pq.popMax() << endl; // 15

    return 0;
}