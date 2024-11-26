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

enum Color {RED, BLACK};

class Node {
    public:
        int key;
        Node *parent, *left, *right;
        Color color;

        Node(int key) : key(key), color(RED) {parent = left = right = nullptr;}
};

class RedandBlack {
    private:
        Node* root;

        void left_rotate(Node* x) {
            Node* y = x->right;
            x->right = y->left;

            if(y->left != nullptr) y->left->parent = x;

            y->parent = x->parent;
            if(x->parent == nullptr)
                root = y;
            else if(x->parent->left == x)
                x->parent->left = y;
            else 
                x->parent->right = y;
            
            x->parent = y;
            y->left = x;
        }

        void right_rotate(Node* y) {
            Node* x = y->left;
            y->left = x->right;
            
            if(x->right != nullptr) x->right->parent = y;

            x->parent = y->parent;
            if(x->parent == nullptr)
                root = x;
            else if(x->parent->left == y)
                x->parent->left = x;
            else 
                x->parent->right = x;

            y->parent = x;
            x->right = y;
        }

        void insert_fixup(Node* node) {
            while(node != root and node->parent->color == RED) {
                Node* granparent = node->parent->parent;
                if(granparent->left == node->parent) {
                    Node* uncle = granparent->right;
                    if(uncle->color == RED) {
                        node->parent->color = BLACK;
                        uncle->color = BLACK;
                        granparent->color = RED;
                        node = granparent;
                    } else {
                        if(node == node->parent->right) {
                            node = node->parent;
                            left_rotate(node);
                        }

                        node->parent->color = BLACK;
                        granparent->color = RED;
                        right_rotate(granparent);
                    }
                } else {
                    Node* uncle = granparent->left;
                    if(uncle->color == RED) {
                        node->parent->color = BLACK;
                        uncle->color = BLACK;
                        granparent->color = RED;
                        node = granparent;
                    } else {
                        if(node == node->parent->left) {
                            node = node->parent;
                            right_rotate(node);
                        }

                        node->parent->color = BLACK;
                        granparent->color = RED;
                        left_rotate(granparent);
                    }
                }
            }

            root->color = BLACK;
        }
    
        void transplant(Node* u, Node* v) {
            Node* parent = u->parent;

            if(parent == nullptr)
                root = v;
            else if(parent->left == u)
                parent->left = v;
            else 
                parent->right = v;

            if(v != nullptr)
                v->parent = parent;
        }
    
        Node* min(Node* node) {
            Node* tmp = node;
            while(tmp->left != nullptr)
                tmp = tmp->left;
            return tmp;
        }
    
        void delete_fixup(Node* node) {
            while (node != root && (node == nullptr || node->color == BLACK)) {
                if (node == node->parent->left) {
                    Node* w = node->parent->right;
                    if (w != nullptr && w->color == RED) { // Caso 1
                        w->color = BLACK;
                        node->parent->color = RED;
                        left_rotate(node->parent);
                        w = node->parent->right;
                    }
                    if ((w->left == nullptr || w->left->color == BLACK) &&
                        (w->right == nullptr || w->right->color == BLACK)) { // Caso 2
                        w->color = RED;
                        node = node->parent;
                    } else {
                        if (w->right == nullptr || w->right->color == BLACK) { // Caso 3
                            if (w->left != nullptr) w->left->color = BLACK;
                            w->color = RED;
                            right_rotate(w);
                            w = node->parent->right;
                        }
                        w->color = node->parent->color; // Caso 4
                        node->parent->color = BLACK;
                        if (w->right != nullptr) w->right->color = BLACK;
                        left_rotate(node->parent);
                        node = root;
                    }
                } else { // Speculare
                    Node* w = node->parent->left;
                    if (w != nullptr && w->color == RED) {
                        w->color = BLACK;
                        node->parent->color = RED;
                        right_rotate(node->parent);
                        w = node->parent->left;
                    }
                    if ((w->left == nullptr || w->left->color == BLACK) &&
                        (w->right == nullptr || w->right->color == BLACK)) {
                        w->color = RED;
                        node = node->parent;
                    } else {
                        if (w->left == nullptr || w->left->color == BLACK) {
                            if (w->right != nullptr) w->right->color = BLACK;
                            w->color = RED;
                            left_rotate(w);
                            w = node->parent->left;
                        }
                        w->color = node->parent->color;
                        node->parent->color = BLACK;
                        if (w->left != nullptr) w->left->color = BLACK;
                        right_rotate(node->parent);
                        node = root;
                    }
                }
            }
            if (node != nullptr) node->color = BLACK;
        }
    public:
        RedandBlack() : root(nullptr) {
            ifstream INPUT("ABR.txt");
            if(!INPUT) return;

            int key;
            while(INPUT >> key) insert(key);

            INPUT.close();
            print(root);
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

            insert_fixup(z);
        }

        Node* search(Node* node, int key) {
            if(node == nullptr or node->key == key)
                return node;
            
            return (node->key > key) ? search(node->left, key) : search(node->right, key);
        }

        void rb_del(int key) {
            Node* z = search(root, key);
            if(z == nullptr) return;

            Color original_color = z->color;
            Node* x = z;

            if(z->left == nullptr) {
                x = z->right;
                transplant(z, z->right);
            } else if(z->right == nullptr) {
                x = z->left;
                transplant(z, z->left);
            } else {
                Node* y = min(z->right);
                original_color = y->color;
                x = y->right;
                if(y->parent == z) {
                    x->parent = y;
                }
                transplant(z, z->right);
            }

            if(original_color == BLACK)
                delete_fixup(z);

            cout << endl;
            print(root);
        }

        void print(Node* node) {
            if(node != nullptr) {
                print(node->left);
                cout << "Node {" << node->key << "} -> color [" << ((node->color == 0) ? "RED" : "BLACK") << "]" << endl;
                print(node->right);
            }
        }
};

int main() {
    RedandBlack tree;

    tree.rb_del(35);

    return 0;
}