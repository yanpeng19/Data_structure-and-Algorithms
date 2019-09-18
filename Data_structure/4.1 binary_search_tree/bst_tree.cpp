#include "stdafx.h"
#include "bst_tree.h"
#include <iostream>

using namespace std;

template<typename T>
void out(bst_BinNodePosi<T> b)
{
	if (!b) return;
	cout << b->get_value() << endl;
	out(b->lChild);
	out(b->rChild);
}


int main()
{
	bst_tree<int> tree;
	for (int i = 100; i > 0; i--)
		tree.insert(rand());

	out<int>(tree._root);
		
	system("pause");
}