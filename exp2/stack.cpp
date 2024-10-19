#include "../unite/Vector/Vector.cpp"  // 假设你已经有一个可用的 Vector 实现
#include <vector>
template <typename T>
class Stack : public Vector<T> {
public:
    // 入栈操作，将元素插入栈顶
    void push(T const& e) {
        this->insert(this->size(), e);  // 插入元素到当前 Vector 尾部
    }

    // 出栈操作，移除栈顶元素并返回
    T pop() {
        return this->remove(this->size() - 1);  // 移除并返回栈顶元素
    }

    // 查看栈顶元素
    T& top() {
        return (*this)[this->size() - 1];  // 返回栈顶元素的引用
    }

    // 判断栈是否为空
    bool is_empty() const {
        return this->size() == 0;  // 若大小为0，则为空
    }
};

