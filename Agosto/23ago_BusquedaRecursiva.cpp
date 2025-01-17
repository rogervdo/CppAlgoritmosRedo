#include <iostream>
#include <vector>

int sequentialSearch(std::vector<int> nums, int valor) {

    int n = nums.size();

    for (int index = 0; index < n; index++) {
        if (nums[index] == valor) {
            return index;
        }
    }

    return -1;
}

int binarySequential(std::vector<int> nums, int valor) {

    int n = nums.size();
    int left = 0;
    int right = n - 1;

    while (left <= right) {
        int index = left + (right - left) / 2;

        if (valor == nums[index])
            return index;

        else if (valor > nums[index])
            left = index + 1;

        else
            right = index - 1;
    }

    return -1;
}

int binaryRecursive(std::vector<int> nums, int valor, int left, int right) {

    int index = left + (right - left) / 2;

    if (valor == nums[index])
        return index;

    else if (valor > nums[index])
        return binaryRecursive(nums, valor, index + 1, right);

    else
        return binaryRecursive(nums, valor, left, index - 1);
}

int binaryRecursive(std::vector<int> nums, int valor) {
    return binaryRecursive(nums, valor, 0, nums.size() - 1);
}

int main() {
    std::vector<int> numeros = {1, 2, 3, 4, 5, 11, 12, 13, 14, 15};
    std::cout << sequentialSearch(numeros, 13) << std::endl;
    std::cout << binarySequential(numeros, 13) << std::endl;
    std::cout << binaryRecursive(numeros, 13) << std::endl;
}

// Two sum
// New line.