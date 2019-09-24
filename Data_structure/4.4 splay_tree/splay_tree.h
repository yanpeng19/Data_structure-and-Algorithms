/*
	������  splay_tree
	------------------------------------------------------------------
	0.������ص㣺 
	��������һ�������ƽ��������������VAL�����Ĳ�ͬ�ǣ�������ÿ�ζ���
	���ϴβ����Ľڵ㣨ɾ���ڵ㣬���丸�ڵ㣬����ڵ��򽫲���ڵ㱾��
	Ų�����ڵ�λ�ã���һ�֡������Ľڵ�����ᱻƵ�������� ��˼����֯����
	�䱾��Ҳ��ͨ�� bst ƽ����������װ�̳ж���

	1.����������
	1.1 search()  ���Ҳ���
	1.2 insert()  �������
	1.3 erase()  ɾ������
	1.4 splay_refact()  ���ε��ع����������ݲ�ͣ��������ڵ�����2�㣬
	                    �����������һ��
	1.5 splay()  �ع����������������Ľڵ�������_root��ͨ��ѭ�� ����
	             splay_refact() ����

	2.ʵ���߼�
	2.1 serach()  �������ܣ�	�Ӹ�Ŀ¼���±������ң���� x Ϊ����ôͨ�� ������������x
	                            ������ x ����� x �����ڣ��� _hot ͨ��������������
	2.2 insert()  ���������    ���Ҳ���ֵt ����� t ���ڣ������κβ�����ֱ�ӷ��� t
	                            �����������ô ֵ t �Ѿ������Һ��� ������ ���ڵ㣩
	                            �������ʧ�ܣ���ô�¹���һ���ڵ� x�����뵽�����ڵ��·���
								���ҽ�ԭ _root->lChild/ _root->rChild �ԱȺ� x �Ĵ�С��
								�˴����ҽڵ�������� x >_root && x < _root->rChild:
								                  _root->rChild->lChild->parent = x;
								                  _root->rChild->parent = x;
												  x->rChild = _root->rChild;
												  x->lChild = _root->lChild;
								������Ӧ�����ӣ�
	2.3 erase()  ɾ��������     ���Ƚ��������������������ʧ�ܣ���ôֱ�ӷ���
	                            ��������ɹ�����ô x �Ѿ�����˸��ڵ㣻
								Ȼ����и��ڵ��ɾ���������ɣ��� succ(x) ��������ɾ����
	2.4 splay_refact()          ���Ĳ�������ĳ���ڵ� x һ���������㣨�������һ�㣩��
                                (1).Ѱ�� �ýڵ�� ���ڵ� f ����ڵ� g
								(2).����AVL����ͨ�����ǵĲ�ͬλ��������� x 
								    ���������ϲ㣬���ҽ�f��g ���뵽�����·�
									(��ͬ������뷽ʽ���Կ�ͼ��
							    (3).����ڵ� gg=g->parent ���ڣ�����g-parent ָ�� x 
								(*).���������g�����ڣ���ôֱ�ӽ� x ������ g֮�ϼ���
								����Ч�� O(h/2)
*/
#pragma once
#include "bst_tree.h"




using namespace std;

template<typename T> using bst_BinNodePosi = bst_BinNode<T>*;


template<typename T>
class splay_tree:public bst_tree<T>
{
public:
	splay_tree() :bst_tree<T>() {};
	~splay_tree() = default;

	bst_BinNodePosi<T> search(const T& T);
	bst_BinNodePosi<T> insert(const T& t);
	bst_BinNodePosi<T> erase(const T& t);
	bst_BinNodePosi<T> splay_refact(bst_BinNodePosi<T> v);
	bst_BinNodePosi<T> splay(bst_BinNodePosi<T> v);

private:
};

template<typename T>
bst_BinNodePosi<T> splay_tree<T>::search(const T& t)
{
	//����t �ɹ��� t������ROOT������ʧ����hot ������ROOT
	//���շ��� t ���� hot

	if (!bst_tree<T>::_root) return bst_tree<T>::_root;
	auto x = bst_tree<T>::_root;
	bst_tree<T>::hot = NULL;

	while(x)
	{
		if (t == x->get_value())
		{
			if (x != bst_tree<T>::_root)
				splay(x);
			return x;
		}
		else if (t> x->get_value())
		{
			bst_tree<T>::hot = x;
			x = x->rChild;
		}
		else
		{
			bst_tree<T>::hot = x;
			x = x->lChild;
		}
	}
	splay(bst_tree<T>::hot);

	return x;
}

template<typename T>
bst_BinNodePosi<T> splay_tree<T>::insert(const T&t)
{
	auto x = search(t); // x ���� ����Ҳ���ܲ�����,������ڣ���ô x.value == t 
	                    // ��������ڣ���xΪ��
	if (x) return x;
	else x = bst_tree<T>::hot;
	bst_tree<T>::size++;

	//��������� ����
	if (!bst_tree<T>::hot)
	{
		bst_tree<T>::_root = new bst_BinNode<T>(t, NULL);
		return bst_tree<T>::_root;
	}

	auto v = new bst_BinNode<T>(t, NULL);
	if (x->get_value() > t) // ���뵽 x->lChild = t
	{
		v->rChild = x;
		v->lChild = x->lChild;
		if (x->lChild) x->lChild->parent = v;
		x->parent = v;
		x->lChild = NULL;
		bst_tree<T>::_root = v;
		return v;
	}
	else
	{
		v->lChild = x;
		v->rChild = x->rChild;
		if (x->rChild) x->rChild->parent = v;
		x->parent = v;
		x->rChild = NULL;
		bst_tree<T>::_root = v;
		return v;
	}
}

template<typename T>
bst_BinNodePosi<T> splay_tree<T>::erase(const T&t)
{
	auto x = bst_tree<T>::seach(t);  //�ò����Ѿ����� ���������͸��¸߶�
	if (x)
	{
		bst_tree<T>::size--;
		auto a = bst_tree<T>::_root->succ();
		if (a)
		{
			a->lChild = x->lChild;
			a->parent = NULL;
			delete bst_tree<T>::_root;
			bst_tree<T>::_root = a;
			return a;
		}
		else if(a= bst_tree<T>::_root->lChild)
		{
			a->parent = NULL;


			delete bst_tree<T>::_root;
			bst_tree<T>::_root = a;
			return a;
		}
		else delete bst_tree<T>::_root;
	}
	else return NULL;
}


template<typename T>
bst_BinNodePosi<T> splay_tree<T>::splay_refact(bst_BinNodePosi<T> v)
{
	bst_BinNodePosi<T> p, g, gg, T0, T1, T2, T3;
	if (v == bst_tree<T>::_root) return NULL;
	p = v->parent;

	if (g = p->parent) //�������
	{
		// g ��root���
		if (gg = g->parent)
		{
			if (g == gg->lChild)
				gg->lChild = v;
			else gg->rChild = v;
		}
		else v->parent = NULL;
		

		//else v->parent = NULL;

		if(p == g->lChild)
		{
			if(v == p->lChild)
			{
				T0 = v->lChild;
				T1 = v->rChild;
				T2 = p->rChild;
				T3 = g->rChild;

				v->lChild = T0;
				v->rChild = g;
				v->parent = gg;
				g->lChild = p;
				g->rChild = T3;
				g->parent = v;
				p->lChild = T1;
				p->rChild = T2;

				bst_tree<T>::BinTree::updateHeighAbove(p);
				if (v->parent == NULL) bst_tree<T>::_root = v;
				return v;
			}
			else if(v==p->rChild)
			{
				T0 = p->lChild;
				T1 = v->lChild;
				T2 = v->rChild;
				T3 = g->rChild;

				v->lChild = p;
				v->rChild = g;
				v->parent = gg;
				p->parent = v;
				g->parent = v;
				p->lChild = T0;
				p->rChild = T1;
				g->lChild = T2;
				g->rChild = T3;
				bst_tree<T>::updateHeight(p);
			
				bst_tree<T>::updateHeight(g);
				bst_tree<T>::updateHeighAbove(v);
				if (v->parent == NULL) bst_tree<T>::_root = v;
				return v;
			}
		}
		else       // p = g->rChild;
		{
			if(v == p->rChild) 
			{
				T1 = v->lChild;

				v->lChild = g;
				v->parent = g->parent;
				if (T1)
				{
					p->rChild = T1;
					T1->parent = p;
				}
				else
					p->rChild = NULL;
				g->parent = v;
				bst_tree<T>::updateHeighAbove(v);
				if (v->parent == NULL) bst_tree<T>::_root = v;
				return v;
			}
			else //  p = g->rChild        v = p -> lChild;
			{
				T0 = g->lChild;
				T1 = v->lChild;
				T2 = v->rChild;
				T3 = p->rChild;

				v->lChild = g;
				v->rChild = p;
				v->parent = g->parent;
				g->lChild = T0;
				g->rChild = T1;
				p->lChild = T2;
				p->rChild = T3;
				p->parent = v;
				bst_tree<T>::updateHeight(p);
				bst_tree<T>::updateHeight(g);
				bst_tree<T>::updateHeighAbove(v);
				if (v->parent == NULL) bst_tree<T>::_root = v;
				return v;
			}
		}
	}
	else // �������
	{
		bst_BinNodePosi<T> g,T0, T1, T2;
		bst_BinNodePosi<T> p = v->parent;

		if (v == p->lChild)
		{
			T1 = v->rChild;
			v->rChild = p;
			v->parent = NULL;
			p->lChild = T1;
			p->parent = v;
			T1->parent = p;
			bst_tree<T>::_root = v;
			return v;
		}
		else
		{
			T1 = v->lChild;
			v->parent = NULL;
			v->lChild = p;
			p->rChild = T1;
			p->parent = v;
			T1->parent = p;
			bst_tree<T>::_root = v;
			return v;
		}
	}
	
}

template<typename T>
bst_BinNodePosi<T> splay_tree<T>::splay(bst_BinNodePosi<T> v)
{
	while(v!=bst_tree<T>::_root)
		v = splay_refact(v);
	return v;
}


