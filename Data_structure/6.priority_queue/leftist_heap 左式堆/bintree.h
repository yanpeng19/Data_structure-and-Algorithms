#pragma once
#include "my_stack.h"
#include "my_quenue.h"
#include <initializer_list>
#include <functional>
//#define BinNodePosi(T) BinNode<T>*

using namespace std;

template<typename T> class BinNode;
template<typename T>
using BinNodePosi = BinNode<T>*;


template<typename T>
class Point
{
public:

	Point(void(*a)(BinNodePosi<T>)) : p(a){};

	~Point() = default;
	void(*p)(BinNodePosi<T>);
	using p_name = void(*)(BinNodePosi<T>);
	p_name get() { return p; };

};

template<typename T> class BinNode
{
	using BinNodePosi = BinNode * ;
	friend BinNodePosi my_merge(BinNodePosi l, BinNodePosi r);
public:
	BinNode() = default;
	BinNode(const T& t, BinNodePosi par) :data(t), parent(par) {};
	BinNode(const T& t) :data(t), parent(NULL)  {};
	

	~BinNode()=default;

	BinNodePosi parent, lChild, rChild;
	BinNodePosi insertAsLC(const T&); // 插入 作为左孩子
	BinNodePosi insertAsRC(const T&); // 插入作为右孩子
	BinNodePosi succ(); // 下一个节点 中序意义的 （不太明白）
	void free() { cout << " BinNode析构"; };

	// 遍历函数在节点上 会很灵活
	template<typename VST> void TravLevel();//层次遍历并处理 VST为处理方法，既函数指针(*P)(T,T);
	template<typename VST> void TravPre();//先序遍历，并处理
	template<typename VST> void TravIn();// 中序遍历，并处理
	template<typename VST> void TravPost();//后续遍历 并处理

	int size();

	T data;
	int height;
};

template<typename T>
using BinNodePosi = BinNode<T>*;

template<typename T>
BinNodePosi<T> BinNode<T>::insertAsLC(const T& t)
{
	if (lChild != NULL) return NULL;
	lChild = new BinNode<T>(t, this);
	//lChild->height = height + 1;
	return lChild;
}

template<typename T>
BinNodePosi<T> BinNode<T>::insertAsRC(const T& t)
{
	if (rChild != NULL) return NULL;
	rChild = new BinNode<T>(t, this);
	//rChild->height = height + 1;
	return rChild;
}

template<typename T>
int BinNode<T>::size()
{
	int i = 1;
	if (lChild) i += lChild->size();
	if (rChild) i += rChild->size();
	return i;
}

//先序
template<typename T,typename VST>
void visitAlongLeftNode(BinNodePosi<T> x, VST visit, my_stack<BinNodePosi<T>> &s)
{
	while (x)
	{
		auto l = x->lChild;
		auto r = x->rChild;
		visit(x);
		s.push(r);
		x = l;
	}
}

template<typename T,typename VST>
void TravPre(BinNodePosi<T> x,VST visit)       
{
	
	my_stack<BinNodePosi<T>> s;
	visitAlongLeftNode(x, visit, s); 
	while (!s.empty())
	{
		x = s.back();
		visitAlongLeftNode(x, visit, s);
	}
}
// 先序结束

// 中序遍历
template<typename T, typename VST>
void goAlongLeftNode(BinNodePosi<T> x, VST visit, my_stack<BinNodePosi<T>> &s) // 左序列入栈
{
	while (x)
	{
		s.push(x);
		x = x->lChild;
	}
}

template<typename T, typename VST>
void TravIn(BinNodePosi<T> x, VST &visit)
{
	my_stack<BinNodePosi<T>> s;
	while (true)
	{
		goAlongLeftNode(x, visit, s);
		if (s.empty()) break;             // 递归基
		x = s.back();
		visit(x);
		x = x->rChild;
	}
}
//中序结束

// 层级遍历
template<typename T,typename VST>
void  TravLevel(BinNodePosi<T> x, VST visit)
{
	my_queue<BinNodePosi<T>> q;
	q.push_back(x);

	while (!q.empty())
	{
		x = q.pop_front();
		if(x) visit(x);
		if(x->lChild) q.push_back(x->lChild);
		if(x->rChild) q.push_back(x->rChild);
	}
}


template <typename T> 
class BinTree
{
	friend BinNodePosi<T> my_merge(BinNodePosi<T> l, BinNodePosi<T> r);
	friend int npl(BinNodePosi<T> x);
public:
	
	BinTree() :_size(-1) {};
	BinTree(const T& t) :_root(new BinNode<T>(t, NULL)), _size(0) { _root->height = 0; };
	
	int size() const { return _size; };
	bool empty() const { return !_root; };
	BinNodePosi<T> root() const { return _root; };
	void trvalLevel() { TravLevel(_root); };

	static void free(BinNodePosi<T> t){ delete t; };

	~BinTree() 
	{
		//Point<T> temp(free);
		TravPre(_root,free); 
	};

	BinNodePosi<T> insertAsRC(BinNodePosi<T>, const T& t); //将某个元素作为右子插入
	BinNodePosi<T> insertAsLC(BinNodePosi<T>, const T& t); //将某个元素作为左子插入
	 

	BinNodePosi<T> _root; // 根
	int _size;          //规模
	virtual int updateHeight(BinNodePosi<T> x); // 更新某个节点的高度 ，在添加和删除元素之后
	void updateHeighAbove(BinNodePosi<T> x);  //更新某个节点->到根节点的  一条线路的高度
};

template<typename T>
int GetNodeHeight(const BinNodePosi<T> &t)
{
	return t ? t->height : -1;
}

template<typename T>
int BinTree<T>::updateHeight(BinNodePosi<T> x)
{
	x->height = 1 + max(GetNodeHeight(x->lChild), GetNodeHeight(x->rChild));
	return x->height;
}

template<typename T>
void BinTree<T>::updateHeighAbove(BinNodePosi<T> x)
{
	int i = updateHeight(x);
	x = x->parent;
	while (x)
	{
		i += 1;
		x->height = i;
	}
}

template<typename T>
BinNodePosi<T> BinTree<T>::insertAsRC(BinNodePosi<T> x, const T& t)
{
	_size++;
	x->insertAsRC(t);
	updateHeighAbove(x);
	return x->rChild;
}

template<typename T>
BinNodePosi<T> BinTree<T>::insertAsLC(BinNodePosi<T> x, const T& t)
{
	_size++;
	x->insertAsLC(t);
	updateHeighAbove(x);
	return x->lChild;
}