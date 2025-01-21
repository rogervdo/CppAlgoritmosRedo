#include <iostream>
#include <algorithm>

struct Node {
    int data; // Datos del nodo
    Node* next; // Pointer al nodo siguiente y previo
    Node* prev;

    Node(int value) { // Constructor basico
        data = value;
        next = prev = nullptr;
    }
};

class List {
    Node* first;
    Node* last;
    int size;

public:
    List() { // Constructor basico
        first = last = nullptr;
        size = 0;
    }

    int getSize() { return size; }
    void showList();
    void showListReverse();

    bool insertFirst(int newValue);
    bool insertLast(int newValue);
    bool insertAtIndex(int newValue, int index);

    bool deleteFirst();
    bool deleteLast();
    bool deleteAtIndex(int index);

    void update(int newValue, int index);
    Node* find(int value, int* indexPointer);
};

void List::showList() { // Recorre la lista desde first
    int i = 0;
    std::cout << "List size = " << getSize() << "\n";

    Node* aux = first;

    while (i < size) {
        std::cout << "Valor num [" << i << "] >> " << aux->data << "\n";
        aux = aux->next;
        i++;
    }
    std::cout << "\n";
}

void List::showListReverse() { // Recorre la lista desde last;
    int i = 0;
    std::cout << "List size = " << getSize() << "\n";

    Node* aux = last;

    while (i < size) {
        std::cout << "Valor num [" << i << "] >> " << aux->data << "\n";
        aux = aux->prev;
        i++;
    }
    std::cout << "\n";
}

bool List::insertFirst(int newValue) {

    Node* newNode = new (std::nothrow) Node(newValue);
    if (newNode == nullptr) {
        std::cerr << "Asignación de memoria fallida.\n";
        return false;
    }

    if (first == nullptr) {
        std::cout << "insertFirst: Lista vacia, agregando primer nodo.\n";
        first = newNode;
        last = newNode;
        size++;
        std::cout << "insertFirst: " << newValue << " Insertado al inicio\n";
        return true;
    }


    first->prev = newNode; // Anterior primer nodo ahora apunta hacia atras al nuevo
    newNode->next = first; // Anterior primer nodo ahora es el que sigue del nuevo
    first = newNode; // Nuevo nodo ahora es el primero

    size++;
    std::cout << "insertFirst: " << newValue << " Insertado al inicio\n";

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

    last->next = newNode; // Mismo procedimiento que en first pero inverso
    newNode->prev = last;
    last = newNode;

    size++; // Incrementar size
    return true;
}

bool List::insertAtIndex(int newValue, int index) {

    if (index < 0 || index > size) {
        std::cerr << "insertAtIndex: Indice fuera de rango\n";
        return false;
    }

    if ((first == nullptr && size == 0) || index == 0) { // Check lista vacía
        std::cout << "insertLast: Lista vacia/Indice 0, agregando primer nodo.\n";
        insertFirst(newValue);
        return true;
    }

    if (index == size) {
        insertLast(newValue);
        return true;
    }

    Node* newNode = new (std::nothrow) Node(newValue); // Crear nuevo nodo
    if (newNode == nullptr) { // Check memoria
        std::cerr << "Asignación de memoria fallida.\n";
        return false;
    }

    Node* aux;

    if (index < size / 2) { // Decide si recorrer desde el inicio o el final
        aux = first;
        for (int i = 0; i < index - 1 && aux != nullptr; i++) {
            aux = aux->next;
        }
    }

    else {
        aux = last;
        for (int i = size - 1; i > index - 1 && aux != nullptr; i++) {
            aux = aux->prev;
        }
    }


    if (aux != nullptr) { // Modifica las conexiones de los dos nodos alrededor del nuevo.
        Node* temp = aux->next;

        newNode->next = temp;
        newNode->prev = aux;

        aux->next = newNode;
        temp->prev = newNode;

        std::cout << "insertAtIndex: Indice [" << index << "] valido\n";
        return true;
    }

    std::cout << "insertAtIndex: Indice no valido\n";
    return false;
}

bool List::deleteFirst() {

    if (size <= 0) {
        std::cerr << "deleteAtIndex: lista vacia\n";
        return false;
    }

    Node* aux = first;
    if (first->next != nullptr) { // Check solo un elemento
        first = first->next;
        first->prev = nullptr;
    }
    else {
        first = last = nullptr; // Lista vacia ahora
    }

    delete aux;
    size--;

    std::cout << "Primer elemento eliminado\n";
    return true;
}

bool List::deleteLast() {

    if (size <= 0) {
        std::cerr << "deleteAtIndex: lista vacia\n";
        return false;
    }
    if (size == 1) {
        deleteFirst();
        return true;
    }

    Node* aux = last;
    if (last->prev != nullptr) { // Check solo un elemento
        last = last->prev;
        last->next = nullptr;
    }
    else {
        first = last = nullptr; // Lista vacia
    }

    delete aux;
    size--;

    std::cout << "Ultimo elemento eliminado\n";
    return true;
}

bool List::deleteAtIndex(int index) {

    if (size <= 0) {
        std::cerr << "deleteAtIndex: lista vacia\n";
        return false;
    }
    if (index < 0 || index >= size) {
        std::cerr << "deleteAtIndex: Indice fuera de rango\n";
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

    Node* aux;
    if (index < size / 2) {
        aux = first;
        for (int i = 0; i < index - 1 && aux != nullptr; i++) {
            aux = aux->next;
        }
    }

    else {
        aux = last;
        for (int i = size - 1; i > index - 1 && aux != nullptr; i++) {
            aux = aux->prev;
        }
    }

    if (aux != nullptr) {
        Node* del = aux->next;
        Node* temp = del->next;

        aux->next = temp; // hacemos "puentes" sobre el nodo a borrar y borramos el nodo.
        temp->prev = aux;

        delete del;

        size--;
        std::cout << "Eliminado en indice " << index << "\n";
        return true;
    }

    std::cout << "deleteAtIndex: Indice no valido\n";
    return false;
}

Node* List::find(int value, int* indexPointer) { // regresa un valor y asigna memoria a indexPointer

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

    if (index < 0 || index >= size) {
        std::cerr << "update: Indice no valido\n";
        return;
    }

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
    lista1.showList();

    lista1.insertLast(4);
    lista1.insertAtIndex(43, 1);
    lista1.showList();

    lista1.deleteFirst();
    lista1.deleteLast();
    lista1.showList();

    lista1.deleteAtIndex(2);
    lista1.showList();

    lista1.update(88, 1);
    lista1.showList();

    int index;
    Node* example = lista1.find(25, &index);

    return 0;
}