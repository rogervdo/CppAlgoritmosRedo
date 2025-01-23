#include <iostream>
#include <string>

struct dNode {
    int data;
    dNode* next;
    dNode* prev;

    dNode(int value) {
        data = value;
        next = prev = nullptr;
    }
};

struct sNode {
    int data;
    sNode* next;

    sNode(int value) {
        data = value;
        next = nullptr;
    }
};

class Stack {
private:
    int size;
    sNode* top;

public:
    Stack() {
        size = 0;
        top = nullptr;
    }

    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }

    bool push(int newValue);
    int pop();
    int getSize() { return size; };
    int getTop();
    bool isEmpty() { return top == nullptr; };
    void show();
};

bool Stack::push(int newValue) {
    sNode* newNode = new sNode(newValue);

    newNode->next = top; // el nuevo nodo apunta al top que será reemplazado
    top = newNode; // top se reemplaza por nuevo nodo

    size++; // queda | newNode(top nuevo) next-> top viejo -> etc. | 
    return true;
}

int Stack::pop() {
    if (isEmpty()) {
        std::cout << "Pila vacia, no se puede quitar valor\n";
        return -1;
    }

    int popData = top->data; // Extraemos dato
    sNode* temp = top; // temp para borrar top
    top = top->next; // asignamos el tag de top al que sigue
    delete temp;

    size--;
    return popData;
}

int Stack::getTop() {
    if (isEmpty()) {
        std::cout << "Pila vacia, no existe valor\n\n";
        return -1;
    }
    return top->data;
}

void Stack::show() {
    sNode* n = top;
    while (n != nullptr) {
        std::cout << "Stack: " << n->data << "\n";
        n = n->next;
    }
}

class Queue {
private:
    dNode* front;
    dNode* end;
    int size;

public:
    Queue() {
        front = nullptr;
        end = nullptr;
        size = 0;
    }

    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    bool enqueue(int newValue);
    int dequeue();
    int getFront();
    bool isEmpty() { return front == nullptr; };
    int getSize() { return size; };
    void show();
};

bool Queue::enqueue(int newValue) { // queue front es prevmost, end es nextmost
    dNode* newNode = new dNode(newValue);
    newNode->prev = end; // anterior es el viejo end

    if (isEmpty()) { front = newNode; } // Sin nodos previos
    else { end->next = newNode; } // Un nodo o mas > end viejo apunta a nuevo end

    end = newNode; // cambia pointer de end a newNode
    size++;
    return true;
}

int Queue::dequeue() {
    if (isEmpty()) {
        std::cout << "Cola vacia, no se puede quitar\n";
        return -1;
    }

    int dequeueData = front->data;
    dNode* temp = front; // valor que se borrará
    front = front->next;

    if (front != nullptr) { // front no tiene prev 
        front->prev = nullptr;
    }
    else {
        end = nullptr; // si si esta vacia entonces end = nullptr
    }

    delete temp; // liberar memoria
    size--;
    return dequeueData;
}

int Queue::getFront() {
    if (isEmpty()) {
        std::cout << "No existe frente, la cola esta vacia\n";
        return -1;
    }
    return front->data;
}

void Queue::show() {
    dNode* n = front;
    while (n != nullptr) {
        std::cout << "Queue: " << n->data << "\n";
        n = n->next;
    }
}

int main() {
    std::cout << "Stack Section\n";

    Stack* stack1 = new Stack();

    stack1->push(3);
    stack1->push(5);
    stack1->push(6);
    stack1->push(8);
    stack1->show();

    std::cout << "\nSize of Stack >> " << stack1->getSize();
    std::cout << "\nTop of Stack >> " << stack1->getTop();
    std::cout << "\n\nOut: " << stack1->pop() << "\n";

    stack1->show();
    std::cout << "\nSize of Stack >> " << stack1->getSize() << std::endl;

    std::cout << "Queue Section\n";
    Queue* queue = new Queue();

    queue->enqueue(6);
    queue->enqueue(9);
    queue->enqueue(8);
    queue->enqueue(1);

    queue->show();
    std::cout << "\nSize of Queue >> " << queue->getSize();
    std::cout << "\nFront of Queue >> " << queue->getFront();
    std::cout << "\nOut: " << queue->dequeue();

    std::cout << "\nQueue size: " << queue->getSize() << "\n";
    queue->show();
}