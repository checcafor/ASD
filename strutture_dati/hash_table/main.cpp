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
#include <sstream>
#include <cmath>  // Per sqrt()

using namespace std;

enum CollisionMethod { LINEAR, QUADRATIC, DOUBLE_HASHING };
enum HashMethod { DIVISION, MULTIPLICATION, UNIVERSAL };

class Bucket {
    public:
        int key;
        string value;
        bool occupied;
        bool deleted;

        Bucket() : occupied(false), deleted(false) {}
};

class HashTable {
    private:
        vector<Bucket> table;
        int capacity;
        CollisionMethod collisionMethod;
        HashMethod hashMethod;

        int get_prime(int n) {
            if(n % 2 == 0)
                n++;

            while(!check_prime(n))
                n += 2;

            return n;
        }

        bool check_prime(int n) {
            if(n < 2)
                return false;
            
            for(int i = 2; i * i <= n; i++)
                if(n % i == 0)
                    return false;
            
            return true;
        }

        // Funzione di hash per il metodo della divisione
        int division_hash(int key) {
            return key % capacity;
        }

        // Funzione di hash per il metodo della moltiplicazione
        int multiplication_hash(int key) {
            const double A = (sqrt(5) - 1) / 2;  // costante di moltiplicazione
            double fractional_part = fmod(key * A, 1.0);  // parte frazionaria
            return static_cast<int>(capacity * fractional_part);  // indici della tabella
        }

        // Funzione di hash per hashing universale
        int universal_hash(int key) {
            int a = rand() % (capacity - 1) + 1;  // coefficiente a (tra 1 e capacity-1)
            int b = rand() % capacity;            // coefficiente b (tra 0 e capacity-1)
            int p = get_prime(capacity * 2);      // p è un primo maggiore di capacity
            return ((a * key + b) % p) % capacity;
        }

        // Funzione di hash che seleziona il metodo di hashing
        int get_hash(int key) {
            switch (hashMethod) {
                case DIVISION:
                    return division_hash(key);
                case MULTIPLICATION:
                    return multiplication_hash(key);
                case UNIVERSAL:
                    return universal_hash(key);
                default:
                    return -1;  // non dovrebbe mai arrivarci
            }
        }

        // Funzione di hash per ispezione lineare
        int linear_probe_hash(int i, int key) {
            return (get_hash(key) + i) % capacity;
        }

        // Funzione di hash per ispezione quadratica
        int quadratic_probe_hash(int i, int key) {
            return (get_hash(key) + i * i) % capacity;
        }

        // Funzione di hash per doppio hashing
        int hash2(int key) {
            return 1 + (key % (capacity - 1));  // Secondo hash per doppio hashing
        }

        // Funzione di hash per doppio hashing
        int double_hash(int i, int key) {
            return (get_hash(key) + i * hash2(key)) % capacity;
        }

        int get_probe_hash(int i, int key) {
            switch (collisionMethod) {
                case LINEAR:
                    return linear_probe_hash(i, key);
                case QUADRATIC:
                    return quadratic_probe_hash(i, key);
                case DOUBLE_HASHING:
                    return double_hash(i, key);
                default:
                    return -1;  // non dovrebbe mai arrivarci
            }
        }

    public:
        HashTable(int n, CollisionMethod collision = LINEAR, HashMethod hash = DIVISION) {
            capacity = get_prime(n);
            table.resize(n);
            this->collisionMethod = collision;
            this->hashMethod = hash;

            ifstream input("IN.txt");
            if (!input) {
                cerr << "Errore nell'aprire il file" << endl;
                return;
            }

            char separatore; int key; string value;
            while (input >> separatore >> key >> separatore >> value) {
                if (!value.empty() && value.back() == '>') {
                    value.pop_back();  // rimuove '>'
                }
                insert_bucket(key, value);
            }

            input.close();
        }

        void insert_bucket(int key, string value) {
            int i = 0, index = get_probe_hash(i, key);
            while (table[index].occupied)
                index = get_probe_hash(++i, key);

            table[index].key = key;
            table[index].value = value;
            table[index].occupied = true;
            table[index].deleted = false;
        }

        void delete_bucket(int key) {
            int i = 0, index = get_probe_hash(i, key);

            while (table[index].occupied || table[index].deleted)
                if (table[index].occupied && table[index].key == key)
                    break;
                else {
                    index = get_probe_hash(++i, key);
                    if (i >= capacity) return;
                }

            table[index].occupied = false;
            table[index].deleted = true;
        }

        string search_bucket(int key) {
            int i = 0, index = get_probe_hash(i, key);

            while (table[index].occupied || table[index].deleted)
                if (table[index].key == key)
                    return table[index].value;
                else {
                    index = get_probe_hash(++i, key);
                    if (i >= capacity) return "";
                }

            return "";
        }

        void print() {
            for (int i = 0; i < capacity; i++) 
                if (table[i].occupied)
                    cout << "posizione " << i << " -> { chiave " << table[i].key << " -> " << table[i].value << "}" << endl;
                else if (table[i].deleted)
                    cout << "posizione " << i << " -> elemento eliminato" << endl;
                else
                    cout << "posizione " << i << " -> posizione vuota" << endl;
        }
};

int main() {
    HashTable hash(5, LINEAR, UNIVERSAL);  // usa Linear probing con hashing universale

    hash.print();

    cout << "cerco elemento 123456789 ..." << endl;
    string val = hash.search_bucket(123456789); 
    cout << (val == "" ? "CHIAVE 123456789 NON TROVATA; " : "Elemento trovato con chiave ") << val << ";" << endl;

    hash.print();

    return 0;
}