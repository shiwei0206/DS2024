//4.1Stack模板类
#include"../Vector/Vector.h"
template<typename T> class Stack:public Vector<T>{
    public:
        void push(T const &e) { insert(size(), e); }
        T pop() { return remove(size() - 1); }
        T &top() { return (*this)[size() - 1]; }
};
 

//4.2进制转化算法(递归版)
void convert(Stack<char> &S, __int64 n, int base){
    static char digit[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    if (0 < n){
        convert(S, n / base, base);
        S.push(digit[n % base]);
    }
}

//4.3进制转化算法(迭代版)
void convert(Stack<char> &S, __int64 n, int base){
    static char digit[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    while (n > 0)
    {
        int remainder = (int)(n % base);
        S.push(digit[remainder]);
        n /= base;
    }
}