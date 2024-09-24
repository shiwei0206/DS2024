typedef int Rank;		   // 秩
#define DEFAULT_CAPACITY 3 // 默认的初始容量（实际应用中可设置为更大）

template <typename T>
class Vector
{ // 向量模板类
protected:
	Rank _size;
	int _capacity;
	T *_elem;									 // 规模、容量、数据区
	void copyFrom(T const *A, Rank lo, Rank hi); // 复制数组区间A[lo, hi]
	void expand();								 // 空间不足时扩容
	void shrink();								 // 填装因子过小时压缩
	bool bubble(Rank lo, Rank hi);				 // 扫描交换
	void bubbleSort(Rank lo, Rank hi);			 // 起泡排序算法
	Rank max(Rank lo, Rank hi);					 // 选取最大值
	void selectionSort(Rank lo, Rank hi);		 // 选择排序法
	void merge(Rank lo, Rank mi, Rank hi);		 // 归并算法
	void mergeSort(Rank lo, Rank hi);			 // 归并排序法
	Rank partition(Rank lo, Rank hi);			 // 轴点构造算法
	void quickSort(Rank lo, Rank hi);			 // 快速排序法
	void heapSort(Rank lo, Rank hi);			 // 堆排序
public:
	// 构造函数
	Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) // 容量为c，规模为s，所有元素初始为v
	{
		_elem = new T[_capacity = c];
		for (_size = 0; _size < s; _elem[_size++] = v)
			;
	} // s <= c
	Vector(T const *A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }				// 数组区间复制
	Vector(T const *A, Rank n) { copyFrom(A, 0, n); }							// 数组整体复制
	Vector(Vector<T> const &V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); } // 向量区间复制
	Vector(Vector<T> const &V) { copyFrom(V._elem, 0, V._size); }				// 向量整体复制
	// 析构函数
	~Vector() { delete[] _elem; } // 释放内部空间
	// 只读访问接口
	Rank size() const { return _size; }						  // 规模
	bool empty() const { return !_size; }					  // 判空
	int disordered() const;									  // 判断向量是否已排序
	Rank find(T const &e) const { return find(e, 0, _size); } // 无序向量整体查找
	Rank find(T const &e, Rank lo, Rank hi) const;			  // 无序向量区间查找
	Rank search(T const &e) const							  // 有序向量整体查找
	{
		return (0 >= _size) ? -1 : search(e, 0, _size);
	}
	Rank search(T const &e, Rank lo, Rank hi) const; // 有序向量整体查找
	// 可写访问接口
	T &operator[](Rank r) const;						 // 重载下标操作符，可以类似于数组形式引出各元素
	Vector<T> &operator=(Vector<T> const &);			 // 重载赋值操作符，以便直接克隆向量
	T remove(Rank r);									 // 删除秩为r的元素
	int remove(Rank lo, Rank hi);						 // 删除秩在区间【lo，hi】之内的元素
	Rank insert(Rank r, T const &e);					 // 插入元素
	Rank insert(T const &e) { return insert(_size, e); } // 默认作为末元素插入
	void sort(Rank lo, Rank hi);						 // 对【lo，hi】排序
	void sort() { sort(0, _size); }						 // 整体排序
	void unsort(Rank lo, Rank hi);						 // 对【lo，hi】置乱
	void unsort() { unsort(0, size); }					 // 整体置乱
	int deduplicate();									 // 无序去重
	int uniquify();										 // 有序去重
	// 遍历
	void traverse(void (*)(T &)); // 遍历（使用函数指针，只读或局部性修改）
	template <typename VST>
	void traverse(VST &); // 遍历（使用函数对象，可全局性修改）
}; // Vector

// 2.2基于复制的向量构造器
template <typename T> // 元素类型
void Vector<T>::copyFrom(T const *A, Rank lo, Rank hi)
{ // 以数组区间A【lo,hi】为蓝本复制向量
	_elem = new T[_capacity = 2 * (hi - lo)];
	_size = 0;					  // 分配空间，规模清零
	while (lo < hi)				  // A【lo，hi】内的元素逐一
		_elem[_size++] = A[lo++]; // 复制至_elem[0,hi-lo]
}

// 2.3重载向量赋值操作符
template <typename T>
Vector<T> &Vector<T>::operator=(Vector<T> const &V)
{ // 重载赋值操作符
	if (_elem)
		delete[] _elem;				// 释放原有内容
	copyFrom(V._elem, 0, V.size()); // 整体复制
	return *this;					// 返回当前对象的应用，以便链式赋值
}

// 2.4向量内部数组动态扩容算法expand()
template <typename T>
void Vector<T>::expand()
{ // 向量空间不足时扩容
	if (_size < _capacity)
		return; // 尚未满员时，不必扩容
	if (_capacity < DEFAULT_CAPACITY)
		_capacity = DEFAULT_CAPACITY; // 不低于最小容量
	T *oldElem = _elem;
	_elem = new T[_capacity <<= 1]; // 容量加倍
	for (int i = 0; i < _size; i++)
		_elem[i] = oldElem[i]; // 复制原向量内容（T为基本类型，或已重载赋值操作符‘=’）
	delete[] oldElem;		   // 释放原空间
}

// 2.5向量内部功能shrink()
template <typename T>
void Vector<T>::shrink()
{ // 填装因子过小时压缩向量所占空间
	if (_capacity < DEFAULT_CAPACITY << 1)
		return; // 不致收缩到DEFAULT_CAPACITY以下
	if (_size << 2 > _capacity)
		return; // 以25%为界
	T *oldElem = _elem;
	_elem = new T[_capacity >>= 1]; // 容量减半
	for (int i = 0; i < _size; i++)
		_elem[i] = oldElem[i]; // 复制原向量内容
	delete[] oldElem;		   // 释放原空间
}

// 2.6重载向量操作符[]
template <typename T>
T &Vector<T>::operator[](Rank r) const // 重载下标操作符
{
	return _elem[r];
} // assert:0<=r<-size

// 2.7向量整体置乱算法permute()
template <typename T>
void permute(Vector<T> &V) // 随机置乱向量，使各元素等概率出现于每一位置
{
	for (int i = V.size(); i > 0; i--) // 自后向前
		swap(V[i - 1], V[rand() % i]); // v[i-1]与v[0,1]中某一随机元素交换
}

// 2.8向量区间置乱接口unsort()
template <typename T>
void Vector<T>::unsort(Rank lo, Rank hi) // 等概率随机置乱向量区间[lo,hi]
{
	T *V = _elem + lo;				   // 将子量——elem[lo,hi]视作另一向量v[0,hi-lo]
	for (Rank i = hi - lo; i > 0; i--) // 自后向前
		swap(V[i - 1], V[rand() % i]); // 将v[i-1]与v[0,i]中某一元素随机交换
}

// 2.9重载比较器以便比较对象指针
template <typename T>
static bool lt(T *a, T *b) { return lt(*a, *b); } // less than
template <typename T>
static bool lt(T &a, T &b) { return a < b; } // less than
template <typename T>
static bool eq(T *a, T *b) { return eq(*a, *b); } // equal
template <typename T>
static bool eq(T &a, T &b) { return a == b; } // equal

// 2.10无序向量元素查找接口find()
template <typename T>									 // 无序向量的顺序查找：返回最后一个元素e的位置；失败是，返回lo-1
Rank Vector<T>::find(T const &e, Rank lo, Rank hi) const // assert:0<=lo<hi<=_size
{
	while ((lo < hi--) && (e != _elem[hi])) // 从后向前，顺序查找
		;
	return hi; // 若hi<lo,则意味着失败，否则hi即命中元素的秩
}

// 2.11向量元素插入接口insert()
template <typename T>					   // 将e作为秩为rr元素插入
Rank Vector<T>::insert(Rank r, T const &e) // assert:0<=r<=size
{
	expand(); // 若有必要，扩容
	for (int i = _size; i > r; i--)
		_elem[i] = _elem[i - 1]; // 自后向前，后继元素顺次后移一个单元
	_elem[r] = e;
	_size++;  // 置入新元素并更新容量
	return r; // 返回秩
}

// 2.12向量区间删除接口remove(lo,hi)
template <typename T>
int Vector<T>::remove(Rank lo, Rank hi) // 删除区间[lo,hi]
{
	if (lo == hi)
		return 0; // 处于效率考虑，单独处理退化情况，比如remove(0,0)
	while (hi < _size)
		_elem[lo++] = _elem[hi]; //[hi,_size]顺次前移hi-lo个单元
	_size = lo;					 // 更新规模，直接丢弃尾部[lo,_size=hi]区间
	shrink();					 // 若有必要则缩容
	return hi - lo;				 // 返回被删除元素数目
}

// 2.13向量单元素删除接口remove()
template <typename T>
T Vector<T>::remove(Rank r) // 删除向量中秩为r的元素，0<=r<=size
{
	T e = _elem[r];	  // 备份被删除元素
	remove(r, r + 1); // 调用区间删除算法，等效于对区间[r,r+1]的删除
	return e;		  // 返回被删除元素
}

// 2.14无序向量清除重复元素接口deduplicate()
template <typename T>
int Vector<T>::deduplicate() // 删除无序向量中重复元素(高效版)
{
	int oldSize = _size;			 // 记录原规模
	Rank i = 1;						 // 从_elem[1]开始
	while (i < _size)				 // 自前向后逐一考查各元素_elem[i]
		(find(_elem[i], 0, i) < 0) ? // 在其后缀中寻找与其雷同者(至多一个)
			i++
								   : remove(i); // 若无雷同则继续考察后继，否则删除雷同者
	return oldSize - _size;						// 向量规模变化量，即被删除元素总数
}

// 2.15向量遍历接口traverse()
template <typename T>
void Vector<T>::traverse(void (*visit)(T &)) // 利用函数指针机制遍历
{
	for (int i = 0; i < _size; i++)
		visit(_elem[i]);
}
template <typename T>
template <typename VST>				 // 元素类型，操作器
void Vector<T>::traverse(VST &visit) // 利用函数对象机制的遍历
{
	for (int i = 0; i < _size; i++)
		visit(_elem[i]);
}

// 2.16基于遍历实现increase()功能
template <typename T>
struct Increase // 函数对象：递增一个T类对象
{
	virtual void operator()(T &e) { e++; } // 架设T可直接递增或已重载++
};
template <typename T>
void increase(Vector<T> &V) // 统一递增向量中的各元素
{
	V.traverse(Increase<T>()); // 以Increase<T>()为基本操作进行遍历
}

// 2.17有序向量甄别算法disordered()
template <typename T>
int Vector<T>::disordered() const
{									// 返回向量中逆序相邻元素对的总数
	int n = 0;						// 计数器
	for (int i = 1; i < _size; i++) // 注意检查
		if (_elem[i - 1] > _elem[1])
			n++; // 逆序则计数
	return n;	 // 向量有序当且仅当n=0
}

//  2.18有序向量uniquify()接口的平凡实现
// template <typename T>
// int Vector<T>::uniquify()
// { // 有序向量重复元素剔除算法(低效版)
// 	int oldSize = _size;
// 	int i = 1;										// 当前比对元素的秩，起始于首元素
// 	while (i < _size)								// 从前向后，逐一比对各对相邻元素
// 		_elem[i - 1] == _elem[1] ? remove(i) : i++; // 若雷同，则删除后者；否则转至后一元素
// 	return oldSize - _size;							// 向量规模变化，即被删除元素总数
// }

// 2.19有序向量uniquify()接口的高效实现
template <typename T>
int Vector<T>::uniquify()
{								   // 有序向量重复元素剔除算法(高效版)
	Rank i = 0, j = 0;			   // 各对互异"相邻"元素的秩
	while (++j < _size)			   // 逐一扫描，直至末元素
		if (_elem[i] != _elem[j])  // 跳过雷同者
			_elem[++i] = _elem[j]; // 发现不同元素时，向前移至紧邻于前者右侧
	_size = ++i;
	shrink();	  // 直接截除尾部多余元素
	return j - i; // 向量规模变化量，即被删除元素总数
}
