
/*
Croatia 2003
National Competition - First day - Juniors
Problem TELEPORT, Programming language C++
*/

#include <cstdio>
#include <list>

#define INF 1000000000
#define MAX 200

using namespace std;

int n;
int g[MAX][MAX];
list< pair< int, int > > l;
typedef list< pair< int, int > >::iterator LI;

void input() {
   int i, m, a, b, t;
   scanf( "%d%d", &n, &m );
   for( a = 0; a < n; ++a ) {
      for( b = 0; b < n; ++b ) {
         g[a][b] = INF;
         if( a == b ) g[a][b] = 0;
         if( a < b ) l.push_back( {a, b});
      }
   }
   for( i = 0; i < m; ++i ) {
      scanf( "%d%d%d", &a, &b, &t ); a--; b--;
      g[a][b] = g[b][a] = t;
   }
}

void floyd_warshall_all_pairs_shortest_path(){
   int i, j, k;
   for( k = 0; k < n; ++k )
      for( i = 0; i < n; ++i )
         for( j = 0; j < n; ++j )
            if( g[i][j] > g[i][k] + g[k][j] )
               g[i][j] = g[i][k] + g[k][j];
}

void solve() {
   int i, m, a, b, t, d;
   scanf("%d", &m);
   for( i = 0; i < m; ++i )
   {
      scanf( "%d%d%d", &a, &b, &t ); a--; b--;
      for( LI tmp, j = l.begin(); j != l.end(); )
      {
         d = g[a][b];
         if( d > g[a][(*j).first] + g[(*j).second][b] )
             d = g[a][(*j).first] + g[(*j).second][b];
         if( d > g[a][(*j).second] + g[(*j).first][b] )
             d = g[a][(*j).second] + g[(*j).first][b];
         tmp = j++;
         if( d != t ) l.erase( tmp );
      }
   }
}

int main( void )
{
   input();
   floyd_warshall_all_pairs_shortest_path();
   solve();
   printf( "%d %d\n", l.front().first + 1, l.front().second + 1 );

   return 0;
}
