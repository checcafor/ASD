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
        string val;
        bool occupied;
        bool deleted;

        Bucket() : occupied(false), deleted(false) {}
};

class HashTable {
    private:
        vector<Bucket> table;
        int capacity;

        bool check_prime(int n) {
            if(n < 2) return false;

            for(int i = 2; i * i < n; i++)
                if(n % i == 0) return false;
            return true;
        }

        int get_prime(int n) {
            if(n % 2 == 0)
                n++;

            while(!check_prime(n))
                n += 2;

            return n;
        }
    
        int hash_func(int i, int key) {
            return (i + key) % capacity;
        }
    public:
        HashTable(int n) {
            capacity = get_prime(n);
            table.resize(n);
        }
};

int main() {

    return 0;
}