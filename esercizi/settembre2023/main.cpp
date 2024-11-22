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

using namespace std;

class Bucket {
    public:
        int key;
        char data;
        bool occupied;
        bool deleted;

        Bucket() : occupied(false), deleted(false) {}
};

class HashTable {
    private:
        int capacity;
        vector<Bucket> table;

        bool checkprime(int n) {
            if(n < 2) return false;
            for(int i = 2; i * i <= n; i ++) 
                if(n % i == 0) return false;
            return true;
        }

        int get_prime(int n) {
            if(n % 2 == 0) n++;

            while(!checkprime(n))
                n += 2;
            
            return n;
        }

        int hash_func(int key, int i) { return (key + i) % capacity; }
    public:
        HashTable (int n) {
            capacity = get_prime(n);
            table.resize(capacity);
        }

        void insert(int key, int data) {
            int i = 0, index = hash_func(key, i);
            while(table[index].occupied) { 
                hash_func(key, ++i); 
                if(i >= capacity) return;
            }

            table[index].key = key;
            table[index].data = data;
            table[index].occupied = true;
            table[index].deleted = false;
        }

        void deleted(int key) {
            int i = 0, index = hash_func(key, i);
            while(table[index].occupied or table[index].deleted) {
                if(table[index].occupied and table[index].key == key) {
                    table[index].occupied = false;
                    table[index].deleted = true;

                    cout << "Nodo con chiave " << key << " eliminato con successo" << endl;
                    return;
                }

                index = hash_func(key, ++i);
                if(i >= capacity) return;
            }
            cout << "Nodo con chiave " << key << " non trovato" << endl;
        }

        int search(int key) {
            int data, i = 0, index = hash_func(key, i);
            while(table[index].occupied or table[index].deleted) {
                if(table[index].occupied and table[index].key == key) {
                    data = table[index].data;

                    cout << "Nodo con chiave " << key << " trovato con successo" << endl;
                    return data;
                }

                index = hash_func(key, ++i);
                if(i >= capacity) return;
            }
            cout << "Nodo con chiave " << key << " non trovato" << endl;
            return -1;
        }
};

int main() {

    return 0;
}