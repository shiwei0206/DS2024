#include <iostream>
#include <cmath>
#include "Vector.cpp"
#include <chrono>
using namespace std;
// 复数类定义
class Complex
{
public:
    double real;
    double imag;
    // 运算符重载
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}
    // 重载 <= 操作符
    bool operator<=(const Complex &other) const
    {return (real <= other.real && imag <= other.imag);}
    // 重载 > 操作符
    bool operator>(const Complex &other) const
    {return (real > other.real || (real == other.real && imag > other.imag));}
    double modulus() const
    {return sqrt(real * real + imag * imag);}
    // 定义小于运算符，用于比较复数模（顺序）
    bool operator<(const Complex &other) const
    {return modulus() < other.modulus() || (modulus() == other.modulus() && real < other.real);}
    // 重载 != 运算符
    bool operator!=(const Complex &other) const
    {return (real != other.real || imag != other.imag);}
    // 重载==
    bool operator==(const Complex &other) const
    {return real == other.real && imag == other.imag;}
    friend ostream &operator<<(ostream &os, const Complex &c)
    {os << "(" << c.real << ", " << c.imag << ")";return os;}
};
// 区间查找算法
Vector<Complex> findM(const Vector<Complex> &arr, double m1, double m2)
{
    Vector<Complex> result;
    for (int i = 0; i < arr.size(); ++i)
    {
        double modulus = arr[i].modulus();
        if (modulus >= m1 && modulus < m2)
        {result.insert(arr[i]);}
    }
    return result;
}
// 输出函数
void print(Complex &e)
{ cout << e.real << "+" << e.imag << "i  ";}
// 随机生成乱序复数向量
Vector<Complex> generateRandomVector(int n)
{
    Vector<Complex> A;
    srand(time(0));
    int uniqueSize = n - 2;
    Vector<Complex> A1;
    for (int i = 0; i < uniqueSize; ++i)
    {
        double real = rand() % 10; // 实部在 0 到 9 之间
        double imag = rand() % 10; // 虚部在 0 到 9 之间
        A1.insert(Complex(real, imag));
        A.insert(Complex(real, imag)); // 插入到最终的向量中
    }
    for (int i = uniqueSize; i < n; ++i)
    {
        int randomIndex = rand() % uniqueSize; // 从唯一元素中随机选择
        A.insert(A1[randomIndex]);
    }
    return A;
}
// 记录排序时间并输出结果
void measureSortingTime(const string &sortType, Vector<Complex> A)
{
    auto start = chrono::high_resolution_clock::now();
    if (sortType == "起泡排序")
    { A.bubbleSort(0, A.size()-1);}
    else if (sortType == "归并排序")
    {A.mergeSort(0, A.size()-1);}
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, micro> duration_us = end - start;
    cout << sortType<<" time: " << duration_us.count() << " us" << endl;
}
int main()
{
    // 1. 生成一个无序的复数向量
    int n = 100;
    Vector<Complex> A= generateRandomVector(n);
    Vector<Complex> A1 = A;//乱序数组A1
    Vector<Complex> A2 = A;
    Vector<Complex> A3 = A; // 逆序数组A3
    A2.sort();//顺序数组A2
    for (int i=n-1; i>=0; i--)
    {
        A3[n-i-1] = A2[i];
    }
    cout << "无序复数向量:\n";
    A.traverse(print);
    cout << endl;
    // 测试置乱
    A.unsort(0, A.size()-1);
    cout << "置乱后的复数向量:\n";
    A.traverse(print);cout << endl;
    // // 测试查找
    Complex search(1, 2); // 用以查找的复数
    Rank foundI = A.find(search);
    if (foundI != -1)
    { cout << "找到元素 " << search << " 在索引 " << foundI << endl;}
    else
    {cout << "未找到元素 " << search << endl;}
    // 测试插入
    A.insert(0, Complex(1, 2));
    cout << "插入元素 (1, 2) 后的复数向量:\n";
    A.traverse(print);cout << endl;
    // 测试删除
    A.remove(0); // 删除插入的元素
    cout << "删除元素后复数向量:\n";
    A.traverse(print);cout << endl;
    // 唯一化复数向量
    A.deduplicate();
    cout << "唯一化后的复数向量:\n";
    A.traverse(print);cout << endl;
    // 排序
    A.sort();
    cout << "排序后的复数向量:\n";
    A.traverse(print);cout << endl;
    // 测试区间查找
    double m1 = 3.0;  // 设置模的下界
    double m2 = 8.0; // 设置模的上界
    Vector<Complex> foundN = findM(A2, m1, m2);
    cout << "在区间 [" << m1 << ", " << m2 << "):的元素\n";
    foundN.traverse(print);cout<<endl;
    // 测试乱序向量
    cout << "乱序排序时间:" << endl;
    Vector<Complex> A12 = A1;
    measureSortingTime("起泡排序",A1);
    measureSortingTime("归并排序", A12);
    // 测试顺序
    cout << "顺序排序时间:" << endl;
    Vector<Complex> A22 = A2;
    measureSortingTime("起泡排序", A2);
    measureSortingTime("归并排序", A22);
    // 测试逆序向量
    cout << "逆序排序时间" << endl;
    Vector<Complex> A32 = A3;
    measureSortingTime("起泡排序", A3);
    measureSortingTime("归并排序", A32);
    return 0;
 }