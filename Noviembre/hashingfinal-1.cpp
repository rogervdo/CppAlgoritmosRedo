#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <limits>
#include <sstream>
#include <cstring>
#include <string>

struct Coord
{ // Estructura basica para coordenadas del mapa
    int x, y;
    Coord(int x = 0, int y = 0) : x(x), y(y) {} // Constructor inicializando en 0
    bool operator==(const Coord &other) const
    { // Comparar igualdad entre dos coordenadas
        return x == other.x && y == other.y;
    }

    // Operador < para comparar coordenadas
    bool operator<(const Coord &other) const
    {
        // Comparación arbitraria basada en suma de componentes
        return (x < other.x) || (x == other.x && y < other.y);
    }
};

bool operator!=(const Coord &lhs, const Coord &rhs)
{ // Operador de desigualdad para coordenadas
    return !(lhs == rhs);
}

template <typename KeyType> // Plantilla para definir traits del hash de una key genérica
struct HashTraits
{
    static unsigned int hash(const KeyType &key)
    {
        // Implementación por default
        return static_cast<unsigned int>(key);
    }
};

// Especializacion del hash para strings
template <>
struct HashTraits<std::string>
{
    static unsigned int hash(const std::string &key)
    {
        unsigned int hash = 0;
        for (char c : key)
        { // Generación del hash usando multiplicación por 31
            hash = hash * 31 + c;
        }
        return hash;
    }
};

// Implementación de Hashmap usando templates
template <typename KeyType, typename ValueType>
class Hashmap
{
private:
    // Estructura interna para cada entrada del hashmap
    struct Entrada
    {
        KeyType key;     // Clave de la entrada
        ValueType value; // Valor de la entrada
        bool occupied;   // Indica si la entrada está ocupada
        bool deleted;    // Indica si la entrada fue eliminada

        Entrada() : occupied(false), deleted(false) {} // Inicializador por default
    };

    Entrada *table;           // Tabla de entradas
    size_t tableSize;         // Tamaño de la tabla
    size_t count;             // Cantidad de entradas
    mutable float loadFactor; // Factor de carga

    // Auxiliares para encontrar indice y redimensionar
    size_t findIndex(const KeyType &key) const //! O(1) promedio, O(n) peor
    {
        unsigned int hash = HashTraits<KeyType>::hash(key);
        size_t index = hash % tableSize;
        size_t originalIndex = index;
        size_t i = 0;

        while (i < tableSize)
        {
            // Si el slot está vacío o contiene la clave buscada, retornar índice
            if (!table[index].occupied ||
                (!table[index].deleted && table[index].key == key))
            {
                return index;
            }

            // Resolución de colisiones (linear probing)
            i++;
            index = (originalIndex + i) % tableSize;
        }

        return tableSize; // Indica que la tabla está llena
    }

    void resize() //! O(n)
    {             // Duplicar tamaño de la tabla, util cuando se llena
        size_t newSize = tableSize * 2;
        Entrada *oldTable = table; // Guardar tabla antigua
        size_t oldTableSize = tableSize;

        // Crear nueva tabla
        table = new Entrada[newSize];
        tableSize = newSize;
        count = 0;

        // Reinsertar elementos en la nueva tabla
        for (size_t i = 0; i < oldTableSize; ++i)
        {
            if (oldTable[i].occupied && !oldTable[i].deleted)
            {
                insert(oldTable[i].key, oldTable[i].value);
            }
        }

        // Liberar memoria de la tabla antigua
        delete[] oldTable;
    }

public:
    // Obtener todas las entradas del hashmap. es O(n) pero solo necesitamos hacerlo una vez
    std::vector<std::pair<KeyType, ValueType>> getAllEntries() const
    { // Iterar sobre la tabla y guardar las entradas ocupadas
        std::vector<std::pair<KeyType, ValueType>> entries;

        for (size_t i = 0; i < tableSize; ++i)
        {
            if (table[i].occupied && !table[i].deleted)
            {
                entries.emplace_back(table[i].key, table[i].value);
            }
        }

        return entries;
    }

    Hashmap(size_t initialSize = 16, float maxLoadFactor = 0.75) // Constructor con tamaño inicial y factor de carga
        : tableSize(initialSize), count(0), loadFactor(maxLoadFactor)
    {
        table = new Entrada[tableSize];
    }

    ~Hashmap()
    { // Destructor
        delete[] table;
    }

    // Copiar y asignar
    Hashmap(const Hashmap &other)
        : tableSize(other.tableSize), count(other.count), loadFactor(other.loadFactor)
    {
        table = new Entrada[tableSize];
        std::memcpy(table, other.table, tableSize * sizeof(Entrada));
    }

    Hashmap &operator=(const Hashmap &other)
    { // Asignación
        if (this != &other)
        {
            delete[] table;
            tableSize = other.tableSize;
            count = other.count;
            loadFactor = other.loadFactor;
            table = new Entrada[tableSize];
            std::memcpy(table, other.table, tableSize * sizeof(Entrada));
        }
        return *this;
    }

    // Insertar un par clave-valor
    void insert(const KeyType &key, const ValueType &value) //! O(1) promedio, O(n) peor
    {
        // Redimensionar si se excede el factor de carga
        if (static_cast<float>(count) / tableSize >= loadFactor)
        {
            resize();
        }

        size_t index = findIndex(key);

        if (index == tableSize)
        {
            throw std::runtime_error("Hashmap is full");
        }

        // Insertar en la tabla
        if (!table[index].occupied || table[index].deleted)
        {
            table[index].key = key;
            table[index].value = value;
            table[index].occupied = true;
            table[index].deleted = false;
            count++;
        }
        else
        {
            // Actualizar clave existente
            table[index].value = value;
        }
    }

    // Encontrar un valor por clave
    ValueType *find(const KeyType &key) //! O(1) promedio, O(n) peor
    {
        size_t index = findIndex(key);

        if (index < tableSize &&
            table[index].occupied &&
            !table[index].deleted &&
            table[index].key == key)
        {
            return &table[index].value;
        }

        return nullptr;
    }

    // Remover un par clave-valor
    void remove(const KeyType &key) //! O(1) promedio, O(n) peor
    {
        size_t index = findIndex(key);

        if (index < tableSize &&
            table[index].occupied &&
            !table[index].deleted &&
            table[index].key == key)
        {
            table[index].occupied = false;
            table[index].deleted = true;
            count--;
        }
    }

    size_t size() const
    { // Obtener tamaño del hashmap
        return count;
    }

    bool empty() const
    { // Verificar si el hashmap está vacío
        return count == 0;
    }
};

// Especialización de traits para coordenadas
template <>
struct HashTraits<Coord>
{
    static unsigned int hash(const Coord &coord)
    {
        unsigned int hash = 17; // Elegimos primos para evitar colisiones
        hash = hash * 31 + static_cast<unsigned int>(coord.x);
        hash = hash * 31 + static_cast<unsigned int>(coord.y);
        return hash;
    }
};

struct Plato
{ //
    std::string nombre;
    int precio;
};

struct Edge
{ // Primero tenemos a las aristas conteniendo coordenadas con el destino y peso
    Coord destino;
    int peso;
};

struct Node
{ // Luego los nodos conteniendo coordenadas x y, y un vector de aristas
    Coord coord;
    std::vector<Edge> vecinos;
};
// ------------------------------------- Grafos ------------------------------------- // ------------------------------------- Grafos ------------------------------------- //
class CityMenuSystem
{
private:
    Hashmap<Coord, Node> graphMap; // Grafo representado como vector de nodos

    Node *findNode(const Coord &coord)
    { // Replaces findNodeIndex
        return graphMap.find(coord);
    }

    std::vector<std::pair<std::string, Coord>> restaurants; // Vector de restaurantes

    // Sistema de menús
    std::vector<std::pair<std::string, std::vector<Plato>>> menuPorRestaurante;
    std::vector<std::pair<std::string, std::vector<std::pair<std::string, int>>>> restaurantesPorPlato;

    // Función auxiliar para encontrar restaurante
    int findRestaurantIndex(const std::string &name) //! O(n)
    {                                                // Nota: se podria implementar con hashmap para O(1) en lugar de O(n)
        for (size_t i = 0; i < restaurants.size(); i++)
        {
            if (restaurants[i].first == name)
            {
                return i;
            }
        }
        return -1;
    }

    void reverseVector(std::vector<Coord> &path) //! O(n)
    {                                            // Función auxiliar para revertir un vector
        size_t n = path.size();
        for (size_t i = 0; i < n / 2; i++)
        {
            Coord temp = path[i];
            path[i] = path[n - 1 - i];
            path[n - 1 - i] = temp;
        }
    }

public:
    // Funciones de input/carga de datos
    void loadGraph(const std::string &filename);       // Cargar grafo desde archivo - O(m*n) ya que por cada arista realiza dos busquedas (m = aristas)
    void loadRestaurants(const std::string &filename); // Cargar restaurantes desde archivo - O(n)
    void loadMenu(const std::string &filename);        // Cargar menú desde archivo - O(n)

    // Funciones de datos
    void agregarPlato(const std::string &restaurante, const std::string &nombrePlato, int precio); // Agregar plato a restaurante - O(n)
    std::pair<std::string, int> extraerPlatoYPrecio(const std::string &str);                       // Extraer nombre y precio de plato - O(n)

    // Funciones de busqueda y display
    void buscarMenuRestaurante(const std::string &restaurante);      // Buscar menú de un restaurante especifico - O(n × m)
    void buscarRestaurantesConPlato(const std::string &nombrePlato); // Buscar restaurantes que ofrezcan un plato especifico - O(n × m)

    // Funciones de procesamiento
    std::pair<int, std::vector<Coord>> googleMaps(const Coord &start, const Coord &end); // Encontrar camino más corto de un punto a otro - O((V + m) × log V) Dijkstra >> V = nodos, m = aristas
    void procesarOrdenes(const std::string &filename);                                   // Procesar pedidos de delivery - O(n × ((V + E) × log V)) Numero de ordenes (n) que se ejecuta googleMaps

    // Funcion de usuario
    void menuInteractivo(); // Menú interactivo para buscar menús y restaurantes - Complejidad depende de operaciones realizadas

    std::vector<std::pair<std::string, int>> encontrarRestaurantesCercanos(const Coord &userLocation, int numRestaurants = 3);
};

void CityMenuSystem::loadGraph(const std::string &filename) //! O(m)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: no se pudo abrir el archivo " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        int x1, y1, x2, y2, weight;
        if (sscanf(line.c_str(), "(%d, %d) (%d, %d) %d", &x1, &y1, &x2, &y2, &weight) == 5)
        {
            Coord node1(x1, y1), node2(x2, y2);

            // Checar si los nodos existen
            if (!graphMap.find(node1))
            {
                graphMap.insert(node1, {node1, {}});
            }
            if (!graphMap.find(node2))
            {
                graphMap.insert(node2, {node2, {}});
            }

            // Agregar arista a ambos nodos
            Node *nodePtr1 = graphMap.find(node1);
            Node *nodePtr2 = graphMap.find(node2);
            nodePtr1->vecinos.push_back({node2, weight});
            nodePtr2->vecinos.push_back({node1, weight});
        }
    }

    std::cout << "Grafo cargado con " << graphMap.size() << " nodos." << std::endl;
}
void CityMenuSystem::loadRestaurants(const std::string &filename) //! O(n)
{
    std::ifstream file(filename); // Abrir archivo restaurantes
    if (!file.is_open())
    { // Verificar si se pudo abrir
        std::cerr << "Error: no se pudo abrir archivo de restaurante con nombre " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line))
    { // Leer cada linea del archivo

        // Definimos variables del archivo
        std::istringstream iss(line);
        std::string name;
        int x, y;

        // Extraemos nombre y coordenadas
        std::getline(iss, name, '(');
        name = name.substr(0, name.length() - 1);

        iss >> x;
        iss.ignore(2);
        iss >> y;

        restaurants.push_back({name, Coord(x, y)}); // Agregamos restaurante al vector
        std::cout << "Restaurante cargado: " << name << ", en posición (" << x << ", " << y << ")" << std::endl;
    }
    std::cout << std::endl;
}
void CityMenuSystem::loadMenu(const std::string &filename) //! O(n)
{
    std::ifstream archivo(filename); // Abrir archivo de menu
    if (!archivo.is_open())
    { // Verificar si se pudo abrir
        std::cerr << "Error: no se pudo abrir archivo de menu con nombre " << filename << std::endl;
        return;
    }

    int numRestaurantes, numPlatos, numLineas; // Definir variables del archivo
    archivo >> numRestaurantes >> numPlatos >> numLineas;
    archivo.ignore();

    std::string linea;
    while (std::getline(archivo, linea))
    {
        size_t posR = linea.find("R:"); // Leer ordenes con indicadores R y O
        size_t posO = linea.find("O:");

        if (posR != std::string::npos && posO != std::string::npos) // Si se encuentran ambos indicadores
        {
            std::string restaurante = linea.substr(posR + 2, posO - (posR + 2) - 1); // Extraer nombre del restaurante
            auto [nombrePlato, precio] = extraerPlatoYPrecio(linea.substr(posO));    // Extraer nombre y precio del plato
            agregarPlato(restaurante, nombrePlato, precio);                          // Agregar plato al restaurante
        }
    }
    std::cout << "Menu Cargado" << std::endl;
}

void CityMenuSystem::agregarPlato(const std::string &restaurante, const std::string &nombrePlato, int precio) //! O(1) promedio, O(n) peor
{
    // Actualizar menú
    bool encontrado = false; // Definir variable para verificar si se encontro el restaurante
    for (auto &menu : menuPorRestaurante)
    { // Recorrer vector de restaurantes
        if (menu.first == restaurante)
        {
            menu.second.push_back({nombrePlato, precio}); // Agregamos plato al restaurante si se encuentra
            encontrado = true;                            // Cambiamos variable a true
            break;
        }
    }
    if (!encontrado)
    {                                                                                           // Si no se encontro el restaurante
        menuPorRestaurante.push_back({restaurante, std::vector<Plato>{{nombrePlato, precio}}}); // Agregamos restaurante con su plato/precio
    }

    // Actualizar platos
    encontrado = false;
    for (auto &plato : restaurantesPorPlato)
    { // Recorrer vector de platos
        if (plato.first == nombrePlato)
        {
            plato.second.push_back({restaurante, precio});
            encontrado = true;
            break;
        }
    }
    if (!encontrado)
    {                                                                                                                   // Si no se encontro el plato
        restaurantesPorPlato.push_back({nombrePlato, std::vector<std::pair<std::string, int>>{{restaurante, precio}}}); // Agregamos plato con su restaurante/precio;
    }
}
std::pair<std::string, int> CityMenuSystem::extraerPlatoYPrecio(const std::string &str) //! O(k), k = longitud de la cadena
{
    // Extraer nombre
    size_t inicio = str.find(":") + 1;
    size_t fin = str.find("(");
    std::string nombre = str.substr(inicio, fin - inicio);

    // Extraer precio
    size_t inicioP = str.find("(") + 1;
    size_t finP = str.find(")");
    int precio = std::stoi(str.substr(inicioP, finP - inicioP));

    return {nombre, precio};
}
void CityMenuSystem::buscarMenuRestaurante(const std::string &restaurante) //! O(m)
{
    for (const auto &menu : menuPorRestaurante)
    { // Recorrer vector de restaurantes
        if (menu.first == restaurante)
        { // Si se encuentra el restaurante
            std::cout << "\nMenú de " << restaurante << ":\n";
            for (const auto &plato : menu.second)
            { // Imprimir menú del restaurante
                std::cout << "- " << plato.nombre << " ($" << plato.precio << ")\n";
            }
            return;
        }
    }
    std::cout << "\nRestaurante no encontrado.\n";
}
void CityMenuSystem::buscarRestaurantesConPlato(const std::string &nombrePlato) //! O(m)
{
    for (const auto &plato : restaurantesPorPlato)
    { // Recorrer vector de platos
        if (plato.first == nombrePlato)
        { // Si se encuentra el plato
            std::cout << "\nRestaurantes que ofrecen " << nombrePlato << ":\n";
            for (const auto &restaurante : plato.second)
            { // Imprimir restaurantes que ofrecen el plato
                std::cout << "- " << restaurante.first << " ($" << restaurante.second << ")\n";
            }
            return;
        }
    }
    std::cout << "\nPlato no encontrado.\n";
}
std::pair<int, std::vector<Coord>> CityMenuSystem::googleMaps(const Coord &start, const Coord &end) //! O(V + E)
{
    Node *startNode = findNode(start);
    Node *endNode = findNode(end);

    if (!startNode || !endNode)
    {
        std::cerr << "Nodo de inicio o fin no encontrado en el grafo" << std::endl;
        return {-1, {}};
    }

    // Inicializamos variables para Djikstra
    Hashmap<Coord, int> distances(graphMap.size());
    Hashmap<Coord, Coord> previous(graphMap.size());

    auto allEntries = graphMap.getAllEntries(); //! O(n)
    for (const auto &Entrada : allEntries)
    {
        distances.insert(Entrada.first, std::numeric_limits<int>::max());
        previous.insert(Entrada.first, Coord(-1, -1));
    }

    // Usamos priority_queue para manejar la cola de prioridad
    using QueueEntrada = std::pair<int, Coord>; // {distancia, nodo}
    std::priority_queue<QueueEntrada, std::vector<QueueEntrada>, std::greater<>> pq;

    distances.insert(start, 0);
    pq.push({0, start}); // Nodo inicial

    while (!pq.empty())
    {
        auto [currentDist, current] = pq.top(); // Nodo con menor distancia
        pq.pop();

        Node *currentNode = findNode(current);
        if (!currentNode)
            continue;

        if (current == end)
            break;

        for (const auto &edge : currentNode->vecinos)
        {
            int newDist = currentDist + edge.peso;

            int *neighborDist = distances.find(edge.destino);
            if (neighborDist && newDist < *neighborDist)
            {
                distances.insert(edge.destino, newDist);
                previous.insert(edge.destino, current);
                pq.push({newDist, edge.destino}); // Insertamos vecino con nueva distancia
            }
        }
    }

    // Reconstruimos la ruta
    std::vector<Coord> path;
    Coord at = end;
    while (at != start)
    {
        if (previous.find(at) == nullptr)
        {
            return {-1, {}}; // Ruta no encontrada
        }
        path.push_back(at);
        at = *previous.find(at);
    }
    path.push_back(start);

    reverseVector(path); // Revertimos el camino para obtener la ruta correcta

    return {*distances.find(end), path};
}

void CityMenuSystem::procesarOrdenes(const std::string &filename) //! O(n * (V + E))
{
    std::ifstream file(filename); // Abrir archivo de ordenes
    if (!file.is_open())
    {
        std::cerr << "Error: no se pudo abrir archivo de ordenes con nombre " << filename << std::endl;
        return;
    }

    std::string line;
    int orderCount = 0;

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string date, time, restaurantPart, orderPart; // Variables de nuestra orden

        iss >> date >> time;

        size_t restaurantPos = line.find("R:");
        if (restaurantPos == std::string::npos) // check si no se encuentra el indicador de restaurante
        {
            std::cerr << "Formato de orden invalido: " << line << std::endl;
            continue;
        }

        // Extraer nombre del restaurante y coordenadas de destino
        size_t orderPos = line.find("O:", restaurantPos);
        std::string restaurant = line.substr(restaurantPos + 2, orderPos - restaurantPos - 3);

        size_t openParenPos = line.rfind("(");
        std::string destCoords = line.substr(openParenPos);

        // Extraer coordenadas de destino
        int destX, destY;
        if (sscanf(destCoords.c_str(), "(%d, %d)", &destX, &destY) != 2)
        {
            std::cerr << "Coordenadas de destino invalidas: " << line << std::endl;
            continue;
        }

        // Encontrar el restaurante
        int restaurantIndex = findRestaurantIndex(restaurant); // Encontrar el indice del restaurante
        if (restaurantIndex == -1)
        {
            std::cerr << "No se encontro restaurante: " << restaurant << std::endl;
            continue;
        }

        // Encontrar la ruta más corta
        auto [distance, path] = googleMaps(restaurants[restaurantIndex].second, Coord(destX, destY));

        if (distance != -1)
        {
            std::cout << "Detalles de la orden [" << orderCount + 1 << "]: " << std::endl;
            std::cout << "Fecha: " << date << " " << time << std::endl;
            std::cout << "Restaurante: " << restaurant << std::endl;
            std::cout << "Destino: (" << destX << ", " << destY << ")" << std::endl;
            std::cout << "Numero de aristas: " << path.size() - 1 << std::endl;
            std::cout << "Distancia total: " << distance << std::endl;
            std::cout << "Ruta: ";
            for (const auto &node : path)
            { // Imprimir ruta nodo por nodo
                std::cout << "(" << node.x << ", " << node.y << ") ";
            }
            std::cout << "\n\n";

            orderCount++;
        }
    }

    std::cout << "Se procesaron " << orderCount << " ordenes" << std::endl;
}

std::vector<std::pair<std::string, int>> CityMenuSystem::encontrarRestaurantesCercanos(const Coord &userLocation, int numRestaurants) //! (n * (V + E))
{
    // Hashmap para almacenar las distancias a los restaurantes
    Hashmap<std::string, int> restaurantDistances(restaurants.size());

    // Calcular las distancias a todos los restaurantes
    for (const auto &restaurant : restaurants)
    {
        auto [distance, _] = googleMaps(userLocation, restaurant.second);
        if (distance != -1)
        {
            restaurantDistances.insert(restaurant.first, distance);
        }
    }

    // Recuperar todas las entradas del hashmap
    auto distanceEntries = restaurantDistances.getAllEntries();

    // Selección personalizada utilizando nuestro Hashmap
    std::vector<std::pair<std::string, int>> nearestRestaurants;

    // Encontrar los numRestaurants restaurantes más cercanos
    for (int i = 0; i < numRestaurants && !distanceEntries.empty(); ++i)
    {
        // Encontrar el restaurante con la distancia mínima
        int minIndex = 0;
        for (size_t j = 1; j < distanceEntries.size(); ++j)
        {
            if (distanceEntries[j].second < distanceEntries[minIndex].second)
            {
                minIndex = j;
            }
        }

        // Agregar el restaurante más cercano a los resultados
        nearestRestaurants.push_back(distanceEntries[minIndex]);

        // Eliminar el restaurante agregado de las entradas
        distanceEntries.erase(distanceEntries.begin() + minIndex);
    }

    return nearestRestaurants;
}

void CityMenuSystem::menuInteractivo()
{
    while (true)
    {
        std::cout << "\n=== Bienvenido a Ouber eats devmode ===\n";
        std::cout << "1. Buscar menú de un restaurante\n";
        std::cout << "2. Buscar restaurantes con un plato\n";
        std::cout << "3. Encontrar restaurantes cercanos\n";
        std::cout << "4. Salir\n";
        std::cout << "Seleccione una opción (ingrese un número): ";

        int opcion;
        std::cin >> opcion;
        std::cin.ignore();

        if (opcion == 4)
            break;

        std::string busqueda;

        switch (opcion)
        {
        case 1:
            std::cout << "Ingrese el nombre a buscar: ";
            std::getline(std::cin, busqueda);
            buscarMenuRestaurante(busqueda);
            break;
        case 2:
            std::cout << "Ingrese el nombre a buscar: ";
            std::getline(std::cin, busqueda);
            buscarRestaurantesConPlato(busqueda);
            break;
        case 3:
        {
            int x, y;
            std::cout << "Ingrese coordenada X: ";
            std::cin >> x;
            std::cout << "Ingrese coordenada Y: ";
            std::cin >> y;

            auto nearestRestaurants = encontrarRestaurantesCercanos(Coord(x, y));

            std::cout << "\nRestaurantes más cercanos:\n";
            for (const auto &restaurant : nearestRestaurants)
            {
                std::cout << restaurant.first << " - Distancia: " << restaurant.second << std::endl;
                std::cout << "Menú:\n";
                buscarMenuRestaurante(restaurant.first);
                std::cout << "\n";
            }
            break;
        }
        default:
            std::cout << "Opcion invalida... intente denuevo\n";
        }
    }
}

int main()
{
    CityMenuSystem sistema;

    sistema.loadGraph("city30x30.txt");
    sistema.loadRestaurants("restaPlaces.txt");
    sistema.loadMenu("menus.txt");
    sistema.procesarOrdenes("orders-city30x30.txt");
    sistema.menuInteractivo();

    return 0;
}