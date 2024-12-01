#include "../unite/Graph/Graph.h"

int main() {
    // 创建一个图实例
    GraphMatrix<string, string> g;

    // 添加顶点
    g.insert("A");
    g.insert("B");
    g.insert("C");
    g.insert("D");
    g.insert("E");

    // 添加边 (顶点1, 顶点2, 权重)
    g.insert("AB", 1, 0, 1);
    g.insert("AC", 4, 0, 2);
    g.insert("BC", 2, 1, 2);
    g.insert("BD", 5, 1, 3);
    g.insert("CD", 1, 2, 3);
    g.insert("DE", 3, 3, 4);
    g.insert("CE", 6, 2, 4);

    // 测试深度优先搜索（DFS）
    cout << "DFS starting from vertex A:" << endl;
    g.dfs(0); // 从顶点 'A' 开始深度优先搜索（0 表示顶点 A）
    for (int i = 0; i < g.n; i++) {
        cout << "Vertex " << g.vertex(i) << ": ";
        cout << "dTime = " << g.dTime(i) << ", ";
        cout << "fTime = " << g.fTime(i) << ", ";
        cout << "parent = " << (g.parent(i) != -1 ? g.vertex(g.parent(i)) : "None") << endl;
    }

    // 测试广度优先搜索（BFS）
    cout << "\nBFS starting from vertex A:" << endl;
    g.bfs(0);
    for (int i = 0; i < g.n; i++) {
        cout << "Vertex " << g.vertex(i) << ": ";
        cout << "dTime = " << g.dTime(i) << ", ";
        cout << "parent = " << (g.parent(i) != -1 ? g.vertex(g.parent(i)) : "None") << endl;
    }

    // 执行 Dijkstra 算法，寻找从 A（顶点0）到其他顶点的最短路径
    cout << "\nDijkstra's Shortest Path from vertex A:" << endl;
    g.dijkstra(0);

    // 执行 Prim 算法，构建最小生成树
    cout << "\nPrim's Minimum Spanning Tree starting from vertex A:" << endl;
    g.prim(0);

    return 0;
}
