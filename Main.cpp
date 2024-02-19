#include <iostream>

using namespace std;

// Insertion Sort Algorithm
void insertionSort(int arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i]; // Copy the i-th element of the array to the 'key' variable.
        j = i - 1; // Start moving backward from the previous element.

        // Shift elements that are greater than 'key' to the right.
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j]; // Move the element one position to the right.
            j = j - 1; // Move to the previous element.
        }
        arr[j + 1] = key; // Place 'key' in its correct position (sorted order).
    }
}

// Merge Sort Algorithm
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1; // Calculate size of left subarray.
    int n2 = right - mid;    // Calculate size of right subarray.

    int Left[n1], Right[n2]; // Create temporary arrays for subarrays.

    // Copy data from the original array to left and right subarrays.
    for (int i = 0; i < n1; i++)
        Left[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        Right[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left; // Initialize indices for merging.

    // Merge left and right subarrays back into the original array.
    while (i < n1 && j < n2) {
        if (Left[i] <= Right[j]) {
            arr[k] = Left[i]; i++; // Place smaller element from left or right subarray
        } else {
            arr[k] = Right[j]; j++;
        }
        k++; // Move to the next position in the original array.
    }

    // Copy any remaining elements in left or right subarrays to the original array.
    while (i < n1) {
        arr[k] = Left[i]; i++; k++;
    }

    while (j < n2) {
        arr[k] = Right[j]; j++; k++;
    }
}


void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        // Calculate the middle point to divide the array into two subarrays.
        int mid = left + (right - left) / 2;

        // Recursively call mergeSort to sort the left subarray.
        mergeSort(arr, left, mid);
        
        // Recursively call mergeSort to sort the right subarray.
        mergeSort(arr, mid + 1, right);

        // Merge the two sorted subarrays.
        merge(arr, left, mid, right);
    }
}


int main() {
    const int maxN = 2500; // Set the maximum value of n
    int arr[maxN];

    cout << "n\tInsertion Sort (time)\tMerge Sort (time)\n";

    for (int n = 1; n <= maxN; n *= 4) {
        // Generate a worst-case scenario input array
        for (int i = 0; i < n; i++) {
            arr[i] = n - i;
        }

        // Measure insertion sort time
        clock_t insertionStart = clock();
        insertionSort(arr, n);
        clock_t insertionStop = clock();
        double insertionDuration = (double)(insertionStop - insertionStart) / CLOCKS_PER_SEC;

        // Reset the array
        for (int i = 0; i < n; i++) {
            arr[i] = n - i;
        }

        // Measure merge sort time
        clock_t mergeStart = clock();
        mergeSort(arr, 0, n - 1);
        clock_t mergeStop = clock();
        double mergeDuration = (double)(mergeStop - mergeStart) / CLOCKS_PER_SEC;

        cout << n << "\t" << insertionDuration << "\t\t\t" << mergeDuration << endl;
    }

    return 0;
}