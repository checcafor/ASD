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

enum Color {RED, BLACK};

class Node {
    public:
        int key;
        Color color;
        Node *parent, *left, *right;

        Node(int key) : key(key), parent(nullptr), left(nullptr), right(nullptr), color(RED) {}
};

class RedBlack {
    private:
        Node* root;

        void leftrotate(Node* x) {
            Node* y = x->right;
            x->right = y->left;

            if (y->left != nullptr)
                y->left->parent = x;

            y->parent = x->parent;
            if (x->parent == nullptr)
                root = y;
            else if (x->parent->left == x)
                x->parent->left = y;
            else
                x->parent->right = y;

            y->left = x;
            x->parent = y;
        }

        void rightrotate(Node* y) {
            Node* x = y->left;
            y->left = x->right;

            if (x->right != nullptr)
                x->right->parent = y;

            x->parent = y->parent;
            if (y->parent == nullptr)
                root = x;
            else if (y->parent->left == y)
                y->parent->left = x;
            else
                y->parent->right = x;

            x->right = y;
            y->parent = x;
        }

        void insert_fixup(Node* node) {
            while (node != root && node->parent->color == RED) {
                Node* granparent = node->parent->parent;
                if (granparent->left == node->parent) {
                    Node* uncle = granparent->right;
                    if (uncle != nullptr && uncle->color == RED) {
                        uncle->color = BLACK;
                        node->parent->color = BLACK;
                        granparent->color = RED;
                        node = granparent;
                    } else {
                        if (node == node->parent->right) {
                            node = node->parent;
                            leftrotate(node);
                        }
                        node->parent->color = BLACK;
                        granparent->color = RED;
                        rightrotate(granparent);
                    }
                } else {
                    Node* uncle = granparent->left;
                    if (uncle != nullptr && uncle->color == RED) {
                        uncle->color = BLACK;
                        node->parent->color = BLACK;
                        granparent->color = RED;
                        node = granparent;
                    } else {
                        if (node == node->parent->left) {
                            node = node->parent;
                            rightrotate(node);
                        }
                        node->parent->color = BLACK;
                        granparent->color = RED;
                        leftrotate(granparent);
                    }
                }
            }
            root->color = BLACK;
        }

    public:
        RedBlack() : root(nullptr) {}

        void insert(int key) {
            Node* z = new Node(key);
            Node* x = root; Node* xp = nullptr;

            while (x != nullptr) {
                xp = x;
                x = (x->key > key) ? x->left : x->right;
            }

            z->parent = xp;
            if (xp == nullptr)
                root = z;
            else if (xp->key > key)
                xp->left = z;
            else
                xp->right = z;

            insert_fixup(z);
        }

        void inorder(Node* node) {
            if (node == nullptr) return;
            inorder(node->left);
            cout << node->key << " (" << (node->color == RED ? "R" : "B") << ") ";
            inorder(node->right);
        }

        void print() {
            inorder(root);
            cout << endl;
        }
};

int main() {
    RedBlack tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.print();  // Output: 10 (B) 20 (R) 30 (B)
    return 0;
}
