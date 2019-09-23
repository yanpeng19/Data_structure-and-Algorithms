#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "my_vector.h"


using namespace std;

template<typename T>
class BT_Node;

template<typename T>
using BTNodePosi = BT_Node<T>*;

template<typename T>
class BT_Node
{
public:
	BT_Node() 
	{ 
		parent = NULL; 
		child.push_back(NULL);
		key.push_back(NULL); 
	}
	BT_Node(const T&t, BTNodePosi<T> lc, BTNodePosi<T> rc)
	{
		parent = NULL;
		key.insert(0, t);
		child.insert(0, lc);
		child.insert(1, rc);
		if (lc) lc->parent = this;
		if (rc) rc->parent = this;
	}
	BT_Node(BTNodePosi<T> b, int pos)
	{
		parent = NULL;
		int i=0;
		for ( i = pos+1; i < b->key.size(); i++)
		{
			key.push_back(b->key[i]);
			child.push_back(b->child[i]);
		}
		child.push_back(b->child[i]);
	}

//protected:
	vec<T> key;
	vec<BTNodePosi<T>> child;
	BTNodePosi<T> parent;
	BTNodePosi<T> _hot;
};
