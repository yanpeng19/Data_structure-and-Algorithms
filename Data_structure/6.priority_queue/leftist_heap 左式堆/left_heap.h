#pragma once

/*
左式堆:基于完全二叉堆和二叉树形成的偏移堆（树）
--------------------------------------------------
0.特点和定义：
    左式堆相对于完全二叉堆来说，整体式向着左边偏移的；
	而左式堆相对于完全二叉堆来说，优点在于堆合并效率，
	可以达到 O(n) ,而不是 O（n+m);

	其特点：
	a.左边的节点较多
	b.他并不是搜索二叉树那样，每个节点都是有序性的；
	  （因为他没有搜索功能，所以这种结构是合法的）

1.操作接口和一些定义：
    get_Max()      获取最大节点       O(1)
    delete_Max()   删除最大节点       O(logn)
    insert()       插入元素           O(logn)
	外部函数：
	merge()        合并两个优先级队列 O(logn)

	合并为什么使用外部的形式？
	因为如果设置成类内形式，并堆自身的修改容易导致逻辑混乱，
	且merge()是通过递归形式实现，容易造成节点失效

2.实现的逻辑：
    2.0 合并操作：   
	  merge(node x,node y)  合并操作堆x和堆y，该实现是通过递归的形式：
      *.递归基：  节点x 或者 y 不为真；
	  a.比较节点x，y的值，如果保证左边节点的值是最大的，如果 x->data < y->data;
	    swap(x,y)
	  b.将 x->rChild 和 y 进行合并，并且返回一个新头节点new_right,接入 x->rChild;
	  c.比较 x->lChild 和 x ->rChild 的npl值（即分支“高”度）
	    把更高的节点放在左边；(#这一步确保了节点左式堆的左倾斜性）
	  #合并操作仅可以保证： 1.最大节点的左节点或者右节点式次大节点
	                        2.左式堆的左倾斜性

    2.1 插入操作：
	  insert(t)  通过将根节点和 用插入值生成的新节点 new_node = node(t); 
	             进行合并，即可；

	2.2 删除操作：
	  delete_Max() 通过将 根节点左子和右子 进行合并树操作，并且返回节点作为
	               新的根节点即可；

*/

#include "PQ.h"
#include "bintree.h"

using namespace std;

/*
npl: 节点x到其最短外部阶段的最短距离的长度
npl(外部节点） = 0；
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
合并两个堆的流程，将两个堆的根节点作为参数 传给函数：

1.比较节点 npl（l) 和 npl（r) 将较高的节点放在左侧
  1.a l或r 都不存在，返回空节点
  1.b 极限情况 l 或 r 不为真，直接return

2.通过递归形式，将l.rChild 和 r 合并 ,l->rChild = merge()
  2.1 递归基 l或者 r 不为真（已经到最底层），如果二者都不为真，则可以直接RETUNR
  2.1.2 判断节点l和节点r 的数值，如果 l<r 则交换二者数值

2.2 判断 npl(l) 和 npl(r)规则是否成立，如果不成立则互换二者位置，同时调整父节点的指向

2.3 回归上一层

3.返回新的根节点
*/
template<typename T>
BinNodePosi<T> my_merge(BinNodePosi<T> l, BinNodePosi<T> r)
{
	if (!l && !r) return NULL; //1.a 极限情况
	else if (!r) return l;  //两个递归基
	else if (!l) return r;
	//值较大的在左边
	if (l->data < r->data) swap(l, r);
	l->rChild = my_merge<T>(l->rChild, r);
	//经过一系列调整后节点高的放左边
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



