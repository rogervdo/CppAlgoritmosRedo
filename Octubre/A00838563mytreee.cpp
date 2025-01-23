// Rogelio Jesús Villarreal De Ochoa A00838563 //
#include <iostream>
#include <sstream>
#include <cstring>
#include <string>

using namespace std;

// Atributos para el nodo: punteros a izquierda y derecha
// El puntero hacia arriba(padre) es opcional
class NodeTree
{
public:
    int data;
    NodeTree *left, *right, *up;

    NodeTree(int value) // Constructor de la clase
    {
        data = value;
        left = NULL;
        right = NULL;
        up = NULL;
    }
};
// ---------------------------------------------------- // ---------------------------------------------------- //
// Nodo para la cola
class QueueNode
{
public:
    NodeTree *data;
    QueueNode *next; // Siguiente nodo en la cola.

    // Constructor
    QueueNode(NodeTree *value)
    {
        data = value;
        next = NULL;
    }
};

// Implementación de queue para levelorder
class Queue
{
private:
    QueueNode *front;
    QueueNode *rear;

public:
    Queue() // Constructor del queue
    {
        front = NULL;
        rear = NULL;
    }

    bool isEmpty() // Metodo queue vacio
    {
        return front == NULL;
    }

    void enqueue(NodeTree *value)
    {
        QueueNode *newNode = new QueueNode(value);
        if (rear == NULL) // Check nuevo
            front = rear = newNode;

        else
        { // Agregar
            rear->next = newNode;
            rear = newNode;
        }
    }

    NodeTree *dequeue()
    {
        if (isEmpty())
            return NULL;

        // Quitar nodo enfrente.
        QueueNode *temp = front;
        front = front->next;

        // Si el frente se convierte en NULL, esta vacia y rear tambien.
        if (front == NULL)
            rear = NULL;

        // Obtener el valor del nodo
        NodeTree *node = temp->data;
        delete temp; // Liberar memoria
        return node;
    }
};
// ---------------------------------------------------- // ---------------------------------------------------- //
// Implementacion stack para dfs
class StackNode
{
public:
    NodeTree *data;
    StackNode *next; // Siguiente nodo en la cola.

    // Constructor
    StackNode(NodeTree *value)
    {
        data = value;
        next = NULL;
    }
};

class Stack
{
private:
    StackNode *topNode; // Puntero al nodo más arriba.

public:
    Stack() // Constructor
    {
        topNode = NULL;
    }

    bool isEmpty()
    {
        return topNode == NULL;
    }

    // Agregar un nodo al stack
    void push(NodeTree *node)
    {
        StackNode *newNode = new StackNode(node);
        newNode->next = topNode; // El nuevo nodo apunta al antiguo top
        topNode = newNode;       // El nuevo nodo se convierte en el top
    }

    // Quitar top (pop)
    void pop()
    {
        if (isEmpty())
            return;

        StackNode *temp = topNode;
        topNode = topNode->next; // Mueve el top al siguiente nodo
        delete temp;             // Libera la memoria del antiguo top
    }

    // Get top
    NodeTree *top()
    {
        if (!isEmpty())
            return topNode->data;

        return NULL;
    }
};
// ---------------------------------------------------- // ---------------------------------------------------- //
class BST
{
public:
    NodeTree *root; // Nodo raiz del arbol

    // Cuando creamos un arbol tiene nodo raiz.
    BST(int value)
    {
        root = new NodeTree(value);
    }

    NodeTree *search(int);
    void insert(int);
    void remove(int);
    void showTree();

    // Recorridos (Funciones default)
    void preorder();
    void inorder();
    void postorder();

    void levelorder();
    void dfs();

private: // El usuario no usa recorridos con argumentos
    void preorder(NodeTree *node);
    void inorder(NodeTree *node);
    void postorder(NodeTree *node);
};
// ---------------------------------------------------- // ---------------------------------------------------- //
NodeTree *BST::search(int value)
{ // Nodo aux para recorrer el arbol
    NodeTree *aux = this->root;

    while (aux != NULL)
    { // el valor buscado es menor que el del nodo actual??
        // desplazate al nodo de su izquierda
        if (aux->data < value)
        {
            aux = aux->right;
        }
        // el valor buscado es mayor que el del nodo actual??
        // desplazate al nodo de su derecha
        else if (aux->data > value)
        {
            aux = aux->left;
        }
        // si no es mayor, ni menor, debe ser igual...
        else
        {
            cout << "Encontrado " << aux->data << endl;
            return aux;
        }
    }
    // Que pasa en este punto??
    cout << "No encontrado " << value << endl;
    return NULL;
}
// ---------------------------------------------------- // ---------------------------------------------------- //
void BST::insert(int value)
{ // Comienza por verificar que el valor que
    // se quiere agregar no exista en el arbol...
    if (search(value) == NULL)
    { // Crea el nodo nuevo
        NodeTree *nuevo = new NodeTree(value);

        // Inicia un proceso similar al de la busqueda...
        // Necesitas encontrar a un nodo que pueda ser el padre del nuevo
        // Inicia desde la raiz...
        NodeTree *aux = this->root;

        // mientras aux no sea null...
        // agrega un return cuando consigas agregar el nodo nuevo...
        while (aux != NULL)
        { // revisa a que lado de aux deberia quedar el nodo nuevo
            // Va a la izquierda si es menor que el valor en aux
            // y a la derecha si no
            if (value < aux->data)
            { // Revisa si el nodo nuevo puede ser el hijo izquierdo de aux
                if (aux->left == NULL)
                {
                    aux->left = nuevo;
                    nuevo->up = aux;
                    break;
                }
                else
                { // si no, continua buscando un lugar a la izauierda de aux
                    aux = aux->left;
                }
            }
            else
            { // Mismo proceso, pero para la derecha
                if (aux->right == NULL)
                {
                    aux->right = nuevo;
                    nuevo->up = aux;
                    break;
                }
                else
                { // si no, continua buscando un lugar a la derecha de aux
                    aux = aux->right;
                }
            }
        }
    }
    else
    {
        cout << "Ya existe " << endl;
    }
}
// ---------------------------------------------------- // ---------------------------------------------------- //
void BST::remove(int value)
{
    NodeTree *aux = search(value);

    if (aux != NULL)
    {
        // Caso donde el nodo es una hoja
        if (aux->left == NULL && aux->right == NULL)
        {
            NodeTree *parent = aux->up;
            if (aux->data > parent->data)
            {
                parent->right = NULL;
            }
            else
            {
                parent->left = NULL;
            }
            delete aux;
        }
        // Caso donde el nodo tiene un solo hijo
        else if (aux->left == NULL || aux->right == NULL)
        {
            NodeTree *parent = aux->up;
            if (aux->left != NULL)
            {
                if (aux->data < parent->data)
                {
                    parent->left = aux->left;
                }
                else
                {
                    parent->right = aux->left;
                }

                aux->left->up = parent;
            }
            else
            {
                if (aux->data < parent->data)
                {
                    parent->left = aux->right;
                }
                else
                {
                    parent->right = aux->right;
                }

                aux->right->up = parent;
            }
            delete aux;
        }
        // Caso donde el nodo tiene dos hijos
        else
        {
            // Encontrar sucesor (inorden)
            NodeTree *successor = aux->right;
            while (successor->left != NULL)
                successor = successor->left;

            // Reemplazar valor eliminador por sucesor
            aux->data = successor->data;

            // Eliminar sucesor (nunca tiene dos hijos porque entonces no es el sucesor correcto)
            if (successor->right != NULL)
            {
                // Sucesor tiene hijo a la derecha
                if (successor->up->left == successor)
                    successor->up->left = successor->right;
                else
                    successor->up->right = successor->right;
                successor->right->up = successor->up;
            }
            else
            {
                // El sucesor no tiene hijos
                if (successor->up->left == successor)
                    successor->up->left = NULL;
                else
                    successor->up->right = NULL;
            }
            delete successor;
        }
    }
    else
        cout << "No existe el valor" << endl;
}
// ---------------------------------------------------- // ---------------------------------------------------- //
void BST::preorder()
{
    cout << "Preorden: ";
    preorder(this->root); // Funcion recursiva desde el root
    cout << endl;
}
void BST::preorder(NodeTree *node)
{
    if (node == NULL)
        return;

    // Imprimor nodo actual y el de su padre
    cout << node->data << "(P:";
    if (node->up != NULL)
    {
        cout << node->up->data;
    }
    else
    {
        cout << "NULL"; // Si no hay padre, entonces es la raíz.
    }
    cout << ") "; // Espacio para separar los nodos

    // Llama recursivamente al subárbol izquierdo
    preorder(node->left);

    // Llama recursivamente al subárbol derecho
    preorder(node->right);
}

// ---------------------------------------------------- // ---------------------------------------------------- //
void BST::inorder()
{
    cout << "Inorder: ";
    inorder(this->root); // Funcion recursiva desde el root
    cout << endl;
}
void BST::inorder(NodeTree *node)
{
    if (node == NULL)
        return;

    // Recorre subarbol izquierdo
    inorder(node->left);

    // Imprimor nodo actual y el de su padre
    cout << node->data << "(P:";
    if (node->up != NULL)
    {
        cout << node->up->data;
    }
    else
    {
        cout << "NULL"; // Si no hay padre, entonces es la raíz.
    }
    cout << ") "; // Espacio para separar los nodos

    // Recorre el subárbol derecho
    inorder(node->right);
}
// ---------------------------------------------------- // ---------------------------------------------------- //
void BST::postorder()
{
    cout << "Postorder: ";
    postorder(this->root);
    cout << endl;
}
void BST::postorder(NodeTree *node)
{
    if (node == NULL)
        return;

    // Recorre el subárbol izquierdo
    postorder(node->left);

    // Recorre el subárbol derecho
    postorder(node->right);

    // Imprimor nodo actual y el de su padre
    cout << node->data << "(P:";
    if (node->up != NULL)
    {
        cout << node->up->data;
    }
    else
    {
        cout << "NULL"; // Si no hay padre, entonces es la raíz.
    }
    cout << ") "; // Espacio para separar los nodos
}
// ---------------------------------------------------- // ---------------------------------------------------- //
void BST::levelorder()
{
    if (root == NULL)
        return;

    Queue q;         // Crear una instancia de la cola personalizada
    q.enqueue(root); // Iniciar el recorrido desde la raíz

    cout << "Level order: ";

    // Mientras la cola no esté vacía
    while (!q.isEmpty())
    {
        // Obtener el nodo al frente de la cola y removerlo
        NodeTree *current = q.dequeue();

        // Imprimir el valor del nodo actual y el valor de su padre
        cout << current->data << "(P:";
        if (current->up != NULL)
        {
            cout << current->up->data; // Imprimir el valor del padre si existe
        }
        else
        {
            cout << "NULL"; // Si no tiene padre, es la raíz
        }
        cout << ") "; // Espacio para separar los nodos

        // Encolar el hijo izquierdo si existe
        if (current->left != NULL)
        {
            q.enqueue(current->left);
        }

        // Encolar el hijo derecho si existe
        if (current->right != NULL)
        {
            q.enqueue(current->right);
        }
    }

    cout << endl; // Salto de línea para organizar la salida
}
// ---------------------------------------------------- // ---------------------------------------------------- //
void BST::dfs()
{
    if (root == NULL)
    {
        return; // Si el árbol está vacío, no hacemos nada
    }

    Stack s;      // Crear una instancia de la pila personalizada
    s.push(root); // Iniciar el recorrido desde la raíz

    cout << "DFS: ";

    // Mientras la pila no esté vacía
    while (!s.isEmpty())
    {
        // Obtener el nodo en la cima de la pila
        NodeTree *current = s.top();
        s.pop(); // Remover el nodo de la cima

        // Imprimir el valor del nodo actual y el valor de su padre
        cout << current->data << "(P:";
        if (current->up != NULL)
        {
            cout << current->up->data; // Imprimir el valor del padre si existe
        }
        else
        {
            cout << "NULL"; // Si no tiene padre, es la raíz
        }
        cout << ") "; // Espacio para separar los nodos

        // Primero empujar el hijo derecho (para que el hijo izquierdo sea procesado primero)
        if (current->right != NULL)
        {
            s.push(current->right);
        }

        // Luego empujar el hijo izquierdo
        if (current->left != NULL)
        {
            s.push(current->left);
        }
    }

    cout << endl; // Salto de línea para organizar la salida
}
// ---------------------------------------------------- // ---------------------------------------------------- //
int main(int argc, char *argv[])
{
    BST *bst = new BST(25);

    cout << "Busqueda" << endl;
    NodeTree *aux = bst->search(25);
    aux = bst->search(30);

    cout << endl
         << "Insertar" << endl;
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
    cout << endl;

    bst->preorder();
    cout << endl;

    bst->remove(34);
    bst->preorder();
    cout << endl;

    bst->remove(32);
    bst->preorder();
    cout << endl;

    bst->remove(39);
    bst->preorder();
    cout << endl;

    bst->remove(40);
    bst->preorder();
    cout << endl;

    cout << "Orders" << endl;
    bst->preorder();
    bst->inorder();
    bst->postorder();
    bst->levelorder();
    bst->dfs();
}