// 列表模板类
#include "listNode.h" //引入列表节点类

template <typename T>
class List // 列表模板类
{

private:
    int _size;               // 规模
    ListNodePosi(T) header;  // 头哨兵
    ListNodePosi(T) trailer; // 尾哨兵

protected:
    void init();                                                         // 列表创建时的初始化
    int clear();                                                         // 清除所有节点
    void copyNodes(ListNodePosi(T), int);                                // 复制列表中自位置p起的n项
    void merge(ListNodePosi(T) &, int, List<T> &, ListNodePosi(T), int); // 有序列表区间归并
    void mergeSort(ListNodePosi(T) &, int);                              // 对从p开始连续的n个节点归并排序
    void selectionSort(ListNodePosi(T), int);                            // 从p开始连续的n个节点选择排序
    void insertionSort(ListNodePosi(T), int);                            // 从p开始连续的n个节点插入排序

public:
    // 构造函数
    List() { init(); }                      // 默认
    List(List<T> const &L);                 // 整体复制列表L
    List(List<T> const &L, Rank r, int n); // 复制列表L中自第r项起的n项
    List(ListNodePosi(T) p, int n);         // 复制列表中自位置p起的n项
    // 析构函数
    ~List(); // 释放(包含头，尾哨兵在内的)所有节点
    // 只读访问接口
    Rank size() const { return _size; }                    // 规模
    bool empty() const { return _size <= 0; }              // 判空
    T &operator[](Rank r) const;                           // 重载，支持循秩访问(效率低)
    ListNodePosi(T) first() const { return header->succ; } // 首节点位置
    ListNodePosi(T) last() const { return trailer->pred; } // 末节点位置
    bool valid(ListNodePosi(T) p)                          // 判断位置p是否对外合法
    {
        return p && (trailer != p) && (header != p);
    } // 将头、尾节点等同于NULL
    int disordered() const;                // 判断列表是否已排序
    ListNodePosi(T) find(T const &e) const // 无序列表查找
    {
        return find(e, _size, trailer);
    }
    ListNodePosi(T) find(T const &e, int n, ListNodePosi(T) p) const; // 无序列区间查找
    ListNodePosi(T) search(T const &e) const                          // 有序列表查找
    {
        return search(e, _size, trailer);
    }
    ListNodePosi(T) search(T const &e, int n, ListNodePosi(T) p) const;    // 有序区间查找
    ListNodePosi(T) selectMax(ListNodePosi(T) p, int n);                   // 在p及前n—1个后继中选出最大者
    ListNodePosi(T) selectMax() { return selectMax(header->succ, _size); } // 整体最大者
    // 可写访问接口
    ListNodePosi(T) insertAsFirst(T const &e);                               // 将e当作首节点插入
    ListNodePosi(T) insertAslast(T const &e);                                // 将e当作末节点插入
    ListNodePosi(T) insertBefore(ListNodePosi(T) p, T const &e);             // 将e当作p的前驱插入
    ListNodePosi(T) insertAfter(ListNodePosi(T) p, T const &e);              // 将e当作p的后驱插入
    T remove(ListNodePosi(T) p);                                             // 删除合法位置p处的节点，返回被删除节点
    void merge(List<T> &L) { merge(first(), _size, L, L.first(), L._size); } // 全列表归并
    void sort(ListNodePosi(T) p, int n);                                     // 列表区间排序
    void sort() { sort(first(), _size); }                                    // 列表整体排序
    int deduplicate();                                                       // 无序去重
    int uniquify();                                                          // 有序去重
    void reverse();                                                          // 前后倒置
    // 遍历
    void traverse(void (*)(T &)); // 遍历，依次实施visit操作
    template <typename VST>       // 操作器
    void traverse(VST &);         // 遍历，依次实施visit操作
}; // List

//3.3列表类内部方法init()
template<typename T>void List<T>::init(){//列表初始化，在创建列表时统一调用
    header = new ListNode<T>;//创建头哨兵节点
    trailer = new ListNode<T>;//创建尾哨兵节点
    hearder->succ = trailer;
    header->pred = NULL;
    trailer->pred = header;
    trailer-> = NULL;
    _size = 0;//记录规模
}

//3.4重载列表类的下标操作符
template <typename T>//重载下标操作符，以通过秩直接访问列表节点（
T& List<T>::operator[](Rank r) const {
    ListNodePosi(T) p = first();
    while(0<r--)
        p = p->succ;
    return p->date;
}