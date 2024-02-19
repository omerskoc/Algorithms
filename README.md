# EECE 7205: Fundamentals of Computer Engineering

## Homework 1:
1. **Question 1:** Coding problems involving insertion sort and mergesort algorithms. Determine the input size where mergesort begins to outperform insertion sort in terms of worst-case running time.
2. **Question 2:** Demonstration of array arrangement during each iteration of insertion sort and quicksort's Partition subroutine for a given array.
3. **Question 3:** True or False statements related to asymptotic notation (Big O, Omega, Theta).
4. **Question 4:** Application of the master method to determine the time complexity of given recurrence relations.
5. **Question 5:** Extra credit question involving drawing a recursion tree for a given recurrence relation and proving the obtained time complexity using the substitution method.

## Homework 2:
1. **Question 1.a:** Implementation of Quicksort and Randomized Quicksort algorithms. Report the running times of randomized quicksort for input array A = {1, 2, 3, ..., 99, 100}.
2. **Question 1.b:** Revision of quicksort algorithms to handle repeated elements. Describe the revisions made and report results for example inputs with repeated elements.
3. **Question 2:** Implementation of Heapsort algorithm. Generate and print a random permutation of input array A = {1, 2, 3, ..., 99, 100} before sorting.
4. **Question 3:** Implementation of Counting Sort algorithm for input array A = {20, 18, 5, 7, 16, 10, 9, 3, 12, 14, 0}.
5. **Question 4:** Implementation of Radix Sort algorithm using counting sort for decimal digits. Input array A = {329, 457, 657, 839, 436, 720, 353}.

## Homework 3:
1. **Question 1:** Implementation of Prim's algorithm for Minimum Spanning Tree (MST).
   - Version 1: Using adjacency matrix to represent the graph and an unsorted array for priority queue Q.
   - Version 2: Using adjacency lists to represent the graph and a heap for priority queue Q.
2. **Question 2:** Implementation of Johnsen’s algorithm using an unsorted array for priority queue Q. This algorithm involves Dijkstra’s and Bellman-Ford algorithms, so implementations and testing for those two algorithms are required beforehand.

## Project 1:
You are given an input array 𝐴[1, … , 𝑁]. A grouping of the array 𝐴 is described by an array 𝐺[1, … , 𝑀], where the array 𝐴 is partitioned into 𝑀 groups. Define array 𝐵[1, … , 𝑀] such that 𝐵[𝑗] is the summation of the elements in the 𝑗-th group of array 𝐴. Use a dynamic programming algorithm to find a grouping of array 𝐴 with 𝑀 groups such that we maximize the minimum element of array 𝐵.

## Project 2:
Implement and test the proposed algorithms in the "Task Scheduling" paper. Set up T_max in task migration around 1.5X of T_total of initial scheduling.
1. Prepare your report using PowerPoint and submit it as a PDF. Include pictures, screenshots, and text in your report.
2. Test your codes with 5 input examples as described below:
   - Example 1: Exact same task graph and execution time as Figure 1 of the paper.
   - Example 2: Same number of tasks as Figure 1 but change task connections.
   - Example 3: Increase task number to 20 and redesign the task graph.
   - Example 4: Redesign the task graph using multiple entry tasks based on Example 3.
   - Example 5: Redesign the task graph using multiple entry and multiple exit tasks based on Example 3.
3. For each input example, include in your report:
   - Figure of the input example (task graph and execution time table).
   - Program output screenshot of initial scheduling, visualized initial scheduling result, total energy consumption of initial scheduling from your program, and manual calculation of total energy consumption.
   - Program output screenshot of final scheduling, visualized final scheduling result, total energy consumption of final scheduling from your program, and manual calculation of total energy consumption.
   - Summary of T_total and E_total of initial and final scheduling.
4. Analyze any differences in the initial scheduling result compared to the paper.
