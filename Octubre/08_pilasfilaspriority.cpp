#include <iostream>
#include <string>

struct dNode {
    int data;
    dNode* next;
    dNode* prev;
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
    std::cout << "\n\nOut: >> " << stack1->pop() << "\n";

    stack1->show();
    std::cout << "\nSize of Stack >> " << stack1->getSize() << std::endl;
}