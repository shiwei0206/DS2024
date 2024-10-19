#include <iostream>
#include "Stack.cpp"
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>
using namespace std;
//2. 基于优先级表实现的字符串计算器
#define N_OPTR 9 
 typedef enum {ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE} Operator; 
 const char pri[N_OPTR][N_OPTR] = { 
/* |-------------- 前运算符 --------------| */
/* + - * / ^ ! ( ) \0 */
 /* -- + */ '>', '>', '<', '<', '<', '<', '<', '>', '>',
 /* | - */ '>', '>', '<', '<', '<', '<', '<', '>', '>',
 /* 栈 * */ '>', '>', '>', '>', '<', '<', '<', '>', '>',
 /* 顶 / */ '>', '>', '>', '>', '<', '<', '<', '>', '>',
 /* 运 ^ */ '>', '>', '>', '>', '>', '<', '<', '>', '>',
 /* 算 ! */ '>', '>', '>', '>', '>', '>', ' ', '>', '>',
 /* 符 ( */ '<', '<', '<', '<', '<', '<', '<', '=', ' ',
 /* | ) */ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
 /* -- \0 */ '<', '<', '<', '<', '<', '<', '<', ' ', '='
 };

Stack<double> numStack;  // 操作数栈
Stack<char> opStack;      // 运算符栈
double evaluate(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) throw runtime_error("Division by zero");
            return a / b;
        case '^': return pow(a, b);
        case '!': return tgamma(a + 1); 
        default: throw runtime_error("Invalid operator");
    }
}
int precedence(char op) {
    switch (op) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '^': return 3;
        case '!': return 4;
        default: return 0;
    }
}
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '!';
}

bool isNumber(char c) {
    return isdigit(c) || c == '.';
}

double calculate(const string &expression) {
    numStack = Stack<double>();
    opStack = Stack<char>();
    string numBuffer;

    for (size_t i = 0; i < expression.size(); ++i) {
        char c = expression[i];

        if (isspace(c)) continue; 
        if (isNumber(c)) {
            numBuffer += c;  
        } else {
            if (!numBuffer.empty()) {
                numStack.push(std::stod(numBuffer));  
                numBuffer.clear();
            }

            if (isOperator(c)) {
                while (!opStack.empty() && precedence(opStack.top()) >= precedence(c)) {
                    double b = numStack.top(); numStack.pop();
                    double a = numStack.top(); numStack.pop();
                    char op = opStack.top(); opStack.pop();
                    numStack.push(evaluate(a, b, op));
                }opStack.push(c);
            } else if (c == '(') { opStack.push(c);} 
            else if (c == ')') {
                while (opStack.top() != '(') {
                    double b = numStack.top(); numStack.pop();
                    double a = numStack.top(); numStack.pop();
                    char op = opStack.top(); opStack.pop();
                    numStack.push(evaluate(a, b, op));
                }
                opStack.pop();  
            }
        }
    }

    if (!numBuffer.empty()) {numStack.push(stod(numBuffer));}
    while (!opStack.empty()) {
        double b = numStack.top(); numStack.pop();
        double a = numStack.top(); numStack.pop();
        char op = opStack.top(); opStack.pop();
        numStack.push(evaluate(a, b, op));
    }
   return numStack.top();
}
// 扩展: 支持三角函数和对数
double three(const string &expression)
{
        size_t start = expression.find('(');
        size_t end = expression.find(')');
        double angle = stod(expression.substr(start + 1, end - start - 1));
        return sin(angle * M_PI / 180.0); // 将角度转换为弧度
}
// 3. 求柱状图中最大矩形面积
int largestarea(const vector<int> &heights)
{
    Stack<int> st;
    int maxArea = 0;
    int n = heights.size();
    for (int i = 0; i < n; i++)
    {
        while (!st.empty() && heights[st.top()] > heights[i])
        {
            int h = heights[st.top()]; st.pop();
            int width = st.empty() ? i : i - st.top() - 1;
            maxArea =max(maxArea, h * width);
        } st.push(i);
    }
    while (!st.empty())
    {
        int h = heights[st.top()];
        st.pop();
        int width = st.empty() ? n : n - st.top() - 1;
        maxArea = max(maxArea, h * width);
    }
    return maxArea;
}


// 4. 随机生成测试数据
void generatetest(int numTests)
{
    srand(time(0)); // 初始化随机数种子
    for (int i = 0; i < numTests; ++i)
    {
        int length = rand() % 105 + 1; // 随机生成柱状图长度 (1 到 105)
        vector<int> heights(length);
        cout << "输入" << i + 1 << ": [ ";
        for (int j = 0; j < length; ++j)
        {
            heights[j] = rand() % 104; // 随机生成柱子高度 (0 到 104)
            cout << heights[j] << " ";
        }
        cout << "]" << endl;
        int maxArea = largestarea(heights);
        cout << "输出: " << maxArea << endl
                  << endl;
    }
}
int main()
{
    // 字符串计算器示例测试
    // 字符串计算器示例测试
    string A1 = {"3 + 3 * 2 - 8 / 2 + 3.2",};
    cout << "数字运算: " << A1 << " = " << calculate(A1) << endl<<endl;
    string A2 = {"( 3 + 4 ) * 5 -24"};
    cout << "数字运算: " << A2 << " = " << calculate(A2)<<endl<<endl;
    string A3 = {"sin(60)"};
    cout << "数字运算: " << A3 << " = " << three(A3)<<endl;
    // 测试柱状图最大矩形面积
    vector<int> B1 = {2, 1, 5, 6, 2, 3};
    vector<int> B2 = {2, 4};
    cout << "输入: " << "[2, 1, 5, 6, 2, 3]" << endl;
    cout << "输出: " << largestarea(B1) << endl <<endl;
    cout << "输入: " << "[2, 4]" << endl;
    cout << "输出: " << largestarea(B2) << endl << endl;
    // 随机生成 10 组测试数据
    generatetest(10);
    return 0;
}
