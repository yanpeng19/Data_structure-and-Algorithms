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
		2.3 solveOverflow()     // 检查并处理（插入导致的）节点上溢：
		2.4 solveUnderflow()    // 检查并处理（删除导致的）节点下溢：
		
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

		3.2 检查并处理上溢 solveOverflow()		(1). 如果节点 X ， x->size() >= _order;那么久发生了上溢出；
												(2). 首先寻找该节点的中间位置的元素， mi =  key.size()/2;
													 并将节点分为连个新节点：
													 
													 原来的节点部分： key[0]-k[mi-1] 内容
													 新生节点： BT_Node new = BT_Node( key[mi+1]~key[key.size()-1])
													 
													 并相应的也将他们的 存放子指针的 向量Chlid 进行分裂， 
													 左半边:  Child[0~mi]
													 右半边： Child[mi+1~Child.size()-1] 并将他的内容给 new;

												(3). 将key[mi] 提升到上升一层中，并在  parent = x->parnet 
													 在 parent->key 合适位置插入 key[mi]
													 插入位置的为：insert_posi = parnet.search(key[mi]);
													 parent.key.insert(insert_posi+1,key[mi];   //插入key[mi] 到父节点中

													 //为什么插入位置是 insert_posi+1 ? 因为我定义的查找函数返回的是 不大于查找值的节点RANK
													 //而我自定义的插入操作 是在 RANK 的位置上 直接插入的
													 // 例如 rank = 0； insret(0,new) ,那么就会在 key[0] 插入 new
													 //方便理解，特此声明

													 同时相应的也在 paretn.Child 进行相应的插入分裂出来的分支，
													 parent->Child.insert( insert_posi+2,new)

													 // 为什么 新插入节点new左边没有进行 子节点接入？
													 // 因为 parent->key[insert_posi+1] == key[mi]
													 // 那么 paretn->child[insert_posi+1] 就是小于等于 key[mi] 分支也就是 原来的 x 节点
													 // 所以无需特意操作

													 * 极限情况 1： x-parnet 不存在，说明 x 是根节点
													 * 此时进行 ： _root = new BT_Node(key[mi],x,new);
													 * 生成一个新节点插入 key[mi] 并连接 x 和 new
												(4). 遍历new_node->child，并让他们指向new_node
													 一般情况是在最底层进行插入操作的，但是，如果上溢向上传导之后，如果在上一层同样发生了上溢出，
													 那么需要遍历新分裂出来的节点，让他们指向 new_node
												(5).因为节点上溢可能会导致，父节点parent 上溢出；所以如果发生了上溢，那么持续检查上一个节点
												(*).伪代码：
												solveOverflow(BT_Node x)
												{
													if(x.key.size()>=_order)
													{
														//发生了上溢出
														using rank = unsigned;
														rank mi = x.key().size()/2;
														T value_mi = key[mi];

														BT_Node* new_node = BT_Node(x,mi+1,key.size()-1);
														// 用x.key[mi+1]~x.key[key.size()-1] 以及 x.child[mi]~x.child[chiled.size()-1]
														// 初始化new_node

														//删除掉key和chid mi之后的值
														for(int i = x.key().size()-1; i>=mi;i--)
														{
															x->key.pop();
															x->child.pop();
														}

														BT_Node* parnet = x->parnet;

														//极限情况1，x为根节点,生成节点并且返回即可
														if(!parent) 
														{
															_root = new BT_Node(value_mi,x,new);
															new_node -> parent = _root;
															_hot -> parent = _root;
															return;
														}

														//否则将value_mi提升到 parent中去,并接入new_node;
														rank posi = parent->key.search(value_key);
														parent->key.insert(posi+1,value_key);
														parent->child.insert(posi+2,new);
														new_node->parnet = parent;

														//并且继续向上一层检查
														solveOverflow(parent);
													}
												}


		3.3 删除 erase(const T& t)				(1).进行搜索 BT_Node* p = search(t);
												(2).如果 p 不存在直接返回，如果 P 为真则继续
												(3).在 P节点中 找到 t 的位置 int pos = p.key.search(t);
												    为了不影响结构，不停的 将 寻找中序序列意义下的节点 并交换
												(4).此时p一定是叶节点，直接删除p.key[0],并执行 p->child.pop();
												(5).p的删除操作可能导致下溢出，进行下溢检查，solveUnderflow(p);
												(*).伪代码：
												erase(cont T&t)
												{
													BT_Node* p = search(t);
													if(!p) return;

													using rank = unsigned int;
													rank posi = p->key.serach(t);

													BT_Node* c = p->child[pos+1];
													if(c) p.key[pos] = c.key[0];
													p = c;
													c = c->child[1];
													//将t降到叶节点，再删除
													while(c)
													{
														p->key[0] = c.key[0];
														p = c;
														c = c->child[1];
													}

													p->key.erase(0);
													p->child->pop();
													solveOverflow(p);
													return ture;
												}

		3.4 下溢出检测		(1).首先进行判断 是否发生下溢出，即 x->key.size() < （order-1）/2; 如果未发生，直接返回
							(2).如果发生了下溢出，那么有两种解决办法，分别是： 合并，旋转； 首先尝试旋转借出元素
							(2.1) 借出处理方法：  找到 节点 x 的左右节点，分别是 l，r;
												  l或者r 存在，且 key.size()>=(order/2)+1;
												  那么就可以通过“借出”一个元素，来给节点 x ，让他不再下溢。
												  
												  a.找到l/r,并确认借出的元素值，如果是l，那么value=l->key[0];
													如果是r 那么 value = r->key[size()-1];
												  b.查找 value 在 parent 中 合适的交换元素
												    int pos_parent = parent->key.search(vllue);
													如果 是 l 那么 pos_parent = pos_parent + 1;
													如果 是 r 那么 pos_parent 无需改变
												  c.将parent 父节点中 pos_parent的元素用 value替代，并且将parent原数值给 x
													pos_value = parent->key[pos_parnet];
												    parent->key[pos_parnet] = value;

													如果是 l 情况，x->key.insert(0,pos_value);
													如果是 r 情况，x->key.push_back(pos_value);
												  d.将l，或r节点的 值删除，
													erase(l,0) 或者 erase(r,size()-1);
													##这个删除可能导致向下传导，多个节点连环下溢出
							(2.2) 合并处理办法： 当节点 x 不存在 l 或者 r节点，以及l，r节点的数不够借出时，就需要进行合并操作
												 思路是 寻找左边的l节点，然后再尝试 右边的r 节点，然后找到父节点中 对应的 key[pos_parent]
												 将 parent->key[pos_parent]拿下来，作为粘合剂，将l/r + parent->key[pos_parent] + x 生成一个新节点
												 并且 调整parent.child

												 ##如果 l/r key().size<(order-1)/2 ,且 x->key.size()<(order-1)/2
												 ##那么 l/r.key()+x.key()+1 一定不会发生上溢出，至多 和 (order-1） 相等
												 ## (order-1)/2*2 >= l.key().size() * 2
												 ## (order-1)/2*2 >= l.key().size() + (x.key.size()+1
												 ## 该公式成立，新节点不会上溢

												 a.在parent节点中，寻找对应的父节点位置： int pos_parent = parent->key.search(x->key[0])
												   l 情况， value_parent = parent[pos_parent+1]
												   r 情况， value_parent = parent[pos_parent];
												 b. 并将 value_parent下降， 作为粘合剂 l/r 以及 x 节点粘合起来，
												    如果是l节点情况，那么： new_node.key = l.key()+value_parent + x.key()
													如果是r节点情况，那么： new_node.key = x.key()+value_parnet+r;
													并相应合并 child 部分
												 c.在parent中，将对应的 parnet->child[pos_parent]/parent->child[pos_paretn+1]剔除
												 d.因为parent 的数值减少了1个，所以需要在对 parent 进行检查是否下溢
													solveUnderflow(parent);
												## 特殊情况，当x.key.size==0.且不存在l，r节点的时候；直接删除节点x，并将parent中对应child置为空
												## 特殊情况2，当x.key.size()==0 且 x = _root 时候，特殊处理流程：
												   a.寻找x是否有子节点，_root_lc = x->child[0] ，如果 _root_lc为NULL，说明树已经空，删除_root即可
												   b.如果存在 _root_lc ,那么将 x-> key[0] 下降到节点 _root_lc + x + _root_rc 中，并交换到最底层，再删除
												     再检测是否下溢出

												(*) 伪代码：
												solveUnderflow(x)
												{
													如果没有下溢，那么返回
													if(x->key.size()>(order-1)/2) return ;
													//特殊情况x为根节点，如果空了，那_root置为空，否则不做操作
													if(x == _root)
													{
														if(x.size()==0)
														{
															delete x;
															_root = NULL;
														}
														return;
													}

													BT_Node* parent = x->parent;
													BT_Node* l = NULL;
													BT_Node* r = NULL;
													unsigned pos_parent;
													unsigned pos_x;
													T value_par;

													//在parent中找到 l和r
													for(pos_x=0;i<parent.child.size();i++)
													{
														if(parnet.child[pos_x]== x)
														{
															if(i-1>=0) l = parent->child[i-1];
															if(i+1<parent.child.size()) r = parent->child[i+1]
														}													
													}

													//特殊情况，x.size()==0；
													if(x->key.size()==0&&!l&&!r)
													{
														// l 和 r 都不存在，直接把节点删除
														delete(x);
														parent[pos_x] = NULL;
														return;
													}
													//进入正式流程，先尝试旋转,先左后右边

													//左节点存在，且可以旋转情况
													if(l&&l->key.size()>(order-1)/2))
													{
														pos_parent = parent->key.serach(x->key[0]);
														value_par = parent->key[pos_parent];
														parent.key[pos_parent] = l.key[l.size()-1];
														x->key.insert(0,value_pos);
														x->child.push_back(NULL);
														erase(l,l->key.size()-1);
														return treu;
													}
													if(r&&r->key.size()>(roder-1)/2) //右节点存在，且可以旋转情况
													{
														pos_parent = parent->key.search(r->key[0]);
														value_par = parent->key[pos_parent];
														parent->key[pos_parent] = r.key[0];
														x->key.push_back(value_par);
														x->child.push_back(NULL);
														erase(r,0);
														return;
													}

													//那么下面进入和并流程
													if(l)
													{
														//确定父节点中合并的值
														pos_parent = parent->key.search(x->key[0]);
														value_par = paretn->key[pos_parent];
														l->key.push_back( value_par);
														//把节点合并
														for(int i=0;i<x->key.size();i++)
														{
															l->key.push_back(x->key[i]);
															l->child.push_back(x->child[i];
														}
														//删除并检查父节点
														delete(x);
														x = NULL;
														parent->key.erase(pos_parent);
														parent->child.erase(pos_parent+1);
														solveUnderflow(parent);
														return;
													}
													else
													{
														//确定父节点中合并的值
														pos_parent = parent->key.search(r->key[0]);
														value_par = paretn->key[pos_parent];
														x->key.push_back( value_par);
														for(int i=0;i<x->key.size();i++)
														{
															x->key.push_back(x->key[i]);
															x->child.push_back(x->child[i];
														}
														//删除节点并检查父节点
														delete(r);
														r = NULL;
														parent->key.erase(pos_parent);
														parent->child.erase(pos_parent+1);
														solveUnderflow(parent);
														return;
													}
												}
		   

*/

#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "BT_Node.h"
#include <queue>

using namespace std;


// BTNodePosi = BT_Node<T>*; 
template<typename T>
class b_tree
{
	friend iostream;
public:
	b_tree(const unsigned &t) :_order(t) { _root = _hot = NULL; _size = 0; };
	~b_tree()
	{
		queue <BTNodePosi<T>> q;
		q.push(_root);
		while (!q.empty())
		{
			BTNodePosi<T> temp = q.front();
			q.pop();
			for (auto a : temp->child)
				if (a) q.push(a);
			delete temp;
		}
	}

	BTNodePosi<T> search(const T& t);
	bool insert(const T&t);
	bool erase(const T&t);

	void solveOverflow(BTNodePosi<T>);
	void solveUnderflow(BTNodePosi<T>);

	void out() //按层级输出树
	{
		queue<BTNodePosi<T>> q, q_line;
		q.push(_root);
		int j=1;
		int z = 0;
		/*
			输出树的函数-按照层级输出：
			1.使用两个队列来输出层级 分别 q 总队列，q_line 层级队列
			2.将节点压入根节点后，判断下一个层级有多少有效节点，并将这些节点给q_line
			3.q_line 输出行
			4.q 控制换行符
		*/
		while (!q.empty())
		{
			for (int i = 0; i < j; i++)
			{
				BTNodePosi<T> temp = q.front();
				q.pop();
				q_line.push(temp);
			}
			
			j = 0;

			while (!q_line.empty())
			{
				BTNodePosi<T> temp = q_line.front();
				q_line.pop();

				cout << "| ";
				for (auto a : temp->key)
				{
					cout << a << " ";
					z++;
				}
				cout << " | ";
				
				
				for (auto a : temp->child)
				{
					if (a)
					{
						q.push(a);
						j++;

						//父子节点不匹配检测
						if (a->parent != temp)
						{
							cout << endl;
							cout << "here have error" << endl;
							cout << "node " << a << " -> " << a->key[0] << " parnet is not" << temp << " -> " << temp->key[0] << endl;
							cout << "is " << a->parent << " -> " << a->parent->key[0] << endl;
						}
					}
				}
			}
			cout << endl;//此时j为下一行有效节点数

		}
		cout << "total : " << z << endl;
	}

protected:
	BTNodePosi<T> _root;
	BTNodePosi<T> _hot;
	int _size;     //
	int _order;    //阶次 即_order-1 = max[child->size()]
};

template<typename T>
BTNodePosi<T> b_tree<T>::search(const T& t)             //成功则return x,失败则return NULL
{
	_hot = NULL;
	int pos;
	BTNodePosi<T> x = _root;
	while(x)
	{
		pos = x->key.search(t);
		if (pos!=-1 && x->key[pos] == t) return x;
		_hot = x;
		x = x->child[pos + 1];
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
template<typename T>
bool b_tree<T>::insert(const T& t)
{
	int pos;
	if (!_root)
	{
		_root = new BT_Node<T>(t, BTNodePosi<T>(NULL), BTNodePosi<T>(NULL)); //极端情况，树不存在_root
		_size++;
		return true;
	}
	BTNodePosi<T> x = search(t);
	//节点存在直接返回
	if (x) return false;

	pos = _hot->key.search(t);
	_hot->key.insert(pos + 1, t);
	_hot->child.push_back(NULL);

	solveOverflow(_hot);
	_size++;


	return true;
}

/*
检查是否上溢出：
1. 检查_hot->key.size() 是否大于 order;如果大于则进行下一步
2. 找到中间的元素 mi,将数据分成两部分，l ,r 
3. 把mi提升到上一层，并且连接l和r
*/
template<typename T>
void b_tree<T>::solveOverflow(BTNodePosi<T> _hot)
{

	if (_hot->key.size() > _order-1)
	{
		int mi = (_hot->key.size() / 2);

		//新节点拷贝mi~size()的 key 和child
		BTNodePosi<T> new_node = new BT_Node<T>(_hot, mi);
		T value_mi = _hot->key[mi];

		for (int i = _hot->key.size() - 1; i >= mi; i--)
		{
			_hot->key.pop_back();
			_hot->child.pop_back();
		}

		BTNodePosi<T> paren = _hot->parent;
		if (!paren)
		{
			//极限情况 节点为_root;
			_root = new BT_Node<T>(value_mi, _hot, new_node);
			_hot->parent = _root;
			new_node->parent = _root;
			for (auto a : new_node->child)
			{
				if (a)
					a->parent = new_node;
			}
			return;
		}
		     
		// 上一节点的正确插入位置 在praen_pos之后
		int parent_pos = paren->key.search(value_mi);
		paren->key.insert(parent_pos + 1, value_mi);
		paren->child.insert(parent_pos + 2, new_node);
		new_node->parent = paren;

		for (auto a : new_node->child)
		{
			if (a)
				a->parent = new_node;
		}
		solveOverflow(paren);
	}
}

/* 
删除逻辑：
1.seach(t) 查找元素是否存在树中，如果存在则进入删除流程
2.删除的流程： 按照流程先在key中找到 t的位置pos，然后将t与child[pos+1][0] 交换 
               直到t到最底层，即 !child[pos+1],然后 执行 key.erase(t) child.pop_back()
3.检查节点是否发生了下溢
*/
template<typename T>
bool b_tree<T>::erase(const T& t)
{
	if (!_root) return false;

	int pos;
	BTNodePosi<T> x = search(t);
	if(x) pos = x->key.search(t);
	else
	{
		cout << "find fail:"  << t << endl;
		return false;
	}

	
	if (x == _root)
	{
		//极端情况 x 为根节点 且 size = 1 删除节点即可
		if (_size == 1)
		{
			delete(_root);
			_root = NULL;
			return true;
		}
		//极端情况2 x 为根节点且 只有一个数
		if (x->key.size() == 1)
		{
			BTNodePosi<T> l = x->child[0];
			BTNodePosi<T> r = x->child[1];
			
			l->key.push_back(x->key[0]);
			for (int i = 0; i < r->child.size(); i++)
			{
				if (i < r->key.size())
					l->key.push_back(r->key[i]);
				l->child.push_back(r->child[i]);
					if (r->child[i]) r->child[i]->parent = l;
			}
			delete(x);
			delete(r);
			x = r = NULL;
			l->parent = NULL;
			_root = x = l;
			//至此，原来的_root已经下降成功，接下来正常处理即可，不过因为_root的size超标，需要再做一次上溢出检测

			pos = x->key.search(t);
			BTNodePosi<T> c = x->child[pos + 1];
			if (c)
			{
				while (c)
				{
					_hot = c;
					c = c->child[0];
				}
				//至此 x 为叶节点
				x->key[pos] = _hot->key[0];
				_hot->key.erase(0);
				_hot->child.pop();
				solveUnderflow(_hot);
				solveOverflow(_root);
				return true;
			}
			else
			{
				//x 为叶节点情况
				x->key.erase(pos);
				x->child.pop();
				solveUnderflow(x);
				solveOverflow(_root);
				return true;
			}
			
		}
			
	}

	//一般情况，将x[pos]于succ(x) 交换，然后删除
	pos = x->key.search(t);
	BTNodePosi<T> c = x->child[pos+1];
	
	if (c)
	{
		while (c)
		{
			_hot = c;
			c = c->child[0];
		}
		//至此 x 为叶节点
		x->key[pos] = _hot->key[0];
		_hot->key.erase(0);
		_hot->child.pop();
		solveUnderflow(_hot);
		return true;
	}
	else
	{
		//x 为叶节点情况
		x->key.erase(pos);
		x->child.pop();
		solveUnderflow(x);
		return true;
	}
}

/*
	下溢出逻辑：
	节点x经过删除后，可能x->key.size() < (_order/2)-2  
	为了保证树的高度以及利用率，需要进行 a.旋转 b.合并

	优先进行
	a.旋转操作，向其左或者右节点借用一个节点
	b.合并操作，将父节点的中对应的数据降下来，作为粘合剂。将x与左或右节点粘合
	  并且检查父节点是否下溢
*/
template<typename T>
void b_tree<T>::solveUnderflow(BTNodePosi<T> x)
{
	//没有发生下溢
	if (x->key.size() >= (_order-1) / 2) return;
	if (x == _root) return;//特殊情况：根节点
	
	BTNodePosi<T> parent = x->parent;
	BTNodePosi<T> l = NULL;
	BTNodePosi<T> r = NULL;
	int pos_parent = 0;
	int pos_x;

	//确认l和r
	for (pos_x = 0; pos_x < parent->child.size();pos_x++)
	{
		if (parent->child[pos_x] == x)
		{
			if (pos_x - 1 >= 0) l = parent->child[pos_x - 1];
			if (pos_x + 1 <= parent->child.size() - 1) r = parent->child[pos_x + 1];
			break;
		}
	}

	// 先尝试L 节点旋转, #注意要把 l或者r的子节点也接过来
	if (l && l->key.size() > (_order-1) / 2)
	{
		pos_parent = parent->key.search(x->key[0]);
		x->key.insert(0, parent->key[pos_parent]);
		x->child.insert(0, l->child[l->child.size() - 1]);

		if(x->child[0]) x->child[0] = x;

		parent->key[pos_parent] = l->key[l->key.size()-1];

		l->key.erase(l->key.size() - 1);
		l->child.pop();
	}
	else if (r && r->key.size() > (_order - 1) / 2) //然后试试r节点旋转
	{
		pos_parent = parent->key.search(r->key[0]);
		x->key.push_back(parent->key[pos_parent]);
		x->child.push_back(r->child[0]);
		if(r->child[0]) r->child[0]->parent = x;

		parent->key[pos_parent] = r->key[0];
		
		r->key.erase(0);
		r->child.erase(0);
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
		}

		pos_parent = parent->key.search(right->key[0]);
		left->key.push_back(parent->key[pos_parent]);

		for (int i = 0; i <= right->key.size(); i++)
		{
			if(i<right->key.size())
				left->key.push_back(right->key[i]);

			left->child.push_back(right->child[i]);
			if (right->child[i]) right->child[i]->parent = left;
		}
		parent->key.erase(pos_parent);
		parent->child.erase(pos_parent + 1);

		//特殊情况 parent为_root,且借了之后 没东西了
		if (parent->key.empty() && parent == _root)
		{
			_root = left;
			delete(parent);
			delete(right);
			parent = right = NULL;
			left->parent= NULL;
			return;
		}
		delete(right);
		solveUnderflow(parent);
	}
}