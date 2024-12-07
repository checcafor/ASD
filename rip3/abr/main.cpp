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
        int value;
        Node *parent, *left, *right;

        Node(int key, int value) : key(key), value(value) {parent = left = right = nullptr;}
};

class BinarySearchTree {
    private:
        Node* root;

         Node* min (Node* node) {
            Node* tmp = node;
            while(tmp->left != nullptr)
                tmp = tmp->left;

            return tmp;
        }

        Node* max(Node* node) {
            Node* tmp = node;
            while(tmp->right != nullptr)
                tmp = tmp->right;
            
            return tmp;
        }
    
        void transplant(Node* u, Node* v) {
            Node* up = u->parent;

            if(up == nullptr)
                root = v;
            else if(up->left == u)
                up->left = v;
            else up->right = v;

            if(v != nullptr)
                v->parent = up;
        }

        void deletion(Node* z) {
            if(z->right == nullptr)
                transplant(z, z->left);
            else if(z->left == nullptr)
                transplant(z, z->right);
            else {
                Node* y = min(z->right);
                if(z->right != y) {
                    transplant(y, y->right);
                    y->right = z->right;
                    z->right->parent = y;
                }

                transplant(z, y);
                y->left = z->left;
                z->left->parent = y;
            }
        }
    public:
        BinarySearchTree() : root(nullptr) {

        }

        Node* search(Node* node, int key) {
            if(!node or node->key == key)
                return node;
            return (node->key > key) ? search(node->left, key) : search(node->right, key);
        }

        Node* minimum () {
            Node* tmp = root;
            while(tmp->left != nullptr)
                tmp = tmp->left;

            return tmp;
        }

        Node* maximum() {
            Node* tmp = root;
            while(tmp->right != nullptr)
                tmp = tmp->right;
            
            return tmp;
        }

        Node* predecessor(Node* x) {
            if(x->left != nullptr)
                return max(x->left);
            
            Node* xp = x->parent;
            while(xp != nullptr and x == xp->left) {
                x = xp;
                xp = xp->parent;
            }

            return xp;
        }

        Node* successor(Node* x) {
            if(x->right != nullptr)
                return min(x->right);
            
            Node* xp = x->parent;
            while(xp != nullptr and x == xp->right) {
                x = xp;
                xp = xp->parent;
            }

            return xp;
        }

        void insert(int key, int value) {
            Node* z = new Node(key, value);
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
            else xp->right = z;
        }

        void del(int key) {
            Node* z = search(root, key);
            if(z == nullptr) return;

            deletion(z); delete(z);
        }

        void sum_leaf(Node* node, int &sum) {
            if(!node) return;
            if(node->left == nullptr and node->right == nullptr)
                sum += node->key;
            else {
                sum_leaf(node->left, sum);
                sum_leaf(node->right, sum);
            }
        }

       void sum_k(Node* node, int &sum, int &count, int k) {
            if (!node || count >= k) return; 

            sum_k(node->left, sum, count, k);

            if (count < k) { 
                sum += node->key;
                count++;
            }

            sum_k(node->right, sum, count, k);
        }

        void sum_btw_mM(Node* node, int& sum, int m, int M) {
            if(!node) return;
            if(node->key > m and node->key > M) sum += node->key;

            sum_btw_mM(node->left, sum, m, M);
            sum_btw_mM(node->right, sum, m, M);
        }

        Node* odd_successor(Node* x) { // successore dispari
            Node* succ = successor(x); // Trova il successore normale
            while (succ != nullptr && succ->key % 2 == 0) { // Finché il successore non è dispari
                succ = successor(succ);
            }
            return succ; 
        }

        Node* fin_mindiff_padrefiglio(Node* curr, Node* &prev, Node*& min_parent, Node*& min_child, int& min_diff) {
            if(!curr) return;

            fin_mindiff_padrefiglio(curr->left, prev, min_parent, min_child, min_diff);

            if(prev != nullptr) {
                int diff = abs(curr->key - prev->key);
                if(diff < min_diff) {
                    min_parent = prev;
                    min_child = curr;
                    min_diff = diff;
                }
            }

            prev = curr;
            fin_mindiff_padrefiglio(curr->right, prev, min_parent, min_child, min_diff);
        }

        void find_min_diff(Node* node, int& prev_key, int& min_diff) {
            if (!node) return;

            find_min_diff(node->left, prev_key, min_diff);
            if (prev_key != -1) { 
                int diff = node->key - prev_key;
                if (diff < min_diff) {
                    min_diff = diff;
                }
            }

            prev_key = node->key;
            find_min_diff(node->right, prev_key, min_diff);
        }
};

int main() {

    return 0;
}