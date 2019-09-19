/*
	AVL树
	---------------------------------------------------------------------------------------
	0.概念：AVL树是一种特殊的二叉搜索树，是一种“动态平衡”的树，规定“每个节点的高度 的差要小于2”
			所以，AVL树相对来说，是一种比较宽 和 平衡的树。

			因为二叉搜索树BST的查找效率取决于节点的高度 O(h),而AVL树的目的就是让 每个节点尽可能的小
			从而来达到搜索和使用的高效率.其查找和插入效率，可以近似O(logn）

			规定空节点的高度为-1，而叶节点高度为0；

	
	1.组织形式：本次实现中，是通过 继承平衡搜索树 BST 和 对插入删除封装，来实现AVL树的一种方式
			
			template<typename T> 
			class avl_tree : public bst_tree<T>;

	2.功能函数：
		2.1 is_banlance(Node* x)		判断节点 x 是否平衡 即 左右节点高度差 是否大于1
										为真则说明 存在不平衡现象，为假 则不存在不平衡
		2.2 Node* insert()		插入功能,成功返回插入节点指针；失败返回空
		2.3 Node* erase()		删除功能，成功则返回删除元素指针，失败返回空；
								//这种返回是将析构但未使用的内存内容返回，存在隐患

		2.4 insert_rotate()		插入平衡函数，如果树插入节点后，出现不平衡现象，
								则通过此函数进行调整，并重新形成平衡
		2.5 erase_rotate()		删除平衡函数，树删除某元素后，如果出现不平衡现象，
								则通过该函数进行调整，形成新平衡
		2.6 link_34()           将不平衡的节点，较高的分支，降低一个高度的功能呢函数

	3.实现逻辑：
		3.1. 插入：		执行 bst 的插入 -> 检查_hot是否失衡  -> a.失衡  通过 插入调整函数调整
															 -> b.不失衡  不做其他操作，直接返回
		3.2  删除：		执行 bst 的删除 -> 检查_hot是否失衡  -> a.失衡  通过 删除调整函数 调整
																  并且向上归溯，直到根节点
															 -> b.不失衡 ，不做操作，正常返回
		3.3  判断节点是否平衡：		比较左右两个节点高度即可（高度的控制在 bst 的插入和删除中）

		3.4  link_34 调整函数：		1.找到不平衡节点的 x 较高的分支，并将 从x开始作为祖父节点
									  找到子孙三代节点， g,p,s (其中 x 为 祖父节点）
									  PS：当树中出现不平衡现象，则说明 x - 左节点比右节点高2个单位；
										  且节点的最低高度为-1；那么可以推定，当出现不平衡的时候；
										  祖父节点一定是存在的，即树的高度一定大于2层；
									2.根据 g,p,s的子孙三代的位置，寻找处于中间位置的节点，并将该节点
									  作为新的父节点，将另外两个节点作为 新父节点的子节点，进行连接
									  使该分支高度实际减少1个单位
									  PS: 过程可以看图；

		3.4  调整函数		在判断节点失衡之后，根据 p,s，g的情况，以相应顺序，将高分支的3节点传递给
							调整函数 Link34即可
							PS：不同情况可以看图

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

	bool is_balance(bst_BinNodePosi<T> t); //判断节点是否平衡
	void erase_rotate(bst_BinNodePosi<T> g);//如果节点不平衡，就需要旋转。将当前节点及父节点，祖父节点 进行操作
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

	if (l > r) //确认g p v
		p = g->lChild;
	else  p = g->rChild;

	l = p->lChild ? p->height : -1;
	r = p->rChild ? p->height : -1;
	if (l>r)
		v = p->lChild;
	else v = p->rChild;
	
	if (p == g->lChild) //左边的子树高
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