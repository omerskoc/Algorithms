#include <iostream>

// Function to find the maximum number to determine the number of digits
int getMax(int arr[], int n) {
    int max = arr[0];
    for (int i = 1; i < n; ++i) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

// Counting Sort function to sort the elements based on significant digit at exp
void countingSort(int arr[], int n, int exp) {
    const int BASE = 10;
    int output[n];
    int count[BASE] = {0};

    // Count the occurrences of digits at the current significant digit position
    for (int i = 0; i < n; ++i) {
        count[(arr[i] / exp) % BASE]++;
    }

    // Update count array to store the actual position of elements in output array
    for (int i = 1; i < BASE; ++i) {
        count[i] += count[i - 1];
    }

    // Build the output array using count array and update count array
    for (int i = n - 1; i >= 0; --i) {
        output[count[(arr[i] / exp) % BASE] - 1] = arr[i];
        count[(arr[i] / exp) % BASE]--;
    }

    // Copy the output array back to the original array
    for (int i = 0; i < n; ++i) {
        arr[i] = output[i];
    }
}

// Radix Sort function to sort the input array using Counting Sort for each digit
void radixSort(int arr[], int n) {
    int max = getMax(arr, n);

    // Perform counting sort for every digit, starting from the least significant digit to the most significant digit
    for (int exp = 1; max / exp > 0; exp *= 10) {
        countingSort(arr, n, exp);
    }
}

int main() {
    // Input array
    int A[] = {329, 457, 657, 839, 436, 720, 353};
    int n = sizeof(A) / sizeof(A[0]);

    // Print the initial array
    std::cout << "Initial Array: ";
    for (int i = 0; i < n; ++i) {
        std::cout << A[i] << " ";
    }
    std::cout << std::endl;
    
    // Perform Radix Sort
    radixSort(A, n);

    // Output the sorted array
    std::cout << "Sorted Array using Radix Sort: ";
    for (int i = 0; i < n; ++i) {
        std::cout << A[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}