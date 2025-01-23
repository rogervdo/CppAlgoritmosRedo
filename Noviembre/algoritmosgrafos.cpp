// Patricio Padilla Enriquez - A01738847
//Rogelio Jesus Villarreal De Ochoa - A00838563

/* Hola miss!! oiga le comentamos que le preguntamos a varios compañeros
si se necesitaba leer los datos del grafo.txt y recibimos respuestas mixtas
por lo cual por si las moscas decidimos hacerlo con la capacidad de leerlo, solo
le agregue un valor de peso random. Este quedo de la siguiente manera:

# Grafo 
6 5
0 1 3
0 2 2
1 3 4
2 4 1
4 5 6

Si no era asi, entonces le pido una disculpa :(

Stack::push - O(1)
Stack::pop - O(1)
Stack::isEmpty - O(1)
Stack::isFull - O(1)
ConjuntoDisj::find - O(1)
ConjuntoDisj::unite - O(1)
TopologicoRecursivo - O(N + E)
topologicalSort - O(N + E)
dijkstra - O(N^2)
prim - O(N^2)
ordenarAristas - O(E^2)
kruskal - O(E^2)
leerGrafo - O(E + N^2)

*/




#include <iostream>
#include <fstream>
#include <string>
// Definir un valor de infinito
const int INFINITO = 1000000000;

using namespace std;

class Stack {
    private:
        int *data;
        int top;
        int maxSize;
    public:
        Stack(int maxSize) {
            this->maxSize = maxSize;
            this->data = new int[this->maxSize];
            this->top = -1;
        }

        void push(int valor) {
            this->top++;
            this->data[this->top] = valor;
        }

        int pop() {
            int value = this->data[top];
            this->top--;
            return value;
        }

        bool isEmpty() {
            return this->top == -1;
        }

        bool isFull() {
            return this->top == (this->maxSize - 1);
        }
};

// Clase para representar una arista
class Arista {
    public:
        int Origen;
        int Destino;
        int Costo;

        Arista() {
            Origen = 0;
            Destino = 0;
            Costo = 0;
        }

        Arista(int s, int d, int w) {
            Origen = s;
            Destino = d;
            Costo = w;
        }
};

// Clase para el conjunto disjunto, sera utilizado en el algoritmo de Kruskal
class ConjuntoDisj {
    private:
        int *parent;
        int *height;
        int n;

    public:
        // Constructor que inicializa el conjunto disjunto con n elementos
        ConjuntoDisj(int n) {
            this->n = n;
            parent = new int[n];
            height = new int[n];

            // Inicializamos cada elemento como 0
            for (int i = 0; i < n; i++) {
                parent[i] = i;
                height[i] = 0;
            }
        }

        // Encuentra el representante del conjunto al que pertenece x
        int find(int x) {
            // Si x no es su propio padre
            if (parent[x] != x) {
                parent[x] = find(parent[x]);
            }
            // Retorna el parent de x
            return parent[x];
        }

        // Unimos dos conjuntos para encontrar los parents de amba, parent x y parent y
        void unite(int x, int y) {
            int px = find(x);
            int py = find(y);

            // Une los conjuntos segun la altura para reducir esta misma
            if (height[px] < height[py]) {
                parent[px] = py;
            } else if (height[px] > height[py]) {
                parent[py] = px;
            } else {
                parent[py] = px;
                height[px]++;
            }
        }
};

// Funcion recursiva que recorre el grafo en orden topologico, es necesaria para la funcion de topol
void TopologicoRecursivo(int v, bool visited[], Stack* stack, int** matrix, int N) {
    visited[v] = true;

    for (int i = 0; i < N; i++) {
        if (matrix[v][i] != INFINITO && !visited[i]) {
            TopologicoRecursivo(i, visited, stack, matrix, N);
        }
    }

    stack->push(v);
}

// Ordenamiento Topológico
void topologicalSort(int N, int** matrix) {
    Stack* stack = new Stack(N);

    // Un arreglo para los nodos visitados
    bool* visited = new bool[N];

    // Definimos todos los nodos como no visitados
    for (int i = 0; i < N; i++) {
        visited[i] = false;
    }

    // Recooremos el grafo en orden topologico
    for (int i = 0; i < N; i++) {
        // Si este nodo no ha sido visitado entonces utilizamos la funcion recursiva previamente definida
        if (!visited[i]) {
            TopologicoRecursivo(i, visited, stack, matrix, N);
        }
    }
    while (!stack->isEmpty()) {
        cout << stack->pop() << " ";
    }
    cout << endl;
}

void dijkstra(int N, int** matrix, int origen) {
    int* dist = new int[N];
    bool* visitado = new bool[N]; // Para rastrear nodos visitados

    // Inicializar todas las distancias como INFINITO y nodos como no visitados
    for (int i = 0; i < N; i++) {
        dist[i] = INFINITO;
        visitado[i] = false;
    }

    // La distancia al nodo origen es 0
    dist[origen] = 0;

    for (int count = 0; count < N - 1; count++) {
        // Encontrar el nodo con la menor distancia que aun no ha sido visitado
        int minDist = INFINITO;
        int u = -1;

        for (int i = 0; i < N; i++) {
            if (!visitado[i] && dist[i] < minDist) {
                minDist = dist[i];
                u = i;
            }
        }

        // Si no se encuentra un nodo, terminar el algoritmo
        if (u == -1) break;

        // Marcar el nodo como visitado
        visitado[u] = true;

        // Actualizar las distancias de los vecinos del nodo seleccionado
        for (int v = 0; v < N; v++) {
            if (!visitado[v] && matrix[u][v] != INFINITO && dist[u] + matrix[u][v] < dist[v]) {
                dist[v] = dist[u] + matrix[u][v];
            }
        }
    }

    // Imprimir las distancias mas cortas
    cout << "\nDistancias desde el nodo " << origen << ":\n";
    for (int i = 0; i < N; i++) {
        if (dist[i] == INFINITO) {
            cout << "Nodo " << i << ": INFINITO\n";
        } else {
            cout << "Nodo " << i << ": " << dist[i] << "\n";
        }
    }
}

// Algoritmo de Prim
void prim(int N, int** matrix) {
    cout << "\nPrim: \n";

    // Inicializacion de estructuras
    bool* visitado = new bool[N];       // Nodos visitados
    int* costo = new int[N];         
    int* padre = new int[N];       

    for (int i = 0; i < N; i++) {
        visitado[i] = false;
        costo[i] = INFINITO;
        padre[i] = -1;
    }

    // Comenzar con el nodo 0
    costo[0] = 0;

    for (int count = 0; count < N - 1; count++) {
        // Encontrar el nodo no visitado con el menor costo
        int u = -1, minCosto = INFINITO;
        for (int i = 0; i < N; i++) {
            if (!visitado[i] && costo[i] < minCosto) {
                minCosto = costo[i];
                u = i;
            }
        }

        // Si no se encontro un nodo vaido, termina
        if (u == -1) break;

        // Marcar el nodo como visitado
        visitado[u] = true;

        // Actualizar costos de nodos vecinos
        for (int v = 0; v < N; v++) {
            if (!visitado[v] && matrix[u][v] != INFINITO && matrix[u][v] < costo[v]) {
                costo[v] = matrix[u][v];
                padre[v] = u;
            }
        }
    }

    // Imprimir el MST
    cout << "MST:\n";
    int totalPeso = 0;
    for (int i = 1; i < N; i++) { // Ignorar el nodo raíz (i = 0)
        if (padre[i] != -1) {
            cout << padre[i] << " - " << i 
                 << " \tCosto: " << matrix[padre[i]][i] << endl;
            totalPeso += matrix[padre[i]][i];
        }
    }
    cout << "Costo total: " << totalPeso << endl;
}


// Función para ordenar aristas por peso
void ordenarAristas(Arista* Aristas, int numAristas) {
    for (int i = 0; i < numAristas - 1; i++) {
        for (int j = 0; j < numAristas - i - 1; j++) {
            if (Aristas[j].Costo > Aristas[j + 1].Costo) {
                Arista temp = Aristas[j];
                Aristas[j] = Aristas[j + 1];
                Aristas[j + 1] = temp;
            }
        }
    }
}

// Algoritmo de Kruskal
void kruskal(int N, int** matrix) {
    cout << "\nKruskal: \n";

    // Contamos las aristas
    int numAristas = 0;
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            if (matrix[i][j] != INFINITO) {
                numAristas++;
            }
        }
    }

    // Crear array de aristas
    Arista* Aristas = new Arista[numAristas];
    int AristaIndex = 0;
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            if (matrix[i][j] != INFINITO) {
                Aristas[AristaIndex++] = Arista(i, j, matrix[i][j]);
            }
        }
    }

    // Ordenar aristas por peso
    ordenarAristas(Aristas, numAristas);

    ConjuntoDisj ds(N);
    Arista* result = new Arista[N-1];
    int resultIndex = 0;

    for (int i = 0; i < numAristas && resultIndex < N-1; i++) {
        int x = ds.find(Aristas[i].Origen);
        int y = ds.find(Aristas[i].Destino);

        if (x != y) {
            result[resultIndex++] = Aristas[i];
            ds.unite(x, y);
        }
    }

    cout << "MST:\n";
    int totalPeso = 0;
    for (int i = 0; i < resultIndex; i++) {
        cout << result[i].Origen << " - " << result[i].Destino 
             << " \tCosto: " << result[i].Costo << endl;
        totalPeso += result[i].Costo;
    }
    cout << "Costo total: " << totalPeso << endl;
}

// Funcion para leer el grafo desde un archivo
int** leerGrafo(const string& nombreArchivo, int& N) {
    ifstream archivo(nombreArchivo);
    string linea;
    int E;
    getline(archivo, linea);
    archivo >> N >> E;

    // Creamos una matriz para guardar los valores
    int** matrix = new int*[N];
    for (int i = 0; i < N; i++) {
        matrix[i] = new int[N];
        for (int j = 0; j < N; j++) {
            matrix[i][j] = INFINITO; 
        }
    }

    // Leer aristas
    int a, b, peso;
    for (int i = 0; i < E; i++) {
        archivo >> a >> b >> peso;
        matrix[a][b] = peso;  
        matrix[b][a] = peso; 
    }

    archivo.close();
    return matrix;
}

int main() {
    int N;
    string nombreArchivo = "grafo.txt";

    // Leer el .txt para conseguir el grafo
    int** matrix = leerGrafo(nombreArchivo, N);

    // Ejecutar todos los algoritmos
    cout << "\nTopological Sort:\n";
    topologicalSort(N, matrix);
    dijkstra(N, matrix, 0); 
    prim(N, matrix);
    kruskal(N, matrix);
}