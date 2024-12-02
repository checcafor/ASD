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

class RedBlackTree {
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
            else x->parent->right = y;

            x->parent = y;
            y->left = x;
        }

        void right_rotate(Node* y) {
            Node* x = y->left;
            y->left = x->right;

            if(x->right != nullptr)
                x->right->parent = y;

            x->parent = y->parent;
            if(y->parent == nullptr)
                root = x;
            else if(y->parent->left == y) 
                y->parent->left = x;
            else y->parent->right = x;

            y->parent = x;
            x->right = y;
        }

        void insert_fixup(Node* node) {
            while(node != root and node->parent->color == RED) {
                Node* granparent = node->parent->parent;
                if(node->parent == granparent->left) {
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
    
        void write_on_file(ofstream &out, Node* node) {
            if(node == nullptr) return;

            write_on_file(out, node->left);

            out << "Nodo: " << node->key << endl;

            write_on_file(out, node->right);
        }
    
        void transplant(Node* u, Node* v) {
            Node* up = u->parent;

            if(up == nullptr)
                root = v;
            else if(up->left == u)
                up->left = v;
            else up->right = v;

            if(v != nullptr) v->parent = up;
        }

        Node* min(Node* node) {
            Node* tmp = node;
            while(tmp->left != nullptr)
                tmp = tmp->left;

            return tmp;
        }

        void deletefixup(Node* x) {
            while(x != root and (x == nullptr or x->color == BLACK)) {
                Node* parent = x->parent;
                if(x == parent->left) {
                    Node* w = parent->right;
                    if(w->color == RED) {
                        w->color = BLACK;
                        parent->color = RED;
                        left_rotate(parent);
                        w = parent->right;
                    } 
                    
                    if((w->right == nullptr or w->right->color == BLACK) and (w->left == nullptr or w->left->color == BLACK)) {
                        w->color = RED;
                        x = parent;
                    } else {
                        if(w->right == nullptr or w->right->color == BLACK) {
                            if(w->left != nullptr)
                                w->left->color = BLACK;
                            w->color = RED;
                            right_rotate(w);
                            w = parent->right;
                        }

                        w->color = parent->color;
                        parent->color = BLACK;
                        if(w->right != nullptr) w->right->color = BLACK;
                        left_rotate(parent);
                        x = root;
                    }
                } else {
                    Node* w = parent->left;
                    if(w->color == RED) {
                        w->color = BLACK;
                        parent->color = RED;
                        right_rotate(parent);
                        w = parent->right;
                    } 
                    
                    if((w->right == nullptr or w->right->color == BLACK) and (w->left == nullptr or w->left->color == BLACK)) {
                        w->color = RED;
                        x = parent;
                    } else {
                        if(w->right == nullptr or w->right->color == BLACK) {
                            if(w->left != nullptr)
                                w->left->color = BLACK;
                            w->color = RED;
                            left_rotate(w);
                            w = parent->left;
                        }

                        w->color = parent->color;
                        parent->color = BLACK;
                        if(w->left != nullptr) w->left->color = BLACK;
                        right_rotate(parent);
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
                deletefixup(x);
        }
    public:
        RedBlackTree() : root(nullptr) {
            ifstream in("ABR.txt");
            if(!in) {
                cout << "Errore nell'apertura del file" << endl;
                return;
            }

            int key;
            while(in >> key) {
                cout << "Inserimento chiave " << key << endl;
                insert(key);
            }

            in.close();

            ofstream out("OUT.txt");
            if(!out) {
                cout << "Errore nell'apertura del file" << endl;
                return;
            }

            write_on_file(out, root);

            out.close();
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
            if(node == nullptr or node->key == key) {
                return node;
            }

            return (node->key > key) ? search(node->left, key) : search(node->right, key);
        }

        void del(int key) {
            Node* z = search(root, key);

            if(z == nullptr) return;
            
            rb_delete(z);
        }
};

int main () {
    RedBlackTree tree;

    return 0;
}