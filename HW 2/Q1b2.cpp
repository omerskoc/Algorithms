#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Three-way Partitioning Algorithm
void threeWayPartition(int arr[], int low, int high, int &i, int &j) {
    // Base case: If the subarray has 1 or 0 elements, it is already sorted
    if (high - low <= 1) {
        // If the last element is smaller than the first, swap them
        if (arr[high] < arr[low])
            swap(arr[high], arr[low]);
        
        // Set i to the last element of the left partition, and j to the first element of the right partition
        i = low;
        j = high;
        return;
    }

    // Initialize mid as the first index of the subarray and pivot as the last element of the subarray
    int mid = low;
    int pivot = arr[high];

    // Traverse the subarray from left to right
    while (mid <= high) {
        // If the current element is smaller than the pivot, move it to the left partition
        if (arr[mid] < pivot)
            swap(arr[low++], arr[mid++]);
        // If the current element is equal to the pivot, move to the next element
        else if (arr[mid] == pivot)
            mid++;
        // If the current element is larger than the pivot, move it to the right partition
        else if (arr[mid] > pivot)
            swap(arr[mid], arr[high--]);
    }

    // Set i to the last element of the left partition, and j to the first element of the right partition
    i = low - 1;
    j = mid;
}

// Quicksort Algorithm with Three-way Partitioning
void quicksort(int arr[], int low, int high) {
    // Base case: If the subarray has 1 or 0 elements, it is already sorted
    if (low >= high) {
        return;
    }

    int i, j;
    // Perform Three-way Partitioning and get the indices of the partitions
    threeWayPartition(arr, low, high, i, j);
    
    // Recursively sort the left partition (elements smaller than the pivot)
    // The left partition is from index 'low' to 'i'
    quicksort(arr, low, i);

    // Recursively sort the right partition (elements greater than the pivot)
    // The right partition is from index 'j' to 'high'
    quicksort(arr, j, high);
}

int main() {
    // Examples
    int A1[18] = {3, 3, 3, 3, 3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 3, 3, 3, 3};
    int A2[18] = {9, 5, 2, 9, 9, 7, 8, 9, 5, 5, 5, 1, 5, 6, 2, 4, 8, 2};
    int A3[18] = {4, 2, 3, 5, 1, 1, 1, 6, 9, 9, 9, 9, 9, 6, 7, 8, 3, 2};

    // Display Example Input 1
    cout << "Example Input 1: ";
    for (int i = 0; i < 18; ++i) {
        cout << A1[i] << " ";
    }
    cout << endl;

    // Sort Example Input 1 using Three-way Quicksort and display sorted Example Input 1
    quicksort(A1, 0, 17);
    
    cout << "Sorted Output 1: ";
    for (int i = 0; i < 18; ++i) {
        cout << A1[i] << " ";
    }
    cout << endl << endl; 

    // Display Example Input 2
    cout << "Example Input 2: ";
    for (int i = 0; i < 18; ++i) {
        cout << A2[i] << " ";
    }
    cout << endl;

    // Sort Example Input 2 using Three-way Quicksort and display sorted Example Input 2
    quicksort(A2, 0, 17);

    cout << "Sorted Output 2: ";
    for (int i = 0; i < 18; ++i) {
        cout << A2[i] << " ";
    }
    cout << endl << endl; 
    
     // Display Example Input 3
    cout << "Example Input 3: ";
    for (int i = 0; i < 18; ++i) {
        cout << A3[i] << " ";
    }
    cout << endl;

    // Sort Example Input 3 using Three-way Quicksort and display sorted Example Input 3
    quicksort(A3, 0, 17);

    cout << "Sorted Output 3: ";
    for (int i = 0; i < 18; ++i) {
        cout << A3[i] << " ";
    }
    cout << endl;

    return 0;
}