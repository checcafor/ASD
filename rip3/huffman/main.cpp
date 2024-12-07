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
        char ch;
        int freq;
        Node *parent, *left, *right;

        Node(char ch, int freq) : ch(ch), freq(freq) {}
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
        map<char, string> codici;
        map<char, int> frequenze;

        void memorizza_frequenze(string text) {
            for(char c : text)
                frequenze[c]++;
        }   

        void memorizza_codici(Node* node, string cod) {
            if(node == nullptr) return;
            if(node->ch != '\0') codici[node->ch] = cod;

            memorizza_codici(node->left, cod + '0');
            memorizza_codici(node->right, cod + '1');
        }

        Node* huffman_coder() {
            priority_queue<Node*, vector<Node*>, Compare> Q;

            for(auto& freq : frequenze)
                Q.push(new Node(freq.first, freq.second));

            int n = Q.size();
            for(int i = 0; i < n - 1; i ++) {
                Node* z = new Node();
                z->left = Q.top(); Q.pop();
                z->right = Q.top(); Q.pop();

                z->freq = z->right->freq + z->left->freq;
                Q.push(z);
            }

            Node* tmp = Q.top(); Q.pop();
            return tmp;
        }
    public:
        Huffman(string text) : root(nullptr) {
            memorizza_frequenze(text);
            root = huffman_coder();
            memorizza_codici(root, "");
        }

        string encode(string text) {
            string encoded = "";
            for(auto& c : text) {
                encoded += codici[c];
            }

            return encoded;
        }

        string decoded(string bin) {
            string decoded = "";
            Node* node = root;

            for(char b : bin) {
                node = (b == '0') ? node->left : node->right;

                if(node->left == nullptr and node->right == nullptr) {
                    decoded += node->ch;
                    node = root;
                }
            }

            return decoded;
        }

        void print_cod() {
            for(auto& el : codici) 
                cout << "carattere: {" << el.first << "} -> [" << el.second << "] " << endl;
        }
};

int main() {
    Huffman h("prova codifica");
    h.print_cod();

    string encoded = h.encode("prova codifica");
    cout << "Stringa codificata : " << encoded << endl;
    cout << "Stringa decodificata: " << h.decoded(encoded) << endl;

    return 0;
}