// g++ listaEnlazadaEj.cpp -o listaEnlazadaDoble; ./listaEnlazadaDoble

#include <iostream>
#include <sstream>
#include <cstring>
#include <string>

using namespace std;

// class node: un valor y un puntero a otro nodo
template <class T> //	Esta linea permite que podamos crear listas de cualquier tipo de dato
class Node
{
public:
    //	Atributos del nodo

    // int value;		// En lugar de que el nodo guarde un entero, guarda algo de tipo T
    T value; // valor almacenado

    Node<T> *prev; //	dir del nodo anterior
    Node<T> *next; //	dir del nodo siguiente

    //	Que deberia recibir el contructor de la clase?
    //		un valor para guardar en el nodo
    //	Que valores por default deberian tener prev y next?
    Node(T valor)
    {
        this->value = valor;
        this->prev = NULL;
        this->next = NULL;
    }
};

//  Clase lista enlazada doble:
template <class T>
class List
{ //	Que atributos necesita??

    Node<T> *first; //	puntero al primer nodo
    Node<T> *last;  //	puntero al ultimo nodo
    int size;       //	numero de elementos que tiene la lista

public:
    //	Que valores deberia tener la lista por default??
    List()
    {
        this->first = NULL;
        this->last = NULL;
        this->size = 0;
    }

    //	Funciones similares a las de la lista simple

    int getSize() { return size; } //	Funcion para obtener el tamanaño de la lista
    void showList();               //	Funcion para mostrar la lista
    void showListReverse();        //	Funcion para mostrar la lista en reversa

    //	Funciones para insertar elementos
    void insertFirst(T);        //	al principio
    void insertLast(T);         //	al final
    bool insertAtIndex(int, T); //	en un indice

    //	Funciones para eliminar elementos
    void deleteFirst();      //	al principio
    void deleteLast();       //	al final
    void deleteAtIndex(int); //	en un indice

    Node<T> *find(T, int *); //	Encontrar un valor
    void update(int, T);     //	Actualizar el valor de un indice
};
/**/
// inserta en un indice especifico
template <class T>
bool List<T>::insertAtIndex(int index, T newValue)
{
    Node<T> *nuevo = new Node<T>(newValue);

    //	insertar al inicio
    if (index == 0)
    {
        this->insertFirst(newValue);
        return true;
    }

    if (index == this->size)
    {
        this->insertLast(newValue);
        return true;
    }

    if (index < size / 2) //	cuando es mas barato insertar desde el inicio
    {
        Node<T> *aux = first;
        int i = 0;

        while (i < size / 2)
        {
            if (i == index - 1)
            {
                nuevo->prev = aux;
                nuevo->next = aux->next;

                nuevo->next->prev = nuevo;
                aux->next = nuevo;
                this->size++;
                return true;
            }

            aux = aux->next;
            i++;
        }
    }
    else //	cuando es mas barato insertar desde el final
    {
        Node<T> *aux = last;
        int i = size - 1;

        while (i >= size / 2)
        {
            if (i == index - 1)
            {
                nuevo->next = aux->next;
                nuevo->prev = aux;

                if (aux->next != NULL)
                {
                    aux->next->prev = nuevo;
                }
                aux->next = nuevo;
                this->size++;
                return true;
            }

            aux - aux->prev;
            i--;
        }
    }
    // Si el índice esta fuera de rango y no lo podemos insertar regresamos false
    return false;
}

template <class T>
Node<T> *List<T>::find(T value, int *index)
{
    Node<T> *aux = this->first;
    int i = 0;

    // Buscamos el valor en la lista
    while (i < this->size)
    {
        if (aux->value == value)
        {
            *index = i; // Actualizamos el índice al encontrar el valor
            // Imprimimos la direccion, indice, valor
            cout << "direccion:" << aux << " index: " << *index << " value: " << aux->value << "\n"
                 << endl;
            return aux;
        }

        aux = aux->next;
        i++;
    }

    // Si no se pudo encontrar el valor regresa NULL y dejams indice en -1
    *index = -1;
    cout << "value " << value << "no encontrado" << endl;
    return NULL;
}

template <class T>
void List<T>::update(int index, T newValue)
{
    // Checar si el indice es valido
    if (index < 0 || index >= this->size)
    {
        cout << "Indice invalido, no se puede actualizar" << endl;
        return;
    }

    // Creamos un nodo auxiliar
    Node<T> *aux;
    // Recorrer desde el inicio o el final dependiendo donde esta el índice
    if (index < size / 2)
    {
        int i = 0;
        aux = first;
        while (i < index)
        {
            aux = aux->next;
            i++;
        }
    }
    else
    {
        int i = size - 1;
        aux = last;
        while (i > index)
        {
            aux = aux->prev;
            i--;
        }
    }

    // Actualizar el valor del nodo en indice
    cout << "Actualizando nodo en indice " << index << " de valor: " << aux->value << " a " << newValue << endl;
}

template <class T>
void List<T>::deleteFirst()
{
    if (size == 0)
    {
        cout << "No puedes borrar una lista vacia" << endl;
        return;
    }
    //  Crear un  auxiliar que guarde la direccion de first
    Node<T> *aux = this->first;

    //	Crear una puntero, llamado segundo, que tome la direccion del nodo siguiente de first
    Node<T> *segundo = aux->next;
    //  Asigna que el anterior a segundo ahora es last
    segundo->prev = last;
    //  Y que el siguiente de last es segundo
    last->next = segundo;

    // Elimina aux con un delete
    delete aux;

    // Ahora first es segundo
    first = segundo;

    // Decrementa el tamaño
    size--;
}

template <class T>
void List<T>::deleteLast()
{
    if (size == 0)
    {
        cout << "No puedes borrar una lista vacia" << endl;
        return;
    }

    // Nodo auxiliar guarda direccion de last
    Node<T> *aux = this->last;

    // Asignar last al penúltimo nodo
    this->last = aux->prev;
    // Ajustar los punteros para hacer la lista circular
    this->last->next = this->first;
    this->first->prev = this->last;

    // Eliminar antiguo ultimo nodo
    delete aux;

    // Decrementar tamaño
    size--;
}

template <class T>
void List<T>::deleteAtIndex(int index)
{
    if (size == 0)
    {
        cout << "No puedes borrar una lista vacia" << endl;
        return;
    }
    // Checar si el indice es valido
    if (index < 0 || index >= this->size)
    {
        cout << "Indice invalido, no se puede actualizar" << endl;
        return;
    }

    // Eliminar el primer nodo si el índice es 0
    if (index == 0)
    {
        deleteFirst();
        return;
    }

    // Eliminar el último nodo si el índice es igual al ultimo (size-1)
    if (index == size - 1)
    {
        deleteLast();
        return;
    }

    // Nodo auxiliar
    Node<T> *aux;

    // Recorrer desde el inicio o el final dependiendo donde esta el índice
    if (index < size / 2)
    {
        int i = 0;
        aux = first;
        while (i < index)
        {
            aux = aux->next;
            i++;
        }
    }
    else
    {
        int i = size - 1;
        aux = last;
        while (i > index)
        {
            aux = aux->prev;
            i--;
        }
    }

    // Ajustar punteros para eliminar nodo
    aux->prev->next = aux->next;
    aux->next->prev = aux->prev;

    // Eliminar el nodo
    delete aux;

    // Decrementar el tamaño de la lista
    size--;
}

// inserta al inicioA
template <class T>
void List<T>::insertFirst(T newValue)
{ // Crear un nodo nuevo
  // Sintaxis:
  // Clase<plantilla> *nombre = new Clase<plantilla>(parametros)
    Node<T> *nuevo = new Node<T>(newValue);
    // Crear un puntero auxiliar que guarde la direccion de first
    Node<T> *aux = this->first;

    // Hacer que el siguiente del nodo nuevo sea el auxiliar
    // Y que el first ahora sea el nodo nuevo
    nuevo->next = aux;
    first = nuevo;

    // Si la lista esta vacia
    if (this->size == 0)
    //	el ultimo es tambien el nodo nuevo
    {
        last = nuevo;
    }
    // Si no,
    else
    // el anterior a aux (viejo first) es ahora el nodo nuevo
    {
        aux->prev = nuevo;
    }
    // Para asegurar que la lista es circular
    // El anterior a first es last
    // El siguiente de last es firs
    first->prev = last;
    last->next = first;

    //	Finalmente actualizar el tamaño de la lista
    size++;
}

// inserta al final
template <class T>
void List<T>::insertLast(T newValue)
{ // Crear un nodo nuevo
  // Sintaxis:
  // Clase<plantilla> *nombre = new Clase<plantilla>(parametros)
    Node<T> *nuevo = new Node<T>(newValue);

    // Crear un puntero auxiliar que guarde la direccion de last
    Node<T> *aux = this->last;

    // Hacer que el anterior del nodo nuevo sea el auxiliar (viejo last)
    // Y que el nuevo last ahora sea el nodo nuevo
    nuevo->prev = aux;
    last = nuevo;

    // Si la lista esta vacia
    if (this->size == 0)
    //	el first es tambien el nodo nuevo
    {
        first = nuevo;
    }
    // Si no,
    else
    // el siguiente a aux (viejo last) es ahora el nodo nuevo
    {
        aux->next = nuevo;
    }

    // Para asegurar que la lista es circular
    // El anterior a first es last
    // El siguiente de last es firsT
    first->prev = last;
    last->next = first;

    //	Finalmente actualizar el tamaño de la lista
    this->size++;
}

template <class T>
void List<T>::showList()
{
    cout << endl
         << "Mostrar lista" << endl;
    // Crea un nodo auxiliar para iterar en la lista
    // auxiliar inicia en first
    Node<T> *aux = this->first;

    // Declara un contador i que inicie en 0
    int i = 0;

    // Imprime el tamaño de la lista
    cout << "Tamaño lista: \t" << this->size << endl;

    // Mientras i sea menor que el tamaño...
    while (i < this->size)
    { // Imprime el nodo
        cout << "\t[" << i << "]:\t" << aux->value << endl;
        // aux avanza a aux->next
        // i incrementa
        aux = aux->next;
        i++;
    }
    cout << endl;
}

template <class T>
void List<T>::showListReverse()
{
    cout << endl
         << "Mostrar lista reves" << endl;
    // Nodo auxiliar para iterar desde el final
    Node<T> *aux = this->last;

    // Contador que inicia desde el final
    int i = this->size - 1;

    // Imprime el tamaño de la lista
    cout << "Tamaño lista: \t " << this->size << endl;

    // Mientras i sea mayor o igual a 0
    while (i >= 0)
    {
        // Imprime el valor del nodo
        cout << "\t[" << i << "]:\t" << aux->value << endl;

        // aux pasa a aux->prev
        aux = aux->prev;

        // Bajar contador
        i--;
    }
    cout << endl;
}

int main(int argc, char *argv[])
{
    List<int> dList;

    cout << "\n -- Insert First -- \n " << endl;

    dList.insertFirst(9);
    dList.showList();
    dList.insertFirst(8);
    dList.showList();
    dList.insertFirst(9);
    dList.showList();
    dList.insertFirst(8);
    dList.showList();
    dList.insertFirst(9);
    dList.showList();
    dList.insertFirst(8);
    dList.showList();

    cout << "\n -- Insert Last --\n " << endl;

    dList.insertLast(5);
    dList.showList();
    dList.showListReverse();
    dList.insertLast(4);
    dList.showList();
    dList.showListReverse();

    cout << "\n -- Insert at index --\n " << endl;

    dList.insertAtIndex(3, 4);
    dList.showList();
    dList.insertAtIndex(1, 2);
    dList.showList();

    cout << "\n -- Delete First --\n " << endl;

    dList.deleteFirst();
    dList.showList();
    dList.deleteFirst();
    dList.showList();

    cout << "\n -- Delete Last --\n " << endl;

    dList.deleteLast();
    dList.showList();
    dList.deleteLast();
    dList.showList();

    cout << "\n -- Delete at index --\n " << endl;

    dList.deleteAtIndex(1);
    dList.showList();
    dList.deleteAtIndex(1);
    dList.showList();

    cout << "\n -- Find value --:\n " << endl;

    int index = -1;
    Node<int> *address = dList.find(8, &index);

    cout << "\n -- Update value at index --\n " << endl;
    dList.update(2, 2);
    dList.showList();

    cout << "\n -- Update at invalid index --\n " << endl;
    dList.update(10, 50);
    dList.showList();
}