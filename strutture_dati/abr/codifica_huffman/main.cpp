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
        Node* left;
        Node* right;

        Node(char ch, int freq) : ch(ch), freq(freq), left(nullptr), right(nullptr) {}
        Node() : Node('\0', 0) {}
};

struct Compare {
    bool operator() (Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

class Huffman {
    private:
        map<char, string> codici;
        map<char, int> frequenze;

        Node* root;

        void calcolafreq(string text) {
            for(char c : text)
                frequenze[c]++;
        }

        Node* extract_min(priority_queue<Node*, vector<Node*>, Compare> Q) {
            Node* tmp = Q.top(); Q.pop();
            return tmp;
        }

        Node* HuffmanCoder() {
            priority_queue<Node*, vector<Node*>, Compare> Q;

            for(auto& f : frequenze)
                Q.push(new Node(f.first, f.second));

            int n = Q.size();
            for(int i = 0; i < n - 1; i++) {
                Node* z = new Node();
                z->left = extract_min(Q);
                z->right = extract_min(Q);

                z->freq = z->left->freq + z->right->freq;
                Q.push(z);
            }

            return extract_min(Q);
        }
    public:
        Huffman(string text) {
            root = nullptr;
            calcolafreq(text);
            root = HuffmanCoder();
        }

        string encode(string text) {
            string encoded = "";
            for(char c : text)
                encoded += codici[c];
            
            return encoded;
        }

        string decode(string bin) {
            Node* tmp = root;
            string decoded = "";

            for(int i = 0; i < (int)bin.size(); i++) {
                (bin[i] == 0) ? tmp->left : tmp->right;

                if(tmp->left == nullptr and tmp->right == nullptr) {
                    decoded += tmp->ch;
                    tmp = root;
                }
            }
            
            return decoded;
        }
};

int main() {

    return 0;
}