#include "../unite/Vector/Vector.cpp" 
#include <stack>
template <typename T>
class Stack : public Vector<T> {
public:
    // 入栈操作，将元素插入栈顶
    void push(T const& e) {
        this->insert(this->size(), e); 
    }

    // 出栈操作，移除栈顶元素并返回
    T pop() {
        return this->remove(this->size() - 1);  
    }

    // 查看栈顶元素
    T& top() {
        return (*this)[this->size() - 1];  
    }

    // 判断栈是否为空
    bool is_empty() const {
        return this->size() == 0;  
    }
};

