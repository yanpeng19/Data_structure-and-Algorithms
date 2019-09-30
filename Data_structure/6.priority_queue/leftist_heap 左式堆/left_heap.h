#pragma once

/*
��ʽ��:������ȫ����ѺͶ������γɵ�ƫ�ƶѣ�����
--------------------------------------------------
0.�ص�Ͷ��壺
    ��ʽ���������ȫ�������˵������ʽ�������ƫ�Ƶģ�
	����ʽ���������ȫ�������˵���ŵ����ڶѺϲ�Ч�ʣ�
	���Դﵽ O(n) ,������ O��n+m);

	���ص㣺
	a.��ߵĽڵ�϶�
	b.������������������������ÿ���ڵ㶼�������Եģ�
	  ����Ϊ��û���������ܣ��������ֽṹ�ǺϷ��ģ�

1.�����ӿں�һЩ���壺
    get_Max()      ��ȡ���ڵ�       O(1)
    delete_Max()   ɾ�����ڵ�       O(logn)
    insert()       ����Ԫ��           O(logn)
	�ⲿ������
	merge()        �ϲ��������ȼ����� O(logn)

	�ϲ�Ϊʲôʹ���ⲿ����ʽ��
	��Ϊ������ó�������ʽ������������޸����׵����߼����ң�
	��merge()��ͨ���ݹ���ʽʵ�֣�������ɽڵ�ʧЧ

2.ʵ�ֵ��߼���
    2.0 �ϲ�������   
	  merge(node x,node y)  �ϲ�������x�Ͷ�y����ʵ����ͨ���ݹ����ʽ��
      *.�ݹ����  �ڵ�x ���� y ��Ϊ�棻
	  a.�ȽϽڵ�x��y��ֵ�������֤��߽ڵ��ֵ�����ģ���� x->data < y->data;
	    swap(x,y)
	  b.�� x->rChild �� y ���кϲ������ҷ���һ����ͷ�ڵ�new_right,���� x->rChild;
	  c.�Ƚ� x->lChild �� x ->rChild ��nplֵ������֧���ߡ��ȣ�
	    �Ѹ��ߵĽڵ������ߣ�(#��һ��ȷ���˽ڵ���ʽ�ѵ�����б�ԣ�
	  #�ϲ����������Ա�֤�� 1.���ڵ����ڵ�����ҽڵ�ʽ�δ�ڵ�
	                        2.��ʽ�ѵ�����б��

    2.1 ���������
	  insert(t)  ͨ�������ڵ�� �ò���ֵ���ɵ��½ڵ� new_node = node(t); 
	             ���кϲ������ɣ�

	2.2 ɾ��������
	  delete_Max() ͨ���� ���ڵ����Ӻ����� ���кϲ������������ҷ��ؽڵ���Ϊ
	               �µĸ��ڵ㼴�ɣ�

*/

#include "PQ.h"
#include "bintree.h"

using namespace std;

/*
npl: �ڵ�x��������ⲿ�׶ε���̾���ĳ���
npl(�ⲿ�ڵ㣩 = 0��
*/

template<typename T>
int npl(BinNodePosi<T> x)
{
	if (!x) return 0;
	return 1 + min(npl(x->lChild), npl(x->rChild));
	 
}

#define _root BinTree<T>::_root
#define _size BinTree<T>::_size

/*
�ϲ������ѵ����̣��������ѵĸ��ڵ���Ϊ���� ����������

1.�ȽϽڵ� npl��l) �� npl��r) ���ϸߵĽڵ�������
  1.a l��r �������ڣ����ؿսڵ�
  1.b ������� l �� r ��Ϊ�棬ֱ��return

2.ͨ���ݹ���ʽ����l.rChild �� r �ϲ� ,l->rChild = merge()
  2.1 �ݹ�� l���� r ��Ϊ�棨�Ѿ�����ײ㣩��������߶���Ϊ�棬�����ֱ��RETUNR
  2.1.2 �жϽڵ�l�ͽڵ�r ����ֵ����� l<r �򽻻�������ֵ

2.2 �ж� npl(l) �� npl(r)�����Ƿ����������������򻥻�����λ�ã�ͬʱ�������ڵ��ָ��

2.3 �ع���һ��

3.�����µĸ��ڵ�
*/
template<typename T>
BinNodePosi<T> my_merge(BinNodePosi<T> l, BinNodePosi<T> r)
{
	if (!l && !r) return NULL; //1.a �������
	else if (!r) return l;  //�����ݹ��
	else if (!l) return r;
	//ֵ�ϴ�������
	if (l->data < r->data) swap(l, r);
	l->rChild = my_merge<T>(l->rChild, r);
	//����һϵ�е�����ڵ�ߵķ����
	if (npl<T>(l) < npl<T>(r))
	{
		BinNodePosi<T> temp = l;
		l = r;
		r = temp;
	}
	if (l->rChild) l->rChild->parent = l;
	return l;
}

template <typename T>
class left_heap :public BinTree<T>,public PQ<T>
{
public:
	void insert(const T&t);
	T get_Max() { return _root->data; };
	T delete_Max();
};

template<typename T>
void left_heap<T>::insert(const T& t)
{
	BinNodePosi<T> temp = new BinNode<T>(t);
	_root = my_merge<T>(_root, temp);
	_root->parent = NULL;
	_size++;
}

template<typename T>
T left_heap<T>::delete_Max()
{
	if (!_root) return NULL;
	T temp = _root->data;
	BinNodePosi<T> l = _root->lChild;
	BinNodePosi<T> r = _root->rChild;
	delete _root;
	_size--;
	_root = my_merge<T>(l, r);
	if(_root) _root->parent = NULL;
	return temp;
}



