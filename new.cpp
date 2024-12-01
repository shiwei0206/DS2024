#include <iostream>
#include <queue>
#include <unordered_map>
#include <fstream>
#include <string>

// 节点类，用于构建 Huffman 树
class Node
{
public:
    char character; // 存储字符
    int frequency;  // 存储字符的频率
    Node *left;     // 左子树指针
    Node *right;    // 右子树指针

    // 构造函数
    Node(char ch, int freq) : character(ch), frequency(freq), left(nullptr), right(nullptr) {}
};

// 比较器，用于优先队列排序
struct Compare
{
    bool operator()(Node *left, Node *right)
    {
        return left->frequency > right->frequency; // 按频率升序排列
    }
};

// Huffman 树类
class HuffTree
{
public:
    Node *root; // 根节点

    // 构造函数，接受字符频率的映射
    HuffTree(const std::unordered_map<char, int> &frequencies)
    {
        // 优先队列用于构建 Huffman 树
        std::priority_queue<Node *, std::vector<Node *>, Compare> minHeap;

        // 将每个字符及其频率添加到优先队列
        for (const auto &pair : frequencies)
        {
            minHeap.push(new Node(pair.first, pair.second));
        }

        // 构建 Huffman 树
        while (minHeap.size() > 1)
        {
            Node *left = minHeap.top();
            minHeap.pop(); // 取出频率最低的两个节点
            Node *right = minHeap.top();
            minHeap.pop();

            // 创建新的内部节点，其频率为两个子节点频率之和
            Node *merged = new Node('\0', left->frequency + right->frequency);
            merged->left = left;   // 左子节点
            merged->right = right; // 右子节点
            minHeap.push(merged);  // 将新节点重新放入优先队列
        }

        root = minHeap.top(); // 最后一个节点为根节点
    }
};

// Huffman 编码类
class HuffCode
{
public:
    std::unordered_map<char, std::string> codes; // 存储字符与编码的映射

    // 生成 Huffman 编码
    void generateCodes(Node *node, const std::string &code)
    {
        if (node)
        {
            if (node->character != '\0')
            {                                  // 叶子节点
                codes[node->character] = code; // 存储编码
            }
            // 递归生成左子树和右子树的编码
            generateCodes(node->left, code + '0');  // 左边加 '0'
            generateCodes(node->right, code + '1'); // 右边加 '1'
        }
    }
};

// Huffman 编码函数
std::pair<std::string, std::unordered_map<char, std::string>> huffmanEncoding(const std::string &text)
{
    std::unordered_map<char, int> frequency; // 字符频率映射

    // 统计字符频率
    for (char ch : text)
    {
        frequency[ch]++; // 统计字符频率（包括大小写和其他字符）
    }

    HuffTree huffTree(frequency);              // 构建 Huffman 树
    HuffCode huffCode;                         // 创建 Huffman 编码对象
    huffCode.generateCodes(huffTree.root, ""); // 生成编码

    std::string encodedText; // 编码后的文本
    for (char ch : text)
    {
        encodedText += huffCode.codes[ch]; // 获取对应编码
    }

    return {encodedText, huffCode.codes}; // 返回编码文本和编码映射
}

// 读取文件内容
std::string readFile(const std::string &filename)
{
    std::ifstream inputFile(filename);
    if (!inputFile)
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return "";
    }

    std::string content, line;
    while (std::getline(inputFile, line))
    {
        content += line + "\n"; // 读取每一行并加上换行符
    }
    inputFile.close();
    return content;
}

// 将编码结果写入文件
void writeFile(const std::string &filename, const std::string &encodedText)
{
    std::ofstream outputFile(filename);
    if (outputFile)
    {
        outputFile << encodedText;
        outputFile.close();
        std::cout << "Encoded text has been written to " << filename << std::endl;
    }
    else
    {
        std::cerr << "Error writing to file: " << filename << std::endl;
    }
}

// 主函数
int main()
{
    std::string inputFileName = "exp3\\word.txt"; // 输入文件名

    // 从文件读取文本内容
    std::string text = readFile(inputFileName);
    if (text.empty())
    {
        return 1; // 如果读取文件失败，直接退出
    }

    std::cout << "Input Text: " << std::endl
              << text << std::endl;

    // 执行 Huffman 编码
    auto [encodedText, codes] = huffmanEncoding(text);

    // 输出编码后的文本
    std::cout << "Encoded Text: " << std::endl
              << encodedText << std::endl;

    // 输出每个字符的 Huffman 编码
    std::cout << "Huffman Codes: " << std::endl;
    for (const auto &pair : codes)
    {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    return 0; // 程序结束
}
