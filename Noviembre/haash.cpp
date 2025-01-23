#include <iostream>
#include <sstream>
#include <cstring>
#include <string>

using namespace std;

// Una clase para un elemento de la tabla
template<class T>
class Element {
    public:
        string str_key;  // Llave string, como "Ana" 
        int int_key;     // Llave entera, resultante de la llave string
        int h_key;       // Indice real asignado con base en int_key
        T item;          // El objeto asociado con la llave
        bool free;       // Una bandera para saber si este elemento de la tabla está libre o no

        // Constructor
        Element<T> () {
            str_key = "";
            int_key = -1;
            h_key = -1;
            item = -1;
            free = true; 
        }
};

// Una clase que será la tabla hash
template<class T>
class Hash {
    public:
        int maxSize;          // Tamaño máximo
        int currentSize;      // Tamaño actual
        int i;                // Contador de colisiones

        Element<T> **table;   // Puntero a arreglo de los elementos

        Hash(int size) {
            int j;
            maxSize = size;
            currentSize = 0;
            i = 0;

            table = new Element<T>*[maxSize];
            for (j = 0; j < maxSize; j++) {
                table[j] = new Element<T>();
            }
        }

        void show();                        // Ver toda la tabla
        void insert(string, T);             // Agregar un elemento nuevo, recibe key y value
        void remove(string);                // Remover un elemento por su key
        void update(string, T);             // Actualizar un elemento con una nueva llave o valor
        int find(string);                   // Encontrar el índice de una llave
};

// Mostrar el contenido de la tabla hash
template<class T>
void Hash<T>::show() {
    for (int j = 0; j < maxSize; j++) {
        cout << j << " " << table[j]->str_key << " " 
             << table[j]->int_key << " " 
             << table[j]->h_key << " " 
             << table[j]->item << endl;
    }
    cout << endl;
}

// Insertar un elemento en la tabla hash
template<class T> 
void Hash<T>::insert(string str_key, T value) { //! O(n)
    if (currentSize < maxSize) {
        cout << "Insertando: " << str_key << " " << value << endl;
        int key = int(str_key[0]);  // Valor numérico del primer caracter de str_key
        int h_key;
        int flag = 0;
        i = 0;

        while (flag == 0) {
            h_key = (key + i) % maxSize;

            if (table[h_key]->free == true) {
                table[h_key]->free = false;
                table[h_key]->str_key = str_key;
                table[h_key]->int_key = key;
                table[h_key]->h_key = h_key;
                table[h_key]->item = value;
                flag = 1;
                currentSize++;
            } else {
                i++;
            }
        }
    } else {
        cout << "No se puede insertar. Tabla llena." << endl;
    }
}

// Encontrar el índice de un elemento basado en su llave
template<class T>
int Hash<T>::find(string str_key) {  //! O(n)
    cout << "Buscando: " << str_key << endl;
    int key = int(str_key[0]);
    i = 0;

    while (i < maxSize) {
        int h_key = (key + i) % maxSize;

        // Si la key coincide y el elemento no está libre
        if (!table[h_key]->free && table[h_key]->str_key == str_key) {
            // Mostrar el índice donde se encontró el elemento
            cout << "Elemento encontrado en el índice: " << h_key << endl;
            return h_key;
        }

        i++;
    }

    cout << "Elemento no encontrado." << endl;
    return -1;
}

// Remover un elemento de la tabla hash
template<class T>
void Hash<T>::remove(string str_key) {  //! O(n)
    cout << "Removiendo: " << str_key << endl;
    // Usamos nuestra funcion find
    int index = find(str_key);

    // Si se encontró el elemento, marcarlo como libre
    if (index != -1) {
        table[index]->free = true;
        table[index]->str_key = "";
        table[index]->int_key = -1;
        table[index]->h_key = -1;
        table[index]->item = -1;
        currentSize--;
        cout << "Elemento removido." << endl;
    } else {
        cout << "No se encontró el elemento para eliminar." << endl;
    }
}

// Actualizar el valor de un elemento existente
template<class T>
void Hash<T>::update(string str_key, T new_value) {  //! O(n)
    // Usamos nuestra funcion find
    int index = find(str_key);

    // Si se encontró el elemento, actualizar su valor
    if (index != -1) {
        table[index]->item = new_value;
        cout << "Elemento actualizado." << endl;
    } else {
        cout << "No se encontró el elemento para actualizar." << endl;
    }
}

int main() {
        Hash<int> htable(10);

    htable.insert("Ana", 24);
    htable.show();
    htable.insert("Annie", 30);		htable.show();

    htable.insert("Armando", 45);   htable.show();
    htable.insert("Alex", 60);		htable.show();
    htable.insert("Axel", 28);		htable.show();
    htable.insert("Andrea", 32);	htable.show();

    htable.insert("Bill", 32);		htable.show();
    htable.insert("Veronica", 32);	htable.show();
    htable.insert("Karla", 32);		htable.show();
    htable.insert("Laura", 32);		htable.show();

    htable.insert("Luis", 32);
    htable.show();

    htable.find("Alex");

    return 0;
}

