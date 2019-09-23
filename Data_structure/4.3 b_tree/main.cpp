// b_tree.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "b_tree.h"
#include "BT_Node.h"
#include <iostream>
#include <vector>


using namespace std;

int main()
{
	b_tree<int> b(5);
	for (int i = 0; i < 200; i++)
	{
		b.insert(i);
	}
	b.out();

	for (int i = 100; i >= 0; i--)
	{
		
			
 		b.erase(i);
		b.out();
		cout << "have delete : " << i << endl;
		cout << endl;
	}
	
	system("pause");
}
 