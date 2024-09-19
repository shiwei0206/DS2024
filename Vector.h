typedef int Rank;  // 秩
#define DEFAULT_CAPACITY  3 //默认的初始容量（实际应用中可设置为更大）

template <typename T> class Vector { //向量模板类
protected:
	Rank _size; int _capacity; T* _elem; //规模、容量、数据区
	void copyFrom(T const* A, Rank lo, Rank hi);//复制数组区间A[lo, hi]
	void expand();//空间不足时扩容
	void shrink();// 填装因子过小时压缩
	bool bubble(Rank lo, Rank hi);//扫描交换
	void bubbleSort(Rank lo, Rank hi);//起泡排序算法
	Rank max(Rank lo, Rank hi);//选取最大值
	void selectionSort(Rank lo, Rank hi);//选择排序法
	void merge(Rank lo, Rank mi, Rank hi);//归并算法
	void mergeSort(Rank lo, Rank hi);//归并排序法
	Rank partition(Rank lo, Rank hi);//轴点构造算法
	void quickSort(Rank lo, Rank hi);//快速排序法
	void heapSort(Rank lo, Rank hi);//堆排序
public:
//构造函数
	Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0);//容量为c，规模为s，所有元素初始为v
	{ _elem = new T[_capacity = c]; for (_size = 0; _size < s; _elem[_size++] = v); }//s <= c
	Vector(T const* A, Rank lo, Rank hi){copyFrom(A, lo, hi);}//数组区间复制
	Vector(T const* A, Rank n){copyFrom(A, 0, n);}//数组整体复制
	Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); }//向量区间复制
	Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); }//向量整体复制
//析构函数
	~Vector() { delete[] _elem; }//释放内部空间
//只读访问接口
	Rank size() const { return _size; }//规模
	bool empty() const { return !_size; }//判空
	int disordered() const;//判断向量是否已排序
	Rank find(T const& e)const { return find(e, 0, _size); }//无序向量整体查找
	Rank find(T const& e, Rank lo, Rank hi) const;//无序向量区间查找
	Rank search(T const& e) const//有序向量整体查找
	{return(0 >= _size) ? -1 : search(e, 0, _size);}
	Rank search(T const& e, Rank lo, Rank hi) const；//有序向量整体查找
//可写访问接口
	T& operator[](Rank r) const;//重载下标操作符，可以类似于数组形式引出各元素
	Vector<T>& operator=(Vector<T> const&);//重载赋值操作符，以便直接克隆向量
	T remove(Rank r);//删除秩为r的元素
	int remove(Rank lo, Rank hi);//删除秩在区间【lo，hi】之内的元素
	Rank insert(Rank r, T const& e);//插入元素
	Rank insert(T const& e) { return insert(_size, e); }//默认作为末元素插入
	void sort(Rank lo, Rank hi);//对【lo，hi】排序
	void sort() { sort(0, _size); }//整体排序
	void unsort(Rank lo, Rank hi);//对【lo，hi】置乱
	void unsort() { unsort(0, size); }//整体置乱
	int deduplicate();//无序去重
	int uniquify();//有序去重
//遍历
	void traverse(void (*)(T&));//遍历（使用函数指针，只读或局部性修改）
	template <typename VST> void traverse(VST&);//遍历（使用函数对象，可全局性修改）
};//Vector
//基于复制的向量构造器
template <typename T>//元素类型
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) {//以数组区间A【lo,hi】为蓝本复制向量
	_elem = new T[_capacity = 2 * （hi - lo)]; _size = 0;//分配空间，规模清零
	while (lo < hi)//A【lo，hi】内的元素逐一
		_elem[_size++] = A[lo++];//复制至_elem[0,hi-lo]
}
//重载向量赋值操作符
template <typename T> Vector<T>& Vector<T>::operator=(Vector<T> const& V) {//重载赋值操作符
	if (_elem) delete[] _elem;//释放原有内容
	copyFrom(V._elem, 0, V.size());//整体复制
	return *this;//返回当前对象的应用，以便链式赋值
}
//向量内部数组动态扩容算法expand()
template <typename T> void Vector<T>::expand() {//向量空间不足时扩容
	if (_size < _capacity) return;//尚未满员时，不必扩容
	if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY;//不低于最小容量
	T* oldElem = _elem; _elem = new T[_capacity <<= 1];//容量加倍
	for (int i = 0; i < _size; i++)
		_elem[i] = oldElem[i];//复制原向量内容（T为基本类型，或已重载赋值操作符‘=’）
	delete [] oldElem;//释放原空间
}
//向量内部功能shrink()
template <typename T> void Vector<T>::shrink() {//填装因子过小时压缩向量所占空间
	if (_capacity < DEFAULT_CAPACITY << 1) return;//不致收缩到DEFAULT_CAPACITY以下
	if (_size << 2 > _capacity)return;//以25%为界
	T* oldElem = _elem; _elem = new T[_capacity >>= 1];//容量减半
	for (int i = 0; i < _size; i++)_elem[i] = oldElem[i];//复制原向量内容
	delete[] oldElem;//释放原空间
}
//重载向量操作符[]
template <typename T> T& Vector<T>::operator[](Rank r) const
{return _elem[r];}
