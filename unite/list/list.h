// 列表模板类
#include "ListNode.h" //引入列表节点类

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
    ListNodePosi(T) insertAsLast(T const &e);                                // 将e当作末节点插入
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
T& List<T>::operator[](Rank r) const {//assert: 0 <= r < size
    ListNodePosi(T) p = first();//从首节点出发
    while(0<r--) p = p->succ;//顺数第r个节点即是
    return p->data;//目标节点，返回其中所存元素
}

//3.5无序列表元素查找接口find()
template <typename T>//在无序列表内节点p(可能是trailer)的n个(真)前驱中，找到等于e的最后者
ListNodePosi(T)List<T>::find(T const& e, int n,ListNodePosi(T) p) const{
    while (0 <= n--)
        if (e == (p = p->pred) return p;)
    return NULL;
}


//3.6列表节点插入接口
template<typename T> ListNodePosi(T) List<T>::insertAsFirst(T const& e)
{_size++;return header->insertAsSucc(e);}
template<typename T>ListNodePosi(T) List<T>::insertAsLast(T const& e)
{_size++;return trailer->insertAsPred(e);}
template<typename T> ListNodePosi(T) List<T>::insertBefore(ListNodePosi(T)p,T const& e)
{_size++;return p->insertAsPred(e)}
template<typename T> ListNodePosi(T) List<T>::insertAfter(ListNodePosi(T) p,T const& e)
{_size++;return p->insertAsSucc(e);}

//3.7ListNode::insertAsPred()算法
template<typename T>
ListNodePosi(T) ListNode<T>::insertAsPred(T const& e){
    ListNodePosi(T) x = new ListNode(e, pred, this);
    pred->succ = x;
    pred = x;
    return x;
}

//3.8ListNode::insertAsSucc()算法
template<typename T>
ListNodePosi(T) ListNode<T>::insertAsSucc(T const& e){
    ListNodePosi(T) x = new ListNode(e, this, succ);
    succ->pred = x;
    succ = x;
    return x;
}

//3.9列表内部方法copyNodes()
template<typename T>
void List<T>::copyNodes(ListNodePosi(T) p, int n){
    init();
    while(n--){
        insertAsLast(p->data);
        p = p->succ;
    }
}

//3.10基于复制的列表构造方法
template <typename T>
List<T>::List(ListNodePosi(T) P, int n) { copyNodes(p, n); }

template <typename T>
List<T>::List(List<T> const &L) { copyNodes(L.first(), L._size); }

template <typename T>
List<T>::List(List<T> const &L, int r, int n) { copyNodes(L[r], n); }

//3.11列表删除接口remove()
template<typename T> T List<T>::remove(ListNodePosi(T) p){
    T e = p->data;
    p->pred->succ = p->succ;
    p->succ->pred = p->pred;
    delete p;
    _size--;
    return e;
}


//3.12列表析构方法
template<typename T> List<T>::~List()
{
    clear();
    delete header;
    delete trailer;
}

//3.13列表清空方法clear()
template<typename T> int List<T>::clear(){
    int oldSize = _size;
    while(0<_size)
        remove(header->succ);
    return oldSize;
}

//3.14无序列表剔除重复节点接口deduplicate()
template<typename T> int List<T>::deduplicate(){
    if (_size<2)
        return 0;
    int oldSize = _size;
    ListNodePosi(T) p = header;
    Rank r = 0;
    while (trailer != (p = p->succ))
    {
        ListNodePosi(T) q = find(p->data, r, p);
        q ? remove(q) : r++;
    }
    return oldSize - _size;
}

//3.15列表遍历接口traverse()
template<typename T> void List<T>::traverse(void (*visit)(T&))
{
    for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ)
        visit(p->data);
}

template<typename T> template<typename VST>
void List<T>::traverse(VST& visit)
{
    for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ)
        visit(p->data);
}


//3.16有序列表剔除重复节点接口uniqify()
template<typename T> int List<T>::uniquify()
{
    if (_size < 2)
        return 0;
    int oldSize = -size;
    ListNodePosi(T) p;
    ListNodePosi(T) q;
    for (p = header, q = p->succ; trailer != q; p = q, q = q->succ)
        if (p->data == q->data){
            remove(q);
            q = p;
        }
    return oldSize - _size;
}

//3.17有序列表查找接口search()
template<typename T>
ListNodePosi(T) List<T>::search(T const& e,int n,ListNodePosi(T)p) const{
    while (0 <= n--)
        if (((p = p->pred)->data) <= e) break;
    return p;
}

//3.18有序列表基于排序的构造方法
template <typename T>
void List<T>::sort(ListNodePosi(T) p, int n){
    switch (rand() % 3)
    {
    case 1 :
        insertionSort(p, n);
        break;
    case 2:
        selectionSort(p, n);
        break;
    default:
        mergeSort(p, n);
        break;
    }
}

//3.19列表的插入排序
template <typename T>
void List<T>::insertionSort(ListNodePosi(T) p, int n){
    for (int r = 0; r < n; r++)
    {
        insertAfter(search(p->data, r, p), p->data);
        p = p->succ;
        remove(p->pred);
    }
}

//3.20列表的选择排序
template <typename T>
void List<T>::selectionSort(ListNodePosi(T) p, int n){
    ListNodePosi(T) head = p->pred;
    ListNodePosi(T) tail = p;
    for (int i = 0; i < n; i++)
        tail = tail->succ;
    while (1 < n){
        ListNodePosi(T) max = selectMax(head->succ, n);
        insertBefore(tail, remove(max));
        tail = tail->pred;
        n--;
    }
}

//3.21列表最大节点的定位
template <typename T>
ListNodePosi(T) List<T>::selectMax(ListNodePosi(T) p, int n){
    ListNodePosi(T) max = p;
    for (ListNodePosi(T) cur = p; 1 < n; n--)
        max = cur;
    return max;
}

//3.22有序列表的二路归并
template <typename T>
void List<T>::merge(ListNodePosi(T) & p, int n, List<T> &L, ListNodePosi(T) q, int m){
    ListNodePosi(T) pp = p->pred;
    while (0 < m)
    {
        if ((0 < n) && (p->data <= q->data))
        {
            if (q == (p = p->succ))
                break;
            n--;
        }
        else
        {
            insertBefore(p, L.remove((q = q->succ)->pred));
            m--;
        }
    }
    p = pp->succ;
}

//列表的归并排序
template <typename T>
void List<T>::mergeSort(ListNodePosi(T) & p, int n){
    if (n < 2)
        return;
    int m = n >> 1;
    ListNodePosi(T) q = p;
    for (int i = 0; i < m; i++)
        q = q->succ;
    mergeSort(p, m);
    mergeSort(q, n - m);
    merge(p, m, *this, q, n - m);
}