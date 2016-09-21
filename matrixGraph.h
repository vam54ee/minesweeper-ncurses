#include<iostream>
#include<vector>
#include<stack>
#include<queue>
#include<cstdlib>
#include <stdexcept>
#include <limits>
using namespace std;

template<typename T>class matrixGraph {
  unsigned int m,n;
  vector<T> graph;

  public:
    matrixGraph( unsigned int x = 1, unsigned int y = 1, T a = NULL) : m(x), n(y) {
      graph = vector<T>(m * n, a);
    };



    T& operator ()(unsigned int index1, unsigned int index2) {
      if (index1 >= m || index2 >= n) {
        throw out_of_range("both indexes should be in range.");
      }
      return graph[index1*m + index2];
    }

    void print() {
      for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < n; j++) {
          cout<<graph[i * m + j]<<"\t";
        }
        cout<< endl;
      }
    }

    static void printValue(T& a){ cout<<a<<endl;}

    void printVector() {
      for (size_t i = 0; i < m * n; i++) {
        cout<<graph[i]<<"\t";
      }
      cout<< endl;
    }

    void bfs(unsigned int index1, unsigned int index2, void(* callback)(T&) = printValue, bool topDown = true, bool crossEdges = true) {
      if (index1 >= m || index2 >= n) {
        throw out_of_range("both indexes should be in range.");
      }

      enum color { WHITE, GREY, BLACK };
      vector<color> visited = vector<color>(m * n, WHITE);
      queue<int> Q;
      stack<int> S;
      visited[index1*m + index2] = GREY;
      Q.push(index1*m + index2);
      while (Q.size() != 0) {
        int current = Q.front();
        Q.pop();
        int currentIndex1 = current / m, currentIndex2 = current % m;

        if (currentIndex1 - 1 >= 0 && currentIndex2 - 1 >= 0 && crossEdges && visited[(currentIndex1 - 1)*m + (currentIndex2 - 1)] == WHITE) {
          visited[(currentIndex1 - 1) * m + currentIndex2 - 1] = GREY;
          Q.push((currentIndex1 - 1) * m + currentIndex2 - 1);
        }
        if (currentIndex1 - 1 >= 0 && visited[(currentIndex1 - 1)*m + (currentIndex2)] == WHITE) {
          visited[(currentIndex1 - 1) * m + currentIndex2] = GREY;
          Q.push((currentIndex1 - 1) * m + currentIndex2);
        }
        if (currentIndex1 - 1 >= 0 && currentIndex2 + 1 < n && crossEdges && visited[(currentIndex1 - 1)*m + (currentIndex2 + 1)] == WHITE) {
          visited[(currentIndex1 - 1) * m + currentIndex2 + 1] = GREY;
          Q.push((currentIndex1 - 1) * m + currentIndex2 + 1);
        }
        if (currentIndex2 - 1 >= 0 && visited[(currentIndex1)*m + (currentIndex2 - 1)] == WHITE) {
          visited[currentIndex1 * m + currentIndex2 - 1] = GREY;
          Q.push(currentIndex1 * m + currentIndex2 - 1);
        }
        if (currentIndex2 + 1 < n && visited[(currentIndex1)*m + (currentIndex2 + 1)] == WHITE) {
          visited[currentIndex1 * m  + currentIndex2 + 1] = GREY;
          Q.push(currentIndex1 * m  + currentIndex2 + 1);
        }
        if (currentIndex1 + 1 < m && currentIndex2 - 1 >= 0 && crossEdges && visited[(currentIndex1 + 1)*m + (currentIndex2 - 1)] == WHITE) {
          visited[(currentIndex1 + 1) * m + currentIndex2 - 1] = GREY;
          Q.push((currentIndex1 + 1) * m + currentIndex2 - 1);
        }
        if (currentIndex1 + 1 < m && visited[(currentIndex1 + 1)*m + (currentIndex2)] == WHITE) {
          visited[(currentIndex1 + 1) * m + currentIndex2] = GREY;
          Q.push((currentIndex1 + 1) * m + currentIndex2);
        }
        if (currentIndex1 + 1 < m && currentIndex2 + 1 < n && crossEdges && visited[(currentIndex1 + 1)*m + (currentIndex2 + 1)] == WHITE) {
          visited[(currentIndex1 + 1) * m + currentIndex2 + 1] = GREY;
          Q.push((currentIndex1 + 1) * m + currentIndex2 + 1);
        }
        if (topDown) callback(graph[currentIndex1 * m + currentIndex2]);
        else S.push(currentIndex1 * m + currentIndex2);
        visited[current] = BLACK;
      }
      while (S.size() != 0) {
        callback(graph[S.top()]);
        S.pop();
      }

    }


    void dfs(int index1, int index2, void(* callback)(T&) = printValue, bool topDown = true, bool crossEdges = true) {
      cout<<index1<<"\t"<<index2<<endl;
      static vector<bool> visited = vector<bool>(m * n, false);
      if (topDown) callback(graph[index1 * m + index2]);
      visited[index1 * m + index2] = true;
      if (index1 - 1 >= 0 && index2 - 1 >= 0 && crossEdges && !visited[(index1 - 1)*m + (index2 - 1)]) {
        dfs(index1 - 1, index2 - 1, callback, topDown, crossEdges);
      }
      if (index1 - 1 >= 0 && !visited[(index1 - 1)*m + (index2)]) {
        dfs(index1 - 1, index2, callback, topDown, crossEdges);
      }
      if (index1 - 1 >= 0 && index2 + 1 < n && crossEdges && !visited[(index1 - 1)*m + (index2 + 1)]) {
        dfs(index1 - 1, index2 + 1, callback, topDown, crossEdges);
      }
      if (index2 - 1 >= 0 && !visited[(index1)*m + (index2 - 1)]) {
        dfs(index1, index2 - 1, callback, topDown, crossEdges);
      }
      if (index2 + 1 < n && !visited[(index1)*m + (index2 + 1)]) {
        dfs(index1, index2 + 1, callback, topDown, crossEdges);
      }
      if (index1 + 1 < m && index2 - 1 >= 0 && crossEdges && !visited[(index1 + 1)*m + (index2 - 1)]) {
        dfs(index1 + 1, index2 - 1, callback, topDown, crossEdges);
      }
      if (index1 + 1 < m && !visited[(index1 + 1)*m + (index2)]) {
        dfs(index1 + 1, index2, callback, topDown, crossEdges);
      }
      if (index1 + 1 < m && index2 + 1 < n && crossEdges && !visited[(index1 + 1)*m + (index2 + 1)]) {
        dfs(index1 + 1, index2 + 1, callback, topDown, crossEdges);
      }
      if (!topDown) callback(graph[index1 * m + index2]);
    }

    vector<double> dijkstra(unsigned int index1, unsigned int index2, bool crossEdges = true) {
      if (index1 >= m || index2 >= n) {
        throw out_of_range("both indexes should be in range.");
      }
      double inf = numeric_limits<double>::infinity();
      int count = m * n;
      vector<double> distanceArray = vector<double>(m*n, inf);
      vector<bool> visitedArray = vector<bool>(m*n, false);
      vector<int> parentArray = vector<int>(m*n, -1);
      distanceArray[index1 * m + index2] = 0;
      while (count != 0) {
        int index = 0;
        double dist = inf;
        int x = 0;
        int y = 0;
        for (size_t i = 0; i < m * n; i++) {
          if (!visitedArray[i] && distanceArray[i] < dist) {
            index = i;
            dist = distanceArray[index];
          }
        }
        visitedArray[index] = true;
        count--;
        x = index / m;
        y = index - x * m;
        double distanceIndex = distanceArray[index] + graph[index];

        if (x - 1 >= 0 && y - 1 >= 0 && crossEdges && !visitedArray[(x - 1)*m + (y - 1)]) {
          if (distanceArray[(x - 1)*m + (y - 1)] > distanceIndex) {
            parentArray[(x - 1)*m + (y - 1)] = index;
            distanceArray[(x - 1)*m + (y - 1)] = distanceIndex;
          }
        }
        if (x - 1 >= 0 && !visitedArray[(x - 1)*m + (y)]) {
          if (distanceArray[(x - 1)*m + (y)] > distanceIndex) {
            parentArray[(x - 1)*m + (y)] = index;
            distanceArray[(x - 1)*m + (y)] = distanceIndex;
          }
        }
        if (x - 1 >= 0 && y + 1 < n && crossEdges && !visitedArray[(x - 1)*m + (y + 1)]) {
          if (distanceArray[(x - 1)*m + (y + 1)] > distanceIndex) {
            parentArray[(x - 1)*m + (y + 1)] = index;
            distanceArray[(x - 1)*m + (y + 1)] = distanceIndex;
          }
        }
        if (y - 1 >= 0 && !visitedArray[(x)*m + (y - 1)]) {
          if (distanceArray[(x)*m + (y - 1)] > distanceIndex) {
            parentArray[(x)*m + (y - 1)] = index;
            distanceArray[(x)*m + (y - 1)] = distanceIndex;
          }
        }
        if (y + 1 < n && !visitedArray[(x)*m + (y + 1)]) {
          if (distanceArray[(x)*m + (y + 1)] > distanceIndex) {
            parentArray[(x)*m + (y + 1)] = index;
            distanceArray[(x)*m + (y + 1)] = distanceIndex;
          }
        }
        if (x + 1 < m && y - 1 >= 0 && crossEdges && !visitedArray[(x + 1)*m + (y - 1)]) {
          if (distanceArray[(x + 1)*m + (y - 1)] > distanceIndex) {
            parentArray[(x + 1)*m + (y - 1)] = index;
            distanceArray[(x + 1)*m + (y - 1)] = distanceIndex;
          }
        }
        if (x + 1 < m && !visitedArray[(x + 1)*m + (y)]) {
          if (distanceArray[(x + 1)*m + (y)] > distanceIndex) {
            parentArray[(x + 1)*m + (y)] = index;
            distanceArray[(x + 1)*m + (y)] = distanceIndex;
          }
        }
        if (x + 1 < m && y + 1 < n && crossEdges && !visitedArray[(x + 1)*m + (y + 1)]) {
          if (distanceArray[(x + 1)*m + (y + 1)] > distanceIndex) {
            parentArray[(x + 1)*m + (y + 1)] = index;
            distanceArray[(x + 1)*m + (y + 1)] = distanceIndex;
          }
        }
      }
      return distanceArray;
    }

};
// int count = 10;
// void print(char& a) { cout<<a<<endl;}
// void insertMine(uint8_t& a) {
//   if (count > 0 && a != 99 && rand() % 2 == 0) {
//     a = 99;
//     count--;
//   }
// }

// int main() {
//   cout<<"hi"<<endl;
//   int m = 5, n = 5;
//
//   matrixGraph<int> a = matrixGraph<int>(m, n, 98);
//   for (int i = 0; i < m; i++) {
//     for (int j = 0; j < n; j++) {
//       a(i,j) = i*m + j;
//     }
//   }
//
//   a.print();
//   a.printVector();
//   // cout<<"bfs top-down"<<endl;
//   // a.bfs(3,2, insertMine);
//   // a.print();
//   // cout<<"bfs bottom-up"<<endl;
//   // a.bfs(3,2, print, false);
//   // cout<<"dfs topdown"<<endl;
//   // a.dfs(3,2, print);
//   // cout<<"dfs bottom up"<<endl;
//   // a.dfs(3,2, print, false);
//
//   vector<double> dist = a.dijkstra(3, 2, false);
//
//   for (int i = 0; i < m; i++) {
//     for (int j = 0; j < n; j++) {
//       cout<<dist[i*m + j]<<"\t";
//     }
//     cout<<endl;
//   }
//
//
//   return 0;
//
// }
