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
基于vector实现的，将Vector抽象成一颗完全二叉树，
并且将优先级最高的元素放在vecotr[0] 根节点位置
get_max O（1）
delete_max  O(logn)
insert    O(logn)

节点的规则：
节点x的父节点都大于等于其两个孩子

将完全二叉树抽象的vector的方法：
有节点 x rank = i;    vector[i];
其父节点  rank = (i-1)/2   vecotr[(i-1)/2]
其左子    rank =  2i+1     vector[2i+1]
其右子    rank =  2i+2     vector[2i+2]

操作接口：
1.insert 插入操作
2.get_Max 获取优先级最大值
3.delete_Max 删除优先级最大值

功能函数：
1.上滤节点操作： 将新插入的元素通过每次向上提升一层，直到 x <= x.parent 为止
2.下虑节点操作： 根节点删除后，将末节点提升到根节点后。不停将尾下沉，直到  x >= l/rChild 为止
3.批量建堆

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
3.
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

1.简单情况和复杂情况：
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
	while(vec<T>::operator[](r) > vec<T>::operator[](p))
	{
		swap(vec<T>::operator[](r), vec<T>::operator[](p));
		r = p;
		p = Parent(r);
	}
	return r;
}