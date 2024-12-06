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

enum Color {RED,BLACK};

class Node {
    public:
        int key;
        Color color;
        Node *parent, *left, *right;

        Node(int key) : key(key), color(RED) {parent = left = right = nullptr;}
};

class RedBlack {
    private:
        Node* root;

        void left_rotate(Node* x) {
            Node* y = x->right;
            x->right = y->left;

            if(x->right != nullptr) x->right->parent = x;

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

            if(y->left != nullptr) y->left->parent = y;

            x->parent = y->parent;
            if(y->parent == nullptr)
                root = x;
            else if(y->parent->left == y)
                y->parent->left = x;
            else y->parent->right = x;

            y->parent = x;
            x->right = y;
        }

        void transplant(Node* u, Node* v) {
            
        }

        void insert_fixup(Node* node) {
            while(node != nullptr and node->parent->color == RED) {
                Node* granparent = node->parent->parent;
                if(node->parent == granparent->left) {
                    Node* uncle = granparent->right;
                    if(uncle->color == RED) {
                        uncle->color = BLACK;
                        node->parent->color = BLACK;
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
                        uncle->color = BLACK;
                        node->parent->color = BLACK;
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
        }
    public:
        RedBlack() : root(nullptr) {

        }

        void insert(int key) {
            Node* z = new Node(key);
            Node* x = root; Node* xp = nullptr;

            while(x != nullptr) {
                xp = x;
                x = (x->key > key) ? x->left : x->right;
            }

            z->parent = xp;
            if(xp == nullptr) root = z;
            else if(xp->key > key) xp->left = z;
            else xp->right = z;

            insert_fixup(z);
        }
};

using namespace std;

int main() {

    return 0;
}