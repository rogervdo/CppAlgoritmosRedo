#include <iostream>
#include <queue>
using namespace std;

//* Decidí no usar template t porque no le entiendo
//* Un arbol AVL a diferencia de uno BSL no permite que la diferencia entre sus subarboles
//* No sea mayor que uno (en sus niveles)

class NodeAVL
{
public:
    int data;
    NodeAVL *left, *right, *up;
    int altura;

    NodeAVL(int value)
    { // Constructor default NodeAVL
        data = value;
        left = nullptr; // nullptr o NULL
        right = nullptr;
        up = nullptr;
        altura = 1;
    }

    void printInfo(int level, int balance)
    { // Funcion para imprimir toda la info de los nodos al usar levelorder
        cout << "Nodo " << data << " (Nivel " << level << ", B=" << balance << "): ";
        cout << "Padre[" << (up ? to_string(up->data) : "NULL") << "] ";            // if Check null
        cout << "Izq[" << (left ? to_string(left->data) : "NULL") << "] ";          // if Check null
        cout << "Der[" << (right ? to_string(right->data) : "NULL") << "]" << endl; // if Check null
    }
};

class AVLTree
{
private:
    NodeAVL *root; // Raiz del arbol

    NodeAVL *search(int value)
    { // Función básica search del BSL
        NodeAVL *aux = this->root;
        while (aux != NULL)
        {
            if (aux->data < value)
                aux = aux->right;
            else if (aux->data > value)
                aux = aux->left;
            else
            {
                cout << "Encontrado " << aux->data << endl;
                return aux;
            }
        }
        // cout << "No encontrado " << value << endl;
        return NULL; //* Como la usamos en otras funciones quitamos el print
        //
    }

    int getAltura(NodeAVL *node)
    { // Función get básica
        if (node == NULL)
        {
            return 0;
        }
        return node->altura;
    }

    void updateAltura(NodeAVL *node)
    { // Actualizamos basando en los hijos
        if (node == NULL)
            return;
        node->altura = 1 + max(getAltura(node->left), getAltura(node->right));
    }

    int getBalance(NodeAVL *node)
    { // Realizamos una resta básica para que nos regrese el valor B
        if (node == nullptr)
            return 0;
        return getAltura(node->left) - getAltura(node->right);
    }

    NodeAVL *rotarRight(NodeAVL *y)
    { // Hacemos una rotacion respecto al nodo y
        NodeAVL *x = y->left;
        NodeAVL *T2 = x->right;

        // Realizamos la rotation
        x->right = y;
        y->left = T2;

        // Actualizamos los punteros up
        x->up = y->up;
        y->up = x;
        if (T2 != NULL)
            T2->up = y;

        // Actualizamos las alturas
        updateAltura(y);
        updateAltura(x);

        return x; // Nuevo nodo raíz de este subárbol
    }

    NodeAVL *rotarLeft(NodeAVL *x)
    { // Lo mismo pero ahora con left
        NodeAVL *y = x->right;
        NodeAVL *T2 = y->left;

        // Rotation
        y->left = x;
        x->right = T2;

        // punteros up
        y->up = x->up;
        x->up = y;
        if (T2 != NULL)
            T2->up = x;

        // nuevas alturas
        updateAltura(x);
        updateAltura(y);

        return y; // Nuevo nodo raíz del subárbol
    }

    NodeAVL *balancear(NodeAVL *nodo)
    {
        if (nodo == nullptr)
            return nullptr;

        updateAltura(nodo);
        int balance = getBalance(nodo);

        // Casos simples (zig y zag)
        // Caso LeftLeft -> rotate simple a la derecha
        if (balance > 1 && getBalance(nodo->left) >= 0)
            return rotarRight(nodo);

        // Caso RightRight -> rotate simple a la izquierda
        if (balance < -1 && getBalance(nodo->right) <= 0)
            return rotarLeft(nodo);

        // Casos simples (zig zag, zag zig)
        // Caso LeftRight -> rotate doble izq der
        if (balance > 1 && getBalance(nodo->left) < 0)
        {
            nodo->left = rotarLeft(nodo->left);
            return rotarRight(nodo);
        }

        // Caso RightLeft -> rotate doble der izq
        if (balance < -1 && getBalance(nodo->right) > 0)
        {
            nodo->right = rotarRight(nodo->right);
            return rotarLeft(nodo);
        }

        return nodo;
    }

    NodeAVL *chiquitoNode(NodeAVL *node)
    { // La usamos para el delete cuando tiene dos hijos (funcion aux)
        // Usamos sucesor inorden en este caso
        NodeAVL *actual = node;
        while (actual && actual->left != NULL)
            actual = actual->left;
        return actual;
    }

    NodeAVL *borrarNode(NodeAVL *root, int key)
    {
        // delete que esta en BST
        if (root == NULL)
            return root;

        // Busca el nodo a eliminar
        if (key < root->data)
        {
            root->left = borrarNode(root->left, key);
            if (root->left != NULL)
                root->left->up = root;
        }
        else if (key > root->data)
        {
            root->right = borrarNode(root->right, key);
            if (root->right != NULL)
                root->right->up = root;
        }
        else // Encontró el nodo a eliminar
        {
            // Un hijo o 0 hijos
            if (root->left == NULL)
            {
                NodeAVL *temp = root->right;
                if (temp != NULL)
                    temp->up = root->up;
                delete root;
                return temp;
            }
            else if (root->right == NULL)
            {
                NodeAVL *temp = root->left;
                if (temp != NULL)
                    temp->up = root->up;
                delete root;
                return temp;
            }

            // Nodo con dos hijos: obtiene el sucesor inorden
            NodeAVL *temp = chiquitoNode(root->right);
            root->data = temp->data;
            root->right = borrarNode(root->right, temp->data);
            if (root->right != NULL)
                root->right->up = root;
        }

        return balancear(root); // Balancea el árbol después de la eliminación
    }

    // Inserta un nuevo valor en el árbol con recursion
    NodeAVL *insertarTree(NodeAVL *node, int data)
    {
        // Si el árbol está vacío crea un nuevo nodo
        if (node == NULL)
            return new NodeAVL(data);

        // Checa si no existe ya
        if (search(data) == NULL)
        { // Aqui tambien podemos usar el método de busqueda de borrarNode
            // Pero podemos usar ambos
            if (data < node->data)
            {
                node->left = insertarTree(node->left, data);
                node->left->up = node;
            }
            else
            {
                node->right = insertarTree(node->right, data);
                node->right->up = node;
            }
        }
        else
        {
            cout << "Ya existe el dato " << data << " que deseas ingresar" << endl;
            return node;
        }

        return balancear(node);
    }

public:
    // Constructor
    AVLTree()
    {
        root = nullptr;
    }

    // Metodo publico insertar para que no escribamos el root y porque es private
    void insertar(int data)
    {
        cout << "Insertando: " << data << endl;
        root = insertarTree(root, data);
    }

    // Método publico para eliminar un valor del árbol, por lo mismo
    void eliminar(int data)
    {
        cout << "Eliminando: " << data << endl;
        root = borrarNode(root, data);
    }

    // Recorrido levelorder, hace mayor sentido en un arbol AVL
    void levelorder()
    {
        if (root == nullptr)
        {
            cout << "Árbol vacío" << endl;
            return;
        }

        cout << "\n====== Estado del Arbol AVL ======" << endl;
        // Espero no tener que haber usado un stack/dll
        // Bueno, tenemos dos vectores, uno para el nivel actual y otro para el siguiente
        vector<NodeAVL *> nivelActual;
        vector<NodeAVL *> siguienteNivel;
        int level = 0;

        // Comenzamos con la raíz
        nivelActual.push_back(root);

        while (!nivelActual.empty())
        {
            // Procesamos todos los nodos del nivel actual
            for (NodeAVL *current : nivelActual)
            {
                // Imprime información del nodo
                current->printInfo(level, getBalance(current));

                // Agregamos los hijos al siguiente nivel
                if (current->left != nullptr)
                    siguienteNivel.push_back(current->left);
                if (current->right != nullptr)
                    siguienteNivel.push_back(current->right);
            }

            // Preparamos para el siguiente nivel
            nivelActual = siguienteNivel;
            siguienteNivel.clear();
            level++;
        }
        cout << "=====================================\n"
             << endl;
    }
};

int main()
{
    // Nuevo arbol
    AVLTree arbol;

    // Inputs
    arbol.insertar(1);
    arbol.insertar(7);
    arbol.insertar(8);
    arbol.insertar(24);
    arbol.insertar(31);
    arbol.levelorder();

    arbol.eliminar(24);
    arbol.levelorder();

    arbol.insertar(24);
    arbol.levelorder();

    arbol.insertar(5);
    arbol.insertar(9);
    arbol.insertar(10);
    arbol.levelorder();

    arbol.eliminar(5);
    arbol.eliminar(7);
    arbol.levelorder();

    return 0;
}