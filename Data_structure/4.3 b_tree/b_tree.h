/*
	B树 - b_tree
	-----------------------------------------------------------------------------
	0.概念：B树是一种依然以树形结构组织的查找搜索树。
			相对于二叉树，AVL树等等，B树关注和优化的部分，是IO部分，即数据读取
			具体应用场景的话，数据库以及磁盘组织用的较多

	1.组织形式： B树的数据单位 BT_Node 是一种普通节点的向量，即BT_Node = vecotor<Node*>
				 可以将 BT_Node 理解成是一个超级节点。
				 而每个超级节点里面，都存储着 N 个节点（之后用order表示n的大小），
				 相应也存储着N+1的子节点信息：

				 (order = 5）
				 key :                 2              5               6             8     
				 child:    BT_Node* a     BT_Node* b      BT_Node*  c    BT_Node* d    BT_Node* e

				 也是通过这种形式，B树来实现一次获取多个节点和数据，来减少IO的操作

				 BT_Node 节点的内容：
				 {
					vector<T> key;						 //存储着数据的值
					vector<BT_Node*<T>> child;			 //存储着数据的子节点信息
					BT_Node<T>* parent;					 //存储着父节点的信息
				 }

				 B树节点的规则：
				 1. key.size() <= order 每个节点的数据不能超过 roder-1个，如果超过了，就发生
				                        节点发生了上溢出，要对节点进行处理。
				 2. key.size() > (order-1)/2 为了保持利用效率，要求每个节点要大于 (order-1)/2个
											 如果不足，则发生了下溢出，同样要进行处理

	2.函数和功能：
		2.1 search()            // 查找操作 
		2.1 insert()            // 插入操作
		2.2 erase()             // 删除操作：
		2.3 solveOverflow()     //（插入导致的）节点上溢处理：
		2.4 solveUnderflow()    //（删除导致的）节点下溢处理：
		
	3.实现逻辑
		3.0 查找 search(const T& t)      (1).从根节点开始遍历，使用二分查找，确认节点位置，如果节点在该层存在，就直接返回
											 否则，向量搜索函数 key.search() 会返回最后一个不大于 t 的节点下标 i;
											 我们进入 child[i+1] 中的序列
											 例如在上面的例子中 寻找 3， i = 0； 我们的目标是 BT_Node* b ，进入i之后的分支
											 所以进入 BT_Node* b;
										 (2).循环以上场景，直到到达外部节点，那么久返回NULL；
										 (*).伪代码：
											 search(t)
											 {
												_hot = NULL; //同样是存父节点，方便之后进行插入
												if(!BT_Node* x = _root) return NULL;

												while(x)
												{
													int i = key.search(t); // key 中查找
													if(key[i]==t) return key[i];   //找到了
													_hot = x;
													x = child[i+1];  //进入下一层
												}
												return NULL;  //到这里就已经进入外部节点了
											 }

		3.1 插入 insert(const T& t)       (1).进行查找操作 BT_Node* p = search(t);
										  (2.0) 查找成功，树中存在该元素，可以增加节点计数或者什么都不做
										  (2.1) 查找失败，此时 _hot 为 最后一个小于 t 的超级节点，那么在
												_hot的 key 中的合适位置插入 t;
										  (3) 判断_hot节点是否因为新插入元素，发生了上溢，如果发生上溢出，
											  那么就使用函数调整。
										  (*)伪代码：
										  inser(const T&t)
										  {
											 //树为空，则直接进行插入
											 using rank = int;
											 if(!_root)
											 {
												_root = new BT_Node(t);
												_size++;
												return _root.key[0];
											 }
											 //一般情况
											 BT_Node x = search(t);
											 if(x) return x;  //找到了，就返回或者自定义一些操作
											 rank i = _hot.key.search(t); //否则找到合适的位置，插入t
											 _hot.key.search(i+1,t);  
											 _hot.child.push_back(NULL);  //_hot 此情况下必定是叶节点，叶节点下面一定都是外部节点，
																			     可直接推入NULL；
											 solveOverflow(_hot);  //检查是否发生上溢出，并处理的函数
										  }

*/

#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "BT_Node.h"

using namespace std;


// BTNodePosi = BT_Node<T>*; 
template<typename T>
class b_tree
{
	friend iostream;
public:
	b_tree(const unsigned &t) :_order(t-1) { _root = _hot = NULL; _size = 0; };
	~b_tree() = default;

	BTNodePosi<T> search(const T& t);
	bool insert(const T&t);
	bool erase(const T&t);

	void solveOverflow(BTNodePosi<T>);
	void solveUnderflow(BTNodePosi<T>);

	void out()
	{
		BTNodePosi<T> x;
		x = _root;
		for (auto a : x->key)
		{
			cout << a << " ";
		}
		cout << endl;

		for (auto x : _root->child)
		{
			if (x)
			{
				for (auto a : x->key)
				{
					cout << a << " ";
				}
				cout << endl;
			}
		}
	}

protected:
	BTNodePosi<T> _root;
	BTNodePosi<T> _hot;
	int _size;     //
	int _order;    //阶次 即max[child->size()]
};

template<typename T>
BTNodePosi<T> b_tree<T>::search(const T& t)             //成功则return x,失败则return NULL
{
	int i;
	BTNodePosi<T> x = _root;
	_hot = NULL;
	while (x)
	{
		i = x->key.search(t);
	
		if (x->key[i] == t) return x;
		_hot = x;
		x = x->child[i + 1];
	}
	return x;
}

//插入逻辑： 
//1.进行seach 
//2.判定元素是否存在 ->  auto x = search(t);   auto i = x->key.search(t);
//                       if(x->key[i]==t) 存在,直接return ;  else  进入插入环节
//3.插入操作         ->  if(!x) 则在_hot 进行插入，else x进行
//                       x/_hot 此时一定是叶节点或者空节点
//                   ->  rank i = x->key.search(t) ; x->key.insert(i,t); x->child.insert(i+1,NULL);
//                   ->  solveOverflow 判断是否超阶次
//                       if(x->key->capcity() >= _order) 则规模超阶 ,需要调整
//4.调整方法：分裂   ->  a.RANK i = m/2+(m%2)?0;1;
//                   ->  b.将 key[i] 给 *this->parent 节点
//                       c.new BTnode l(key=0->i-1,child=0->i-1)  new BTNode r(key=i+1->end child= i+1->end)
//                       d.使得t提升后的  将新得到的l,r插入到 parent->key[i] parent->key[i+1]
//                       e.循环检查 parent 直到_root
//                       f.极端情况： _root 节点上溢，需要提升一个新的节点    solveOverflow() {if(x==_root) _root= new BTNode(t,l,r)}
template<typename T>
bool b_tree<T>::insert(const T& t)
{
	int rank;
	if (!_root)
	{
		_root = new BT_Node<T>(t, BTNodePosi<T>(NULL), BTNodePosi<T>(NULL)); //极端情况，树不存在_root
		_size++;
		return true;
	}
	BTNodePosi<T> x = _root;
	

	while (x)
	{
		rank = x->key.search(t); //rank 范围  -1~ key.size()
		if (!x->key.empty() && x->key[rank] == t) return false; //关键词非空且值相同 说明数值存在无需进行插入
		_hot = x;
		x = x->child[rank+1];
	}
	//程序到此 说明 x 为外部节点，_hot已经到了合适位置，直接在hot[rank]位置进行插入
	_hot->key.insert(rank+1, t);
	_hot->child.push_back(NULL);

	solveOverflow(_hot);
	_size++;

	return true;
}

/*
检查是否上溢出：
1. 检查_hot->key.size() 是否大于 order;如果大于则进行下一步
2. 分裂节点_hot 使之 size() 减少,分裂点 pos = size()/2 + 1节点
3. 原节点 复制 key[pos+1]-key[size()-1] 以及 child[pos] 至 child[size()-1]
   到 新节点中 n_node = new(BT_Node<T>),并且执行复制一一更新
4. 删除原节点 key[pos]及之后的内容，删除原节点 child (key.size()+1) 之后的内容
5. 在上一层_hot->parent中 进行插入
   a.如果_hot->parent为真 则rank = _hot->parent->seach(t);BN
     child.insert(rank,n_node)
   b.如果_hot->paren为假,则new(child[pos],*this,n_node) = _root;
*/
template<typename T>
void b_tree<T>::solveOverflow(BTNodePosi<T> _hot)
{

	if (_hot->key.size() > _order)
	{
		// pilt_pos 即 _hot[spilt_pos] 这个节点,从0开始计算
		int spilt_pos = (_hot->key.size() / 2);

		BTNodePosi<T> new_node = new BT_Node<T>(_hot, spilt_pos);
		T t = _hot->key[spilt_pos];

		for (int i = _hot->key.size() - 1; i >= spilt_pos; i--)
		{
			_hot->key.pop_back();
			_hot->child.pop_back();
		}

		BTNodePosi<T> paren = _hot->parent;
		if (!paren)
		{
			_root = new BT_Node<T>(t, _hot, new_node);
			_hot->parent = _root;
			new_node->parent = _root;
		}
		else       
		{
			// 上一节点的正确插入位置 在praen_pos之后
			int parent_pos = paren->key.search(t);
			paren->key.insert(parent_pos + 1, t);
			paren->child.insert(parent_pos + 2, new_node);
			new_node->parent = paren;
		}

		auto x = _hot->parent;
		while (x)
		{
			solveOverflow(x);
			x = x->parent;
		}
	}
}

/* 
删除逻辑：
1.seach(t) 查找元素是否存在树中，如果存在则进入删除流程
2.删除的流程： 按照流程先在key中找到 t的位置pos，然后将t与child[pos+1][0] 交换 
               直到t到最底层，即 !child[pos+1],然后 执行 key.erase(t) child.pop_back()
			    auto x = _root;
				_hot = NULL;
			   while(x)
			   {
			        pos = x->key.seach(t);
					if(x->key[pos]==t) //找到了
					{
					    while(x->child[pos+1])
						{
						    x->key[pos] = x->child[pos+1]->key[0];
							_hot = x;
						    x = x->child[pos+1]
						}
						到此x 为外部节点
						_hot->key.erase(t);
						_hot->child.pop_back();
						return ture;
					}
					
					_hot = x;
					x = x->child[pos+1];
			   }
			   没找到最终 
			   return false;
3.检查节点是否发生了下溢
*/
template<typename T>
bool b_tree<T>::erase(const T& t)
{
	BTNodePosi<T> x = _root;
	while (x)
	{
		int pos = x->key.search(t);
		if (pos == -1)
		{
			_hot = x;
			x = x->child[pos + 1];
			continue;
		}
		if (x->key[pos] == t)
		{

			_hot = x;
			//极端情况 x 为根节点 且 size = 1

			while (x->child[pos + 1])
			{
				x->key[pos] = x->child[pos + 1]->key[0];
				_hot = x;
				x = x->child[pos + 1];
			}
			//极端情况 x 为叶节点
			if (_hot == x)
			{
				_hot->key.erase(pos);
				_hot->child.pop_back();
			}
			else
			{
				_hot->key.erase(0);
				_hot->child.pop_back();

			}
			solveUnderflow(_hot);
			_size--;
			return true;
		}
		_hot = x;
		x = x->child[pos + 1];
	}
	return false;
}

/*
	下溢出逻辑：
	节点x经过删除后，可能x->key.size() < (_order/2)-2  
	                     x->child.size() < (_order/2)-1;
	if(x->key.size < (_order/2)-2) ;

	为了保证树的高度以及利用率，需要进行 a.旋转 b.合并

	优先进行a.旋转操作，向其左或者右节点
	（l,r && l.size()>=_roder/2)
	parent = x->parent;
	pos_parent = parent->key.search(x->key[0])  //  找到父节点中 X的位置
	然后将l[size()-1] 或者 r[0] 与 parent->key[pos] 交换数值,并将
	parent->key[pos] 插入到 x->key中 头部或者尾部
	x->child.push_back(NULL);
	
	如果左右节点 不存在，或者key.size() 数位不够，则需要进行合并操作：
	将节点x的在父节点对应左/右 节点进行合并，并将key[pos] 下沉到 x中，
	使x->key.size() >(_order/2)-2;

	parent = x->parent;
	pos_parent = parent->key.search(x->key[0])
	if(pos_parent>0)
	auto l = parent->child[pos_praent];
	if(pos_parent < pos->child.size()-2)
	auto r = parent->child[pos_parent+2];

	auto left;
	auto right;
	if(l)
	{
		letf = l;
		right = x;
	}
	else
	{
		pos_parent++;
		left = x;
		right = r;
	}
		left.key.push_back(parent->key[pos_parent];
		left.child.push_back(NULL);
		for(int i = 0;i < right->key.size();i++)
		{
			left.key.push_back(right->key[i]);
			left.child.push_back(NULL);
		}
		delete(r);
		parent->key.erase[pos_parent];
		praent->child.erase[pos_parent+1];

	x = parent;
	while(x) solveUnderflow;
*/

template<typename T>
void b_tree<T>::solveUnderflow(BTNodePosi<T> x)
{
	//极端情况 x 为 key为0
	if (x->key.size() >= _order / 2) return;
	if (x == _root) return;
	BTNodePosi<T> parent = x->parent;
	BTNodePosi<T> l = NULL;
	BTNodePosi<T> r = NULL;

	//if (parent == _root) return;

	//    -1 <=  pos_parent < pos->key.size() 
	int pos_parent;
	if (!x->key.empty()) 
		pos_parent = parent->key.search(x->key[0]);
	else
	{
		for (int i = 0; i < parent->child.size();i++)
		{
			if (parent->child[i] == x)
			{
				pos_parent = i;
				break;
			}
		}
	}
	if (pos_parent >= 0) l = parent->child[pos_parent]; 
	if (pos_parent < (parent->child.size() - 2) )
		r = parent->child[pos_parent + 2];

	//先尝试旋转
	if (l && l->key.size() > _order / 2)
	{
		x->key.insert(0, parent->key[pos_parent]);
		x->key.push_back(NULL);
		parent->key[pos_parent] = l->key.pop();
		l->key.pop_back();
		l->child.pop_back();
	}
	else if (r && r->key.size() > _order / 2)
	{
		pos_parent++;
		x->key.push_back(parent->key[pos_parent]);
		parent->key[pos_parent] = r->key[0];
		r->key.erase(0);
		r->child.pop_back();
	}

	//如果旋转不成功，则进行合并
	else
	{
		BTNodePosi<T> left, right;
		if (l)
		{
			left = l;
			right = x;
		}
		else
		{
			left = x;
			right = r;
			pos_parent++;
		}
		left->key.push_back(parent->key[pos_parent]);
		left->child.push_back(NULL);
		for (int i = 0; i < right->key.size(); i++)
		{
			left->key.push_back(right->key[i]);
			left->child.push_back(NULL);
		}
		parent->key.erase(pos_parent);
		parent->child.erase(pos_parent + 1);
		if (parent->key.empty() && parent == _root)
		{
			_root = left;
			left->parent= NULL;
			delete(parent);
			parent = _root;
		}
		delete(right);
	}
	x = parent;
	while (x)
	{
		solveUnderflow(x);
		x = x->parent;
	}
}