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
enum Color {RED, BLACK};

class Node {
    public:
        int key;
        Color color;
        Node* left;
        Node* right;
        Node* parent;

        Node(int key) : key(key), color(RED) {parent = left = right = nullptr;}
};

class RedAndBlack {
    private:
        Node* root;

        void left_rotate(Node* x) {
            Node* y = x->right;
            x->right = y->left;

            if(y->right != nullptr)
                y->right->parent = x;
            
            y->parent = x->parent;
            if(y->parent == nullptr)
                root = y;
            else if(x == x->parent->left)
                x->parent->left = y;
            else
                x->parent->right = y;
            
            y->left = x;
            x->parent = y;
        }

        void right_rotate(Node* y) {
            Node* x = y->left;
            y->left = x->right;

            if(x->right != nullptr)
                x->right->parent = y;
            
            if(y->parent == nullptr)
                root = x;
            else if(y == y->parent->left)
                y->parent->left = x;
            else
                y->parent->right = x;
            
            x->left = y;
            y->parent = x;
        }
        
        void fix_insert(Node* node) {
            while (node != root && node->parent->color == RED) {
                Node* grandparent = node->parent->parent;

                if (node->parent == grandparent->left) {
                    Node* uncle = grandparent->right;
                    if (uncle != nullptr && uncle->color == RED) { // Caso 1: zio rosso
                        node->parent->color = BLACK;
                        uncle->color = BLACK;
                        grandparent->color = RED;
                        node = grandparent;
                    } else {
                        if (node == node->parent->right) { // Caso 2: zio nero, nodo destro
                            node = node->parent;
                            left_rotate(node);
                        }
                        node->parent->color = BLACK;      // Caso 3: zio nero, nodo sinistro
                        grandparent->color = RED;
                        right_rotate(grandparent);
                    }
                } else {
                    Node* uncle = grandparent->left;
                    if (uncle != nullptr && uncle->color == RED) { // Caso 1: zio rosso
                        node->parent->color = BLACK;
                        uncle->color = BLACK;
                        grandparent->color = RED;
                        node = grandparent;
                    } else {
                        if (node == node->parent->left) { // Caso 2: zio nero, nodo sinistro
                            node = node->parent;
                            right_rotate(node);
                        }
                        node->parent->color = BLACK;      // Caso 3: zio nero, nodo destro
                        grandparent->color = RED;
                        left_rotate(grandparent);
                    }
                }
            }
            root->color = BLACK;
        }
    
        Node* tree_search(Node* node, int key) {
            if(node == nullptr or node->key == key) 
                return node;
            
            return (node->key < key) ? tree_search(node->left, key) : tree_search(node->right, key);
        }

        void transplant(Node* u, Node* v) {
            Node* p = u->parent;
            if(p == nullptr)
                this->root = v;
            else if(u == p->left)
                p->left = v;
            else 
                p->right = v;
            
            if(v != nullptr)
                v->parent = p;
        }

        Node* min_node(Node* u) {
            Node* tmp = u;
            while (tmp->left != nullptr)
                tmp = tmp->left;
            return tmp;
        }

        void fix_delete(Node* x) {
            while (x != root && (x == nullptr || x->color == BLACK)) {
                if (x == x->parent->left) {
                    Node* w = x->parent->right; // Fratello
                    if (w->color == RED) { 
                        w->color = BLACK;
                        x->parent->color = RED;
                        left_rotate(x->parent);
                        w = x->parent->right;
                    }

                    if ((w->left == nullptr || w->left->color == BLACK) &&
                        (w->right == nullptr || w->right->color == BLACK)) {
                        w->color = RED;
                        x = x->parent;
                    } else {
                        if (w->right == nullptr || w->right->color == BLACK) {
                            if (w->left != nullptr) w->left->color = BLACK;
                            w->color = RED;
                            right_rotate(w);
                            w = x->parent->right;
                        }
                        w->color = x->parent->color;
                        x->parent->color = BLACK;
                        if (w->right != nullptr) w->right->color = BLACK;
                        left_rotate(x->parent);
                        x = root;
                    }
                } else {
                    Node* w = x->parent->left; // Fratello
                    if (w->color == RED) {
                        w->color = BLACK;
                        x->parent->color = RED;
                        right_rotate(x->parent);
                        w = x->parent->left;
                    }

                    if ((w->right == nullptr || w->right->color == BLACK) &&
                        (w->left == nullptr || w->left->color == BLACK)) {
                        w->color = RED;
                        x = x->parent;
                    } else {
                        if (w->left == nullptr || w->left->color == BLACK) {
                            if (w->right != nullptr) w->right->color = BLACK;
                            w->color = RED;
                            left_rotate(w);
                            w = x->parent->left;
                        }
                        w->color = x->parent->color;
                        x->parent->color = BLACK;
                        if (w->left != nullptr) w->left->color = BLACK;
                        right_rotate(x->parent);
                        x = root;
                    }
                }
            }

            if (x != nullptr)
                x->color = BLACK;
        }

        void rb_delete(Node* z) {
            Node* y = z;
            Node* x;
            Color original_color = y->color;

            if (z->left == nullptr) {
                x = z->right;
                transplant(z, z->right);
            } else if (z->right == nullptr) {
                x = z->left;
                transplant(z, z->left);
            } else {
                y = min_node(z->right);
                original_color = y->color;
                x = y->right;

                if (y->parent == z) {
                    if (x != nullptr)
                        x->parent = y;
                } else {
                    transplant(y, y->right);
                    y->right = z->right;
                    y->right->parent = y;
                }

                transplant(z, y);
                y->left = z->left;
                y->left->parent = y;
                y->color = z->color;
            }

            if (original_color == BLACK)
                fix_delete(x);
        }
    public:
        void insert(int key) {
            Node* z = new Node(key);
            Node* x = root;
            Node* xp = nullptr;

            while(x != nullptr) {
                xp = x;
                x = (key < x->key) ? x->left : x->right;
            }

            z->parent = xp;
            if(xp == nullptr) {
                root = z;
                z->color = BLACK;
                return;
            } else if(key < z->key)
                xp->left = z;
            else
                xp->right = z;
            fix_insert(z);
        }

        void deletion(int key) {
            Node* z = tree_search(root, key);
            if(z == nullptr)
                return;
            
            rb_delete(z);
            delete(z);
        }
};

int main() {

    return 0;
}