//
// Created by HUSTW on 2017/11/19.
//

#include <iostream>
#include <vector>
#include <windows.h>

using namespace std;

struct Point {
    Point() = default;
    Point(int x, int y): x(x), y(y) {}
    int x;
    int y;
};

int main() {

    string x("adaadadadadsssdjaijiaiciajdidadadjcaicioabvdoouiddlskadajpjpipajdpdjpapivi"
                     "daaadadadadadadadadafsdadadadadadadadadaddadadadvasasvafaasvxaaxsa"
                     "xxvvsjidsjdpssdfffdcddadadadadadjjcaocjokpppkjifaslaasdasaddabfd");
    string y("ffsaaadffffffxmasidjiasdaspdaspdpapvfaddaffffffgdbgbqpqpppcckpsasvsdvdsvvv"
                     "feepidjpdapdasdaadadpdsapasdspaodasvdiasjiivjifasfffffffgsddasdaadada"
                     "dasdadadaddadsifokfbfpgppgpbfbpgpodsfpffasdadacasxsxd");
    int M = x.length();
    int N = y.length();

    // define the delta value of each letter in alphabet (a~z)
    int delta[26];
    for (int &i: delta)
        i = 30;

    // define alpha value of each pair
    int alpha[26][26];
    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < 26; ++j) {
            alpha[i][j] = abs(i-j);
        }
    }

    SYSTEMTIME st;
    GetSystemTime(&st);
    int start_time = st.wMilliseconds;
    //////////////////// Dynamic programming ///////////////////////
    ///  The following codes is a solution by dynamic programming

    // to store tmp optimal value
    int tmp_opt[M+1][N+1];

    // initiate the first line
    tmp_opt[0][0] = 0;
    for (int i = 1; i < N+1; ++i) {
        tmp_opt[i][0] = delta[x[i-1] - 'a'] + tmp_opt[i-1][0];
        tmp_opt[0][i] = delta[y[i-1] - 'a'] + tmp_opt[0][i-1];
    }

    // for each i and j, fill the table tmp_opt
    for (int i = 1; i <= M; ++i) {
        for (int j = 1; j <= N; ++j) {
            int p1 = tmp_opt[i-1][j-1] + alpha[x[i] - 'a'][y[j] - 'a'];
            int p2 = tmp_opt[i-1][j] + delta[x[i] - 'a'];
            int p3 = tmp_opt[i][j-1] + delta[y[j] - 'a'];
            tmp_opt[i][j] = min(min(p1, p2), p3);
        }
    }
    /*
    // print the last value
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("%3d", tmp_opt[i][j]);
        }
        printf("\n");
    }*/

    //////////////////////////////////////////////////////////////////
    GetSystemTime(&st);
    int stop_time = st.wMilliseconds;
    cout << "The result of dp is " << tmp_opt[M][N] << endl;
    cout << "Dynamic Programming cost " << stop_time - start_time << " ms" << endl;


    GetSystemTime(&st);
    start_time = st.wMilliseconds;
    ////////////////////////// Shortest Path /////////////////////////
    /// The following code is a solution of ST algorithm

    // to store all distances
    int dist[M+1][N+1];
    vector<Point> Q;
    for (int i = 0; i < M+1; ++i) {
        for (int j = 0; j < N+1; ++j) {
            dist[i][j] = INT32_MAX;
            Q.emplace_back(Point(i, j));
        }
    }
    dist[0][0] = 0;

    // dijkstra
    while (!Q.empty()) {
        // find one vertex in Q with min distance: u
        Point u = Q[0];
        auto it = Q.begin();
        while (it != Q.end()) {
            auto p = *it;
            if (dist[p.x][p.y] <= dist[u.x][u.y]) {
                u = p;
                it = Q.erase(it);
                break;
            } else it++;
        }

        if (u.x < M)
            dist[u.x+1][u.y] = min(dist[u.x+1][u.y], dist[u.x][u.y] + delta[x[u.x+1] - 'a']);
        if (u.y < N)
            dist[u.x][u.y+1] = min(dist[u.x][u.y+1], dist[u.x][u.y] + delta[y[u.y+1] - 'a']);
        if (u.x < M && u.y < N)
            dist[u.x+1][u.y+1] = min(dist[u.x+1][u.y+1], dist[u.x][u.y] + alpha[x[u.x+1] - 'a'][y[u.y+1] - 'a']);
    }

    /*
    for (int i = 0; i < M+1; ++i) {
        for (int j = 0; j < N+1; ++j) {
            if (dist[i][j] == INT32_MAX)
                printf("   ");
            else printf("%3d", dist[i][j]);
        }
        printf("\n");
    }*/
    //////////////////////////////////////////////////////////////////////
    GetSystemTime(&st);
    stop_time = st.wMilliseconds;
    cout << "The result of dijkstra is " << dist[M][N] << endl;
    cout << "ST Algorithm cost " << stop_time - start_time << " ms" << endl;

    return 0;
}