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
#define INF __INT_MAX__

class Node {
    public:
        int key;
        int val;
        Node *parent, *left, *right;

        Node(int key, int val) : key(key), val(val) {parent = left = right = nullptr;}
};

class ABR {
    private:
        void inOrder(Node* node, Node*& prev, Node*& p, Node*& f, int& minDiff) {
            if (!node) return;

            inOrder(node->left, prev, p, f, minDiff);

            if (prev) {
                int diff = abs(node->key - prev->key);
                if (diff < minDiff) {
                    minDiff = diff;
                    p = prev;
                    f = node;
                }
            }

            prev = node;
            inOrder(node->right, prev, p, f, minDiff);
        }
    public:
        Node* root;
        ABR() : root(nullptr) {}

        void insert(int key, int value) {
            Node* z = new Node(key, value);
            Node* x = root; Node* xp = nullptr;

            while(x != nullptr) {
                xp = x;
                x = (key < x->key) ? x->left : x->right;
            }

            z->parent = xp;
            if(xp == nullptr)
                root = z;
            else if(xp->key > key)
                xp->left = z;
            else
                xp->right = z;
        }

        void mindiff(Node* p, Node* f) {
            if(!root) nullptr;
            int min = INF;
            Node* prev = nullptr; 
            inOrder(root, prev, p, f, min);
        }
};

int main() {

    return 0;
}