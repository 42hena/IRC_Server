#include <iostream>
using namespace std;


int n, m;

int dist[201][201];
int main()
{
    cin >> n >> m;

    for (int i = 1; i < n + 1 ; ++i)
    {
        for (int j = 1; j < n + 1 ; ++j)
        {
            if (i != j)
                dist[i][j] = 1e9;
        }
    }

    for (int i = 0 ; i < m ; ++i)
    {
        int a, b, t;
        cin >> a >> b >> t;
        dist[a][b] = t;
        dist[b][a] = t;
    }

    for (int i = 1; i < n + 1 ; ++i)
    {
        for (int j = 1; j < n + 1 ; ++j)
        {
            printf(" %10d", dist[i][j]);
        }
        printf("\n");
    }

    for (int k = 1; k < n + 1 ; ++k)
        for (int i = 1; i < n + 1 ; ++i)
            for (int j = 1; j < n + 1 ; ++j)
            {
                if (i == j || j == k || k == i)
                    continue;
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }

    for (int i = 1; i < n + 1 ; ++i)
    {
        for (int j = 1; j < n + 1 ; ++j)
        {
            printf(" %10d", dist[i][j]);
        }
        printf("\n");
    }
    // int k;
    // cin >> k;
    // for (int i = 0 ; i < k ; ++i)
    // {
    //     int a, b, t;
    //     cin >> a >> b >> t;
    //     dist[a][b] = t;
    //     dist[b][a] = t;
    // }
}