typedef int Rank;  // ��
#define DEFAULT_CAPACITY  3 //Ĭ�ϵĳ�ʼ������ʵ��Ӧ���п�����Ϊ����

template <typename T> class Vector { //����ģ����
protected:
	Rank _size; int _capacity; T* _elem; //��ģ��������������
	void copyFrom(T const* A, Rank lo, Rank hi);//������������A[lo, hi]
	void expand();//�ռ䲻��ʱ����
	void shrink();// ��װ���ӹ�Сʱѹ��
	bool bubble(Rank lo, Rank hi);//ɨ�轻��
	void bubbleSort(Rank lo, Rank hi);//���������㷨
	Rank max(Rank lo, Rank hi);//ѡȡ���ֵ
	void selectionSort(Rank lo, Rank hi);//ѡ������
	void merge(Rank lo, Rank mi, Rank hi);//�鲢�㷨
	void mergeSort(Rank lo, Rank hi);//�鲢����
	Rank partition(Rank lo, Rank hi);//��㹹���㷨
	void quickSort(Rank lo, Rank hi);//��������
	void heapSort(Rank lo, Rank hi);//������
public:
//���캯��
	Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0);//����Ϊc����ģΪs������Ԫ�س�ʼΪv
	{_elem = new T[_capacity = c];for (_size = 0; _size < s, _elem[_size++] = v);}//s <= c
	Vector(T const* A, Rank lo, Rank hi){copyFrom(A, lo, hi);}//�������临��
	Vector(T const* A, Rank n){copyFrom(A, 0, n);}//�������帴��
	Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); }//�������临��
	Vector(Vector<T> const$ V) { copyFrom(V._elem, 0, V._size); }//�������帴��
//��������
	~Vector() { delete[] _elem; }//�ͷ��ڲ��ռ�
//ֻ�����ʽӿ�
	Rank size() const { return _size; }//��ģ
	bool empty() const { return !_size; }//�п�
	int disordered() const;//�ж������Ƿ�������
	Rank find(T const& e)const { return find(e, 0, _size); }//���������������
	Rank find(T const& e, Rank lo, Rank hi) const;//���������������
	Rank search(T const& e) const//���������������
	{return(0 >= _size) ? -1 : search(e, 0, _size);}
	Rank search(T const& e, Rank lo, Rank hi) comst��//���������������
//��д���ʽӿ�
	T& operator[](Rank r) const;//�����±������������������������ʽ������Ԫ��
	Vector<T>& operator=(Vector<T> const&);//���ظ�ֵ���������Ա�ֱ�ӿ�¡����
	T remove(Rank r);//ɾ����Ϊr��Ԫ��
	int remove(Rank lo, Rank hi);//ɾ���������䡾lo��hi��֮�ڵ�Ԫ��
	Rank insert(Rank r, T const& e);//����Ԫ��
	Rank insert(T const& e) { return insert(_size, e); }//Ĭ����ΪĩԪ�ز���
	void sort(Rank lo, Rank hi);//�ԡ�lo��hi������
	void sort() { sort(0, _size); }//��������
	void unsort(Rank lo, Rank hi);//�ԡ�lo��hi������
	void unsort() { unsort(0, size); }//��������
	int deduplicate();//����ȥ��
	int uniquify();//����ȥ��
//����
	void traverse(void (*)(T&));//������ʹ�ú���ָ�룬ֻ����ֲ����޸ģ�
	template <typename VST> void traverse(VST&);//������ʹ�ú������󣬿�ȫ�����޸ģ�
};//Vector
//���ڸ��ƵĹ��췽��
template <typename T>//Ԫ������
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) {//����������A��lo,hi��Ϊ������������
	_elem = new T[_capacity = 2 * ��hi - lo)]; _size = 0;//����ռ䣬��ģ����
	while (lo < hi)//A��lo��hi���ڵ�Ԫ����һ
		_elem[_size++] = A[lo++];//������_elem[0,hi-lo]
}
template <typename T> Vector<T>& Vector<T>::operator=(Vector<T> const& V) {//���ظ�ֵ������
	if (_elem) delete[] _elem;//�ͷ�ԭ������
	copyFrom(V._elem, 0, V.size());//���帴��
	return *this;//���ص�ǰ�����Ӧ�ã��Ա���ʽ��ֵ
}
//����
template <typename T> void Vector<T>::expand() {//�����ռ䲻��ʱ����
	if (_size < _capacity) return;//��δ��Աʱ����������
	if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY;//��������С����
	T* oldElem = _elem; _elem = new T[_capacity <<= 1];//�����ӱ�
	for (int i = 0; i < _size; i++)
		_elem[i] = oldElem;//����ԭ�������ݣ�TΪ�������ͣ��������ظ�ֵ��������=����
	delete [] oldElem;//�ͷ�ԭ�ռ�
}