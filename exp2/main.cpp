#include <iostream>
#include <stack>
#include <string>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>
using namespace std;
// 1. 实现栈数据结构
template <typename T>
class Stack
{
public:
    void push(T value)
    {s.push(value);}
    T pop()
    {
        if (s.empty())
            throw runtime_error("栈为空");
        T topValue = s.top();
        s.pop();
        return topValue;
    }
    T top() const
    {
        if (s.empty())
            throw runtime_error("栈为空");
        return s.top();
    }
    bool Empty() const
    {return s.empty();}
private:
    stack<T> s;
};
// 2. 基于优先级表实现的字符串计算器
int precedence(char op)
{
    if (op == '+' || op == '-')return 1;
    if (op == '*' || op == '/')return 2;
    if (op == '^')return 3;
    return 0;
}
double operation(double a, double b, char op)
{
    switch (op)
    {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/':
        if (b == 0)
            throw runtime_error("除以零错误");
        return a / b;
    case '^': return pow(a, b);
    default: return 0;
    }
}
bool Number(char c)
{return c >= '0' && c <= '9';}
double evaluate(const string &expression)
{
    Stack<double> values; 
    Stack<char> ops;     
    for (size_t i = 0; i < expression.length(); i++)
    {
        if (isspace(expression[i]))
            continue;
        if (Number(expression[i]))
        {
            double value = 0;
            while (i < expression.length() && (Number(expression[i]) || expression[i] == '.'))
            {
                if (expression[i] == '.')
                {i++; double fraction = 1.0;
                    while (i < expression.length() && Number(expression[i]))
                    {
                        fraction /= 10;
                        value += (expression[i] - '0') * fraction; i++;
                    }i--; 
                }else
                {value = (value * 10) + (expression[i] - '0');}
                i++;
            }
            i--; values.push(value);
        }
        else if (expression[i] == '(')
        { ops.push(expression[i]);}
        else if (expression[i] == ')')
        {
            while (!ops.Empty() && ops.top() != '(')
            {
                double val2 = values.pop();
                double val1 = values.pop();
                char op = ops.pop();
                values.push(operation(val1, val2, op));
            }ops.pop();
        }else
        {
            while (!ops.Empty() && precedence(ops.top()) >= precedence(expression[i]))
            {
                double val2 = values.pop();
                double val1 = values.pop();
                char op = ops.pop();
                values.push(operation(val1, val2, op));
            } ops.push(expression[i]);
        }
    }
    while (!ops.Empty())
    {
        double val2 = values.pop();
        double val1 = values.pop();
        char op = ops.pop();
        values.push(operation(val1, val2, op));
    }return values.pop();
}
// 扩展: 支持三角函数和对数
double sin(const string &expression)
{
        size_t start = expression.find('(');
        size_t end = expression.find(')');
        double angle = stod(expression.substr(start + 1, end - start - 1));
        return sin(angle * M_PI / 180.0); // 将角度转换为弧度
    return evaluate(expression);
}
// 3. 求柱状图中最大矩形面积
int largestRectangleArea(const vector<int> &heights)
{
    stack<int> st;
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
void generateRandomTestCases(int numTests)
{
    srand(time(0)); // 初始化随机数种子
    for (int i = 0; i < numTests; ++i)
    {
        int length = rand() % 105 + 1; // 随机生成柱状图长度 (1 到 105)
        vector<int> heights(length);
        cout << "输入" << i + 1 << ":[ ";
        for (int j = 0; j < length; ++j)
        {
            heights[j] = rand() % 104; // 随机生成柱子高度 (0 到 104)
            cout << heights[j] << " ";
        }
        cout << "]" << endl;
        int maxArea = largestRectangleArea(heights);
        cout << "输出: " << maxArea << endl
                  << endl;
    }
}
int main()
{
        // 字符串计算器示例测试
        string expression1 = "3 + 5 * 2 - 4";
        string expression2 = "sin(60)";
        cout << "数字运算: " << expression1 << " = " << evaluate(expression1) << endl <<endl;
        cout << "三角函数: " << expression2 << " = " << sin(expression2) << endl << endl;
        // 测试柱状图最大矩形面积
        vector<int> heights1 = {2, 1, 5, 6, 2, 3};
        vector<int> heights2 = {2, 4};
        cout << "输入: " << "[2, 1, 5, 6, 2, 3]" << endl;
        cout << "输出: " << largestRectangleArea(heights1) << endl <<endl;
        cout << "输入: " << "[2, 4]" << endl;
        cout << "输出: " << largestRectangleArea(heights2) << endl << endl;
        // 随机生成 10 组测试数据
        generateRandomTestCases(10);
    return 0;
}
