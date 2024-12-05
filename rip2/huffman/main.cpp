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
#include <map>
#include <queue>

using namespace std;

class Node {
    public:
        char ch;
        int freq;
        Node *left, *right;

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
            if(!node) return;

            if(node->ch != '\0') codici[node->ch] = cod;

            memorizza_codici(node->left, cod + '0');
            memorizza_codici(node->right, cod + '1');
        }

        Node* extract_min(priority_queue<Node*, vector<Node*>, Compare>& Q) {
            Node* tmp = Q.top();
            Q.pop();
            return tmp;
        }

        Node* huffman_coder() {
            priority_queue<Node*, vector<Node*>, Compare> Q;

            for(auto& el : frequenze)
                Q.push(new Node(el.first, el.second));

            int n = Q.size();
            for(int i = 0; i < n - 1; i++) {
                Node* z = new Node();
                Node* y = extract_min(Q);
                Node* x = extract_min(Q);

                z->left = y; z->right = x;

                z->freq = x->freq + y->freq;
                Q.push(z);
            }

            return extract_min(Q);
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

        string decode(string bin) {
            string decoded = "";
            Node* tmp = root;

            for(char b : bin) {
                if(b == '0') tmp = tmp->left;
                else tmp = tmp->right;

                if(tmp->left == nullptr and tmp->right == nullptr) {
                    decoded += tmp->ch;
                    tmp = root;
                }
            }

            return decoded;
        }

        void print_codes() {
            cout << "Codifica testo" << endl;
            for(auto& el : codici) {
                cout << "Carattere " << el.first << " -> [" << el.second << "] " << endl;
            }
        }
};

int main() {
    // Input di esempio
    string testo = "ciao ricciolino";
    
    // Creazione dell'oggetto Huffman
    Huffman huffman(testo);

    // Stampa delle codifiche generate
    cout << "Tabella delle codifiche:\n";
    huffman.print_codes();

    // Codifica del testo
    string testo_cifrato = huffman.encode(testo);
    cout << "\nTesto originale: " << testo << endl;
    cout << "Testo codificato: " << testo_cifrato << endl;

    // Decodifica del testo
    string testo_decifrato = huffman.decode(testo_cifrato);
    cout << "Testo decodificato: " << testo_decifrato << endl;

    return 0;
}