// Rogelio Villarreal A00838563
// Act 2.2 - Pilas, filas y listas enlazadas

#include <iostream>
#include <sstream>
#include <cstring>
#include <string>

using namespace std;

// Definimos un nodo para usarlo en las listas dobles (queue, pqueue)
struct DListNode
{
    int data;
    DListNode *next;
    DListNode *prev;
};

// Y un nodo para listas simples (stack)
struct ListNode
{
    int data;
    ListNode *next;
};

// Stack ahora usando el enlace simple
class Stack
{
private:
    int size;      // Num elementos en pila
    ListNode *top; // Puntero al nodo más arriba

public:
    Stack()
    {
        size = 0;      // Tamaño inicial
        top = nullptr; // Empezamos como una pila vacia con nullpointer
    }

    ~Stack()
    {
        while (!isEmpty())
        {
            pop(); // Metodo para liberar valores de los nodos
        }
    }

    bool push(int valor); // Agregar
    int pop();            // Restar
    int getSize();        // Obtener tamaño
    int getTop();         // Obtener el elemento tope
    bool isEmpty();       // Checar si pila esta vacia
    void show();          // Mostrar todo
};

// Agregar un elemento x al tope
bool Stack::push(int valor)
{
    ListNode *newNode = new ListNode(); // Nuevo nodo
    newNode->data = valor;              // Asignar input
    newNode->next = top;
    top = newNode; // El nuevo nodo apunta al top ahora y top se convierte en el nuevo nodo
    size++;
    return true;
}

// Saca el elemento en la cima
int Stack::pop()
{
    if (!isEmpty())
    {
        int val = top->data;  // Obtener el tope como valor
        ListNode *temp = top; // Almacenar el nodo temporalmente
        top = top->next;
        delete temp;
        size--; // Borramos el nodo, reducimos tamaño y regresamos valor
        return val;
    }
    else
    {
        cout << "Pila vacia, no se puede quitar valor" << endl;
        return -1;
    }
}

bool Stack::isEmpty()
{
    return top == nullptr; // Si es nullpointer no se ha iniciado
}

int Stack::getSize()
{
    return size;
}

int Stack::getTop()
{
    if (!isEmpty())
    {
        return top->data; // Nos da el valor tope
    }
    else
    {
        cout << "Pila vacia, no existe valor" << endl
             << endl;
        return -1;
    }
}

// Muestra los elementos que guardamos hasta terminarlos
void Stack::show()
{
    ListNode *current = top;
    while (current != nullptr)
    {
        cout << "Stack: " << current->data << endl;
        current = current->next;
    }
}

class Queue
{
private:
    DListNode *front;
    DListNode *end;
    int size; // Implementacion doble

public:
    Queue()
    {
        front = nullptr;
        end = nullptr; // Ambos son null al inicio
        size = 0;      // Al igual que el tamaño
    }

    ~Queue()
    {
        while (!isEmpty())
        {
            dequeue(); // Memoria
        }
    }

    bool enqueue(int value); // agregar elemento al final de la cola
    int dequeue();           // quitar elemento al frente de la cola
    int getFront();
    bool isEmpty();
    int getSize();
    void show();
};

bool Queue::enqueue(int value)
{
    DListNode *newNode = new DListNode(); // nuevo nodo, se asigna el valor
    newNode->data = value;
    newNode->next = nullptr; // no tiene siguiente
    newNode->prev = end;     // apunta al antiguo ultimo

    if (isEmpty())
    {
        front = newNode; // si la cola esta vacia front se convierte en nuevo
    }
    else
    {
        end->next = newNode;
    }

    end = newNode; // el nuevo nodo ahora es el ultimo
    size++;
    return true;
}

int Queue::dequeue()
{
    if (!isEmpty())
    {
        int val = front->data; // obtener valor enfrente, guardarlo en temp
        DListNode *temp = front;
        front = front->next;

        if (front != nullptr)
        {
            front->prev = nullptr; // no tiene anterior el nuevo frente
        }
        else
        {
            end = nullptr; // si esta vacia entonces el final tambien
        }

        delete temp; // liberar memoria
        size--;
        return val;
    }
    else
    {
        cout << "Cola vacia, no se puede quitar" << endl;
        return -1;
    }
}

int Queue::getFront()
{
    if (!isEmpty())
    {
        return front->data; // Regresa el valor del frente
    }
    else
    {
        cout << "No existe frente, la cola esta vacia" << endl;
        return -1;
    }
}

bool Queue::isEmpty()
{
    return size == 0; // True or false si el tamaño es 0
    // Es posible hacerlo con nullptr como lo usamos en otras funciones
}

// No hay isFull

int Queue::getSize()
{
    return size;
}

void Queue::show()
{
    DListNode *current = front;
    while (current != nullptr)
    {
        cout << "Queue: " << current->data << endl;
        current = current->next;
    }
}

// Priorityqueue es muy similar a queue
class PriorityQueue
{
private:
    DListNode *front; // Implementacion doble
    DListNode *end;   // front y end en este caso usan prioridad
    int size;

public:
    PriorityQueue()
    {
        front = nullptr;
        end = nullptr;
        size = 0;
    }

    ~PriorityQueue()
    {
        while (!isEmpty())
        {
            dequeue();
        }
    }

    bool enqueue(int value);
    int dequeue();
    int getFront();
    bool isEmpty();
    int getSize();
    void show();
};

// basando en prioridad
bool PriorityQueue::enqueue(int value)
{
    DListNode *newNode = new DListNode(); // Nuevo nodo
    newNode->data = value;
    newNode->next = nullptr;
    newNode->prev = nullptr; // No hay next ni prev

    // si la cola esta vacia el frente y atras es newnode
    if (isEmpty())
    {
        front = newNode;
        end = newNode;
    }
    else
    {
        // Insertar a base de prioridad
        DListNode *current = front;
        while (current != nullptr && current->data <= value)
        {
            current = current->next;
        }

        // Front
        if (current == front)
        {
            newNode->next = front;
            front->prev = newNode;
            front = newNode;
        }
        // End
        else if (current == nullptr)
        {
            newNode->prev = end;
            end->next = newNode;
            end = newNode;
        }
        // Middle
        else
        {
            newNode->next = current;
            newNode->prev = current->prev;
            current->prev->next = newNode;
            current->prev = newNode;
        }
    }

    size++;
    return true;
}

int PriorityQueue::dequeue() // Mismo que queue normal
{
    if (!isEmpty())
    {
        int val = front->data;
        DListNode *temp = front;
        front = front->next;

        if (front != nullptr)
        {
            front->prev = nullptr;
        }
        else
        {
            end = nullptr;
        }

        delete temp;
        size--;
        return val;
    }
    else
    {
        cout << "Cola vacia, no se puede quitar." << endl;
        return -1;
    }
}

int PriorityQueue::getFront() // Mismo que queue normal
{
    if (!isEmpty())
    {
        return front->data;
    }
    else
    {
        cout << "Cola vacia, no existe" << endl;
        return -1;
    }
}

bool PriorityQueue::isEmpty()
{
    return size == 0;
}

int PriorityQueue::getSize()
{
    return size;
}

// Mostrar elementos de Pqueue
void PriorityQueue::show()
{
    DListNode *current = front;
    while (current != nullptr)
    {
        cout << "Pqueue: " << current->data << endl;
        current = current->next;
    }
}

int main()
{
    // STACK
    cout << " -- Stack TEST -- " << endl
         << endl;

    Stack *stack = new Stack();

    stack->push(3);
    stack->push(5);
    stack->push(6);
    stack->push(8);
    stack->show();

    cout << endl;
    cout << "Size of Stack -> " << stack->getSize() << endl;

    cout << "Top of Stack -> " << stack->getTop() << endl
         << endl;

    int val = stack->pop();
    cout << "Out: " << val << endl;

    stack->show();
    cout << "Size of Stack -> " << stack->getSize() << endl;

    // QUEUE
    cout
        << " -- QUEUE TEST -- " << endl
        << endl;

    Queue *queue = new Queue();

    queue->enqueue(6);
    queue->enqueue(9);
    queue->enqueue(8);
    queue->enqueue(1);

    queue->show();

    cout << endl;
    cout << "Size of Queue -> " << queue->getSize() << endl;
    cout << "Front of Queue -> " << queue->getFront() << endl
         << endl;

    int valQueue = queue->dequeue();
    cout << "Out:" << valQueue << endl
         << endl;

    queue->show();
    cout << "Queue size: " << queue->getSize() << endl
         << endl;

    // PRIORITY QUEUE
    cout << " -- PRIORITY QUEUE TEST -- " << endl
         << endl;
    PriorityQueue *pq = new PriorityQueue();
    pq->enqueue(8);
    pq->enqueue(1);
    pq->enqueue(49);
    pq->enqueue(5);

    pq->show();

    cout << endl;
    cout << "Pqueue Size ->" << pq->getSize() << endl;
    cout << "Front of Pqueue -> " << pq->getFront() << endl
         << endl;

    int valPQ = pq->dequeue();
    cout << "Out: " << valPQ << endl
         << endl;

    pq->show();
    cout << "Pqueue Size: " << pq->getSize() << endl;

    return 0;
}