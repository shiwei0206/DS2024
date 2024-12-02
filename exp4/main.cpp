#include "../unite/Graph/Graph.h"
int main() {
    GraphMatrix<string, string> g;
    // 添加顶点
    g.insert("A");
    g.insert("B");
    g.insert("C");
    g.insert("D");
    g.insert("E");
    // 添加边 (顶点1, 顶点2, 权重)
    g.insert("AB", 1, 0, 3);
    g.insert("AC", 4, 0, 1);
    g.insert("BC", 2, 1, 2);
    g.insert("BD", 5, 1, 5);
    g.insert("CD", 1, 2, 4);
    g.insert("DE", 3, 3, 6);
    g.insert("CE", 6, 2, 4);
    // 测试DFS
    cout << "DFS starting from  A:" << endl;
    g.dfs(0); // 从顶点 'A' 开始深度优先搜索（0 表示顶点 A）
    for (int i = 0; i < g.n; i++) {
        cout << "Vertex " << g.vertex(i) << ": ";
        cout << "dTime = " << g.dTime(i) << ", ";
        cout << "fTime = " << g.fTime(i) << ", ";
        cout << "parent = " << (g.parent(i) != -1 ? g.vertex(g.parent(i)) : "None") << endl;
    }
    // 测试BFS
    cout << "\nBFS starting from  A:" << endl;
    g.bfs(0);
    for (int i = 0; i < g.n; i++) {
        cout << "Vertex " << g.vertex(i) << ": ";
        cout << "dTime = " << g.dTime(i) << ", ";
        cout << "parent = " << (g.parent(i) != -1 ? g.vertex(g.parent(i)) : "None") << endl;
    }
    // 测试 Dijkstra 算法，寻找从 A（顶点0）到其他顶点的最短路径
    cout << "\nDijkstra  A:" << endl;
    g.dijkstra(0);
    // 测试 Prim 算法，构建最小生成树
    cout << "\nPrim  A:" << endl;
    g.prim(0);
    return 0;
}
