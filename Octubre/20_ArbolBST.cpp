#include <iostream>
#include <string>

// Node (simple node)
class tNode {
public:
    int data;
    tNode* left, * right, * up;

    tNode(int newValue) {
        data = newValue;
        left = right = up = nullptr;
    }
};

class Node { // 
public:
    tNode* data;
    Node* next; // siguiente nodo en la cola

    Node(tNode* newValue) {
        data = newValue;
        next = nullptr;
    }
};

class Queue {

private:
    Node* front;
    Node* end;

public:
    Queue() { front = end = nullptr; } // Constructor default

    bool isEmpty() { return front == nullptr; }

    void enqueue(tNode* newValue) {

        Node* newNode = new Node(newValue);
        if (end == nullptr) {
            front = end = newNode;
        }
        else {
            end->next = newNode;
            end = newNode;
        }
    }

    tNode* dequeue() {
        if (isEmpty()) { return nullptr; }

        Node* temp = front; // Empezamos a quitar el node de enfrente
        front = front->next;

        if (front == nullptr) { end = nullptr; } // si front se vuelve null, no quedan elementos

        tNode* node = temp->data; // nodo a regresar esta en formato tree
        delete temp;
        return node;
    }
};

class Stack {
private:
    Node* topNode;

public:
    Stack() { topNode = nullptr; }
    bool isEmpty() { return topNode == nullptr; }
    void push(tNode* treeNode) {
        Node* newNode = new Node(treeNode);
        newNode->next = topNode;
        topNode = newNode;
    }

    void pop() {
        if (isEmpty()) { return; }

        Node* temp = topNode;
        topNode = topNode->next;
        delete temp;
    }

    tNode* getTop() {
        if (isEmpty()) { return nullptr; }
        else { return topNode->data; }
    }
};

class BST {
public:
    tNode* root;

    BST(int rootValue) { root = new tNode(rootValue); }

    tNode* search(int value);
    void insert(int newValue);
    void remove(int value);
    void showTree();

    // Recorridos
    void preOrder();
    void inOrder();
    void postOrder();
    void levelOrder();
    void dfs();

private:
    void preOrder(tNode* node);
    void inOrder(tNode* node);
    void postOrder(tNode* node);
};

tNode* BST::search(int value) {
    tNode* aux = root;

    while (aux != nullptr) {
        if (aux->data < value) { aux = aux->right; }
        else if (aux->data > value) { aux = aux->left; }
        else {
            std::cout << "[search] Encontrado: " << aux->data << std::endl;
            return aux;
        }
    }
    std::cout << "[search] No encontrado: " << value << std::endl;
    return nullptr;
}

void BST::insert(int newValue) {
    if (search(newValue) != nullptr) {
        std::cout << "[insert] Ya existe el valor: " << newValue << "\n";
        return;
    }

    tNode* newNode = new tNode(newValue);
    tNode* aux = root;

    while (aux != nullptr) {
        if (newValue < aux->data) { // compara valor con el nodo aux

            if (aux->left == nullptr) { // revisa si el nodo nuevo puede ser hijo izquierdo de aux;
                aux->left = newNode; // EXITO >> Enlaza ambos nodos
                newNode->up = aux;
                std::cout << "[insert] Insertado: " << newValue << "\n";
                return;
            }
            else { aux = aux->left; } // FRACASO >> pasamos al siguiente nivel y repetimos
        }
        else { // mismo proceso pero para la derecha
            if (aux->right == nullptr) { // EXITO >> Enlaza ambos nodos
                aux->right = newNode;
                newNode->up = aux;
                std::cout << "[insert] Insertado: " << newValue << "\n";
                return;
            }
            else { aux = aux->right; } // FRACASO >> pasamos al siguiente nivel y repetimos
        }
    }
}

void BST::remove(int removeValue) {

    tNode* aux = search(removeValue);
    if (aux == nullptr) {
        std::cout << "[remove] No existe el valor " << removeValue << "\n";
        return;
    }

    // Caso donde solo hay un nodo en el arbol
    if (aux->left == nullptr && aux->right == nullptr && aux->up == nullptr) {
        delete aux;
    }

    // Caso donde el nodo a borrar no tiene hijos
    if (aux->left == nullptr && aux->right == nullptr) { // Caso donde nodo es hoja
        tNode* parent = aux->up;
        //checa si borramos del parent el pointer izq o rerecho
        if (aux->data > parent->data) { parent->right = nullptr; }
        else { parent->left = nullptr; }
        std::cout << "[remove] Borrado: " << removeValue << "\n";
        delete aux;
    }

    // Caso donde el nodo a borrar tiene un hijo
    else if (aux->left == nullptr || aux->right == nullptr) {
        tNode* parent = aux->up;

        if (aux->left != nullptr) { // si el nodo hijo esta en el lado derecho
            // el parent de aux adopta el hijo izq de aux. como parent > aux > hijo, es linea recta y solo se borra.
            if (aux->data < parent->data) { parent->left = aux->left; }
            // el parent de aux adopta el hijo der de aux. En este caso: aux > hijo > parent.
            // Aunque esta a la izquierda de aux, es mayor que parent, por ende va a la derecha.
            else { parent->right = aux->left; }
            aux->left->up = parent; // enlazamos hijo con parent.
        }

        else {
            if (aux->data < parent->data) { parent->left = aux->right; }
            else { parent->right = aux->right; }
            aux->right->up = parent;
        }
        std::cout << "[remove] Borrado: " << removeValue << "\n";
        delete aux;
    }

    // Caso donde nodo tiene dos hijos
    else {
        tNode* temp = aux->right; // Encontrar sucesor mediante inorden
        // Leftmost node from right subtree of given node
        while (temp->left != nullptr) { temp = temp->left; }

        aux->data = temp->data;

        tNode* parent = temp->up;
        // Eliminar sucesor (nunca tiene dos hijos por no poder ir a la izq)
        if (temp->right != nullptr) {
            if (parent->left == temp) { parent->left = temp->right; } // zig "/ \"
            else { parent->right = temp->right; } // linea "\ \"

            temp->right->up = parent;
        }
        else {
            if (parent->left == temp) { parent->left = nullptr; }
            else { parent->right = nullptr; }
        }
        std::cout << "[remove] Borrado: " << removeValue << "\n";
        delete temp;
    }
}

void BST::preOrder() {
    std::cout << "preOrder: ";
    preOrder(root);
    std::cout << "\n";
}

void BST::preOrder(tNode* node) {
    if (node == nullptr) { return; }

    std::cout << node->data << "(P:";
    // Imprime si existe el node padre, si no, imprime nullptr
    if (node->up != nullptr) { std::cout << node->up->data; }
    else { std::cout << "nullptr"; }
    std::cout << ") ";

    preOrder(node->left); // Llamadas recursivas
    preOrder(node->right);
}

void BST::inOrder() {
    std::cout << "inOrder: ";
    inOrder(root);
    std::cout << "\n";
}

void BST::inOrder(tNode* node) {
    if (node == nullptr) { return; }

    inOrder(node->left);

    std::cout << node->data << "(P:";

    if (node->up != nullptr) { std::cout << node->up->data; }
    else { std::cout << "nullptr"; }
    std::cout << ") ";

    inOrder(node->right);
}

void BST::postOrder() {
    std::cout << "postOrder: ";
    postOrder(root);
    std::cout << "\n";
}

void BST::postOrder(tNode* node) {
    if (node == nullptr) { return; }

    postOrder(node->left);
    postOrder(node->right);

    std::cout << node->data << "(P:";

    if (node->up != nullptr) { std::cout << node->up->data; }
    else { std::cout << "nullptr"; }
    std::cout << ") ";
}

void BST::levelOrder() {
    if (root == nullptr) { return; }

    Queue q;
    q.enqueue(root);

    std::cout << "levelOrder: ";

    while (!q.isEmpty()) {
        tNode* current = q.dequeue();

        std::cout << current->data << "(P:";
        if (current->up != nullptr) { std::cout << current->up->data; }
        else { std::cout << "nullptr"; }
        std::cout << ") ";

        if (current->left != nullptr) { q.enqueue(current->left); }
        if (current->right != nullptr) { q.enqueue(current->right); }
    }
    std::cout << "\n";
}

void BST::dfs() {
    if (root == nullptr) { return; }

    Stack s;
    s.push(root);

    std::cout << "DFS: ";

    while (!s.isEmpty()) {
        tNode* current = s.getTop();
        s.pop();

        std::cout << current->data << "(P:";
        if (current->up != nullptr) { std::cout << current->up->data; }
        else { std::cout << "nullptr"; }
        std::cout << ") ";

        if (current->right != nullptr) { s.push(current->right); }
        if (current->left != nullptr) { s.push(current->left); }
    }
    std::cout << "\n";
}

int main() {
    BST* bst = new BST(25);

    std::cout << "Busqueda\n";
    tNode* aux = bst->search(25);
    aux = bst->search(30);

    std::cout << "\nInsertar\n";
    bst->insert(20);
    bst->insert(34);
    bst->insert(2);
    bst->insert(22);
    bst->insert(30);
    bst->insert(40);
    bst->insert(26);
    bst->insert(31);
    bst->insert(32);
    bst->insert(45);
    bst->insert(39);
    std::cout << "\n";

    bst->preOrder();
    std::cout << "\n";

    bst->remove(34);
    bst->preOrder();
    std::cout << "\n";

    bst->remove(32);
    bst->preOrder();
    std::cout << "\n";

    bst->remove(39);
    bst->preOrder();
    std::cout << "\n";

    bst->remove(40);
    bst->preOrder();
    std::cout << "\n";

    std::cout << "Orders\n";
    bst->preOrder();
    bst->inOrder();
    bst->postOrder();
    bst->levelOrder();
    bst->dfs();
}