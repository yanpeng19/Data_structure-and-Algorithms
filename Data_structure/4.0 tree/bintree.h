/*
	#������
	------------------------------------------------------------------------------------
	������ bintree ��һ���ɽڵ�Node��ɶ����ṹ��ÿ���ڵ�ӵ��һ�� ���ڵ㣬�����������ӽ�
	�㹹��

	���ص������۲�����߲��ҽڵ㣬��������log(n����ʱ������ɣ������˵����һ�ֲ���ͷ�
	�ʶ��Ƚ�ƽ��Ľṹ��
	------------------------------------------------------------------------------------
	#�������ı�����

	0.������������ڽڵ�X���ԣ��ȱ���X��ֵ��Ȼ��ֱ�������Ӻ�����
		        α����ݹ�ʵ�֣�
				void trav_pre(Node x)
				{
					if(!x) return;
					cout << x ;
					trav_pre(x->lChild);
					trav_pre(x->rChild);
				}

				����ʵ��˼·��
				1.һ·������ڵ������ȥ�����ҽ��ҽڵ�������
				2.���ʵ������ҽڵ�֮����ѭ����ջ�нڵ�ȡ������ͬ�����մ��߼�����

				void trav_pre(Node x,vector<Node> vec;)  //ȷ�� vec_n�ǿյ�
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


	1.�������: ���ڽڵ� x ��˵��Ҫ�ȱ�������ڵ㣬Ȼ���������Ȼ��������ҽڵ�
				α����ݹ�ʵ�֣�
				void trav_in(Node x)
				{
					if(!x) return;
					trav_in(x->lChild);
					cout <<  x ;
					trav_in(x->rChild);
				}

				�ݹ�ʵ��˼·��
				1.��;һֱ�����ҵ�����Ľڵ㣬���սڵ��Լ�����ѹ��ջ��
				2.���ջ�ǿ��� ջ��Ԫ�ص��������� �ڵ� �� l->rChild;
				void trav_in(Node x,vector<int> vec)
				{
					if(!x) return;
					auto temp_x = x;
					while(temp_x)  //��������Ӷ�����ջ
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


	2.��������:  ���ڽڵ� x ��˵��Ҫ�ȱ������ҽڵ㣬Ȼ���������Ȼ���������ڵ�
				 ��������ݹ�α���룺
				 void trav_in(Node x)
				 {
					 if(!x) return;
					trav_in(x->rChild);
					cout <<  x ;
					trav_in(x->lChild);
				 }

				 �ݹ�˼·������һ�£����Ƿ���������⡣��д�����˾ͣ��������Ȥ 262������ϸʵ��

	4.�㼶������ �������Ľڵ�ĸ߶Ƚ��б�������������ʵ�֣��Ƚ� �ڵ� x �Ƶ������У����ջ�ǿ�
				 ��ô��
				 1. x = queue.pop();
				 2. �� x �����ҽڵ�ֱ����������

				 α����ʵ�֣�
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

	BinNodePosi insertAsLC(const T&); // ������Ϊ����
	BinNodePosi insertAsRC(const T&); // ������Ϊ�Һ���
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

	BinNodePosi<T> insertAsRC(BinNodePosi<T>, const T& t); //��ĳ��Ԫ����Ϊ���Ӳ���
	BinNodePosi<T> insertAsLC(BinNodePosi<T>, const T& t); //��ĳ��Ԫ����Ϊ���Ӳ���

protected:
	BinNodePosi<T> _root;                       // ��
	int _size;                                  // ��ģ
	virtual int updateHeight(BinNodePosi<T> x); // ����ĳ���ڵ�ĸ߶� ������Ӻ�ɾ��Ԫ��֮��
	void updateHeighAbove(BinNodePosi<T> x);    // ����ĳ���ڵ�->�����ڵ��  һ����·�ĸ߶�
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

//����Ϊ��������
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
// �������

// �������
template<typename T, typename VST>
void goAlongLeftNode(BinNodePosi<T> x, VST visit, my_stack<BinNodePosi<T>> &s) // ��������ջ
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
		if (s.empty()) break;             // �ݹ��
		x = s.back();
		visit(x);
		x = x->rChild;
	}
}
//�������

//��������
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
		if (s.empty()) break;             // �ݹ��
		x = s.back();
		visit(x);
		x = x->lChild;
	}
}


// �㼶����
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
