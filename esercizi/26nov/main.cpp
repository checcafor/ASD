/*
    - [ ]  Utilizzando il paradigma della programmazione ad oggetti ed il linguaggio C++ 11,
    progettare ed implementare, come classe, una struttura dati hashtable con
    indirizzamento aperto che consente di memorizzare le coppie < int, string >
    contenute nel file IN.txt. Dotare la classe di un metodo PRINT per scrivere nel
    file OUT.txt il contenuto della hash table.
    - [ ]  Implementare il metodo FIND(int) che restituisce in output la stringa associata
    alla chiave passata come argomento
    - [ ]  Implementare il metodo DELETE(int) che consente di eliminare la coppia < int,
    string > associata alla chiave passata come argomento (dopo la cancellazione
    visualizzare a video il contenuto della hash table).
*/

#include <iostream>
#include <vector>

using namespace std;

class Bucket {
    public:
        int key;
        int value;
        bool occupied;
        bool deleted;

        Bucket() : occupied(false), deleted(false) {}
};

class HashTable {
    private:
        vector<Bucket> table;
        int capaciity;

        bool checkprime(int n) {
            if(n < 2) return false;
            for(int i = 2; i < n * n; i++) 
                if((n % i) == 0) return false;
            return true;
        }

        int get_prime(int n) {
            if(n % 2 == 0)
                n++;
            while(!checkprime(n))
                n += 2;
            return n;
        }

        int hash_func(int i, int key) {
            return (i + key) % capaciity;
        }
    public:
        HashTable(int n) {
            capaciity = get_prime(n);
        }
};

int main() {

    return 0;
}