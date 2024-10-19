#include <iostream>
#include <unordered_map>
#include "../exp1/Vector.cpp"
#include <string>
#include <cctype>
#include "Stack.hpp"
using namespace std;

class Calculator
{
public:
    int evaluate(const std::string &expression)
    {
        Vector<string> tokens = tokenize(expression);
        Stack<string> ops;  // 运算符栈
        Vector<int> output; // 输出的后缀表达式

        for (const auto &token : tokens)
        {
            if (isdigit(token[0]))
            {
                output.push_back(std::stoi(token));
            }
            else if (priority.count(token))
            {
                while (!ops.empty() && priority[token] <= priority[ops.top()])
                {
                    output.push_back(calc(ops.top(), output));
                    ops.pop();
                }
                ops.push(token);
            }
            else if (token == "(")
            {
                ops.push(token);
            }
            else if (token == ")")
            {
                while (!ops.empty() && ops.top() != "(")
                {
                    output.push_back(calc(ops.top(), output));
                    ops.pop();
                }
                ops.pop(); // pop the '('
            }
        }

        while (!ops.empty())
        {
            output.push_back(calc(ops.top(), output));
            ops.pop();
        }

        return output.back();
    }

private:
    std::unordered_map<std::string, int> priority = {{"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}};

    std::vector<std::string> tokenize(const std::string &expression)
    {
        std::vector<std::string> tokens;
        std::string current;
        for (char ch : expression)
        {
            if (isspace(ch))
            {
                if (!current.empty())
                {
                    tokens.push_back(current);
                    current.clear();
                }
            }
            else
            {
                current += ch;
            }
        }
        if (!current.empty())
        {
            tokens.push_back(current);
        }
        return tokens;
    }

    int calc(const std::string &op, std::vector<int> &output)
    {
        int b = output.back();
        output.pop_back();
        int a = output.back();
        output.pop_back();
        if (op == "+")
            return a + b;
        if (op == "-")
            return a - b;
        if (op == "*")
            return a * b;
        if (op == "/")
            return a / b; // 整数除法
        return 0;
    }
};

// 测试字符串计算器
int main()
{
    Calculator calc;
    std::string expression1 = "3 + 5 * 2 - 8 / 4";
    int result1 = calc.evaluate(expression1);
    std::cout << "Result of '" << expression1 << "': " << result1 << std::endl;

    std::string expression2 = "10 + 2 * ( 3 - 1 )";
    int result2 = calc.evaluate(expression2);
    std::cout << "Result of '" << expression2 << "': " << result2 << std::endl;

    return 0;
}
