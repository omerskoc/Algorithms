#include <iostream>
#include <vector>

using namespace std;

// Function max_min_grouping takes a vector A, representing an array of integers, 
// Two integers N and M as input.
vector<int> max_min_grouping(vector<int> A, int N, int M) {
    vector<vector<int>> dp(M, vector<int>(N, 0));
    vector<vector<int>> P(M, vector<int>(N, 0));
    vector<int> G(M, 0);

    // Base case: initialize the first row of dp.
    dp[0][0] = A[0];
    for (int i = 1; i < N; ++i) {
        dp[0][i] = dp[0][i - 1] + A[i];
    }

    // Dynamic programming to fill dp and P tables.
    for (int j = 1; j < M; ++j) {
        for (int i = j; i < N; ++i) {
            for (int k = j - 1; k <= i - 1; ++k) {
                // Calculate the minimum of two values:
                // 1. Previous group's maximum sum (dp[j - 1][k])
                // 2. Sum of elements from k+1 to i (dp[0][i] - dp[0][k])
                int temp = min(dp[j - 1][k], dp[0][i] - dp[0][k]);
                // If the temporary value is greater than the current value in dp table, update dp[j][i] and store the position in P table.
                if (temp > dp[j][i]) {
                    dp[j][i] = temp;
                    P[j][i] = k;
                }
            }
        }
    }
    
    // Calculate the group sizes based on the positions stored in the P table.
    int idx = N - 1;
    for (int j = M - 1; j >= 0; --j) {
        G[j] = (idx) - P[j][idx];
        idx = P[j][idx];
    }
    G[0] = G[0] + 1;

    // Return the final group sizes.
    return G;
}

// The main function takes user input for the array size, elements, and number of groups.
int main() {
    while (true) {
        int N, M;
        cout << "Please enter the size of the array(N): ";
        cin >> N;
    
        vector<int> A(N);
        cout << "Please enter the array elements separated by spaces: ";
        for (int i = 0; i < N; ++i) {
            cin >> A[i];
        }
    
        cout << "Please enter the number of groups(M): ";
        cin >> M;
    
        // Call the max_min_grouping function and get the group sizes.
        vector<int> G = max_min_grouping(A, N, M);
    
        // Output the group sizes.
        cout << "Group sizes: ";
        for (int i = 0; i < G.size(); ++i) {
            cout << G[i] << " ";
        }
        cout << endl;
    }
    return 0;
}