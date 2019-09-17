/*
	#二叉树
	------------------------------------------------------------------------------------
	二叉树 bintree 是一种由节点Node组成多链结构。每个节点拥有一个 父节点，和左右两个子节
	点构成

	其特点是无论插入或者查找节点，都可以在log(n）的时间内完成，相对来说，是一种插入和访
	问都比较平衡的结构。
	------------------------------------------------------------------------------------
	#二叉树的遍历：

	0.先序遍历：对于节点X而言，先遍历X的值，然后分别遍历左子和右子
		        伪代码递归实现：
				void trav_pre(Node x)
				{
					if(!x) return;
					cout << x ;
					trav_pre(x->lChild);
					trav_pre(x->rChild);
				}

				迭代实现思路：
				1.一路沿着左节点访问下去，并且将右节点存入堆中
				2.访问到最后的右节点之后，再循环将栈中节点取出，并同样按照此逻辑访问

				void trav_pre(Node x,vector<Node> vec;)  //确保 vec_n是空的
				{
					cout << x << endl;
					vec_n.push_back(x->rChild);
					x = x->lChild;

					while(!vec_n.empty())
					{
					    auto r = vec.pop();
						vec_n.pop_back();
						trav_pre(r,vec);
					}
				}


	1.中序遍历: 对于节点 x 来说，要先遍历其左节点，然后遍历本身，然后遍历其右节点
				伪代码递归实现：
				void trav_in(Node x)
				{
					if(!x) return;
					trav_in(x->lChild);
					cout <<  x ;
					trav_in(x->rChild);
				}

				递归实现思路：
				1.沿途一直向下找到最左的节点，按照节点自己自身压入栈中
				2.如果栈非空则将 栈中元素弹出，处理 节点 和 l->rChild;
				void trav_in(Node x,vector<int> vec)
				{
					if(!x) return;
					auto temp_x = x;
					while(temp_x)  //所有左侧子都已入栈
					{
						vec.push_back(temp_x);
						temp_x = temp_x->lChild;
					}
					while(!vec.empty())
					{
						auto x = vec.pop();
						vec.pop_back();
						cout << x ;
						trav_in(x->rChild);
					}
				}


	2.后续遍历:  对于节点 x 来说，要先遍历其右节点，然后遍历本身，然后遍历其左节点
				 中序遍历递归伪代码：
				 void trav_in(Node x)
				 {
					 if(!x) return;
					trav_in(x->rChild);
					cout <<  x ;
					trav_in(x->lChild);
				 }

				 递归思路和中序一致，就是方向调整问题。不写代码了就，如果有兴趣 262行有详细实现

	4.层级遍历： 按照树的节点的高度进行遍历，借助队列实现，先将 节点 x 推到队列中，如果栈非空
				 那么：
				 1. x = queue.pop();
				 2. 将 x 的左右节点分别推入队列中

				 伪代码实现：
				 void TravLevel(Node x)
				 {
					queue<BinNodePosi<T>> q;
					q.push_back(x);

					while (!q.empty())
					{
						x = q.pop_front();
						if (x) visit(x);
						if (x->lChild) q.push_back(x->lChild);
						if (x->rChild) q.push_back(x->rChild);
					}
				 }

*/

#pragma once

#include "my_stack.h"
#include "my_quenue.h"
#include <initializer_list>
#include <functional>

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
public:
	BinNode() = default;
	BinNode(const T& t, BinNodePosi par) :data(t), parent(par) {};
	BinNode(const T& t,const int&par) :data(t), parent(NULL)  {};
	~BinNode()=default;

	BinNodePosi insertAsLC(const T&); // 插入作为左孩子
	BinNodePosi insertAsRC(const T&); // 插入作为右孩子
	int size();

	BinNodePosi parent, lChild, rChild;
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
	return lChild;
}

template<typename T>
BinNodePosi<T> BinNode<T>::insertAsRC(const T& t)
{
	if (rChild != NULL) return NULL;
	rChild = new BinNode<T>(t, this);
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

template <typename T>
class BinTree
{
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
		TravPre(_root,free);
	};

	BinNodePosi<T> insertAsRC(BinNodePosi<T>, const T& t); //将某个元素作为右子插入
	BinNodePosi<T> insertAsLC(BinNodePosi<T>, const T& t); //将某个元素作为左子插入

protected:
	BinNodePosi<T> _root;                       // 根
	int _size;                                  // 规模
	virtual int updateHeight(BinNodePosi<T> x); // 更新某个节点的高度 ，在添加和删除元素之后
	void updateHeighAbove(BinNodePosi<T> x);    // 更新某个节点->到根节点的  一条线路的高度
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

//以下为遍历方法
template<typename T, typename VST>
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

template<typename T, typename VST>
void TravPre(BinNodePosi<T> x, VST visit)
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

//后续遍历
template<typename T, typename VST>
void goAlongRightNode(BinNodePosi<T> x, VST visit, my_stack<BinNodePosi<T>> &s)
{
	while (x)
	{
		s.push(x);
		x = x->rChild;
	}
}

template<typename T, typename VST>
void TravPos(BinNodePosi<T> x, VST &visit)
{
	my_stack<BinNodePosi<T>> s;
	while (true)
	{
		goAlongRightNode(x, visit, s);
		if (s.empty()) break;             // 递归基
		x = s.back();
		visit(x);
		x = x->lChild;
	}
}


// 层级遍历
template<typename T, typename VST>
void  TravLevel(BinNodePosi<T> x, VST visit)
{
	my_queue<BinNodePosi<T>> q;
	q.push_back(x);

	while (!q.empty())
	{
		x = q.pop_front();
		if (x) visit(x);
		if (x->lChild) q.push_back(x->lChild);
		if (x->rChild) q.push_back(x->rChild);
	}
}
