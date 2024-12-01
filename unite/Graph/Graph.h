#include <iostream>
#include <climits>
#include <queue>
#include <vector>
#include <stack>
#include <functional>
using namespace std;

// 图状态和边的状态定义
typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus;
typedef enum { E_UNDISCOVERED, TREE, CROSS, FORWARD, BACKWARD } EStatus;

// 顶点类
template<typename Tv>
struct Vertex {
    Tv data; 
    int inDegree, outDegree; 
    VStatus status;
    int dTime, fTime;
    int parent;
    int priority;

    Vertex(Tv const& d = Tv()) 
        : data(d), inDegree(0), outDegree(0), 
          status(UNDISCOVERED), dTime(-1), fTime(-1), parent(-1), priority(INT_MAX) {}
};

// 边类
template <typename Te>
struct Edge {
    Te data;  // 边的权重或其他信息
    int weight;  // 边的权重
    EStatus status;  // 边的状态

    Edge(Te const& d = Te(), int w = 0)
        : data(d), weight(w), status(E_UNDISCOVERED) {}
};

// 基础图类
template<typename Tv, typename Te>
class Graph {
protected:
    virtual void reset() = 0;
    virtual void BFS(int, int&) = 0;
    virtual void DFS(int, int&) = 0;
     virtual void PFS(int, function<void(int, int)> updateFunc) = 0; 

public:
    int n;  // 顶点数
    int e;  // 边数

    virtual int insert(Tv const&) = 0;
    virtual Tv remove(int) = 0;
    virtual Tv& vertex(int) = 0;
    virtual int inDegree(int) = 0;
    virtual int outDegree(int) = 0;
    virtual int firstNbr(int) = 0;
    virtual int nextNbr(int, int) = 0;
    virtual VStatus& status(int) = 0;
    virtual int& dTime(int) = 0;
    virtual int& fTime(int) = 0;
    virtual int& parent(int) = 0;
    virtual int& priority(int) = 0;

    virtual bool exists(int, int) = 0;
    virtual void insert(Te const&, int, int, int) = 0;
    virtual Te remove(int, int) = 0;    
    virtual EStatus& status(int, int) = 0;
    virtual Te& edge(int, int) = 0;
    virtual int& weight(int, int) = 0;
    void bfs(int);
    void dfs(int);
    void prim(int);
    void dijkstra(int);
};

// 邻接矩阵图类实现
template<typename Tv, typename Te>
class GraphMatrix : public Graph<Tv, Te> {
private:
    vector<Vertex<Tv>> V;  // 顶点数组
    vector<vector<Edge<Te>*>> E;  // 邻接矩阵（每个位置存储指向边的指针）

public:
    GraphMatrix() { this->n = this->e = 0; }
    ~GraphMatrix() {
        for (int j = 0; j < this->n; j++)
            for (int k = 0; k < this->n; k++)
                delete E[j][k];
    }

    virtual Tv& vertex(int i) { return V[i].data; } 
    virtual int inDegree(int i) { return V[i].inDegree; } 
    virtual int outDegree(int i) { return V[i].outDegree; } 
    virtual int firstNbr(int i) { return nextNbr(i, this->n); } 
    virtual int nextNbr(int i, int j) { while ((-1 < j) && (!exists(i, --j))); return j; } 
    virtual VStatus& status(int i) { return V[i].status; } 
    virtual int& dTime(int i) { return V[i].dTime; } 
    virtual int& fTime(int i) { return V[i].fTime; } 
    virtual int& parent(int i) { return V[i].parent; } 
    virtual int& priority(int i) { return V[i].priority; } 

    virtual int insert(Tv const& vertex) { 
        for (int j = 0; j < this->n; j++) E[j].push_back(nullptr); 
        this->n++; 
        E.push_back(vector<Edge<Te>*>(this->n, nullptr)); 
        V.push_back(Vertex<Tv>(vertex)); 
        return this->n - 1;
    }

    virtual Tv remove(int i) { 
        for (int j = 0; j < this->n; j++) 
            if (exists(i, j)) { delete E[i][j]; V[j].inDegree--; } 
        E.erase(E.begin() + i); 
        V.erase(V.begin() + i); 
        this->n--; 
        for (int j = 0; j < this->n; j++) 
            if (exists(j, i)) { delete E[j][i]; V[j].outDegree--; } 
        Tv vBak = vertex(i); 
        return vBak; 
    }

    virtual bool exists(int i, int j) { return (0 <= i) && (i < this->n) && (0 <= j) && (j < this->n) && E[i][j] != nullptr; }

    virtual EStatus& status(int i, int j) { return E[i][j]->status; } 
    virtual Te& edge(int i, int j) { return E[i][j]->data; } 
    virtual int& weight(int i, int j) { return E[i][j]->weight; }

    virtual void insert(Te const& edge, int w, int i, int j) { 
        if (exists(i, j)) return; 
        E[i][j] = new Edge<Te>(edge, w); 
        this->e++; 
        V[i].outDegree++; 
        V[j].inDegree++; 
    }

    virtual Te remove(int i, int j) { 
        Te eBak = edge(i, j); 
        delete E[i][j]; 
        E[i][j] = nullptr; 
        this->e--; 
        V[i].outDegree--; 
        V[j].inDegree--; 
        return eBak; 
    }

    // BFS 实现
    void BFS(int v, int& clock) { 
        queue<int> Q;
        status(v) = DISCOVERED; 
        Q.push(v);
        while (!Q.empty()) { 
            int v = Q.front(); Q.pop(); 
            dTime(v) = ++clock; 
            for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) 
                if (UNDISCOVERED == status(u)) { 
                    status(u) = DISCOVERED; 
                    Q.push(u); 
                    status(v, u) = TREE; 
                    parent(u) = v; 
                } else { 
                    status(v, u) = CROSS; 
                }
            status(v) = VISITED; 
        }
    }

    void bfs(int s) { 
        reset(); 
        int clock = 0; 
        int v = s; 
        do { 
            if (UNDISCOVERED == status(v)) 
                BFS(v, clock);  
        } while (s != (v = (++v % this->n))); 
    }

    // DFS 实现
    void DFS(int v, int& clock) {
        dTime(v) = ++clock; 
        status(v) = DISCOVERED;
        for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) {
            switch (status(u)) {
                case UNDISCOVERED:
                    status(v, u) = TREE;
                    parent(u) = v; 
                    DFS(u, clock); break;
                case DISCOVERED:
                    status(v, u) = BACKWARD; break;
                default:
                    status(v, u) = (dTime(u) < dTime(v)) ? FORWARD : CROSS; break;
            }
        }
        status(v) = VISITED; 
        fTime(v) = ++clock;
    }

    void dfs(int s) {
        reset(); 
        int clock = 0;
        int v = s;
        do {
            if (UNDISCOVERED == status(v)) 
                DFS(v, clock);
        } while (s != (v = (++v % this->n)));
    }

    // Dijkstra 算法
    void dijkstra(int s) {
        reset(); 
        priority(s) = 0;
        status(s) = VISITED;
        parent(s) = -1;
        while (1) {
                       for (int w = firstNbr(s); -1 < w; w = nextNbr(s, w)) {
                if (status(w) == UNDISCOVERED) {
                    int newPriority = priority(s) + weight(s, w);
                    if (newPriority < priority(w)) {
                        priority(w) = newPriority;
                        parent(w) = s;
                    }
                }
            }

            int minPriority = INT_MAX;
            int minVertex = -1;

            for (int i = 0; i < this->n; i++) {
                if (status(i) == UNDISCOVERED && priority(i) < minPriority) {
                    minPriority = priority(i);
                    minVertex = i;
                }
            }

            if (minVertex == -1) break;

            s = minVertex;
            status(s) = VISITED;
        }

        // 输出最短路径
        for (int i = 0; i < this->n; i++) {
            if (i != s) {
                cout << "Vertex " << i << " is at distance " << priority(i) << " from source " << s << endl;
                int parentVertex = parent(i);
                cout << "Path: " << i;
                while (parentVertex != -1) {
                    cout << " <- " << parentVertex;
                    parentVertex = parent(parentVertex);
                }
                cout << endl;
            }
        }
    }

    // 实现 PFS 函数
    void PFS(int s, function<void(int, int)> updateFunc) override {
        reset();
        priority(s) = 0;
        status(s) = VISITED;
        parent(s) = -1;

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, s});

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            for (int v = firstNbr(u); v >= 0; v = nextNbr(u, v)) {
                if (status(v) == UNDISCOVERED) {
                    int weightEdge = weight(u, v);
                    if (priority(v) > weightEdge) {
                        priority(v) = weightEdge;
                        parent(v) = u;
                        pq.push({priority(v), v});
                        updateFunc(u, v);  // 调用更新函数
                    }
                }
            }
            status(u) = VISITED;
        }
    }

    // Prim 算法（最小生成树）
    void prim(int s) {
        reset();
        priority(s) = 0;
        status(s) = VISITED;
        parent(s) = -1;

        // 使用优先队列来选择当前边权最小的点
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, s});

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            // 遍历所有邻接的顶点
            for (int v = firstNbr(u); v >= 0; v = nextNbr(u, v)) {
                if (status(v) == UNDISCOVERED) {
                    int weightEdge = weight(u, v);
                    if (priority(v) > weightEdge) {
                        priority(v) = weightEdge;
                        parent(v) = u;
                        pq.push({priority(v), v});
                    }
                }
            }
            status(u) = VISITED;
        }

        // 输出最小生成树
        cout << "Minimum Spanning Tree using Prim's Algorithm:" << endl;
        for (int i = 0; i < this->n; i++) {
            if (i != s) {
                cout << "Edge (" << parent(i) << ", " << i << ") with weight " << weight(parent(i), i) << endl;
            }
        }
    }

    // 重置图的状态
    void reset() {
        for (int i = 0; i < this->n; i++) {
            status(i) = UNDISCOVERED;
            parent(i) = -1;
            dTime(i) = fTime(i) = -1;
            priority(i) = INT_MAX;
        }
    }
};