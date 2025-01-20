#include <iostream>
#include <algorithm>

struct Node {
    int data; // Datos del nodo
    Node* next; // Pointer al nodo siguiente

    Node(int value) { // Constructor
        data = value;
        next = nullptr;
    }
};

class List {
    Node* first;
    int size;

public:
    List() {
        first = nullptr;
        size = 0;
    }

    int getSize() { return size; }
    void showList();

    bool insertFirst(int newValue);
    bool insertLast(int newValue);
    bool insertAtIndex(int newValue, int index);

    void deleteFirst();
    void deleteLast();
    bool deleteAtIndex(int index);

    void update(int newValue, int index);
    Node* find(int value, int* indexPointer);
};

void List::showList() {
    int i = 0;
    std::cout << "List size = " << getSize() << "\n";

    Node* aux = first;

    while (aux != nullptr) {
        std::cout << "Valor num [" << i << "] >> " << aux->data << "\n";
        aux = aux->next;
        i++;
    }
    std::cout << "\n";
}

bool List::insertFirst(int newValue) {

    if (first == nullptr) {
        std::cout << "insertFirst: Lista vacia, agregando primer nodo.\n";
    }

    Node* newNode = new (std::nothrow) Node(newValue);
    if (newNode == nullptr) {
        std::cerr << "Asignación de memoria fallida.\n";
        return false;
    }

    newNode->next = first; // Anterior primer nodo ahora es el que sigue del nuevo
    first = newNode; // Nuevo nodo ahora es el primero

    size++;
    std::cout << "insertFirst: Insertado al inicio\n";

    return true;
}

bool List::insertLast(int newValue) {

    if (first == nullptr && size == 0) { // Check lista vacía
        std::cout << "insertLast: Lista vacia, agregando primer nodo.\n";
        insertFirst(newValue);
        return true;
    }

    Node* newNode = new (std::nothrow) Node(newValue); // Crear nuevo nodo
    if (newNode == nullptr) { // Check memoria
        std::cerr << "Asignación de memoria fallida.\n";
        return false;
    }

    Node* aux = first; // Recorrer desde first hasta node con next vacío
    while (aux->next != nullptr) {
        aux = aux->next;
    }

    aux->next = newNode; // Asignar nuevo valor
    size++; // Incrementar size
    return true;
}

bool List::insertAtIndex(int newValue, int index) {

    if (index < 0 || index > size) {
        std::cerr << "insertAtIndex: Indice fuera de rango\n";
        return false;
    }

    if (first == nullptr && size == 0) { // Check lista vacía
        std::cout << "insertLast: Lista vacia, agregando primer nodo.\n";
        insertFirst(newValue);
        return true;
    }

    Node* newNode = new (std::nothrow) Node(newValue); // Crear nuevo nodo
    if (newNode == nullptr) { // Check memoria
        std::cerr << "Asignación de memoria fallida.\n";
        return false;
    }

    Node* aux = first;

    for (int i = 0; i < index - 1 && aux != nullptr; i++) {
        aux = aux->next;
    }

    if (aux != nullptr) {
        newNode->next = aux->next;
        aux->next = newNode;
        size++;
        std::cout << "insertAtIndex: Indice [" << index << "] valido\n";
        return true;
    }

    std::cout << "insertAtIndex: Indice no valido\n";
    return false;
}

void List::deleteFirst() {

    Node* aux = first;
    first = first->next;
    delete aux;
    size--;

    std::cout << "Primer elemento eliminado\n";
    return;
}

void List::deleteLast() {

    if (size == 1) {
        deleteFirst();
        return;
    }

    Node* aux = first;

    while (aux->next->next != nullptr) {
        aux = aux->next;
    }

    delete aux->next;
    aux->next = nullptr;

    size--;
    std::cout << "Ultimo elemento eliminado\n";
    return;
}

bool List::deleteAtIndex(int index) {

    if (index < 0 || index > size) {
        std::cerr << "insertAtIndex: Indice fuera de rango\n";
        return false;
    }
    if (size == 1) { // Check por si podemos hacer deletefirst o deletelast
        deleteFirst();
        return true;
    }
    else if (index == size - 1) {
        deleteLast();
        return true;
    }

    Node* aux = first;

    for (int i = 0; i < index - 1 && aux != nullptr; i++) {
        aux = aux->next; // Recorremos al nodo antes de 
    }

    if (aux != nullptr) {
        Node* temp = aux->next;
        aux->next = temp->next;
        delete temp;
        this->size--;
        std::cout << "Eliminado en indice " << index << "\n";

        return true;
    }

    std::cout << "deleteAtIndex: Indice no valido\n";
    return false;
}

Node* List::find(int value, int* indexPointer) {
    Node* aux = first;

    int i = 0;
    while (aux != nullptr) {
        if (aux->data == value) {

            *indexPointer = i;
            std::cout << "find: Valor [" << value << "] encontrado en indice " << i << "\n";
            return aux;
        }
        aux = aux->next;
        i++;
    }
    std::cout << "find: Valor [" << value << "] no encontrado\n";
    *indexPointer = -1;
    return nullptr;
}

void List::update(int newValue, int index) {

    Node* aux = first;

    for (int i = 0; i < index && aux != nullptr; i++) {
        aux = aux->next; // Cicla hasta el indice
    }

    if (aux == nullptr) {
        std::cerr << "update: Indice no valido\n";
        return;
    }

    aux->data = newValue; // Reemplaza el valor con el nuevo

    std::cout << "Actualizado en indice " << index << "\n";
    return;
}

int main() {
    List lista1;

    lista1.insertFirst(1);
    lista1.insertFirst(5);
    lista1.insertFirst(25);
    lista1.insertFirst(9);
    lista1.insertLast(4);
    lista1.insertAtIndex(43, 1);
    lista1.showList();

    lista1.deleteFirst();
    lista1.deleteLast();
    lista1.showList();

    lista1.deleteAtIndex(3);
    lista1.showList();

    lista1.update(88, 2);
    lista1.showList();

    int index;
    Node* example = lista1.find(25, &index);

    return 0;
}