#include<stdlib.h>
#include <algorithm>
typedef int Rank;
#define DEFAULT_CAPACITY  3

template <typename T> class Vector {
protected:
	Rank _size; int _capacity ; T* _elem;
	void copyFrom(T const* A, Rank lo, Rank hi);
	void expand();
	void shrink();
	bool bubble(Rank lo, Rank hi);
	
	Rank max(Rank lo, Rank hi);
	void selectionSort(Rank lo, Rank hi);
	void merge(Rank lo, Rank mi, Rank hi);
	
	Rank partition(Rank lo, Rank hi);
	void quickSort(Rank lo, Rank hi);
	void heapSort(Rank lo, Rank hi);
public:
    void bubbleSort(Rank lo, Rank hi);
    void mergeSort(Rank lo, Rank hi);
	Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0)
	{
		_elem = new T[_capacity = c]; for (_size = 0; _size < s; _elem[_size++] = v);
	}
	Vector(T const* A, Rank n) { copyForm(A, 0, n); }
	Vector(T const* A, Rank lo, Rank hi) { copyForm(A, 0, hi); }
	Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); }
	Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, 0, lo, hi); }
	~Vector() { delete[] _elem; }
	Rank size() const { return _size; }
	bool empty() const { return !_size; }
	int disordered() const;
	Rank find(T const& e) const { return find(e, 0, _size); }
	Rank find(T const& e, Rank lo, Rank hi)const;
	Rank search(T const& e) const
	{
		return (0 >= _size) ? -1 : search(e, 0, _size);
	}
	Rank search(T const& e, Rank lo, Rank hi)const;
	T& operator [](Rank r)const;
	Vector<T>& operator= (Vector<T> const&);
	T remove(Rank r);
	int remove(Rank lo, Rank hi);
	Rank insert(Rank r, T const& e);
	Rank insert(T const& e) { return insert(_size, e); }
	void sort(Rank lo, Rank hi);
	void sort() { sort(0, _size); }
	void unsort(Rank lo, Rank hi);
	void unsort() { unsort(0, _size); }
	int deduplicate();
	int uniquify();
	void traverse(void(* )(T&));
	template <typename VST> void traverse( VST& );
};
template <typename T>
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) {
	_elem = new T[_capacity = 2 * (hi - lo)]; _size = 0;
	while (lo < hi)
		_elem[_size++] = A[lo++];
};
template <typename T> Vector<T>& Vector <T>::operator = (Vector<T> const& V) {
    if(_elem) delete [] _elem;
    copyFrom (V._elem,0,V.size());
	return *this;
}
template<typename T>void Vector<T>::expand(){
	if (_size <_capacity)return;
	if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY;
	T* oldElem = _elem; _elem = new T[_capacity <<=1];
	for (int i=0; i < _size; i++)
	    _elem[i]=oldElem[i];
	delete [ ] oldElem;	

}
template <typename T> void Vector<T>::shrink(){
	if( _capacity < DEFAULT_CAPACITY << 1)return;
	if( _size<< 2 >_capacity)return;
	T* oldElem = _elem; _elem=new T[_capacity>>=1];
	for (int i=0; i< _size; i++)_elem[i] = oldElem[i];
	delete [] oldElem;
}
template <typename T> T& Vector<T>::operator[] (Rank r) const
{ return _elem[r];}
template <typename T> void permute (Vector<T>& V){
	for ( int i=V.size(); i > 0; i--)
	    swap ( V[i-1],V[rand() % i] );
}
template <typename T> void Vector<T>::unsort (Rank lo,Rank hi){
	T* V = _elem + lo;
	for ( Rank i=hi-lo;i > 0;i--)
	    std::swap ( V[i-1],V[rand() % i]);
}
template < typename T> static bool lt (T* a, T* b){ return lt ( *a, *b);}
template < typename T> static bool lt (T& a, T& b){ return a<b; }
template < typename T> static bool eq (T* a, T* b){ return ep (*a,*b);}
template < typename T> static bool eq (T& a, T& b){ return a==b;}
template < typename T>
Rank Vector<T>::find (T const& e,Rank lo,Rank hi )const{
	while ((lo<hi--) && (e !=_elem[hi]));
	return hi;
}
template <typename T>
Rank Vector<T>::insert (Rank r,T const& e ){
	expand();
	for (int i=_size;i>r;i--)_elem[i]=_elem[i-1];
	_elem[r] = e;_size++;
	return r;
}

template <typename T> int Vector<T>::remove(Rank lo,Rank hi){
	if (lo == hi) return 0;                                                       //41
	while (hi < _size) _elem[lo++]=_elem[hi++];
	_size = lo;
	shrink();
	return hi - lo;

}
template <typename T> T Vector<T>::remove ( Rank r){
	T e =_elem[r];
	remove (r,r+1);
	return e;
}
template <typename T> int Vector<T>::deduplicate(){
	int oldSize = _size;                                                           //42
	Rank i=1;
	while (i<_size)
	(find (_elem[i],0,i)< 0)?
	i++:remove(i);
	return oldSize - _size;
}
template <typename T> void Vector<T> :: traverse ( void (*visit)(T&))
{
for (int i=0;i<_size;i++)visit(_elem[i]);
}
template<typename T>template <typename VST>
void Vector<T>:: traverse (VST& visit)
{
	for (int i=0;i<_size;i++) visit (_elem[i]);                                    //43
}
template<typename T>struct Increase

{
	virtual void operator() (T& e ){ e++;}
};
template <typename T> void increase (Vector<T> & V)
{V.traverse (Increase<T>());}
template <typename T> int Vector<T>:: disordered() const{
	int n=0;
	for (int i=1;i < _size;i++)
	if (_elem[i-1] > _elem[i])n++;
	return n;
}
template <typename T> int Vector<T>:: uniquify(){
	int oldSize = _size; int i=1;
	while (i <_size)
	_elem[i-1] == _elem[i] ? remove(i) : i++;
	return oldSize - _size;
}                                                                               //45
// template <typename T> int Vector<T>:: uniquify(){
// 	Rank i=0 ,j=0;
// 	while(++j< _size)
// 	if (_elem[i] != _elem[j])
// 	_elem[++i]=_elem[j];
// 	_size =++i;shrink();
// 	return j-i;
// }
template <typename T>
Rank Vector<T> :: search ( T const& e,Rank lo,Rank hi) const{
	return (rand()%2)?
	binSearch (_elem,e,lo,hi) : fibSearch ( _elem,e,lo,hi);
}                                                                        //47
template<typename T > static Rank binSearch (T* A,T const& e,Rank lo,Rank hi){
	while(lo<hi){
		Rank mi = (lo +hi) >>1;
		if   (e <A[mi]) hi =mi;
		else if (A[mi]<e) lo =mi +1;
		else    return mi;
	}
	return -1;
}                                                                         //48
// template <typename T> static Rank binSearch (T* A,T const& e,Rank lo,Rank hi){
// 	while (1<hi -lo){
// 		Rank mi =(lo + hi) >>1;
// 		(e <A[mi]) ? hi = mi : lo =mi;
// 	}
// 	return (e == A[lo])?lo : -1;

// }
                                                                     //55
// template <typename T> static Rank binSearch (T* A,T const& e,Rank lo,Rank hi){
// 	while (lo < hi){
// 		Rank mi =(lo + hi) >> 1;
// 		(e < A[mi])? hi = mi : lo = mi +1;
// 	}
// 	return --lo;
// }
template<typename T> void Vector<T>::sort (Rank lo,Rank hi){
	switch ( rand() % 5){
		case 1: bubbleSort (lo,hi); break;
		case 2: selectionSort (lo,hi); break;
		case 3: mergeSort (lo,hi); break;
		case 4: heapSort (lo,hi); break;
		default: quickSort (lo,hi);break;
	}
}
template <typename T>
void Vector<T>:: bubbleSort (Rank lo,Rank hi)
{ while ( !bubble ( lo, hi--));}


template <typename T>
bool Vector<T>::bubble(Rank lo, Rank hi) // 一趟扫描交换
{
	bool sorted = true; // 整体有序标志
	while (++lo < hi)	// 自左向右，逐一检查各对相邻元素
		if (_elem[lo - 1] > _elem[lo])
		{										 // 若逆序，则
			sorted = false;						 // 意味着尚未整体有序，并需要
			std::swap(_elem[lo - 1], _elem[lo]); // 通过交换使局部有序
		}
	return sorted; // 返回有序标志
}

template <typename T>
void Vector<T>::mergeSort (Rank lo,Rank hi){
	if (hi - lo < 2) return;
	int mi = (lo + hi) / 2;
	mergeSort (lo,mi); mergeSort (mi,hi);
	merge ( lo,mi,hi);
}
template <typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi) {
    T* A = _elem + lo;
    int lb = mi - lo;
    T* B = new T[lb];
    for (Rank i = 0; i < lb; i++) B[i] = A[i]; // 复制前子向量
    
    int lc = hi - mi;
    T* C = _elem + mi; // 后子向量C[0, lc)就地
    
    for (Rank i = 0, j = 0, k = 0; j < lb; ) { // 归并：反复从B和C中取出更小者
        if (k >= lc || B[j] < C[k]) A[i++] = B[j++];
        else A[i++] = C[k++];
    }
    
    delete [] B; // 释放临时空间B
}