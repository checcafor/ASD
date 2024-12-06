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
#include <map>
#include <queue>

using namespace std;

class Node {
    public:
        char key;
        int freq;
        Node *parent, *left, *right;

        Node(char key, int freq) : key(key), freq(freq) {parent = left = right = nullptr;}
        Node() : Node('\0', 0) {}
};

struct Compare {
    bool operator() (Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

class Huffman {
    private:
        Node* root;
        
        map<char, string> codici; map<char, int> frequenze;

        void memorizza_frequenze(string text) {
            for(char c : text)
                frequenze[c]++;
        }

        void memorizza_codici(Node* node, string cod) {
            if(!node) return;
            if(node->key != '\0') codici[node->key] = cod;

            memorizza_codici(node->left, cod + '0');
            memorizza_codici(node->right, cod + '1');
        }

        Node* huffman_coder() {
            priority_queue<Node*, vector<Node*>, Compare> Q;

            for(auto& f : frequenze) 
                Q.push(new Node(f.first, f.second));

            for(int i = 0; i < (int)Q.size() - 1; i++) {
                Node* z = new Node();
                z->left = Q.top(); Q.pop();
                z->right = Q.top(); Q.pop();

                z->freq = z->left->freq + z->right->freq;
            }

            Node* root_ = Q.top(); Q.pop();
            return root_;
        }
    public:
        Huffman(string text) : root(nullptr) {
            memorizza_frequenze(text);
            root = huffman_coder();
            memorizza_codici(root, "");
        }

        string encode(string text) {
            string encoded = "";
            for(char c : text)
                encoded += codici[c];

            return encoded;
        }

        string decode(string text) {
            string decoded = "";
            Node* curr = root;

            for(char c : text) {
                if(c == '0') curr = curr->left;
                else curr = curr->right;

                if(curr->left == nullptr and curr->right == nullptr) {
                    decoded += curr->key;
                    curr = root;
                }
            }

            return decoded;
        }
};

int main() {

    return 0;
}