
#include "stdafx.h"
#include "bintree.h"

using namespace std;

template<typename T>
void out_test(BinNodePosi<T> t)
{
	cout << t->data << endl;
}

//template<typename T>
//class Point
//{
//public:
//
//	void(*p)(BinNodePosi<T>)=out_test<T>;
//	using p_name = void(*)(BinNodePosi<T>);
//	p_name get() { return p; };
//
//};

int main()
{


	//测试树类

	BinTree<string> b("1");

	b.insertAsLC(b.root(),"2");
	b.insertAsRC(b.root(),"3");

	Point<string> d(out_test);                // 模板函数指针类

	void (*point)(BinNodePosi<string>)= d.get();
	TravLevel(b.root(), point);
	cout << endl;
	TravPre(b.root(), point);
	cout << endl;
	TravIn(b.root(), point);
	cout << endl;
	TravPos(b.root(), point);


	system("pause");
	return 0;

}

