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
#include <map>

using namespace std;

class Node {
    public:
        int key;
        char value;
        Node *parent, *left, *right;

        Node(int key, char value) : key(key), value(value) {parent = left = right = nullptr;}
};

class ABR {
    private:
        Node* root;
        map<char, string> codici;

        void save(ofstream &out, Node* node) {
            if(node != nullptr) {
                out << "Chiave[" << node->key << "] valore: " << node->value << endl;
                save(out, node->left);
                save(out, node->right);
            }
        }

        void memorizzacodici(Node* node, string code) {
            if (node == nullptr)
                return;

            if (node->left == nullptr && node->right == nullptr) {
                codici[node->value] = code;
            }

            memorizzacodici(node->left, code + '0');
            memorizzacodici(node->right, code + '1');
        }
    public:
        ABR() : root(nullptr) {
            ifstream input("ABR.txt");
            if(!input) return;

            int key; char value;
            while(input >> key >> value)
                insert(key, value);
            
            input.close();

            memorizzacodici(root, "");

            ofstream output("OUT.txt");
            if(!output) return;

            save(output, root);

            output.close();
        }

        string encode(string text) {
            string encoded = "";
            for(char c : text)
                encoded += codici[c];
            return encoded;
        }

        string decode(string bin) {
            string decoded = ""; Node* curr = root;
            for(char b : bin) {
                curr = (b == '0') ? curr->left : curr->right;
                if(curr->left == nullptr and curr->right == nullptr) {
                    decoded += curr->value;
                    curr = root;
                }
            }

            return decoded;
        }

        void insert(int key, char value) {
            Node* z = new Node(key, value);
            Node *x = root, *xp = nullptr;

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
        }
};

int main() {
    ABR a;

    string cod = a.encode("Hello_World");
    cout << "La stringa codificata è " << cod << ", decodificata { " << a.decode(cod) << " }" << endl;

    return 0;
}