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
#include <fstream>
#include <iomanip>

using namespace std;

class Node {
    public:
        int key;
        Node* parent, *left, *right;

        Node(int key) : key(key) {parent = left = right = nullptr;}
};

class ABR {
    private:
        int sum = 0;

        void load() {
            ifstream in("ABR.txt");
            if(!in)
                return;
            int val;
            while(in >> val)
                insert(val);

            in.close();

            cout << "Dati inseriti . . . " << endl;
            print(root, 0);  // Stampa l'albero con il livello iniziale 0
        }
    
        // algoritmo che sommi le chiavi di tutte le foglie
        void sum_leaf(Node* node) {
            if (node == nullptr) return;

            if(node->left == nullptr and node->right == nullptr) {
                sum += node->key;
                return;
            }
            
            sum_leaf(node->left);
            sum_leaf(node->right);
        }
    
        Node* findNode(int key) {
            Node* current = root;
            while (current) {
                if (key == current->key)
                    return current;
                else if (key < current->key)
                    current = current->left;
                else
                    current = current->right;
            }
            return nullptr; 
        }

        Node* findMin(Node* node) {
            while (node && node->left)
                node = node->left;
            return node;
        }

        Node* findSuccessor(Node* node) {
            if (node->right)
                return findMin(node->right);

            Node* parent = node->parent;
            while (parent && node == parent->right) {
                node = parent;
                parent = parent->parent;
            }
            return parent;
        }
    public:
        Node* root;
        ABR() {root = nullptr; load();}

        void print(Node* node, int level) {
            if(node != nullptr) {
                cout << setw(level * 4) << "" // Indenta a seconda del livello
                    << "Node {" << node->key << "} " << endl;
                print(node->left, level + 1);
                print(node->right, level + 1);
            }
        }

        void insert(int key) {
            Node* z = new Node(key);
            Node* x = root;
            Node* xp = nullptr;

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

        int getsum_leaf() {
            sum = 0;
            sum_leaf(root);
            return sum;
        }

        // Trova il successore dispari di k
        Node* findOddSuccessor(int k) {
            Node* node = findNode(k);
            if (!node)
                return nullptr;

            Node* successor = findSuccessor(node);

            while (successor && successor->key % 2 == 0) {
                successor = findSuccessor(successor);
            }

            return successor;
        }
};

int main() {
    ABR a;

    cout << "La somma delle chiavi di tutte le foglie è " << a.getsum_leaf() << endl;

    return 0;
}