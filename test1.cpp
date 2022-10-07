#include <iostream>
using namespace std;
#define MAX 201
long long dist[MAX][MAX];
#include <vector>

void init(int n)
{
	for (int i = 1; i < n + 1; ++i)
	{
		for (int j = 1; j < n + 1; ++j)
		{
			if (i != j)
				dist[i][j] = 1e10;
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

int main()
{
	// case input
	int t;
	cin >> t;
	
	int C = 0;
	while (C < t)
	{
		// city, friend, path
		int n, p, m;
		cin >> n >> p >> m;
		
		init(n);
		vector<pair<int, int> > data;
		for (int i = 0; i < p; ++i)
		{
			int x, v;
			// 출발 번호, 걸리는 시간
			cin >> x >> v;
			data.push_back({ x, v});
		}

		for (int i = 0; i < m; ++i)
		{
			long long d, count;
			cin >> d >> count;
			vector<int> path;
			for (int j = 0; j < count; ++j)
			{
				int value;
				cin >> value;
				path.push_back(value);
			}
			for (int j = 0; j < count - 1; ++j)
			{
				int v1 = path[j];
				int v2 = path[j + 1];
				dist[v1][v2] = d;
				dist[v2][v1] = d;
			}
		}
		FloydWarshall(n);
		long long minValue = 1e10;
		
		int cnt = 0;
		for (int i = 1; i < n + 1; ++i)
		{
			long long total = 0;
			for (int j = 0 ;j < p; ++j)
			{
				int s = data[j].first;
				long long cost = data[j].second;
				total += dist[s][i] * cost;
			}
			minValue = min(minValue, total);
		}

		for (int i = 1 ; i < n + 1 ; ++i)
		{
		    for (int j = 1 ; j < n + 1 ; ++j)
		    {
		        printf(" %10lld", dist[i][j]);
		    }
		    printf("\n");
		}

		cout << "Case #" << ++C << ": ";
		if (minValue == 1e10)
			cout << -1 << '\n';
		else
			cout << minValue << '\n';
	}
}

// N: 도시의 숫자
// P: 친구의 수
// M: 도로의 숫자
// 각 도시는 순서대로 1부터 N까지의 번호가 붙여져 있다.

// 또한, 1부터 P까지의 번호가 붙여져 있는 각 친구 i에 대해, 다음과 같은 것이 주어진다.

// Xi: 친구가 출발하는 도시의 번호.
// Vi: 친구가 거리 1 만큼 움직이는 데 걸리는 시간.
 

// 각 도시를 잇는 도로 j에 대해서는 다음과 같은 것이 주어진다. 도로는 단순히 두 도시를 잇는 길이 아니라, 여러 도시를 순서대로 잇는 연속된 길의 모임이다.

// Dj: 도로가 지나가는 도시들 사이의 거리. (한 도로 위에서, 인접한 도시 사이의 거리는 Dj로 같다.)
// Lj: 도로가 지나가는 도시들의 숫자
// {Cj,k}: 도로가 이어주는 도시의 번호가 순서대로 나열된다.