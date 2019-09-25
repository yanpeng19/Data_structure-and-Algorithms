#pragma once
#include "bintree.h"

using namespace std;
//
//template <typename T>
//class BinTree
//{
//public:
//
//	BinTree() :_size(-1) {};
//	BinTree(const T& t) :_root(new BinNode<T>(t, NULL)), _size(0) { _root->height = 0; };
//
//	int size() const { return _size; };
//	bool empty() const { return !_root; };
//	BinNodePosi<T> root() const { return _root; };
//	void trvalLevel() { TravLevel(_root); };
//
//	static void free(BinNodePosi<T> t) { delete t; };
//
//	~BinTree()
//	{
//		//Point<T> temp(free);
//		TravPre(_root, free);
//	};
//
//	BinNodePosi<T> insertAsRC(BinNodePosi<T>, const T& t); //��ĳ��Ԫ����Ϊ���Ӳ���
//	BinNodePosi<T> insertAsLC(BinNodePosi<T>, const T& t); //��ĳ��Ԫ����Ϊ���Ӳ���
//
//protected:
//	BinNodePosi<T> _root; // ��
//	int _size;          //��ģ
//	virtual int updateHeight(BinNodePosi<T> x); // ����ĳ���ڵ�ĸ߶� ������Ӻ�ɾ��Ԫ��֮��
//	void updateHeighAbove(BinNodePosi<T> x);  //����ĳ���ڵ�->�����ڵ��  һ����·�ĸ߶�
//};


template<typename T> class bst_BinNode;

template<typename T> using bst_BinNodePosi = bst_BinNode<T>* ;

template<typename T> class bst_tree;

enum color { BLACK, RED };

template<typename T>
class bst_BinNode 
{
	template<typename B> friend void swap_bst_node(bst_BinNodePosi<B> a, bst_BinNodePosi<B> b);
	template<typename A, typename VST> friend void visitAlongLeftNode(bst_BinNodePosi<A>, VST visit, my_stack<bst_BinNodePosi<A>> &s);
	template<typename A, typename VST> friend void visitAlongLeftNode(bst_BinNodePosi<A>, VST visit, vector<bst_BinNodePosi<A>> s, vector<bst_BinNodePosi<A>> vt);
	template<typename T2> friend class bst_tree;
	template<typename T2> friend class br_tree;
public:
	bst_BinNode() = default;

	bst_BinNode(const T& t, bst_BinNodePosi<T> par) : data(t),parent(par)//value ��ʱ��string ��֯
	{
		c = RED;
		/*stringstream ss;
		ss << t;
		value = ss.str();*/
		value = data;
	}
	bst_BinNode(const T& t, bst_BinNodePosi<T> par, color col) : bst_BinNode(t, par) { c = col; };
	bst_BinNode(const T& t) :bst_BinNode(t, NULL, RED) {}; //value ��ʱ��string ��֯

	bool operator >(const bst_BinNode&b) { return value > b.value; };
	bool operator <(const bst_BinNode&b) { return value < b.value; };
	bool operator ==(const bst_BinNode&b) { return value == b.value; };
	bool operator !=(const bst_BinNode&b) { return value != b.value; };
	bool operator <=(const bst_BinNode&b) { return value > b.value; };
	bool operator >=(const bst_BinNode&b) { return value < b.value; };

	bool operator >(const bst_BinNodePosi<T> b) { return value > b->value; };
	bool operator <(const bst_BinNodePosi<T> b) { return value < b->value; };
	bool operator ==(const bst_BinNodePosi<T> b) { return value == b->value; };
	bool operator !=(const bst_BinNodePosi<T> b) { return value != b->value; };

	
	bst_BinNodePosi<T> succ();
	T& get_value() { return value; };
	bool operator=(bst_BinNodePosi<T> b)
	{
		data = b->data;
		value = b->value;
		lChild = b->lChild;
		rChild = b->rChild;
		parent = b->parent;
	}
	~bst_BinNode() = default;

protected:
	T data;
	string key;
	T value;
	bst_BinNodePosi<T> lChild;
	bst_BinNodePosi<T> rChild;
	bst_BinNodePosi<T> parent;
	color c;
};


template<typename T>
class bst_tree:public BinTree<T>
{
public:
	bst_tree() = default;
	bst_tree(const T& t):_root(new bst_BinNode<T>(t,NULL)){};
	void LDR(void(*f)(bst_BinNodePosi<T>)); //������� ��ROOT��ʼ

	bst_BinNodePosi<T> search(const T& t);
	bst_BinNodePosi<T> insert(const T&t);
	bst_BinNodePosi<T> erase(const T&);
	static void free(bst_BinNodePosi<T> t)
	{
		delete t;
	}

	~bst_tree()
	{
		LDR(free);
	}

protected:
	bst_BinNodePosi<T> _root; // ��
	bst_BinNodePosi<T> _hot;
	int size;
	//int _size;          //��ģ
	//int updateHeight(bst_BinNodePosi<T> x); // ����ĳ���ڵ�ĸ߶� ������Ӻ�ɾ��Ԫ��֮��
	//void updateHeighAbove(bst_BinNodePosi<T> x);  //����ĳ���ڵ�->�����ڵ��  һ����·�ĸ߶�
	
};

template<typename T>
void bst_tree<T>::LDR(void(*f)(bst_BinNodePosi<T>)) //������� ����ִ��
{
	TravPre(_root, f);
}

template<typename T, typename VST>
void visitAlongLeftNode(bst_BinNodePosi<T> x, VST visit, my_stack<bst_BinNodePosi<T>> &s)
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
void visitAlongLeftNode(bst_BinNodePosi<T> x, VST visit, vector<bst_BinNodePosi<T>> s, vector<bst_BinNodePosi<T>> vt)
{
	while (x)
	{
		auto l = x->lChild;
		auto r = x->rChild;
		visit(x,vt);
		if(r) s.push_back(r);
		x = l;
	}
}

template<typename T, typename VST>
void TravPre(bst_BinNodePosi<T> x, VST visit)
{
	my_stack<bst_BinNodePosi<T>> s;
 	visitAlongLeftNode(x, visit, s);
	while (!s.empty())
	{
		x = s.back();
		visitAlongLeftNode(x, visit, s);
	}
}

template<typename T, typename VST>
void TravPre(bst_BinNodePosi<T> x, VST visit, vector<bst_BinNodePosi<T>> vt)
{
	vector<bst_BinNodePosi<T>> s;
	visitAlongLeftNode(x, visit, s,vt);
	while (!s.empty())
	{
		x = s.back();
		s.pop_back();
		visitAlongLeftNode(x, visit, s,vt);
	}
}


template<typename T>
bst_BinNodePosi<T> bst_tree<T>::search(const T&t)
{
	if (!_root) return _root;

	auto x = _root;
	_hot = NULL;

	while(x)
	{
		if (t == x->get_value()) return x;
		else if (t > x->get_value())
		{
			_hot = x;
			x = x->rChild;
		}
		else
		{
			_hot = x;
			x = x->lChild;
		}
	}
	return x;   //
}

template<typename T>
bst_BinNodePosi<T> bst_tree<T>::insert(const T&t)
{
	
	bst_BinNodePosi<T> posi = search(t);
	if (posi&&posi!=_root) return NULL;

	size++;
	posi = new bst_BinNode<T>(t, _hot);
	
	// �����������û�и��ڵ�
	if (!_hot) return _root = posi;

	if (posi->get_value() > _hot->get_value()) 
		_hot->rChild = posi;
	else
		_hot->lChild = posi;

	return posi;
}

template<typename T>
bst_BinNodePosi<T> bst_tree<T>::erase(const T&t)
{
	bst_BinNodePosi<T> posi = search(t);
	if (!posi) return NULL;
	size--;

	if (!(posi->lChild) && !(posi->rChild))
	{
		bst_BinNode<T> temp(posi->data,NULL);
		if (_hot)
		{
			if (t > _hot->get_value())
				_hot->rChild = NULL;
			else _hot->lChild = NULL;
		}
		
		delete posi;
		if (!_hot)
			posi = _root = NULL;
		return &temp;
	}

	else if(posi->lChild && posi->rChild)
	{
		auto p = posi->succ();
		bst_BinNode<T> temp(posi->data, NULL);
		swap_bst_node(posi, p);

		//if (p->rChild) p->parent->lChild = p->rChild;
		//else p->parent->lChild = NULL;
		if (p == p->parent->lChild) p->parent->lChild = NULL;
		else p->parent->rChild = NULL;
		
		delete p;
		return &temp;
	}
	else
	{
		bst_BinNode<T> temp(posi->data, NULL); //temp ��Ȼ�Ǿֲ������������ˣ��������ݲ�û�б����������������ڶ�ʱ���ڿ��Է��ʺ�ʹ��

		// _hot == posi.parent 
		// a=�ǿսڵ�
		bst_BinNodePosi<T> a = posi->rChild ? posi->rChild : posi->lChild;
		a->parent = _hot;
		if (!_hot) _root = a;
		else if (_hot->get_value() > t) _hot->lChild = a;
		else _hot->rChild = a;
		delete posi;
		return &temp;
	}

}


template<typename T>
bst_BinNodePosi<T> bst_BinNode<T>::succ()
{
	bst_BinNodePosi<T> posi;
	if (rChild) posi = rChild;
	else if (lChild) posi = lChild;
	else return NULL;

	while (posi->lChild)
		posi = posi->lChild;
	return posi;
}

template<typename T>
void swap_bst_node(bst_BinNodePosi<T> a, bst_BinNodePosi<T> b) //ֻ���� data��value ������λ�ü� parent,lc,rc
{
	auto data = a->data;
	auto value = a->value;
	a->data = b->data;
	a->value = b->value;
	b->data = data;
	b->value = value;
}