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
enum Color {RED, BLACK};

class Node {
    public:
        int key;
        // int value;
        Node* parent, *left, *right;
        Color color;

        Node(int key) : key(key) {parent = left = right = nullptr;}
};

class ABR {
    protected:
        Node* min(Node* x) {
            Node* tmp = x;
            while(tmp->left != nullptr)
                tmp = tmp->left;
            return tmp;
        }

        Node* max(Node* x) {
            Node* tmp = x;
            while(tmp->right != nullptr)
                tmp = tmp->right;
            return tmp;
        }

        void transplant(Node* u, Node* v) {
            Node* up = u->parent;
            if(up == nullptr)
                root = v;
            else if(u == up->right)
                up->right = v;
            else
                up->left = v;

            if(v != nullptr)
                v->parent = up;
        }

    public:
        Node* root;
        ABR() : root(nullptr) {}

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

            postInsert(z);
        }

        Node* search(int key, Node* node) {
            if(node == nullptr || node->key == key)
                return node;

            return (node->key > key) ? search(key, node->left) : search(key, node->right);
        }

        bool del(int key) {
            Node* z = search(key, root);
            if(z == nullptr) {
                cerr << "Nodo con chiave {" << key << "} non trovato! " << endl;
                return false;
            }

            deletion(z);
            delete(z);
            print(root, 0);

            return true;
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

            postDeletion(z);
        }

        virtual void postInsert(Node* node) {}

        virtual void postDeletion(Node* node) {}

        void print(Node* node, int level) {
            if(node != nullptr) {
                cout << setw(level * 4) << "" // Indenta a seconda del livello
                    << "Node {" << node->key << "} "
                    << "(Color: " << (node->color == RED ? "RED" : "BLACK") << ")" << endl;
                print(node->left, level + 1);
                print(node->right, level + 1);
            }
        }
};

class redblack : public ABR {
    private:
        void leftrotate(Node* x) {
            Node* y = x->right;
            x->right = y->left;
            if(y->left != nullptr)
                y->left->parent = x;
            y->parent = x->parent;

            if(x->parent == nullptr)
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

        void insertFixup(Node* node) {
            while(node != nullptr && node->parent != nullptr && node->parent->color == RED) {
                Node* grandparent = node->parent->parent;
                if(node->parent == grandparent->left) {
                    Node* uncle = grandparent->right;
                    if(uncle != nullptr && uncle->color == RED) {
                        node->parent->color = BLACK;
                        uncle->color = BLACK;
                        grandparent->color = RED;
                        node = grandparent;
                    } else {
                        if(node == node->parent->right) {
                            node = node->parent;
                            leftrotate(node);
                        }

                        node->parent->color = BLACK;
                        grandparent->color = RED;
                        right_rotate(grandparent);
                    }
                } else {
                    Node* uncle = grandparent->left;
                    if(uncle != nullptr && uncle->color == RED) {
                        node->parent->color = BLACK;
                        uncle->color = BLACK;
                        grandparent->color = RED;
                        node = grandparent;
                    } else {
                        if(node == node->parent->left) {
                            node = node->parent;
                            right_rotate(node);
                        }

                        node->parent->color = BLACK;
                        grandparent->color = RED;
                        leftrotate(grandparent);
                    }
                }
            }
            root->color = BLACK;
        }

        void deleteFixup(Node* x) {
            while(x != root && x->color == BLACK) {
                if(x == x->parent->left) {
                    Node* w = x->parent->right;
                    if(w->color == RED) {
                        w->color = BLACK;
                        x->parent->color = RED;
                        leftrotate(x->parent);
                        w = x->parent->right;
                    }
                    if((w->left == nullptr || w->left->color == BLACK) && (w->right == nullptr || w->right->color == BLACK)) {
                        w->color = RED;
                        x = x->parent;
                    } else {
                        if(w->right == nullptr || w->right->color == BLACK) {
                            if(w->left != nullptr)
                                w->left->color = BLACK;
                            w->color = RED;
                            right_rotate(w);
                            w = x->parent->right;
                        }
                        w->color = x->parent->color;
                        x->parent->color = BLACK;
                        if(w->right != nullptr)
                            w->right->color = BLACK;
                        leftrotate(x->parent);
                        x = root;
                    }
                } else {
                    Node* w = x->parent->left;
                    if(w->color == RED) {
                        w->color = BLACK;
                        x->parent->color = RED;
                        right_rotate(x->parent);
                        w = x->parent->left;
                    }
                    if((w->left == nullptr || w->left->color == BLACK) && (w->right == nullptr || w->right->color == BLACK)) {
                        w->color = RED;
                        x = x->parent;
                    } else {
                        if(w->left == nullptr || w->left->color == BLACK) {
                            if(w->right != nullptr)
                                w->right->color = BLACK;
                            w->color = RED;
                            leftrotate(w);
                            w = x->parent->left;
                        }
                        w->color = x->parent->color;
                        x->parent->color = BLACK;
                        if(w->left != nullptr)
                            w->left->color = BLACK;
                        right_rotate(x->parent);
                        x = root;
                    }
                }
            }
            if(x != nullptr)
                x->color = BLACK;
        }

    public:
        void postInsert(Node* node) override {
            node->color = RED;
            insertFixup(node);
        }

        void postDeletion(Node* node) override {
            deleteFixup(node);
        }
};

int main() {
    // redblack tree;
    ABR tree;

    tree.load();  // Carica i dati dal file

    // Test di eliminazione
    tree.del(60);  // Prova a eliminare un nodo esistente

    tree.insert(25);  // Inserisci un nuovo nodo

    // tree.del(100);  // Prova a eliminare un nodo che non esiste

    return 0;
}