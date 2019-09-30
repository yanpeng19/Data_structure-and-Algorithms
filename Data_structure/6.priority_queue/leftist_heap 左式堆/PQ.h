#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "my_vector.h"

using namespace std;

/*
���ȼ����У�
����Ԫ�صġ����ȼ������������Լ��洢�Ľṹ��

����������
1.insert(T &t) �������
2.get_Max() ��ȡ�������ֵ
3.delete_Max() ɾ�����������ȼ����ֵ

*/

//PQ Ϊ�����Դ��麯���࣬
template<typename T>
class PQ
{
public:
	virtual void insert(const T&t) = 0;
	virtual T get_Max() = 0;
	virtual T delete_Max() = 0;
};


/*
PQ_ComplHead ��ȫ����� ���ȼ�����
����vectorʵ�ֵģ���Vector�����һ����ȫ��������
���ҽ����ȼ���ߵ�Ԫ�ط���vecotr[0] ���ڵ�λ��
get_max O��1��
delete_max  O(logn)
insert    O(logn)

�ڵ�Ĺ���
�ڵ�x�ĸ��ڵ㶼���ڵ�������������

����ȫ�����������vector�ķ�����
�нڵ� x rank = i;    vector[i];
�丸�ڵ�  rank = (i-1)/2   vecotr[(i-1)/2]
������    rank =  2i+1     vector[2i+1]
������    rank =  2i+2     vector[2i+2]

�����ӿڣ�
1.insert �������
2.get_Max ��ȡ���ȼ����ֵ
3.delete_Max ɾ�����ȼ����ֵ

���ܺ�����
1.���˽ڵ������ ���²����Ԫ��ͨ��ÿ����������һ�㣬ֱ�� x <= x.parent Ϊֹ
2.���ǽڵ������ ���ڵ�ɾ���󣬽�ĩ�ڵ����������ڵ�󡣲�ͣ��β�³���ֱ��  x >= l/rChild Ϊֹ
3.��������

*/

using Rank = int;

template<typename T>
class PQ_ComplHead : public PQ<T>, public vec<T>
{

#define Parent(i) ((i-1)/2)
#define lChile(i) (i*2+1)
#define rChile(i) (i*2+2)

public:
	//�����ӿ�
	PQ_ComplHead() = default;
	PQ_ComplHead(T* t, Rank n)
	{
		t = t + n;
		while (n >= 0)  // n is rank >= 0
		{
			vec<T>::push_back(*t--);
			n--;
		}
		hepify();
	}
	PQ_ComplHead(vector<T> v) :vec<T>(v)
	{
		hepify();
	}
	void insert(const T& t);
	T get_Max() { return vec<T>::front(); };
	T delete_Max();

	//���ܺ���
	Rank percolateDown(Rank x,Rank n);
	Rank percolateUp(Rank i);
	void hepify();
};

/*
�������Ѻ�����
1.���ڵ����¶��Ͻ������磬�Ӷ�ȷ��ÿ���ڵ����ȷλ��
2.Ҷ�ڵ�����������磬���Դӣ�n/2-1)�ڵ㿪ʼ���в���
3.
*/
template<typename T>
void PQ_ComplHead<T>::hepify()
{
	Rank i = (vec<T>::size() - 1) / 2;
	while (i >= 0)
		percolateDown(i--, vec<T>::size() - 1);
}

/*
ɾ���߼���
1.�����ڵ㱸�ݲ�ɾ�� 
  a���������impty()Ϊ�棬ֱ�ӷ���
2.��ĩ�ڵ��Ԫ�طŵ����ڵ㣬��ִ���¸�����
*/
template<typename T>
T PQ_ComplHead<T>::delete_Max()
{
	if (vec<T>::empty()) return NULL;
	swap(vec<T>::operator[](0), vec<T>::operator[](vec<T>::size() - 1));

	T temp = vec<T>::operator[](vec<T>::size() - 1);
	vec<T>::erase(vec<T>::size() - 1);
	if(!vec<T>::empty()) percolateDown(0,vec<T>::size()-1);
	return temp;
} 

/*
�¸�������
1.��������(x->l/rChild && x < max(x.lChild,x.rChlid)
2.����x��ϴ�ĺ��ӵ���ֵ
*/
template<typename T>
Rank PQ_ComplHead<T>::percolateDown(Rank x,Rank n)
{
	T temp = vec<T>::operator[](0);
	Rank l, r;
	Rank max_rank;
	l = lChile(x);
	r = rChile(x);
	if (l <= n && r <= n) max_rank = vec<T>::operator[](l) > vec<T>::operator[](r) ? l : r;
	else if (l <= n)  max_rank = l;
	else if (r <= n) max_rank = r;
	else return x;

	while (vec<T>::operator[](x) < vec<T>::operator[](max_rank))
	{
		swap(vec<T>::operator[](x), vec<T>::operator[](max_rank));
		x = max_rank;
		l = lChile(x);
		r = rChile(x);

		if (l <= n && r <= n) max_rank = vec<T>::operator[](l) > vec<T>::operator[](r) ? l : r;
		else if (l <= n)  max_rank = l;
		else if (r <= n) max_rank = r;
		else return x;
	}
}


/*
�����߼�������

1.������͸��������
  a.��Ϊ�գ�����Ϊ�գ�ʱ��ֱ�ӽ��в�����������ؼ���
  b.���ǿգ����������
2.���²����������Ϊĩβ�ڵ���뵽�������飩�У��������ϸ�������ʹ����ѺϷ��������²���ڵ�
*/
template<typename T>
void PQ_ComplHead<T>::insert(const T& t)
{
	vec<T>::push_back(t);
	Rank r = vec<T>::size();
	if (r < 2) return;
	percolateUp(r-1);
}

/*
�ϸ�������
1.�жϹ��� x < Parent(x) && Parent(x) >=0 
2.������� �򽻻�x��Parent(x)
*/
template<typename T>
Rank PQ_ComplHead<T>::percolateUp(Rank r)
{
	Rank p = Parent(r);
	while(vec<T>::operator[](r) > vec<T>::operator[](p))
	{
		swap(vec<T>::operator[](r), vec<T>::operator[](p));
		r = p;
		p = Parent(r);
	}
	return r;
}