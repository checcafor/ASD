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
#include <list>

using namespace std;

class HashTable {
private:
    vector<list<pair<int, string>>> table;  // Una lista di coppie chiave/valore per ogni indice
    int capacity;

    // Funzione di hash semplice
    int hash_function(int key) {
        return key % capacity;
    }

public:
    HashTable(int size) {
        capacity = size;
        table.resize(capacity);
    }

    // Inserimento di un elemento nella hash table
    void insert(int key, const string& value) {
        int index = hash_function(key);
        for (auto& pair : table[index]) {
            if (pair.first == key) {
                pair.second = value;  // Aggiorna il valore se la chiave esiste già
                return;
            }
        }
        table[index].emplace_back(key, value);  // Aggiungi la nuova coppia chiave/valore
    }

    // Ricerca di un elemento nella hash table
    string search(int key) {
        int index = hash_function(key);
        for (auto& pair : table[index]) {
            if (pair.first == key) {
                return pair.second;  // Ritorna il valore se la chiave è trovata
            }
        }
        return "Non trovato";  // Se la chiave non esiste
    }

    // Cancellazione di un elemento dalla hash table
    void remove(int key) {
        int index = hash_function(key);
        table[index].remove_if([key](const pair<int, string>& pair) {
            return pair.first == key;
        });
    }

    // Stampa della hash table
    void print() {
        for (int i = 0; i < capacity; ++i) {
            cout << "Indice " << i << ": ";
            for (auto& pair : table[i]) {
                cout << "(" << pair.first << ", " << pair.second << ") ";
            }
            cout << endl;
        }
    }
};

int main() {
    HashTable hashTable(5);

    hashTable.insert(10, "valore10");
    hashTable.insert(15, "valore15");
    hashTable.insert(25, "valore25");
    hashTable.insert(30, "valore30");

    hashTable.print();

    cout << "Cerca chiave 15: " << hashTable.search(15) << endl;
    hashTable.remove(15);
    cout << "Dopo la rimozione, cerca chiave 15: " << hashTable.search(15) << endl;

    hashTable.print();

    return 0;
}