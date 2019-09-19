/*
	AVL��
	---------------------------------------------------------------------------------------
	0.���AVL����һ������Ķ�������������һ�֡���̬ƽ�⡱�������涨��ÿ���ڵ�ĸ߶� �Ĳ�ҪС��2��
			���ԣ�AVL�������˵����һ�ֱȽϿ� �� ƽ�������

			��Ϊ����������BST�Ĳ���Ч��ȡ���ڽڵ�ĸ߶� O(h),��AVL����Ŀ�ľ����� ÿ���ڵ㾡���ܵ�С
			�Ӷ����ﵽ������ʹ�õĸ�Ч��.����ҺͲ���Ч�ʣ����Խ���O(logn��

			�涨�սڵ�ĸ߶�Ϊ-1����Ҷ�ڵ�߶�Ϊ0��

	
	1.��֯��ʽ������ʵ���У���ͨ�� �̳�ƽ�������� BST �� �Բ���ɾ����װ����ʵ��AVL����һ�ַ�ʽ
			
			template<typename T> 
			class avl_tree : public bst_tree<T>;

	2.���ܺ�����
		2.1 is_banlance(Node* x)		�жϽڵ� x �Ƿ�ƽ�� �� ���ҽڵ�߶Ȳ� �Ƿ����1
										Ϊ����˵�� ���ڲ�ƽ������Ϊ�� �򲻴��ڲ�ƽ��
		2.2 Node* insert()		���빦��,�ɹ����ز���ڵ�ָ�룻ʧ�ܷ��ؿ�
		2.3 Node* erase()		ɾ�����ܣ��ɹ��򷵻�ɾ��Ԫ��ָ�룬ʧ�ܷ��ؿգ�
								//���ַ����ǽ�������δʹ�õ��ڴ����ݷ��أ���������

		2.4 insert_rotate()		����ƽ�⺯�������������ڵ�󣬳��ֲ�ƽ������
								��ͨ���˺������е������������γ�ƽ��
		2.5 erase_rotate()		ɾ��ƽ�⺯������ɾ��ĳԪ�غ�������ֲ�ƽ������
								��ͨ���ú������е������γ���ƽ��
		2.6 link_34()           ����ƽ��Ľڵ㣬�ϸߵķ�֧������һ���߶ȵĹ����غ���

	3.ʵ���߼���
		3.1. ���룺		ִ�� bst �Ĳ��� -> ���_hot�Ƿ�ʧ��  -> a.ʧ��  ͨ�� ���������������
															 -> b.��ʧ��  ��������������ֱ�ӷ���
		3.2  ɾ����		ִ�� bst ��ɾ�� -> ���_hot�Ƿ�ʧ��  -> a.ʧ��  ͨ�� ɾ���������� ����
																  �������Ϲ��ݣ�ֱ�����ڵ�
															 -> b.��ʧ�� ��������������������
		3.3  �жϽڵ��Ƿ�ƽ�⣺		�Ƚ����������ڵ�߶ȼ��ɣ��߶ȵĿ����� bst �Ĳ����ɾ���У�

		3.4  link_34 ����������		1.�ҵ���ƽ��ڵ�� x �ϸߵķ�֧������ ��x��ʼ��Ϊ�游�ڵ�
									  �ҵ����������ڵ㣬 g,p,s (���� x Ϊ �游�ڵ㣩
									  PS�������г��ֲ�ƽ��������˵�� x - ��ڵ���ҽڵ��2����λ��
										  �ҽڵ����͸߶�Ϊ-1����ô�����ƶ��������ֲ�ƽ���ʱ��
										  �游�ڵ�һ���Ǵ��ڵģ������ĸ߶�һ������2�㣻
									2.���� g,p,s������������λ�ã�Ѱ�Ҵ����м�λ�õĽڵ㣬�����ýڵ�
									  ��Ϊ�µĸ��ڵ㣬�����������ڵ���Ϊ �¸��ڵ���ӽڵ㣬��������
									  ʹ�÷�֧�߶�ʵ�ʼ���1����λ
									  PS: ���̿��Կ�ͼ��

		3.4  ��������		���жϽڵ�ʧ��֮�󣬸��� p,s��g�����������Ӧ˳�򣬽��߷�֧��3�ڵ㴫�ݸ�
							�������� Link34����
							PS����ͬ������Կ�ͼ

*/


#pragma once
#include "bst_tree.h"

using namespace std;

template<typename T>
class val_tree : public bst_tree<T>
{
public:
	val_tree() = default;
	val_tree(const T& t) :bst_tree<T>(t) {};
	~val_tree() = default;

	bool is_balance(bst_BinNodePosi<T> t); //�жϽڵ��Ƿ�ƽ��
	void erase_rotate(bst_BinNodePosi<T> g);//����ڵ㲻ƽ�⣬����Ҫ��ת������ǰ�ڵ㼰���ڵ㣬�游�ڵ� ���в���
	void insert_rotate(bst_BinNodePosi<T> v, bst_BinNodePosi<T> p, bst_BinNodePosi<T> g);

	bst_BinNodePosi<T> insert(const T& t);
	bst_BinNodePosi<T> erase(const T& t);
	bst_BinNodePosi<T> get_root() { return bst_tree<T>::_root; };
	void link34(bst_BinNodePosi<T>g, bst_BinNodePosi<T>p, bst_BinNodePosi<T>v, bst_BinNodePosi<T>gg, bst_BinNodePosi<T>T0, bst_BinNodePosi<T>T1, bst_BinNodePosi<T>T2, bst_BinNodePosi<T>T3);

};

template<typename T>
bst_BinNodePosi<T> val_tree<T>::erase(const T& t)
{
	auto p = bst_tree<T>::erase(t);
	auto x = bst_tree<T>::hot;
	while (x)
	{
		if (!is_balance(x))
			erase_rotate(x);
		x = x->parent;
	}
	return p;
}


template<typename T>
bool val_tree<T>::is_balance(bst_BinNodePosi<T> t)
{
	int a, b;
	if (!t->lChild) a = -1; 
	else a = t->lChild->height;
	if (!t->rChild) b = -1;
	else b = t->rChild->height;
	int c = a - b;

	if (c > -2 && c < 2) return true;
	return false;
}

template<typename T>
void val_tree<T>::link34(bst_BinNodePosi<T>g, bst_BinNodePosi<T>p, bst_BinNodePosi<T>v, bst_BinNodePosi<T> gg,
	bst_BinNodePosi<T>T0, bst_BinNodePosi<T>T1, bst_BinNodePosi<T>T2, bst_BinNodePosi<T>T3)
{

	auto r = gg;
	if (r)
	{
		if (g == r->lChild) r->lChild = p;
		else r->rChild = p;
	}

	p->lChild = g;
	p->rChild = v;
	p->parent = r;
	g->lChild = T0;
	g->rChild = T1;
	g->parent = p;
	v->lChild = T2;
	v->rChild = T3;
	v->parent = p;
	if(T0) T0->parent = g;
	if(T1) T1->parent = g;
	if(T2) T2->parent = v;
	if(T3) T3->parent = v;
}

template<typename T>
void val_tree<T>::erase_rotate(bst_BinNodePosi<T> g)
{
	bst_BinNodePosi<T> p, v, T0, T1, T2, T3;
	int l, r;
	l = g->lChild ? g->lChild->height : -1;
	r = g->rChild ? g->rChild->height : -1;

	if (l > r) //ȷ��g p v
		p = g->lChild;
	else  p = g->rChild;

	l = p->lChild ? p->height : -1;
	r = p->rChild ? p->height : -1;
	if (l>r)
		v = p->lChild;
	else v = p->rChild;
	
	if (p == g->lChild) //��ߵ�������
	{
		T3 = p->rChild;
		if (v == p->lChild)
		{
			T0 = v->lChild;
			T1 = v->rChild;
			T2 = p->rChild;
			link34(v, p, g,g->parent, T0, T1, T2, T3);
		}
		else // v == p->rChild
		{
			T0 = p->lChild;
			T1 = p->lChild;
			T2 = p->rChild;
			link34(p, v, g, g->parent, T0, T1, T2, T3);
		}
	}
	else // p == g->rChild
	{
		T0 = g->lChild;
		if (v == p->rChild)
		{
			T1 = p->lChild;
			T2 = v->lChild;
			T3 = v->rChild;
			link34(g, p, v, g->parent, T0, T1, T2, T3);
		}
		else
		{
			T1 = v->lChild;
			T2 = v->rChild;
			T3 = p->rChild;
			link34(g, v, p, g->parent, T0, T1, T2, T3);
		}
	}
	
	bst_tree<T>::updateHeight(g);
	bst_tree<T>::updateHeight(v);
	bst_tree<T>::updateHeighAbove(p);
}

template<typename T>
void val_tree<T>::insert_rotate(bst_BinNodePosi<T> v, bst_BinNodePosi<T> p, bst_BinNodePosi<T> g)
{
	bst_BinNodePosi<T> a, b, c;
	bst_BinNodePosi<T> gg = g->parent;
	if (v->get_value() > p->get_value())
	{
		b = p;
		c = v;
		a = g;
	}
	else
	{
		b = v;
		c = p;
		a = g;
	}
	if (g == g->parent->rChild)
		g->parent->rChild = b;
	else g->parent->lChild = b;

	b->lChild = a;
	b->rChild = c;
	b->parent = gg;
	c->parent = b;
	c->lChild = NULL;
	c->rChild = NULL;
	a->parent = b;
	a->lChild = NULL;
	a->rChild = NULL;
	a->height = 0;
	c->height = 0;
	bst_tree<T>::updateHeighAbove(b);
}

template<typename T>
bst_BinNodePosi<T> val_tree<T>::insert(const T&t)
{
	auto v = bst_tree<T>::insert(t);
	if (bst_tree<T>::_root&&bst_tree<T>::_root->height > 1)
	{
		if (!is_balance(bst_tree<T>::hot->parent))
		{
			insert_rotate(v, v->parent, v->parent->parent);
			return v;
		}
	}
		
	return v;
}