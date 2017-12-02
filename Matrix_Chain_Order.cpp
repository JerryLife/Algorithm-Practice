#include <iostream>

using namespace std;

int opt(int A[]) {

    const int N = 6;

    // to store the result of i~j
    int m[N][N], s[N][N];

    // initiate m with all 0
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            s[i][j] = 0;
            if (i == j)
                m[i][j] = 0;
            else m[i][j] = INT32_MAX;
        }
    }

    // for each length
    for (int l = 2; l <= N; ++l) {
        // for each starting point
        for (int i = 1; i < N - l + 1; ++i) {
            int j = i + l - 1;  // get end point
            for (int k = i; k < j; ++k) {
                int q = m[i][k] + m[k+1][j] + A[i-1] * A[k] * A[j];
                if (q < m[i][j]) {
                    m[i][j] = q;    // mark as current min value
                    s[i][j] = k;    // mark the position
                }
            }
        }
    }

    // print out the results
    cout << m[1][N-1] << endl;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << s[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    // elements of N matrix 1x2 2x5 5x6 ...
    int A[] {5, 1, 2, 3, 2, 6};
    opt(A);
    return 0;
}