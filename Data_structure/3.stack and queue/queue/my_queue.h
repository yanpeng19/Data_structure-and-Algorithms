/*
	0.队列基于list封装而成，其结构是先入先出
	1.功能
	 1.1 enqueue()          //插入元素到最后
	 1.2 dequeue()          //删除（头）元素
	 1.3 front()            //返回（头）元素
*/

#pragma once
#include "my_list.h"

using namespace std;

template<typename T>
class my_queue:public my_list<T>
{
public:
	void enqueue(T const &t) { insert_after(t); }
	T dequeue() { remove(this->begin()); };
	T & front() { return front(); };
};

