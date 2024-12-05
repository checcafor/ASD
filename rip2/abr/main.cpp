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

using namespace std;

class Node {
    public:
        int key;
        int val;
        Node *parent, *left, *right;

        Node(int key, int val) : key(key), val(val) {parent = left = right = nullptr;}
};

class ABR {
    private:
        Node* root;

        void transplant(Node* u, Node* v) {
            Node* parent = u->parent;

            if(parent == nullptr)
                root = v;
            else if(parent->left == u)
                parent->left = v;
            else
                parent->right = v;

            if(v != nullptr) v->parent = parent;
        }

        Node* min(Node* u) {
            Node* tmp = u;
            while(tmp->left != nullptr)
                tmp = tmp->left;
            return tmp;
        }

        Node* max(Node* u) {
            Node* tmp = u;
            while(tmp->right != nullptr)
                tmp = tmp->right;
            return tmp;
        }

        void deletion(Node* z) {
            if(z->left == nullptr)
                transplant(z, z->right);
            else if(z->right == nullptr)
                transplant(z, z->left);
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

        }

        void insert(int key, int val) {
            Node* z = new Node(key, val);
            Node* x = root; Node* xp = nullptr;

            while(x != nullptr) {
                xp = x;
                x = (x->key > key) ? x->left : x->right;
            }

            if(xp == nullptr)
                root = z;
            else if(xp->key > key) 
                xp->left = z;
            else xp->right = z;

            z->parent = xp;
        }

        Node* search(Node* node, int key) {
            if(!node or node->key == key)
                return node;
            
            return (node->key > key) ? search(node->left, key) : search(node->right, key);
        }

        void del(int key) {
            Node* z = search(root, key);
            if(!z) return;

            deletion(z);
            delete(z);
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
};

int main() {
    ABR tree;

    tree.insert(50, 100);
    tree.insert(30, 60);
    tree.insert(70, 140);
    tree.insert(20, 40);
    tree.insert(40, 80);
    tree.insert(60, 120);
    tree.insert(80, 160);

    cout << "Cancellazione del nodo con chiave 30" << endl;
    tree.del(30);

    return 0;
}