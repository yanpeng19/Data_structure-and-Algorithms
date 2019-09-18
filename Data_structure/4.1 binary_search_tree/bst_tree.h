/*
	二叉搜索树 Binary Search Tree
	----------------------------------------------------------------------
	0.特点：
		每个节点的都是“值”大小进行组织，规则为 x.lChild < x , x.rChild > x
		所以，在二叉搜索树中，可以进行有效率的搜索，其搜索的效率为 O(logn）
		Node 为节点类
	
	0.5 析构方法： 
		因为树结构存储的是 Node* 类型指针形，需要通过遍历所有节点，并且释放
		每个节点内存，来进行析构

	1.功能和数据
		1.1. insert()     插入值/节点
		1.2. erase()      删除值/节点
		1.3. seach()      查找特定值的元素位置

		1.4  Node* root   根节点 可以为空
		1.5  Node* _rot   辅助借点,用来记录搜索时候，节点进入下一层之前的位置，以便进行插入操作
		1.6  size         树的节点数量

	2.功能逻辑：

		2.1 查找逻辑：	  
		Node* seach(const T& t)
		{
			Node* x = _root;
			Node* _hot = NULL;
			while(x)
			{
				if( x.data == t ) return x;//找到节点

				//否则进入左或者右分支，并记录当前节点位置
				_hot = x;
				if(t>x.data) x = x.lChild;
				else t = x.rChlid;
			}
			return NULL; //没有找到
		}

		2.2 插入逻辑: 插入是基于查找功能上进行的，需要一个变量 Node* _hot,用来记录节点进入下一层之前的位置；
			          目的是为了当查找节点不存在是，用来定位 插入的位置。
					  当查找值不存在时，节点x一定会到达树的外部；

			三种情况：a.树为空树         -> 直接插入到根节点
					  b.树中存在插入值   -> 不做行为，或者对节点数据自定义操作，例如计数
					  c.树中不存在插入值 -> 生成新节点并插入

		Node* insert(const T& t)
		{
			if(!_root) _root = new Node(t);  //没有根节点情况，可以任意插入节点作为根节点
			Node* x = seach(t);              //寻找插入位置
			if(_root&&x) return NULL;        //节点存在，可以直接返回，或者自定义一些操作，例如增加节点里面的计数变量
			else                             //x为空，表示树中没有该节点，构造一个新的节点进行与hot的连接
			{	
				Node* posi = new Node(t);

				//连接插入节点和父节点
				if(t > _hot->lChlid) _hot->lChild = posi;
				else _hot->rChlid = posi;
				return posi;
			}
		}

		3.3 删除逻辑：首先在树中查找删除值 t 是否存在，如果不存在，直接返回即可，
					  若存在，则根据 删除节点 x 的非空子节点数量，进行操作,
					  同样是三种情况：

					  a. x 没有子节点      -> 直接进行删除，并将其 _hot 指向 x 的分支置为空
					  b. x 有一个子节点    -> 删除节点x，并将其 _hot 指向 x 的子节点
					  c. x 有两个子节点    -> 寻找 x 节点中序意义下的 下一个子节点 p（也是树中和x值最接近的一个节点）,
					                          此时p注定是没有左子节点的，交换 x 和 p 两者的值 swap（x,p)
											  此时，节点 p->data = t; _hot = p->parent;
											  如果 p 没有子节点，那么直接删除 p , 否则将 p 右节点与 _hot连接 并且 删除p

		伪代码逻辑：
		void erase(const T& t)
		{
			//两种极端情况
			if(!_root) return ;
			if(t == _root->data)
			{
				delete _root;
				_root = NULL;
			}

			Node* x = seach(t);
			if(!x->lChild&&!x->rChild) // 删除节点没有孩子
			{
				if(p == _hot->lChild) _hot->lChild = NULL;
				else _hot->rChild = NULL;
				delete p;
			}
			else if(x->lChild && x->rChild) //删除节点有两个孩子
			{
				Node* p = x;
				while (p->lChild) 
					p = p->lChild;

				swap(x,p);
				_hot = p->parent;

				if(!p->lChild)
				{
					//节点p不存在右孩子，直接删除和调整_hot关系
					if(p ==_hot->lChild) _hot->lChild = NULL;    //理论上p只可能 是_hot 的左子
					else _hot ->rChild = NULL;
					delete p;
				}
				else
				{
					if(p==_hot->lChild) _hot->lChild = p->rChild;
					delete p ;
				}
			}
			else     //删除节点就一个孩子
			{
				Node* p = _hot->lChild ? _hot->lChild : _hot->rChild;
				if(x == _hot->lChild) _hot->lChild = p;
				else _hot->rCHild = p;
				p ->parent = _hot;
				delete p;
			}
		}
	----------------------------------------------------------------------
*/

#pragma once
#include "my_stack.h"

using namespace std;

template<typename T> class bst_BinNode;

template<typename T> using bst_BinNodePosi = bst_BinNode<T>* ;

template<typename T> class bst_tree;

template<typename T>
class bst_BinNode 
{
	template<typename B> friend void swap_bst_node(bst_BinNodePosi<B> a, bst_BinNodePosi<B> b);
	template<typename A, typename VST> friend void visitAlongLeftNode(bst_BinNodePosi<A>, VST visit, my_stack<bst_BinNodePosi<A>> &s);
	template<typename T2> friend class bst_tree;
public:
	bst_BinNode() = default;
	bst_BinNode(const T& t, bst_BinNodePosi<T> par) : data(t),parent(par)//value 暂时用string 组织
	{
		stringstream ss;
		ss << t;
		value = t;
	}
	bst_BinNode(const T& t) :bst_BinNode(t, NULL) {}; //value 暂时用string 组织

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
	
	T data;
	string key;
	T value;
	bst_BinNodePosi<T> lChild;
	bst_BinNodePosi<T> rChild;
	bst_BinNodePosi<T> parent;
};


template<typename T>
class bst_tree
{
public:
	bst_tree() = default;
	bst_tree(const T& t):_root(new bst_BinNode<T>(t,NULL)){};
	void LDR(void(*f)(bst_BinNodePosi<T>)); //中序遍历 从ROOT开始

	bst_BinNodePosi<T> seach(const T& t);
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

	bst_BinNodePosi<T> _root; // 根
	bst_BinNodePosi<T> hot;
	int size;
};

template<typename T>
void bst_tree<T>::LDR(void(*f)(bst_BinNodePosi<T>)) //中序遍历 并且执行
{
	TravPre(_root, f);
}

//遍历函数
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
//遍历函数 结束

template<typename T>
bst_BinNodePosi<T> bst_tree<T>::seach(const T&t)
{
	if (!_root) return _root;
	auto x = _root;
	hot = NULL;

	while(x)
	{
		if ( t == x->get_value()) return x;
		else if ( t > x->get_value())
		{
			hot = x;
			x = x->rChild;
		}
		else
		{
			hot = x;
			x = x->lChild;
		}
	}
	return x;   //
}

template<typename T>
bst_BinNodePosi<T> bst_tree<T>::insert(const T&t)
{
	
	bst_BinNodePosi<T> posi = seach(t);
	if (posi&&posi!=_root) return NULL;

	size++;
	posi = new bst_BinNode<T>(t, hot);
	
	if (!hot) return _root = posi;

	if (posi->get_value() > hot->get_value()) 
		hot->rChild = posi;
	else
		hot->lChild = posi;

	return posi;
}

template<typename T>
bst_BinNodePosi<T> bst_tree<T>::erase(const T&t)
{
	bst_BinNodePosi<T> posi = seach(t);
	if (!posi) return NULL;
	size--;

	if (!(posi->lChild) && !(posi->rChild))
	{
		bst_BinNode<T> temp(posi->data,NULL);
		if (hot)
		{
			if (t > hot->get_value())
				hot->rChild = NULL;
			else hot->lChild = NULL;
		}
		
		delete posi;
		if (!hot)
			posi = _root = NULL;
		return &temp;
	}

	else if(posi->lChild && posi->rChild)
	{
		auto p = posi->succ();
		bst_BinNode<T> temp(posi->data, NULL);

		//删除根节点时，为空hot
		if(hot)
		{
			if (posi == hot->lChild)
			{
				hot->lChild = p;
			}
			else hot->rChild = p;
		}
		
		swap_bst_node(posi, p);

		if (p->rChild)
		{   
			p->parent->rChild = p->rChild;
		}

		delete p;
		return &temp;
	}

	else
	{
		bst_BinNode<T> temp(posi->data, NULL);
		bst_BinNodePosi<T> a = posi->rChild ? posi->rChild : posi->lChild;
		a->parent = hot;
		if (!hot) _root = a;
		else if (posi->get_value() > t) hot->rChild = a;
		else hot->lChild = a;
		delete posi;
		return &temp;

	}

}

template<typename T>
bst_BinNodePosi<T> bst_BinNode<T>::succ()
{
	auto posi = rChild;
	while (posi->lChild)
		posi = posi->lChild;
	return posi;
}

template<typename T>
void swap_bst_node(bst_BinNodePosi<T> a, bst_BinNodePosi<T> b) //只交换 data和value 不交换位置即 parent,lc,rc
{
	auto data = a->data;
	auto value = a->value;
	a->data = b->data;
	a->value = b->value;
	b->data = data;
	b->value = value;
}