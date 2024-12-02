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
        int value;
        Color color;
        Node *parent, *left, *right;

        Node(int key, int value) : key(key), value(value) {parent = left = right = nullptr;}
};

class RedAndBlack {
    private:
        Node* root;
    public:
        RedAndBlack() : root(nullptr) {
            
        }
};

int main() {

    return 0;
}