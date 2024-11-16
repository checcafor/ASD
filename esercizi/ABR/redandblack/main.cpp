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

enum Color {RED, BLACK};

using namespace std;

class Node {
    public:
        int key;
        Node *parent, *left, *right;
        Color color;

        Node(int key) : key(key), parent(nullptr), left(nullptr), right(nullptr), color(RED) {}
};

class RedBlack {
    private:
        Node* root;

        Node* min(Node* node) {
            while(node->left != nullptr) 
                node = node->left;
            return node;
        }

        void leftrotate(Node* x) {
            Node* y = x->right;
            x->right = y->left;
            if(y->left != nullptr)
                y->left->parent = x;
            
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

        void rightrotate(Node* y) {
            Node* x = y->left;
            y->left = x->right;
            if(x->right != nullptr)
                x->right->parent = y;

            x->parent = y->parent;
            if(x->parent == nullptr)
                root = x;
            else if(y == y->parent->left)
                y->parent->left = x;
            else
                y->parent->right = x;

            x->right = y;
            y->parent = x;
        }

        void transplant(Node* u, Node* v) {
            if(u->parent == nullptr)
                root = v;
            else if(u == u->parent->left)
                u->parent->left = v;
            else
                u->parent->right = v;

            if(v != nullptr)
                v->parent = u->parent;
        }

        void insert_fixup(Node* z) {
            while(z->parent != nullptr && z->parent->color == RED) {
                Node* grandparent = z->parent->parent;
                if(z->parent == grandparent->left) {
                    Node* uncle = grandparent->right;
                    if(uncle != nullptr && uncle->color == RED) {
                        z->parent->color = BLACK;
                        uncle->color = BLACK;
                        grandparent->color = RED;
                        z = grandparent;
                    } else {
                        if(z == z->parent->right) {
                            z = z->parent;
                            leftrotate(z);
                        }
                        z->parent->color = BLACK;
                        grandparent->color = RED;
                        rightrotate(grandparent);
                    }
                } else {
                    Node* uncle = grandparent->left;
                    if(uncle != nullptr && uncle->color == RED) {
                        z->parent->color = BLACK;
                        uncle->color = BLACK;
                        grandparent->color = RED;
                        z = grandparent;
                    } else {
                        if(z == z->parent->left) {
                            z = z->parent;
                            rightrotate(z);
                        }
                        z->parent->color = BLACK;
                        grandparent->color = RED;
                        leftrotate(grandparent);
                    }
                }
            }
            root->color = BLACK;
        }

        void delete_fixup(Node* x) {
            while(x != root && (x == nullptr || x->color == BLACK)) {
                if(x == x->parent->left) {
                    Node* w = x->parent->right;
                    if(w != nullptr && w->color == RED) {
                        w->color = BLACK;
                        x->parent->color = RED;
                        leftrotate(x->parent);
                        w = x->parent->right;
                    }
                    if((w->left == nullptr || w->left->color == BLACK) && 
                       (w->right == nullptr || w->right->color == BLACK)) {
                        w->color = RED;
                        x = x->parent;
                    } else {
                        if(w->right == nullptr || w->right->color == BLACK) {
                            if(w->left != nullptr) w->left->color = BLACK;
                            w->color = RED;
                            rightrotate(w);
                            w = x->parent->right;
                        }
                        w->color = x->parent->color;
                        x->parent->color = BLACK;
                        if(w->right != nullptr) w->right->color = BLACK;
                        leftrotate(x->parent);
                        x = root;
                    }
                } else {
                    Node* w = x->parent->left;
                    if(w != nullptr && w->color == RED) {
                        w->color = BLACK;
                        x->parent->color = RED;
                        rightrotate(x->parent);
                        w = x->parent->left;
                    }
                    if((w->right == nullptr || w->right->color == BLACK) && 
                       (w->left == nullptr || w->left->color == BLACK)) {
                        w->color = RED;
                        x = x->parent;
                    } else {
                        if(w->left == nullptr || w->left->color == BLACK) {
                            if(w->right != nullptr) w->right->color = BLACK;
                            w->color = RED;
                            leftrotate(w);
                            w = x->parent->left;
                        }
                        w->color = x->parent->color;
                        x->parent->color = BLACK;
                        if(w->left != nullptr) w->left->color = BLACK;
                        rightrotate(x->parent);
                        x = root;
                    }
                }
            }
            if(x != nullptr) x->color = BLACK;
        }

        void rb_delete(Node* z) {
            Node* y = z;
            Color originalColor = y->color;
            Node* x = nullptr;

            if(z->left == nullptr) {
                x = z->right;
                transplant(z, z->right);
            } else if(z->right == nullptr) {
                x = z->left;
                transplant(z, z->left);
            } else {
                y = min(z->right);
                originalColor = y->color;
                x = y->right;
                if(y->parent == z) {
                    if(x != nullptr) x->parent = y;
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

            if(originalColor == BLACK) 
                delete_fixup(x);
        }

    public:
        RedBlack() : root(nullptr) {}

        Node* search(int key) {
            Node* current = root;
            while(current != nullptr && current->key != key) {
                current = (key < current->key) ? current->left : current->right;
            }
            return current;
        }

        void insert(int key) {
            Node* z = new Node(key);
            Node* y = nullptr;
            Node* x = root;

            while(x != nullptr) {
                y = x;
                x = (key < x->key) ? x->left : x->right;
            }

            z->parent = y;
            if(y == nullptr)
                root = z;
            else if(key < y->key)
                y->left = z;
            else
                y->right = z;

            insert_fixup(z);
        }

        void del(int key) {
            Node* z = search(key);
            if(z == nullptr) {
                cout << "Nodo con chiave " << key << " non trovato\n";
                return;
            }
            rb_delete(z);
            delete z;
        }

        void print_in_order(Node* node) {
            if (node == nullptr) return;

            print_in_order(node->left);
            cout << "Chiave: " << node->key 
                    << " | Colore: " << (node->color == RED ? "Rosso" : "Nero") << std::endl;
            print_in_order(node->right);
        }
};

int main() {
    RedBlack tree;

    tree.insert(10);
    tree.insert(20);
    tree.insert(30);

    tree.del(20);
    
    Node* searchNode = tree.search(10);
    cout << (searchNode ? "Trovato" : "Non trovato") << "\n";
}