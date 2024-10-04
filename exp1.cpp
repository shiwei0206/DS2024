#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include "unite\Vector\Vector.h"
#include <algorithm>
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
// 复数模顺序排序
void Sort1(Vector<Complex> &arr)
{
    int n = arr.size();
    for (int i = 1; i < n; ++i)
    {
        Complex key = arr[i];
        int j = i - 1;
        // 按复数的模顺序排序，模相等时按实部排序
        while (j >= 0 && key < arr[j])
        {arr[j + 1] = arr[j];j = j - 1;}
        arr[j + 1] = key;
    }
}
// 区间查找算法
Vector<Complex> findInRange(const Vector<Complex> &arr, double m1, double m2)
{
    Vector<Complex> result;
    for (size_t i = 0; i < arr.size(); ++i)
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
Vector<Complex> generateRandomVector(int size)
{
    Vector<Complex> vec;
    srand(time(0));
    int uniqueSize = size - 2;
    Vector<Complex> vec1;
    for (int i = 0; i < uniqueSize; ++i)
    {
        double real = rand() % 10; // 实部在 0 到 9 之间
        double imag = rand() % 10; // 虚部在 0 到 9 之间
        vec1.insert(Complex(real, imag));
        vec.insert(Complex(real, imag)); // 插入到最终的向量中
    }
    for (int i = uniqueSize; i < size; ++i)
    {
        int randomIndex = rand() % uniqueSize; // 从唯一元素中随机选择
        vec.insert(vec1[randomIndex]);
    }
    return vec;
}

Vector<Complex> generateOrderedVector(size_t n)
{
    Vector<Complex> vec;
    for (size_t i = 0; i < n; ++i)
    {vec.insert(static_cast<double>(i), static_cast<double>(i));}
    return vec;
}
// 生成逆序复数向量
Vector<Complex> generateReverseOrderedVector(size_t n)
{
    Vector<Complex> vec;
    for (size_t i = n; i > 0; --i)
    { vec.insert(static_cast<double>(i), static_cast<double>(i));}
    return vec;
}

// 记录排序时间并输出结果
void measureSortingTime(const string &sortType, Vector<Complex> vec)
{
    auto start = chrono::high_resolution_clock::now();
    if (sortType == "起泡排序")
    { vec.bubbleSort(0, vec.size()-1);}
    else if (sortType == "归并排序")
    {vec.mergeSort(0, vec.size()-1);}
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, micro> duration_us = end - start;
    cout << sortType<<" time: " << duration_us.count() << " us" << endl;
}

int main()
{
    // 1. 生成一个无序的复数向量
    int n = 10;
    Vector<Complex> vec = generateRandomVector(n);
    cout << "无序复数向量:\n";
    vec.traverse(print);
    cout << endl;
    // 测试置乱
    vec.unsort(0, vec.size()-1);
    cout << "置乱后的复数向量:\n";
    vec.traverse(print);cout << endl;
    // // 测试查找
    Complex searchComplex(1, 2); // 用以查找的复数
    Rank foundIndex = vec.find(searchComplex);
    if (foundIndex != -1)
    { cout << "找到元素 " << searchComplex << " 在索引 " << foundIndex << endl;}
    else
    {cout << "未找到元素 " << searchComplex << endl;}
    // 测试插入
    vec.insert(0, Complex(1, 2));
    cout << "插入元素 (1, 2) 后的复数向量:\n";
    vec.traverse(print);cout << endl;
    // 测试删除
    vec.remove(0); // 删除插入的元素
    cout << "删除元素后复数向量:\n";
    vec.traverse(print);cout << endl;
    // 唯一化复数向量
    vec.deduplicate();
    cout << "唯一化后的复数向量:\n";
    vec.traverse(print);
    cout << endl;

    // 排序
    Sort1(vec);
    cout << "排序后的复数向量:\n";
    vec.traverse(print);
    cout << endl;

    // 测试区间查找
    double m1 = 3.0;  // 设置模的下界
    double m2 = 10.0; // 设置模的上界
    Vector<Complex> foundInRange = findInRange(vec, m1, m2);
    cout << "在区间 [" << m1 << ", " << m2 << "):的元素\n";
    foundInRange.traverse(print);
    cout<<endl;

    // 测试顺序
    cout << "顺序排序时间:" << endl;
    Vector<Complex> orderedVec = generateOrderedVector(n);
    measureSortingTime("起泡排序", orderedVec);
    measureSortingTime("归并排序", orderedVec);
    // 测试乱序向量
    cout << "\n乱序排序时间:" << endl;
    Vector<Complex> randomVec = generateRandomVector(n);
    measureSortingTime("起泡排序", randomVec);
    measureSortingTime("归并排序", randomVec);
    // // 测试逆序向量
    cout << "\n逆序排序时间" << endl;
    Vector<Complex> reverseVec = generateReverseOrderedVector(n);
    measureSortingTime("起泡排序", reverseVec);
    measureSortingTime("归并排序", reverseVec);
    return 0;
 }