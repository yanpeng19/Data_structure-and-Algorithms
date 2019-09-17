/*
	0.my_stack 基于vector封装而成的堆结构
*/

#pragma once

#include "vec.h"
#include <sstream>

using namespace std;

template<typename T>
class my_stack :public vec<T>
{
public:
	void push(T const&t) { vec<T>::push_back(t); }
	T pop() { return vec<T>::back(); }
	T & top() { return  (*this)[vec<T>::size() - 1]; }

};
