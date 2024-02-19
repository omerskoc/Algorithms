#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Partition subroutine
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    
    // Divide the array based on the pivot value
    for (int j = low; j <= high - 1; ++j) {
        if (arr[j] < pivot) {
            ++i;
            swap(arr[i], arr[j]);
        }
    }
    
    // Place the pivot element in its correct position
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Quicksort algorithm
void quicksort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        
        // Recursive calls for the partitions before and after the pivot element
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

// Randomized partition subroutine
int randomizedPartition(int arr[], int low, int high) {
    srand(time(0));
    
    // Randomly select a pivot index and swap it with the last element
    int random = low + rand() % (high - low + 1);
    swap(arr[random], arr[high]);
    return partition(arr, low, high);
}

// Randomized quicksort algorithm
void randomizedQuicksort(int arr[], int low, int high) {
    if (low < high) {
        int pi = randomizedPartition(arr, low, high);
        
        // Recursive calls for the partitions before and after the pivot element
        randomizedQuicksort(arr, low, pi - 1);
        randomizedQuicksort(arr, pi + 1, high);
    }
}

int main() {
    int A[100];
    for (int i = 0; i < 100; ++i) {
        A[i] = i + 1;
    }
    
    // Display the initial array
    cout << "Initial Array: ";
    for (int i = 0; i < 100; ++i) {
        cout << A[i] << " ";
    }
    cout << endl;

    // Run randomized quicksort 5 times and report the running times and sorted arrays
    for (int i = 0; i < 5; ++i) {
        clock_t start = clock();
        
        // Perform randomized quicksort
        randomizedQuicksort(A, 0, 99);
        clock_t end = clock();
        double elapsedTime = double(end - start) / CLOCKS_PER_SEC;

        // Display the running time of the current iteration
        cout << "Running Time " << i + 1 << ": " << elapsedTime << " seconds" << endl;

    }

    return 0;
}