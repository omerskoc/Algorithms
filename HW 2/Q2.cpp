#include <iostream>
#include <algorithm> 
#include <random>    

using namespace std;

// Heapify Function
void heapify(int arr[], int N, int i)
{
    int largest = i; // Initialize the largest as the root
    int left = 2 * i + 1; // Left child
    int right = 2 * i + 2; // Right child

    // If the left child is larger than the root
    if (left < N && arr[left] > arr[largest])
        largest = left;

    // If the right child is larger than the largest so far
    if (right < N && arr[right] > arr[largest])
        largest = right;

    // If the largest is not the root
    if (largest != i)
    {
        // Swap the root with the largest element
        swap(arr[i], arr[largest]);
        // Apply heapify
        heapify(arr, N, largest);
    }
}

// Function to perform heapsort 
void heapSort(int arr[], int N)
{
    // Build a heap (rearrange the array)
    for (int i = N / 2 - 1; i >= 0; i--)
        heapify(arr, N, i);

    // One by one extract elements from the heap
    for (int i = N - 1; i > 0; i--)
    {
        // Move the current root to the end
        swap(arr[0], arr[i]);

        // Call heapify on the reduced heap
        heapify(arr, i, 0);
    }
}

int main()
{
    // Generate a random permutation of A={1, 2, 3, ..., 99, 100}
    int A[100];
    for (int i = 0; i < 100; ++i)
    {
        A[i] = i + 1;
    }

    // Shuffle the array to get a random permutation
    random_device rd;
    default_random_engine rng(rd());
    shuffle(begin(A), end(A), rng);

    // Print the random permutation
    cout << "Random Permutation: ";
    for (int i = 0; i < 100; ++i)
    {
        cout << A[i] << " ";
    }
    cout << endl;

    // Perform heap sort on the random permutation
    heapSort(A, 100);

    // Print the sorted array
    cout << "Sorted Array using Heap Sort: ";
    for (int i = 0; i < 100; ++i)
    {
        cout << A[i] << " ";
    }
    cout << endl;

    return 0;
}