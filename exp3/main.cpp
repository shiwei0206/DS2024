#include <iostream>
#include <queue>
#include <string>
#include <fstream>
#include <cctype>  
using namespace std;
// Bitmap 类：表示一个位图，支持通过索引存取和扩展位图大小。
class Bitmap {
public:
    Bitmap(int n) : bits(n) {} // 构造函数，初始化位图，位图的大小为n
    void set(int index, bool value) { if (index < bits.size()) bits[index] = value; }
    bool test(int index) const { return index < bits.size() && bits[index]; }
    void expand(int index) { if (index >= bits.size()) bits.resize(index + 1); }
private:
    vector<bool> bits; // 存储位图数据的容器
};
// HuffCode 类：继承自 Bitmap 类，表示霍夫曼编码。
// 用于存储和管理霍夫曼编码结果。
class HuffCode : public Bitmap {
public:
    HuffCode() : Bitmap(8) {}
    HuffCode(int n) : Bitmap(n) {}
};
// HuffChar 结构体：表示霍夫曼树中的字符节点。
// 包含字符、字符的权重（频率）以及霍夫曼编码。
struct HuffChar {
    char ch;
    int weight;
    HuffCode code;
    HuffChar(char c = '^', int w = 0) : ch(c), weight(w) {}
};
template <typename T>
struct BinNode {
    T data;
    BinNode<T>* left;
    BinNode<T>* right;
    BinNode<T>* parent;
    
    BinNode(T e, BinNode<T>* l = nullptr, BinNode<T>* r = nullptr, BinNode<T>* p = nullptr)
        : data(e), left(l), right(r), parent(p) {}
};
// HuffTree 类：表示霍夫曼树，提供霍夫曼编码的生成和编码的操作。
// 该类通过构建霍夫曼树并生成每个字符的霍夫曼编码，支持编码和打印编码结果。
class HuffTree {
private:
    BinNode<HuffChar>* root;  // 霍夫曼树的根节点
    vector<BinNode<HuffChar>*> forest; 
        void buildHuffTree(int* freq, int n) {
        struct CompareNode {
            bool operator()(BinNode<HuffChar>* a, BinNode<HuffChar>* b) {
                return a->data.weight > b->data.weight; // 按权重升序排列
            }
        };
        priority_queue<BinNode<HuffChar>*, vector<BinNode<HuffChar>*>, CompareNode> pq;  // 优先队列，按权重排序
        for (int i = 0; i < n; i++) {
            if (freq[i] > 0) {
                BinNode<HuffChar>* node = new BinNode<HuffChar>(HuffChar(i + 'a', freq[i]));
                forest.push_back(node);
                pq.push(node);
            }
        }
        // 合并节点直到只剩一个根节点
        while (pq.size() > 1) {
            BinNode<HuffChar>* left = pq.top(); pq.pop();
            BinNode<HuffChar>* right = pq.top(); pq.pop();          
            BinNode<HuffChar>* parent = new BinNode<HuffChar>(
                HuffChar('^', left->data.weight + right->data.weight), left, right, nullptr);
            left->parent = parent;
            right->parent = parent;  
            forest.push_back(parent);
            pq.push(parent);
        }
        root = pq.empty() ? nullptr : pq.top(); // 设置根节点
    }
    // 递归生成霍夫曼编码
    void generateCodes(BinNode<HuffChar>* node, HuffCode& prefix, int length, HuffCode codeTable[], int n) {
        if (!node) return; // 遇到空节点返回
        // 叶子节点（字符节点），保存编码
        if (!node->left && !node->right) {
            codeTable[node->data.ch - 'a'] = prefix;
            codeTable[node->data.ch - 'a'].expand(length);
            return;
        }
        // 递归生成左子树的编码（0）
        if (node->left) {
            prefix.set(length, 0);
            generateCodes(node->left, prefix, length + 1, codeTable, n);
        }
        // 递归生成右子树的编码（1）
        if (node->right) {
            prefix.set(length, 1);
            generateCodes(node->right, prefix, length + 1, codeTable, n);
        }
    }
public:
    // 构造函数：通过频率数组构建霍夫曼树
    HuffTree(int* freq, int n) : root(nullptr) {
        buildHuffTree(freq, n);
    }
    // 析构函数，释放所有动态分配的节点
    ~HuffTree() {
        for (auto node : forest) {
            delete node;
        }
    }
    // 生成霍夫曼编码
    void generateCodes(HuffCode codeTable[], int n) {
        if (!root) return;
        HuffCode prefix(n * 8); // 编码前缀（最大支持8位编码）
        generateCodes(root, prefix, 0, codeTable, n);
    }
    // 打印霍夫曼编码
    void printCodes(HuffCode codeTable[], int n) {
        cout << "字母编码：\n";
        for (int i = 0; i < n; i++) {
            if (forest.size() > 0) {
                for (auto node : forest) {
                    if (node->data.ch == char(i + 'a')) {
                        cout << char(i + 'a') << ": ";
                        for (int j = 0; j < n * 8; j++) {
                            if (j > 0 && !codeTable[i].test(j)) {
                                bool allZeros = true;
                                for (int k = j + 1; k < n * 8; k++) {
                                    if (codeTable[i].test(k)) {
                                        allZeros = false;
                                        break;
                                    }
                                }
                                if (allZeros) break;
                            }
                            cout << (codeTable[i].test(j) ? "1" : "0");
                        }
                        cout << "\n";
                        break;
                    }
                }
            }
        }
    }
};
// encodeWord 函数：将指定的单词编码并输出。
// word 为要编码的单词，codeTable 为霍夫曼编码表，n 为字符数。
void encodeWord(const string& word, HuffCode codeTable[], int n) {
    cout << "单词编码 " << word << " : ";
    for (char c : word) {
        if (isalpha(c)) {
            HuffCode& code = codeTable[tolower(c) - 'a'];
            for (int j = 0; j < n * 8; j++) {
                if (j > 0 && !code.test(j)) {
                    bool allZeros = true;
                    for (int k = j + 1; k < n * 8; k++) {
                        if (code.test(k)) {
                            allZeros = false;
                            break;
                        }
                    }
                    if (allZeros) break;
                }
                cout << (code.test(j) ? "1" : "0");
            }
            cout << " ";
        }
    }
    cout << "\n";
}
void huffmanExample() {
    const int N_CHAR = 26; // 26个字母
    // 打开文件
    ifstream inputFile("word.txt");  
    string text;
    // 读取文件内容
    char c;
    while (inputFile.get(c)) {
        text += c;
    }
    inputFile.close();
    int freq[N_CHAR] = {0};
    // 计算频率
    for (char c : text) {
        if (isalpha(c)) {
            freq[tolower(c) - 'a']++;
        }
    }
    HuffTree* tree = new HuffTree(freq, N_CHAR);
    HuffCode codeTable[N_CHAR];
    tree->generateCodes(codeTable, N_CHAR);
    tree->printCodes(codeTable, N_CHAR);
    // 编码特定单词
    encodeWord("dream", codeTable, N_CHAR);
    encodeWord("have", codeTable, N_CHAR);
    encodeWord("day", codeTable, N_CHAR);
    cout << "\n编码结果: ";
    HuffCode encodedText(text.length() * 8);
    int encodedLen = 0;
    for (char c : text) {
        if (isalpha(c)) {
            HuffCode& code = codeTable[tolower(c) - 'a'];
            for (int j = 0; j < N_CHAR * 8; j++) {
                if (j > 0 && !code.test(j)) {
                    bool allZeros = true;
                    for (int k = j + 1; k < N_CHAR * 8; k++) {
                        if (code.test(k)) {
                            allZeros = false;
                            break;
                        }
                    }
                    if (allZeros) break;
                }
                encodedText.set(encodedLen++, code.test(j));
            }
        }
    }
    for (int i = 0; i < encodedLen; i += 8) {
        for (int j = 0; j < 8 && i + j < encodedLen; j++) {
            cout << (encodedText.test(i + j) ? "1" : "0");
        }
        cout << " ";
    }
    cout << endl;
    delete tree;
}
int main() {
    huffmanExample();
    return 0;
}