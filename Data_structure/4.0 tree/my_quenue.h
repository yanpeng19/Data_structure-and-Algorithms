#pragma once
#include "my_list.h"

using namespace std;

template<typename T>
class my_queue :public my_list<T>
{
public:
	void enqueue(T const &t) { insert_after(t); }
	T dequeue() { remove(this->begin()); };
	T & front() { return front(); };
};
