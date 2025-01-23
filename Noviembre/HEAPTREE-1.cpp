#include <iostream>
#include <math.h>
#include <cstdlib>
using namespace std;

/* un heap implementado sobre un arreglo de cualquier tipo de dato */
template <class T>
class Heap
{
public:
    //	un arreglo de tipo T, tamano maximo y actual del heap
    T *data;
    int maxSize;
    int currentSize;

    Heap(int n)
    {
        maxSize = n;
        currentSize = 0;
        data = new T(maxSize);
    }

    ~Heap()
    {
        delete[] data;
        maxSize = 0;
        currentSize = 0;
    }

    bool isEmpty();
    bool isFull();

    T getTop();
    int parent(int);
    int left(int);
    int right(int);

    void push(T);
    T pop();
    void heapify(int);
    void swap(int, int);
    void show();
};

template <class T>
bool Heap<T>::isEmpty()
{
    return currentSize == 0;
}

template <class T>
bool Heap<T>::isFull()
{
    return currentSize == maxSize;
}

//	Regresa la raiz
template <class T>
T Heap<T>::getTop()
{
    return data[0];
}

template <class T>
int Heap<T>::parent(int j)
{
    int p = (j - 1) / 2;
    return p;
}

template <class T>
int Heap<T>::left(int j)
{
    int l = 2 * j + 1;
    return l;
}

template <class T>
int Heap<T>::right(int j)
{
    int r = 2 * j + 2;
    return r;
}

template <class T>
void Heap<T>::push(T value)
{
    if (isFull())
        return;

    int j = currentSize;
    int p = parent(j);

    while (j > 0 && value < data[p])
    {
        data[j] = data[p];
        j = p;
        p = parent(p);
    }

    data[j] = value;
    currentSize++;
}

//	intercanbia los valores de dos posiciones a y b
template <class T>
void Heap<T>::swap(int a, int b)
{
    T aux = data[a];
    data[a] = data[b];
    data[b] = aux;
}

template <class T>
void Heap<T>::heapify(int index)
{
    int L = left(index);
    int R = right(index);
    int i = index;

    if (L < currentSize && data[L] < data[i])
    {
        i = L;
    }

    if (R < currentSize && data[R] < data[i])
    {
        i = R;
    }

    if (i != index)
    {
        swap(index, i);
        heapify(i);
    }
}

template <class T>
T Heap<T>::pop()
{
    if (isEmpty())
        throw "Heap vacio";

    T root = data[0];

    data[0] = data[currentSize - 1];
    currentSize--;

    if (!isEmpty())
        heapify(0);

    return root;
}

template <class T>
void Heap<T>::show()
{
    int i;
    int level = 0;
    cout << "Size: " << currentSize << endl;

    i = pow(2, level);
    while (i - 1 < currentSize)
    {
        cout << "\t" << data[i - 1] << "(" << level << ")";
        i++;
        if ((i) == pow(2, level + 1))
        {
            cout << endl;
            level++;
        }
    }
    cout << endl;
}

int main(int argc, char *argv[])
{
    int i, aux;
    int n = 20;

    Heap<int> heap(n); //	recibe tamaño max

    // Agregar
    heap.push(2);
    heap.push(1);
    heap.push(5);
    heap.push(7);
    heap.push(9);
    heap.push(8);
    heap.push(4);
    heap.push(3);
    heap.push(6);
    heap.push(7);

    heap.show();

    // Eliminar top
    heap.pop();

    heap.show();

    // Mas agregar
    heap.push(3);
    heap.push(1);
    heap.push(4);

    heap.show();

    cout << "Extraer:" << endl;
    while (!heap.isEmpty())
    {
        cout << heap.pop() << " ";
    }

    exit(0);
}
