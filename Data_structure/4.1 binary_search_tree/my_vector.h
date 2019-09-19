#pragma once
#include <string>  
#include <memory>  
#include <utility>   
#include <initializer_list>  
#include <algorithm>  
#include <iostream>
#include <vector>

using namespace std;

template<typename T>
class vec
{
	using fuck = T * ;
	static allocator<T> alloc;
	typedef T* iterator;
	//typename vec<T>::const_iterator iterator;

protected:
	vec() :element(NULL), first_free(NULL), cap(NULL) {};
	vec(const vec&v) :element(NULL), first_free(NULL), cap(NULL)
	{
		int i = v.size();
		element = alloc.allocate(i);
		first_free = element;
		cap = element + i + 1;
		auto temp = v.element;
		while (temp != v.first_free)
		{
			alloc.construct(first_free++, *temp++);
		}

	};
	vec(initializer_list<T>);
	vec(const vec&&v);
	vec<T> operator=(const vec<T>&);
	T& operator[](unsigned i);
	~vec() { free(); };

	//以下为功能函数
	void push_back(T);
	void pop_back();
	T pop();

public:
	bool empty()const { return size() ? 0 : 1; };
	unsigned size()const { return first_free - element; };
	unsigned capcity() { return cap - element; };
	void check();
	void free();
	void relocate();


	//dsa相关操作
	void erase(const int &i); //删除元素操作,使用下标
	void erase(const iterator &fir, const iterator &last);// 删除元素操作，使用迭代器

														  //排序及去重
	int uniquify();  // 去重复操作，需要元素向量有序之后
	void bubble_sort();  //冒泡排序
	int bubble(int, int);
	void merge_sort(int, int);  //归并排序
	void merge(int, int, int);

	//查找相关
	int binary_seach(const T&); //二分查找
	int binary_seach(const T&, int lo, int hi);
	int fibnacci_seach(const T&); // Fibnacci 查找
	int fibnacci_seach(const T&, int lo, int hi);
	int interpolation_seach(const T&);//插值查找
	int interpolation_seach(const T&, int lo, int hi);


	T& front();
	T& back();
	T* begin() const { return element; };
	T* end() const { return first_free; };

private:
	T * element;
	T* first_free;
	T* cap;
};

template<typename T>
allocator<T> vec<T>::alloc;

template<typename T>
int vec<T>::interpolation_seach(const T& t)
{
	int lo, hi, mi;
	lo = 0;
	hi = this->size();
	mi = 0;
	while (lo <= hi)                              //简单方法
	{
		mi = lo + (hi - lo)*(mi - *(element + lo)) / (*(element + hi) - *element);
		if (t < *(element + mi))
			hi = mi - 1;
		else if (t > *(element + mi))
			lo = mi + 1;
		else if (t == *(element + mi))
			return mi;
	}
	return --lo;
}

template<typename T>
int vec<T>::interpolation_seach(const T& t, int lo, int hi)
{
	int mi;
	while (lo <= hi)                              //简单方法
	{
		int mi = lo + (hi - lo)*(mi - *element / (*(element + lo) - *(element)));
		if (t < *(element + mi))
			hi = mi - 1;
		else if (t > *(element + mi))
			lo = mi + 1;
		else if (t == *(element + mi))
			return mi;
	}
	return --lo;
}



void gen_fib(vector<int>&vt, int n)
{
	int i, j;
	i = 0;
	j = 1;
	vt.push_back(i);
	vt.push_back(j);
	for (int t = 2; t < n; t++)
	{
		int temp = j;
		j = i + j;
		i = temp;
		vt.push_back(i + j);
	}
	//for (auto a : vt)
	//	cout << a << endl;
}

template<typename T>
int vec<T>::fibnacci_seach(const T&t, int lo, int hi)
{
	vector<int> fib;
	gen_fib(fib, hi - lo);
	int i = 0;
	int mi;
	while (hi - lo < fib[i]) i++; // 找到一个 可以划分 hi-lo的fib[i]；

	while (lo <= hi)
	{
		while (fib[i] > hi - lo) i--;
		mi = fib[i];
		if (t < *(element + mi))
			hi = mi - 1;
		else if (t > *(elements + mi))
			lo = mi + i;
		else if (t == *(elements + mi))
			return mi;
	}
	return --lo;
}

template<typename T>
int vec<T>::fibnacci_seach(const T&t)
{
	int lo, hi;
	lo = 0;
	hi = size();
	vector<int> fib;
	gen_fib(fib, hi - lo);
	int i = 0;
	while (hi - lo > fib[i]) i++; // 找到一个 可以划分 hi-lo的fib[i]；
	i--;

	while (lo <= hi)
	{
		int mi;
		while (fib[i] > hi - lo) i--;
		mi = lo + fib[i];
		if (t < *(element + mi))
			hi = mi - 1;
		else if (t > *(element + mi))
			lo = mi + 1;
		else if (t == *(element + mi))
			return mi;
	}
	return --lo;
}

template<typename T>
int vec<T>::binary_seach(const T& t, int lo, int hi)
{
	while (lo <= hi)                              //简单方法
	{
		int mi = (lo + hi) / 2;
		if (t < *(element + mi))
			hi = mi - 1;
		else if (t > *(element + mi))
			lo = mi + 1;
		else if (t == *(element + mi))
			return mi;
	}
	return --lo;

	//while (lo < hi)                 //高级方法
	//{
	//	int mi = (lo + hi) / 2;
	//	if (t < *(element + mi))
	//		hi = mi;
	//	else lo = mi+1;
	//}
	//return --lo;
}

template<typename T>
int vec<T>::binary_seach(const T& t)
{
	int lo = 0;
	int hi = size();
	while (lo <= hi)                              //简单方法
	{
		int mi = (lo + hi) / 2;
		if (t < *(element + mi))
			hi = mi - 1;
		else if (t > *(element + mi))
			lo = mi + 1;
		else if (t == *(element + mi))
			return mi;
	}
	return --lo;
}

template<typename T>
void vec<T>::merge_sort(int lo, int hi) // 分而治之
{
	if (hi - lo<2) return;  // 递归基 为单元素。

	int mi = (hi + lo) / 2;
	merge_sort(lo, mi);
	merge_sort(mi, hi);
	merge(lo, mi, hi);
	return;
}

template<typename T>
void vec<T>::merge(int lo, int mi, int hi) // 合并过程 对lo-mi , mi-hi按照从小到大合并
{
	vec<T> temp;
	int low = lo;
	int left_size = mi - lo;
	int right_size = hi - mi;
	while (left_size > 0 || right_size > 0) // 说明还有元素未完成分配
	{
		if (left_size && right_size) // 两个数列 都没有比较完的情况
		{
			if (*(element + lo) < *(element + mi))
			{
				temp.push_back(*(element + lo));
				lo++;
				left_size--;
			}
			else
			{
				temp.push_back(*(element + mi));
				mi++;
				right_size--;
			}
		}
		else if (left_size == 0) //左边的数列空了，就直接压入右边的数列
		{
			temp.push_back(*(element + mi));
			mi++;
			right_size--;
		}
		else if (right_size == 0)
		{
			temp.push_back(*(element + lo));
			lo++;
			left_size--;
		}

	}
	for (auto a : temp)
	{
		*(element + low) = a;
		low++;
	}
	return;
}

template<typename T>
void vec<T>::bubble_sort()
{
	bool flag = true;
	int lo = 0;
	int hi = size();
	int last = hi;
	vec<T> &temp = *this;

	while (lo < hi)
	{
		hi = bubble(lo, hi);
	}
}

template<typename T>
int vec<T>::bubble(int lo, int hi)
{
	int last = hi--;
	bool flag = true;
	for (int i = lo + 1; i <= hi; i++)
	{
		if (*(element + i) < *(element + i - 1))
		{
			flag = false;
			swap(*(element + i), *(element + i - 1));
			last = i;
		}
	}
	if (flag) return 0;
	else return last;
}

template<typename T>
int vec<T>::uniquify()
{
	int n = 0;
	T* temp_ele = element;
	vec<T> temp;
	if (!empty())
		temp.push_back(*temp_ele++);
	else return 0;

	while (temp_ele != first_free)
	{
		if (*temp_ele != temp.pop())
		{
			temp.push_back(*temp_ele);
			n++;
		}
		temp_ele++;
	}
	if (n)
	{
		free();
		element = temp.element;
		first_free = temp.first_free;
		cap = temp.cap;
		temp.element = temp.first_free = temp.cap = NULL;
	}
	return n;
}

template<typename T>
void vec<T>::erase(const int &i)
{
	int j = i;
	first_free--;
	while (element + j != first_free)
	{
		*(element + j) = *(element + j + 1);
		j++;
	}

}

template<typename T>
void vec<T>::erase(const iterator &fir, const iterator &last)
{
	iterator f = fir;
	iterator l = last;
	while (l + 1 != end())
	{
		*f = *(l + 1);
		f++;
		l++;
	}
	first_free = first_free - (last - fir) - 1;
}


template<typename T>
vec<T>::vec(initializer_list<T> li) :element(NULL), first_free(NULL), cap(NULL)
{
	while (capcity() < li.size())
		relocate();
	for (auto a : li)
		alloc.construct(first_free++, a);
}

template<typename T>
vec<T>::vec(const vec<T>&& vc) : element(vc.element), first_free(vc.first_free), cap(vc.cap)
{
	vc.element = NULL;
	vc.first_free = NULL;
	vc.cap = NULL;
}

template<typename T>
vec<T> vec<T>::operator=(const vec<T>&vc)
{
	free();
	element = alloc.allocate(vc.size());
	first_free = element;
	cap = element + vc.size() + 1;
	for (auto a : vc)
		alloc.construct(first_free++, a);
	return *this;
}

template<typename T>
T& vec<T>::operator[](unsigned i)
{
	return *(element + i);
}

template<typename T>
void vec<T>::check()
{
	if (size() == capcity())
		relocate();
}

template<typename T>
void vec<T>::free()
{
	if (element)
	{
		for (auto a = element; a != first_free; a++)
			alloc.destroy(a);
		alloc.deallocate(element, capcity());
	}
}

template<typename T>
void vec<T>::relocate()
{
	auto n = capcity() ? capcity() * 2 : 1;

	T* temp_element = alloc.allocate(n);
	T* temp_cap = temp_element + n;
	T* temp_first_free = temp_element;

	//cout << (temp_first_free - temp_element) << endl;

	for (auto i = element; i != first_free; i++)
	{
		alloc.construct(temp_first_free, *i);
		temp_first_free++;
	}

	free();
	element = temp_element;
	first_free = temp_first_free;
	cap = temp_cap;
}

template<typename T>
void vec<T>::push_back(T t)
{
	check();
	alloc.construct(first_free++, t);
}

template<typename T>
void vec<T>::pop_back()
{
	check();
	alloc.destroy(--first_free);
}

template<typename T>
T vec<T>::pop()
{
	if (!empty())
		return *(first_free - 1);
	else return NULL;
}

template<typename T>
T& vec<T>::front()
{
	check();
	return *element;
}

template<typename T>
T& vec<T>::back()
{
	check();
	return *(--first_free);
}

