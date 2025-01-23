#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <string>
using namespace std;

class Stack
{
private:
	int *data;
	int top;
	int maxSize;

public:
	Stack(int maxSize)
	{
		this->maxSize = maxSize;
		this->data = (int *)malloc(this->maxSize * sizeof(int));
		this->top = -1;
	}

	~Stack()
	{ // free(this->data);
		delete[] this->data;
	}

	void push(int Valor);
	int getTop();
	int pop();
	bool isEmpty();
	bool isFull();
};

//	solo consulta el dato en top
int Stack::getTop()
{
	int value = this->data[top];
	return value;
}

//	extrae el dato en top
int Stack::pop()
{
	int value = this->data[top];
	this->top--;
	return value;
}

// 	agrega elemento
void Stack::push(int value)
{
	this->top++;
	this->data[this->top] = value;
}

//	revisa si la pila esta llena
bool Stack::isFull()
{
	return this->top == (this->maxSize - 1);
}

//	 revisa si la pila esta vacia
bool Stack::isEmpty()
{
	return this->top == -1;
}

class Queue
{
private:
	int *data;
	int front;
	int end;
	int maxSize;

public:
	Queue(int maxSize)
	{
		this->maxSize = maxSize;
		this->data = (int *)malloc(this->maxSize * sizeof(int));
		this->front = -1;
		this->end = -1;
	}

	~Queue()
	{
		free(this->data);
		//	delete[] this->data;
	}

	void enqueue(int Valor);
	int dequeue();
	int getFront();
	bool isEmpty();
	bool isFull();
};

//	muestra el valor al frente
int Queue::getFront()
{
	return this->data[this->front];
}

//	extrae elemento del frente
int Queue::dequeue()
{
	int value = this->data[this->front];

	if (this->front == this->end)
	{
		this->front = -1;
		this->end = -1;
	}
	else
	{
		this->front = (this->front + 1) % this->maxSize;
	}

	return value;
}

//	agrega elemento
void Queue::enqueue(int value)
{
	this->end = (this->end + 1) % this->maxSize;
	this->data[end] = value;

	if (this->front == -1)
	{
		this->front++;
	}
}

//	si esta llena
bool Queue::isFull()
{
	return (end + 1) % this->maxSize == front;
}

//	si esta vacia
bool Queue::isEmpty()
{
	return front == -1;
}

//	Usa una stack para procesar los nodos
// N: numero de nodos
//! matrix: matrix de adyacencia
void recDFS(int N, int **matrix)
{ // Crear stack nueva con # de nodos del grafo
	Stack *stack = new Stack(N);

	int origen = 0;
	int i;

	// con calloc se inicializa como 0 y no como basura.
	int *visitados = (int *)calloc(N, sizeof(int));
	int *predecesores = (int *)calloc(N, sizeof(int));

	// for (i = 0; i < N; i++)
	// {
	// 	cout << visitados[i] << " ";
	// }
	// cout << endl;
	stack->push(origen);   // nodo origen entra a la stack
	visitados[origen] = 1; // y se marca como visitado.
	predecesores[origen] = -1;

	int u, v;
	while (!stack->isEmpty())
	{
		u = stack->pop(); // Nodo sale de la stack
		cout << "Sale: " << u << endl;

		for (i = 0; i < N; i++)
		{
			if (matrix[u][i] == 1) // Buscamos sus vecinos
			{
				v = i;
				cout << "Vecinos->> u:" << u << " y v:" << i << endl;
				// Si el vecino no ha sido visitado
				if (visitados[v] == 0)
				{
					stack->push(v);
					visitados[v] = 1;
					predecesores[v] = u;
				}
			}
		}
	}
	cout << "predecesores: ";
	for (i = 0; i < N; i++)
	{
		cout << predecesores[i] << " ";
	}
	cout << endl
		 << "node: ";
	for (i = 0; i < N; i++)
	{
		cout << i << " ";
	}
	cout << endl;
}

//	Usa una queue para procesar los nodos
// N: numero de nodos
void recBFS(int N, int **matrix)
{ // Crear queue nueva con # de nodos del grafo
	Queue *queue = new Queue(N);

	int origen = 0;
	int i;

	// con calloc se inicializa como 0 y no como basura.
	int *visitados = (int *)calloc(N, sizeof(int));
	// usamos distancia nodo en vez de predecesores ya que es la utilidad de BFS
	int *distanciaNodo = (int *)calloc(N, sizeof(int));

	queue->enqueue(origen); // nodo origen entra a la queue
	visitados[origen] = 1;	// y se marca como visitado.
	distanciaNodo[origen] = 0;

	int u, v;
	while (!queue->isEmpty())
	{
		u = queue->dequeue(); // Nodo sale del queue
		cout << "Sale: " << u << endl;

		for (i = 0; i < N; i++)
		{
			if (matrix[u][i] == 1) // Buscamos sus vecinos
			{
				v = i;
				cout << "Vecinos->> u:" << u << " y v:" << i << endl;
				// Si el vecino no ha sido visitado
				if (visitados[v] == 0)
				{
					queue->enqueue(v);
					visitados[v] = 1;
					// Incrementamos la distancia
					distanciaNodo[v] = distanciaNodo[u] + 1;
				}
			}
		}
	}

	// Imprimimos las distancias
	cout << "DistanciaNodo" << endl
		 << "dist: ";
	for (i = 0; i < N; i++)
	{
		cout << distanciaNodo[i] << " ";
	}
	cout << endl
		 << "node: ";
	for (i = 0; i < N; i++)
	{
		cout << i << " ";
	}
	cout << endl;
}

int main(int argc, char *argv[])
{
	//	C++
	string sline, line, aux;
	int index;
	int a, b, current = -1;

	int **matrix;
	// int *start, *end;

	ifstream inFile("grafo.txt"); //  input file stream

	//	Verifica que los archivos se hayan abierto correctamente
	int i = 0, j;
	int N, E;
	if (inFile.is_open())
	{ //	Lee liniea a linea
		while (getline(inFile, sline))
		{
			if (i == 1)
			{ // cout << sline << endl;

				line = sline;
				index = line.find(" ");
				aux = line.substr(0, index);
				N = stoi(aux);

				line = line.substr(index + 1);
				index = line.find(" ");
				aux = line.substr(0, index);
				E = stoi(aux);

				//	Imprimir el orden y tamaño
				cout << "nodos N: " << N << endl;
				cout << "aristas E: " << E << endl
					 << endl;

				matrix = (int **)malloc(N * sizeof(int *));
				for (j = 0; j < N; j++)
				{
					matrix[j] = (int *)calloc(N, sizeof(int));
				}

				//	Otra forma de inicializar la matriz +
				// int** matrix = new int*[rows];
				// for (int i = 0; i < rows; ++i)
				// matrix[i] = new int[cols];
			}
			else if (i > 1)
			{
				cout << sline << endl;

				line = sline;
				index = line.find(" ");
				aux = line.substr(0, index);
				a = stoi(aux);

				line = line.substr(index + 1);
				index = line.find(" ");
				aux = line.substr(0, index);
				b = stoi(aux);

				cout << "a y b: " << a << " " << b << endl;

				matrix[a][b] = 1;
				matrix[b][a] = 1;
			}
			i++;
		}

		cout << "N" << N << " " << E << endl;

		for (i = 0; i < N; i++)
		{
			for (j = 0; j < N; j++)
			{
				cout << "[" << i << "," << j << "] " << matrix[i][j] << "\t\t";
			}
			cout << endl;
		}
		cout << endl;

		for (i = 0; i < N; i++)
		{
			for (j = 0; j < i; j++)
			{
				cout << "[" << i << "," << j << "] " << matrix[i][j] << "\t\t";
			}
			cout << endl;
		}
		cout << endl;

		cout << "DFS" << endl;
		recDFS(N, matrix);
		cout << "BFS" << endl;
		recBFS(N, matrix);
	}

	inFile.close();
	// outFile.close();
}
