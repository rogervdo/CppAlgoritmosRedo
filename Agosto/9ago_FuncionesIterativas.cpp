#include <iostream>

int sumaIterativa(int n) {
    int totalSum = 0;

    for (int i = 1; i <= n; i++) {
        //std::cout << i << std::endl;
        totalSum += i;
    }

    //std::cout << "Suma total = " << totalSum << std::endl;
    return totalSum;
}

int sumaRecursiva(int n) {
    if (n <= 0) {
        std::cout << "Invalid Input Iterativo" << std::endl;
        return -1;
    }
    else if (n == 1) {
        return 1;
    }
    else {
        return n + sumaRecursiva(n - 1);
    }
}

int sumaDirecta(int n) {
    int totalSuma = n * (n + 1) / 2;
    return totalSuma;
}

int main() {
    std::cout << "Hello" << std::endl;
    std::cout << sumaIterativa(4) << std::endl;
    std::cout << sumaRecursiva(4) << std::endl;
    std::cout << sumaDirecta(4) << std::endl;
}