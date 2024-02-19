#include <iostream>

void countingSort(int A[], int n) {
    // Find the maximum value in array A to determine the range (k)
    int k = A[0];
    for (int i = 1; i < n; ++i) {
        if (A[i] > k) {
            k = A[i];
        }
    }

    // Create a count array and initialize with zeros
    int count[k + 1] = {0};

    // Count the occurrences of each element in the input array A
    for (int j = 0; j < n; ++j) {
        count[A[j]] = count[A[j]] + 1;
    }

    // Update count array to store the actual position of elements in the output array
    for (int i = 1; i <= k; ++i) {
        count[i] = count[i] + count[i - 1];
    }

    int B[n]; // Temporary array to store the sorted output

    // Build the sorted output array using count array
    for (int j = n - 1; j >= 0; --j) {
        B[count[A[j]] - 1] = A[j];
        count[A[j]] = count[A[j]] - 1;
    }

    // Copy the sorted array back to the input array A
    for (int i = 0; i < n; ++i) {
        A[i] = B[i];
    }
}

int main() {
    // Initial input array
    int A[] = {20, 18, 5, 7, 16, 10, 9, 3, 12, 14, 0};
    int n = sizeof(A) / sizeof(A[0]);

    // Print the initial array
    std::cout << "Initial Array: ";
    for (int i = 0; i < n; ++i) {
        std::cout << A[i] << " ";
    }
    std::cout << std::endl;

    // Perform Counting Sort
    countingSort(A, n);

    // Print the sorted array
    std::cout << "Sorted Array using Counting Sort: ";
    for (int i = 0; i < n; ++i) {
        std::cout << A[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}