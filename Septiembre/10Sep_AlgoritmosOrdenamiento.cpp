#include <vector>
#include <iostream>
#include <algorithm>

std::vector<int> bubbleSort(std::vector<int> nums) {
    //* Bubblesort repetidamente cambia a los dos elementos adyacentes si estan en el orden incorrecto
        // Se puede hacer otra versión con & para modificar el vector directamente en vez de hacer una copia.
    int n = nums.size();
    bool swapped;

    // i = iteracion
    // j = comparación
    for (int i = 0; i < n - 1; i++) {
        swapped = false;


        for (int j = 0; j < n - 1 - i; j++) {
            //* Si el número a la izquierda es mayor al de la derecha
            if (nums[j] > nums[j + 1]) {

                //* Intercambiarlos

                // int aux = nums[j + 1];
                // nums[j + 1] = nums[j];
                // nums[j] = aux;

                // Nota, no es lo mismo con pointers
                std::swap(nums[j], nums[j + 1]);

                swapped = true;
            }
        }
        if (!swapped)
            return nums;
    }
    //* Caso donde nunca se cambian jonentes
    return nums;
}

void selectionSort(std::vector<int>& nums) {
    //Busca el menor elemento desde el lado organizado, y luego hace un swap, continunando hasta terminar de ordenar.
    int n = nums.size();

    for (int i = 0; i < n - 1; i++) {
        int min_index = i;

        for (int j = i + 1; j < n; j++) {
            if (nums[j] < nums[min_index]) // Compara valores, agarrando el más pequeño
                min_index = j;
        }
        if (min_index != i) { // Si encuentra otro valor más pequeño
            int aux = nums[min_index];
            nums[min_index] = nums[i];
            nums[i] = aux;
        }
    }
}

void insertionSort(std::vector<int>& nums) {
    //*Inserta cada elemento a una lista ordenada en el lado izq del arreglo, uno por uno
    int n = nums.size();

    for (int i = 1; i < n; i++) {

        int key = nums[i];
        int j = i - 1;

        while (j >= 0 && nums[j] > key) {
            nums[j + 1] = nums[j];
            --j;
        }
        nums[j + 1] = key;
    }
}

void merge(std::vector<int>& nums, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> L(n1), R(n2);

    // Copia de numeros a vectores left y right
    for (int i = 0; i < n1; i++) {
        L[i] = nums[left + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = nums[mid + 1 + j];
    }

    int i = 0, j = 0;
    int k = left;

    while (i < n1 && j < n2) {
        // Decidimos el numero menor para insertar en nums[k]. 
        // Como copiamos los numeros en L() y R(), podemos reemplazar en nums() sin preocuparnos
        if (L[i] < R[j]) {
            nums[k] = L[i];
            i++; // Recorremos el vector Left ya que ingresamos el # en nums
        }
        else {
            nums[k] = R[j];
            j++; // Recorremos el vector Right ya que ingresamos el # en nums
        }
        k++; // Recorremos nums
    }

    // Estos dos whiles significan que nos terminamos L() o R()
    // por lo que podemos insertar los # del otro vector directamente ya que estan ordenados.
    while (i < n1) {
        nums[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        nums[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(std::vector<int>& nums, int left, int right) {

    if (left >= right)
        return;

    //* Con mid, podemos dividir el vector en dos partes, y dividirlo de poco en poco hasta ser segmentos de 1 unidad.
    int mid = left + (right - left) / 2;

    mergeSort(nums, left, mid); // Lado izquierda
    mergeSort(nums, mid + 1, right); // Lado derecho

    // Generamos un tipo de BFS tree con las separaciones, y ahora vamos a juntarlos.

    // Primero se ordenan los mas pequeños, y luego los más grandes. Cuando ordenemos los más grandes, ya tendremos a los pequeños ordenados.
    merge(nums, left, mid, right);

}

void mergeSort(std::vector<int>& nums) {
    //* Creamos left y right, mandando a función sobrecargada.
    int left = 0;
    int right = nums.size() - 1;
    mergeSort(nums, left, right);
}

int quickSort(std::vector<int> nums) {
    int n = nums.size();

}

void printVector(const std::vector<int>& vec) {
    for (int val : vec) {
        std::cout << val << " ";
    }
    std::cout << std::endl; // End the line after printing
}

int main() {
    std::vector<int> numbers = {11, 7, 5, 3, 29, 23, 19, 17, 31, 37, 43, 41};
    std::vector<int> numbers5 = {11, 7, 5, 3, 29, 23, 19, 17, 31, 37, 43, 41};
    std::vector<int> numbers2 = {11, 7, 5, 3, 29, 23, 19, 17, 31, 37, 43, 41};
    std::vector<int> numbers3 = {11, 7, 5, 3, 29, 23, 19, 17, 31, 37, 43, 41};
    std::vector<int> numbers4 = {11, 7, 5, 3, 29, 23, 19, 17, 31, 37, 43, 41};
    std::cout << "Vector original\n";
    printVector(numbers);

    std::cout << "Bubble Sort\n";
    printVector(bubbleSort(numbers)); // Bubblesort no usa pointers

    std::cout << "Selection Sort\n";
    selectionSort(numbers2); // Selectionsort usa pointers
    printVector(numbers2);

    std::cout << "Insertion Sort\n";
    insertionSort(numbers3);
    printVector(numbers3);

    std::cout << "Merge Sort\n";
    mergeSort(numbers4);
    printVector(numbers4);

    std::cout << "Quicksort\n";
    //quickSort(numbers5);
    printVector(numbers5);

    std::vector<int> nums = {5, 13, 6, 11, 12, 7};

    std::cout << "Mergesort example\n";
    std::cout << "Original array:\n";
    for (int num : nums) std::cout << num << " ";
    std::cout << "\n";

    mergeSort(nums);

    std::cout << "Sorted array:\n";
    for (int num : nums) std::cout << num << " ";
    std::cout << "\n";

    return 0;
}