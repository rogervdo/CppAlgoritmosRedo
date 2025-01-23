#include <iostream>
#include <string>
#include <vector>
#include <queue>

using namespace std;
// ---------------------------------- // ------------------------------- //
class NodeSplay
{
public:
    int data;
    NodeSplay *left, *right, *up;

    NodeSplay(int value)
    { // Constructor default NodeSplay
        data = value;
        left = nullptr; // nullptr o NULL
        right = nullptr;
        up = nullptr;
    }

    void printInfo(int level) // Incluimos printInfo aqui en vez de otro lado debido a que podemos utilizarlo para cada nodo
    {                         // Funcion para imprimir toda la info de los nodos al usar levelorder
        cout << "Nodo " << data << " (lvl " << level << ") ";
        cout << "Padre[" << (up ? to_string(up->data) : "NULL") << "] ";
        cout << "L[" << (left ? to_string(left->data) : "NULL") << "] ";
        cout << "R[" << (right ? to_string(right->data) : "NULL") << "]" << endl;
    }
};
// ---------------------------------- // ------------------------------- //
class SplayTree
{
private:
    NodeSplay *root; // Raiz del arbol

    // Rotación a la derecha (zig)
    void rotarRight(NodeSplay *y) //* O(1)
    {                             // Rotar subtree sobre nodo y
        if (!y || !y->left)       // nullcheck
            return;

        NodeSplay *x = y->left;
        NodeSplay *T2 = x->right;
        NodeSplay *p = y->up;

        // Actualizar padre de y
        if (p)
        {
            if (p->left == y)
                p->left = x;
            else
                p->right = x;
        }

        // Realizar rotación
        x->up = p;
        x->right = y;
        y->up = x;
        y->left = T2;
        if (T2)
            T2->up = y;

        // Si y era la raíz, actualizamso la raiz
        if (y == root)
            root = x;
    }

    // Rotación a la izquierda (zag)
    void rotarLeft(NodeSplay *x) //* O(1)
    {                            // Rotar subtree sobre nodo x
        if (!x || !x->right)
            return; // Cehck null

        NodeSplay *y = x->right;
        NodeSplay *T2 = y->left;
        NodeSplay *p = x->up;

        // Actualizar padre de x
        if (p)
        {
            if (p->left == x)
                p->left = y;
            else
                p->right = y;
        }

        // Realizar rotation
        y->up = p;
        y->left = x;
        x->up = y;
        x->right = T2;
        if (T2)
            T2->up = x;

        // Si x era la raíz, actualizar raiz
        if (x == root)
            root = y;
    }

    void splay(NodeSplay *node) //* O(log n)
    {                           // Movemos un nodo a la root del arbol mediante muchas rotaciones
        if (!node)
            return; // Caso para null

        while (node->up)
        {
            NodeSplay *parent = node->up;
            NodeSplay *abuelo = parent->up; // mejor usamos "abuelo" porque grandparent se confunde

            if (!abuelo)
            { // Zig o Zag (una rotacion nadamas)
                if (parent->left == node)
                    rotarRight(parent);
                else
                    rotarLeft(parent);
            }
            else
            { // Zig-zig, Zag-zag, Zig-zag, o Zag-zig
                if (abuelo->left == parent)
                { // Aplicamos los casos que vimos en la diapositiva
                    if (parent->left == node)
                    { // Zig-zig
                        rotarRight(abuelo);
                        rotarRight(node->up);
                    }
                    else
                    { // Zig-zag
                        rotarLeft(parent);
                        rotarRight(node->up);
                    }
                }
                else
                {
                    if (parent->right == node)
                    { // Zag-zag
                        rotarLeft(abuelo);
                        rotarLeft(node->up);
                    }
                    else
                    { // Zag-zig
                        rotarRight(parent);
                        rotarLeft(node->up);
                    }
                }
            }
        }
        root = node;
    }

    NodeSplay *buscarSplay(int value) //* O(log n)
    {                                 // Una función de busqueda básica, como en los otros arboles.
                                      // A diferencia de los otros arboles, un search modifica el arbol
        NodeSplay *node = root;
        NodeSplay *last = nullptr;

        while (node)
        {
            last = node; // Vamos left o right
            if (value < node->data)
                node = node->left;
            else if (value > node->data)
                node = node->right;
            else
            {
                // Ahora debemos colocar nuesto valor encontrado en root
                splay(node);
                return node;
            }
        }

        if (last) // Hacemos splay al último nodo visitado incluso si no encontramos
            splay(last);
        return nullptr;
    }

    NodeSplay *insertSplay(NodeSplay *node, int value) //* O(log n)
    {
        if (!node) // Invalido
            return new NodeSplay(value);

        NodeSplay *parent = nullptr;
        while (node)
        {
            parent = node; // Un search que busca lugar de inserción
            if (value < node->data)
                node = node->left;
            else if (value > node->data)
                node = node->right;
            else
                return node; // Valor duplicado
        }

        // Aqui creamos el nuevo nodo y lo conectamos
        NodeSplay *newNode = new NodeSplay(value);
        newNode->up = parent;

        if (value < parent->data)
            parent->left = newNode;
        else
            parent->right = newNode;

        return newNode;
    }

    NodeSplay *quitarNode(NodeSplay *node) //* O(log n)
    {
        if (!node) // check null
            return nullptr;

        // Splaying antes de eliminar
        splay(node);

        // Un hijo derecho
        if (!node->left)
        {
            NodeSplay *right = node->right;
            if (right)
                right->up = nullptr;
            delete node;
            return right;
        }
        // Un hijo izquierdo
        else if (!node->right)
        {
            NodeSplay *left = node->left;
            if (left)
                left->up = nullptr;
            delete node;
            return left;
        }
        // Tiene dos hijos
        else
        {
            // Encuentra el máximo en el subárbol derecho con in order
            NodeSplay *leftTree = node->left;
            leftTree->up = nullptr;
            NodeSplay *rightTree = node->right;
            rightTree->up = nullptr;

            // Encuentra el máximo en el subárbol izquierdo con in order
            NodeSplay *maxLeft = leftTree;
            while (maxLeft->right)
                maxLeft = maxLeft->right;

            // Splay el máximo al root del subárbol izquierdo
            splay(maxLeft);
            maxLeft->right = rightTree;
            if (rightTree)
                rightTree->up = maxLeft;

            delete node;
            return maxLeft;
        }
    }
    // ---------------------------------- // ------------------------------- //
public:
    SplayTree() : root(nullptr) {}

    bool buscar(int value) //* O(log n)
    {                      //! Es literalmente la funcion de splay
        cout << "Buscando: " << value << "... ";
        NodeSplay *result = buscarSplay(value);
        if (result)
        {
            cout << "success ^_^ " << endl;
            return true;
        }
        cout << "no encontrado :(" << endl;
        return false;
    }

    void insertar(int value) //* O(log n)
    {                        //! Insertar, + splay
        cout << "Insertando: " << value << endl;
        if (!root)
        {
            root = new NodeSplay(value);
            return;
        }

        NodeSplay *newNode = insertSplay(root, value); // función insertar + splay
        splay(newNode);
    }

    void eliminar(int value) //* O(log n)
    {                        //! Buscar + eliminar(y splay de nuevo)
        cout << "Eliminando: " << value << "...";
        NodeSplay *node = buscarSplay(value); // Funcion buscar + splay
        if (!node)
        {
            cout << value << " no encontrado :(" << endl;
            return;
        }
        root = quitarNode(node); // Funcion eliminar + splay
        cout << "success ^_^ " << endl;
    }

    void levelorder(int etapa) //* O(log n)
    {                          // Usamos para imprimir el arbol. A diferencia del arbol anterior AVL quitamos el factor B ya que no lo usa
        if (!root)
        {
            cout << "Arbol vacio" << endl;
            return;
        }

        cout << "\n+===== Arbol splay etapa (" << etapa << ") =====+" << endl;
        // Espero no tener que haber usado un stack/dll personalizado
        // Bueno, tenemos dos vectores, uno para el nivel actual y otro para el siguiente
        vector<NodeSplay *> nivelActual;
        vector<NodeSplay *> siguienteNivel;
        int level = 0;

        // Comenzamos con la raíz
        nivelActual.push_back(root);

        while (!nivelActual.empty())
        { // Procesamos todos los nodos del nivel actual
            for (NodeSplay *current : nivelActual)
            {
                // Imprime información del nodo
                current->printInfo(level);

                // Agregamos los hijos al siguiente nivel
                if (current->left)
                    siguienteNivel.push_back(current->left);
                if (current->right)
                    siguienteNivel.push_back(current->right);
            }

            // Preparamos para el siguiente nivel
            nivelActual = siguienteNivel;
            siguienteNivel.clear();
            level++;
        }
        cout << "+===================================+\n"
             << endl;
    }
};
// ---------------------------------- // ------------------------------- //
int main()
{
    SplayTree arbol;

    arbol.insertar(1);
    arbol.insertar(7);
    arbol.insertar(8);
    arbol.insertar(24);
    arbol.insertar(30);
    arbol.insertar(9);
    arbol.levelorder(1);

    arbol.eliminar(7);
    arbol.levelorder(2);

    arbol.buscar(9);
    arbol.levelorder(3);

    arbol.insertar(5);
    arbol.insertar(10);
    arbol.levelorder(4);

    return 0;
}