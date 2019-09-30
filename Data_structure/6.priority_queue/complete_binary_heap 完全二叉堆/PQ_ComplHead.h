#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "my_vector.h"

using namespace std;

/*
优先级队列：
基于元素的‘优先级’进行排序以及存储的结构，

基本操作：
1.insert(T &t) 插入操作
2.get_Max() 获取队列最大值
3.delete_Max() 删除队列中优先级最大值

*/

//PQ 为概念性纯虚函数类，
template<typename T>
class PQ
{
public:
	virtual void insert(const T&t) = 0;
	virtual T get_Max() = 0;
	virtual T delete_Max() = 0;
};


/*
PQ_ComplHead 完全二叉堆 优先级队列
-------------------------------------------------------
0.概念：完全二叉堆是基于vector实现的，将Vector抽象成一颗完全二叉树，
        并且将优先级最高的元素放在vecotr[0] 根节点位置

1.节点的组织形式和规则：
        节点的规则：节点x的父节点都大于等于其两个孩子

		将完全二叉树抽象的vector的方法：
                 有节点 x rank = i;    vector[i];
                 其父节点  rank = (i-1)/2   vecotr[(i-1)/2]
                 其左子    rank =  2i+1     vector[2i+1]
                 其右子    rank =  2i+2     vector[2i+2]

2.各种操作的复杂度：
        get_max O（1）
        delete_max  O(logn)
        insert    O(logn)

3.操作接口：
       3.0.insert 插入操作 
       3.1.get_Max 获取优先级最大值
       3.2.delete_Max 删除优先级最大值
       
       3.3.percolateUp()    上滤节点操作： 将新插入的元素通过每次向上提升一层，直到 x <= x.parent 为止
	   3.4.percolateDown()  下虑节点操作： 根节点删除后，将末节点提升到根节点后。不停将尾下沉，直到  x >= l/rChild 为止
	   3.5.初始化：批量建堆 

4.功能逻辑：
       4.1 插入操作：   a.如果树为空，直接插入到根节点即可；
	                    b.如果树非空，那么将节点插入到末节点,并执行上浮操作，将节点移动到合适位置

	   4.2 删除操作：   a.如果树为空，那么反回；
	                    b.如果树非空，那么将末节点元素与首节点交换，并且删除末节点；
						  然后对新的首节点进行下浮操作，将他调整到合适的位置；

       4.3 上浮操作：   将节点x插入到堆中后，位置大多数情况下是不对的；
	                    循环将x与其父节点p对比，如果 x 大于父节点，那么就交换两人的位置；
						直到 x >=p;或者 x==vector[0];

       4.4 下浮操作：   在删除操作之后，需要对提升上来的末节点 x 进行下浮调整，让他处在合理的位置；
                        分别将x和x->lChild, x和x->rChild 做比较，并与其中大于且数值最大的 节点交换
						位置；这样不会影响树规则的同时，也进行了实际的下浮，问题得到了收缩；
						直到 !x->lChild && !x->rCHild 或者 x <x->lchild && x<x->rCHild

       4.5 建堆操作：   a.将数据按顺序插入到树中
                        b.将节点从0~size()-1/2逐个进行下溢操作（也就是树的非底层的上半部分节点）
						复杂度：O(n)+O(nlogn);
						分析：为什么只需对 前半部分的元素进行下溢操作？
						答：  因为根节点无需进行下溢出，在非最底层的部分进行完下溢操作之后，
						      所有节点就已经就位；

5.特点和应用：    完全二叉堆，是建堆速度较快；
                  可以在nlogn的时间内，对数据进行排序；
*/

using Rank = int;

template<typename T>
class PQ_ComplHead : public PQ<T>, public vec<T>
{

#define Parent(i) ((i-1)/2)
#define lChile(i) (i*2+1)
#define rChile(i) (i*2+2)

public:
	//操作接口
	PQ_ComplHead() = default;
	PQ_ComplHead(T* t, Rank n)
	{
		t = t + n;
		while (n >= 0)  // n is rank >= 0
		{
			vec<T>::push_back(*t--);
			n--;
		}
		hepify();
	}
	PQ_ComplHead(vector<T> v) :vec<T>(v)
	{
		hepify();
	}
	void insert(const T& t);
	T get_Max() { return vec<T>::front(); };
	T delete_Max();

	//功能函数
	Rank percolateDown(Rank x,Rank n);
	Rank percolateUp(Rank i);
	void hepify();
};

/*
批量建堆函数：
1.将节点自下而上进行下溢，从而确定每个节点的正确位置
2.叶节点无需进行下溢，所以从（n/2-1)节点开始进行操作

整体复杂度：O(n)
*/
template<typename T>
void PQ_ComplHead<T>::hepify()
{
	Rank i = (vec<T>::size() - 1) / 2;
	while (i >= 0)
		percolateDown(i--, vec<T>::size() - 1);
}

/*
删除逻辑：
1.将根节点备份并删除 
  a极限情况：impty()为真，直接返回
2.将末节点的元素放到跟节点，并执行下浮操作
*/
template<typename T>
T PQ_ComplHead<T>::delete_Max()
{
	if (vec<T>::empty()) return NULL;
	swap(vec<T>::operator[](0), vec<T>::operator[](vec<T>::size() - 1));

	T temp = vec<T>::operator[](vec<T>::size() - 1);
	vec<T>::erase(vec<T>::size() - 1);
	if(!vec<T>::empty()) percolateDown(0,vec<T>::size()-1);
	return temp;
} 

/*
下浮操作：
1.根据条件(x->l/rChild && x < max(x.lChild,x.rChlid)
2.交换x与较大的孩子的数值
*/
template<typename T>
Rank PQ_ComplHead<T>::percolateDown(Rank x,Rank n)
{
	T temp = vec<T>::operator[](0);
	Rank l, r;
	Rank max_rank;
	l = lChile(x);
	r = rChile(x);
	if (l <= n && r <= n) max_rank = vec<T>::operator[](l) > vec<T>::operator[](r) ? l : r;
	else if (l <= n)  max_rank = l;
	else if (r <= n) max_rank = r;
	else return x;

	while (vec<T>::operator[](x) < vec<T>::operator[](max_rank))
	{
		swap(vec<T>::operator[](x), vec<T>::operator[](max_rank));
		x = max_rank;
		l = lChile(x);
		r = rChile(x);

		if (l <= n && r <= n) max_rank = vec<T>::operator[](l) > vec<T>::operator[](r) ? l : r;
		else if (l <= n)  max_rank = l;
		else if (r <= n) max_rank = r;
		else return x;
	}
}


/*
插入逻辑操作：

1.树是否空：
  a.树为空（数组为空）时，直接进行插入操作并返回即可
  b.树非空，则继续流程
2.将新插入的数，作为末尾节点插入到树（数组）中，并进行上浮操作，使二叉堆合法并返回新插入节点
*/
template<typename T>
void PQ_ComplHead<T>::insert(const T& t)
{
	vec<T>::push_back(t);
	Rank r = vec<T>::size();
	if (r < 2) return;
	percolateUp(r-1);
}

/*
上浮操作：
1.判断规则： x < Parent(x) && Parent(x) >=0 
2.规则成立 则交换x和Parent(x)
*/
template<typename T>
Rank PQ_ComplHead<T>::percolateUp(Rank r)
{
	Rank p = Parent(r);
	while(r!=0&&vec<T>::operator[](r) > vec<T>::operator[](p))
	{
		swap(vec<T>::operator[](r), vec<T>::operator[](p));
		r = p;
		p = Parent(r);
	}
	return r;
}