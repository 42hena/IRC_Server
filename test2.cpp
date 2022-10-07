#include <iostream>
using namespace std;
#define MAX 101
int dist[MAX][MAX];
#include <vector>

void init(int n)
{
	for (int i = 1; i < n + 1; ++i)
	{
		for (int j = 1; j < n + 1; ++j)
		{
			if (i != j)
				dist[i][j] = 1e6;
			else
				dist[i][j] = 0;
		}
	}
}

void FloydWarshall(int n)
{
	for (int k = 1; k < n + 1; ++k)
		for (int i = 1; i < n + 1; ++i)
			for (int j = 1; j < n + 1; ++j)
				dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
}

void print(int n)
{
    for (int i = 1; i < n + 1; ++i)
    {
        for (int j = 1; j < n + 1; ++j)
            printf("%10d", dist[i][j]);
        printf("\n");
    }
}

int main()
{
    while (1)
    {
	    int n, m, s, g1, g2;
        cin >>n >> m >> s >> g1 >> g2;
        if (!n && !m && !s && !g1 && !g2)
        {
            break;    
        }
        
        init(n);
            for (int i = 0; i < m ; ++i)
            {
                int v1, v2, d;
                cin >> v1 >> v2 >> d;
                dist[v1][v2] = d;
                // dist[v2][v1] = d;
            }
            FloydWarshall(n);
            int min_value = 1e9;
            for (int i = 1 ; i < n + 1 ; ++i)
            {
                min_value =  min(min_value, dist[s][i] + dist[i][g1] + dist[i][g2]);
            }
            // print(n);
            cout << min_value << '\n';
    }
}