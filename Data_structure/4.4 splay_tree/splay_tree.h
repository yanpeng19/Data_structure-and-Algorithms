/*
	伸缩树  splay_tree
	------------------------------------------------------------------
	0.概念和特点： 
	伸缩树是一种特殊的平衡搜索树，他和VAL树最大的不同是，伸缩树每次都会
	将上次操作的节点（删除节点，则将其父节点，插入节点则将插入节点本身）
	挪到根节点位置，是一种“操作的节点区域会被频繁操作” 的思想组织的树
	其本身也是通过 bst 平衡搜索树封装继承而成

	1.函数及功能
	1.1 search()  查找操作
	1.2 insert()  插入操作
	1.3 erase()  删除操作
	1.4 splay_refact()  单次的重构操作，根据不停情况，将节点提升2层，
	                    特殊情况提升一层
	1.5 splay()  重构操作，将操作过的节点提升到_root，通过循环 进行
	             splay_refact() 进行

	2.实现逻辑
	2.1 serach()  搜索功能：	从根目录向下遍历查找，如果 x 为真那么通过 调整函数处理x
	                            并返回 x ；如果 x 不存在，则将 _hot 通过调整提升到根
	2.2 insert()  插入操作：    查找插入值t ，如果 t 存在，不做任何操作，直接返回 t
	                            （如果存在那么 值 t 已经被查找函数 提升到 根节点）
	                            如果查找失败，那么新构造一个节点 x，插入到树根节点下方，
								并且将原 _root->lChild/ _root->rChild 对比和 x 的大小，
								此处用右节点举例，即 x >_root && x < _root->rChild:
								                  _root->rChild->lChild->parent = x;
								                  _root->rChild->parent = x;
												  x->rChild = _root->rChild;
												  x->lChild = _root->lChild;
								进行相应的连接；
	2.3 erase()  删除操作：     首先进行搜索操作，如果搜索失败，那么直接返回
	                            如果搜索成功，那么 x 已经变成了根节点；
								然后进行根节点的删除操作即可（与 succ(x) 交换并且删除）
	2.4 splay_refact()          核心操作，将某个节点 x 一次提升两层（特殊情况一层）：
                                (1).寻找 该节点的 父节点 f 及祖节点 g
								(2).类似AVL树，通过他们的不同位置情况，将 x 
								    提升到最上层，并且将f和g 接入到他的下方
									(不同情况接入方式可以看图）
							    (3).如果节点 gg=g->parent 存在，并将g-parent 指向 x 
								(*).特殊情况：g不存在，那么直接将 x 提升到 g之上即可
								操作效率 O(h/2)
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
	//查找t 成功则将 t提升到ROOT，查找失败则将hot 提升到ROOT
	//最终返回 t 或者 hot

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
	auto x = search(t); // x 可能 存在也可能不存在,如果存在，那么 x.value == t 
	                    // 如果不存在，则x为假
	if (x) return x;
	else x = bst_tree<T>::hot;
	bst_tree<T>::size++;

	//特殊情况： 空树
	if (!bst_tree<T>::hot)
	{
		bst_tree<T>::_root = new bst_BinNode<T>(t, NULL);
		return bst_tree<T>::_root;
	}

	auto v = new bst_BinNode<T>(t, NULL);
	if (x->get_value() > t) // 插入到 x->lChild = t
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
	auto x = bst_tree<T>::seach(t);  //该操作已经进行 调整层数和更新高度
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

	if (g = p->parent) //三点情况
	{
		// g 非root情况
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
	else // 两点情况
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


