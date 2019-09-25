#pragma once

#include "bst_tree.h"

using namespace std;

/*
红黑树概述及特点：
红黑树（br_tree),基于二叉搜索树封装和实现。
红黑树的4个原则：
（1）根节点是黑色。
（2）每个外子节点（NIL）是黑色。
（3）如果一个节点是红色的，则它的子节点必须是黑色的。
（4）所有外部结点到跟路径上包含相同数目的黑节点。

特点：
1.由于其特点（3）限制，使得红黑树的所有分支高度相对平衡的，其差在1。整个树也不会出现偏斜现象
2.高效的检索（logn),插入（logn1+1),删除(logn).同时相对于AVL树来说，其删除操作对于拓扑结构的影响
  大多数情况只需要一次拓扑结构调整
3.理解：红黑树是基于节点“黑”高度进行平衡的，红色的节点可以视为高度为“0”

操作和接口：
1.插入操作 insert()
2.删除操作 erase()
3.插入操作导致可能出现的“双红问题”调整接口 DoubleRed()
4.删除操作导致可能出现的“双黑问题”调整接口 DoubleBlack()
5.3+4重构接口 p,g,T0,T1,T2,T3  link34()
6.其他操作均可继承

实现逻辑：
1.插入  首先找到插入节点位置 p，然后将节点 x 以红色的形式（即不影响树原先高度的形式）
        插入到树中；
		然后进行双红检测，查看节点x和其父节点是否都是红色，从而违反原则3；
		如果是双红情况，则进行 DoubleRed 处理

2.删除  首先判断节点是否存在，如果不存在则返回。如果存在，则将节点 x 删除后
		进行情况略微复杂的双黑判断流程

3.双红问题处理函数DoubleRed()  
	双红问题的实质：双红问题是由于其父亲节点是红色，导致违反原则3；
	                那么需要通过调整颜色和结构，该分支的黑高度不变

	解决问题的背景：如果产生了双红问题，那么说明父节点 p 为红色，同样也说明 祖父节点 p 一定存在且为黑色；
	                所以解决问题的背景，是 x,f，g 都存在的情况下进行的；
	解决问题的思路：(1).首先寻找到父节点f，祖父节点g以及组父节点的另外一个子节点 u（下面称为叔节点，即父亲的兄弟）  
	                (2).祖节点g是关注的节点，我们的所有调整，目的都是：让节点g下面的所有子节点黑高度不变；
					(3.1).简单情况，x:红色  f:红色 g:黑色 这三个已经确定；
					      如果节点 u 是红色的，那么我们可以通过将u和f同时染黑，
						  然后将 g 染为红色，从而使得g节点下面的所有节点黑高度不变；
						  并且想上一层，判断 gg = g->parent; 和 g 是否存在双红问题；
						  #特殊情况，如果g为根节点，那么可以直接置为黑色，并且返回

					(3.2).稍微复杂一点的情况： x:红色 f:红色 g:黑色 这三个已确定
					      且节点 u 为黑色，此时如果通过 f/x 染黑，则会使得f分支高度增加1
						  并且没有办法弥补 u 分支的高度；
						  所以，需要对 g，f, x,三个节点进行重构，使用类似AVL树的重构方法，
						  将大小居中的节点提升到上方 变成黑色，同时将 g 和 f 节点都变为红；
						  （因为f节点原来就是红色的，所以 f 的另外一个分支必定是黑色，可以
						    直接接入红色的父节点，而无需判断；同理 u 也是黑色的无需判断双红）
						  这样之后，f分支的高度就实际性又降低了一个单位，黑高度没有变化；
						  （解决的实质：将某个红节点插入到f分支，且因为g层级的节点为黑色无变化
						                从而使得原g节点以下的部分高度不变）
						  并直接返回；

4.双黑问题处理判断函数 DoubleBlack()
	双黑问题的实质：双黑问题的出现，是因为某个分支的删除，导致该分支的高度减少1；
	                所以只能通过各种方式的重构 p ,g 使得 g 下的各个分支高度不变；
					着眼和调整的节点是 g；

	双黑问题背景：  如果出现了双黑问题，那么 f：黑色，g: 红/黑，gg：红/黑,u:红/黑

	双黑问题思路：  双黑问题的本质是通过调整节点，使得 f 分支的黑高度 + 1；
	                所以根据 叔 节点的不同情况，来进行调整，使得 u分支 高度不变的同时；
					f 分支高度 + 1；

					(1.0).简单情况：如果 u 为黑色，且存在红色的孩子 u_child;
					      那么将 将 g,u,u_child 三者 进行旋转重构；
						  a.将节点的大小关系 x>y>z 中的 y 替换到 g 的位置，同时颜色不变；
						  b.将节点 x 的颜色 染为 黑色，将节点 u_child 也染为黑色；
						 (情况分析：此时 新的g节点颜色没有变化，u分支 减去一个黑，得到一个黑
						  高度没有变化，f分支被接到了新的黑色节点下，从而使得高度+1，从而形成
						  合法的红黑树）
						 (该重构方式，是否合法分析：
						  T0<T1<T2<T3,这四个被重构的分支，因为都会被接到黑色的节点上，所以
						  他们是红是黑，都不会重现双红的不合法情况，可以直接返回）



*/


template<typename T>
class br_tree:protected bst_tree<T>
{
public:
	br_tree() :bst_tree<T>() {};
	br_tree(const T& t) :bst_tree<T>(t, NULL, BLACK) {};
	bst_BinNodePosi<T> insert(const T&t);
	bool erase(const T&t);
	//bst_BinNodePosi<T> erase(const T&t);
	void DoubleRed(bst_BinNodePosi<T> x);
	void DoubleBlack(bst_BinNodePosi<T> x);

	void DoubleRed_R1(bst_BinNodePosi<T> x);  // 叔叔节点u为黑色处理方案
	void DoubleRed_R2(bst_BinNodePosi<T> x); // 叔叔节点u为红色处理方案
	void link34_insert(bst_BinNodePosi<T>g, bst_BinNodePosi<T>p, bst_BinNodePosi<T>v, bst_BinNodePosi<T> gg,bst_BinNodePosi<T>T0,
		bst_BinNodePosi<T>T1, bst_BinNodePosi<T>T2, bst_BinNodePosi<T>T3);
	void link34_erase(bst_BinNodePosi<T>g, bst_BinNodePosi<T>p, bst_BinNodePosi<T>v, bst_BinNodePosi<T> gg, bst_BinNodePosi<T>T0,
		bst_BinNodePosi<T>T1, bst_BinNodePosi<T>T2, bst_BinNodePosi<T>T3);

	bool br_tree_judge(void(*f)(bst_BinNodePosi<T>, vector<bst_BinNodePosi<T>>));
	static void judge_node(bst_BinNodePosi<T> t, vector<bst_BinNodePosi<T>> vt);
	void judeg()
	{
		br_tree_judge(judge_node);
	}
	color get_root_c() { return bst_tree<T>::_root->c; };
};

/*
红黑树检测：
1.根节点是否是黑色？
2.两个红色节点是否相连？
3.每个叶节点的黑高度是否一致？

思路：
1.检测根节点颜色  
2.遍历树，如果某个节点是红色，则判断是否有红色的父母和子节点
3.遍历树找到所有根节点 （l->child=NULL r->child = NULL）
  并将第一个根节点到 父亲的黑距离存为，black_height;
  并且遍历其他节点，出错时候，就行记录及回溯

*/
template<typename T>
void br_tree<T>::judge_node(bst_BinNodePosi<T> t, vector<bst_BinNodePosi<T>> vt)
{
	/*if (root->c == RED)
	{
		cout << " root color is red " << endl;
		system("pause");
		exit(1);
	}*/
	bst_BinNodePosi<T> l = t->lChild;
	bst_BinNodePosi<T> r = t->rChild;
	if (t->c == RED)
	{
		
		if (l&&l->c == RED)
		{
			cout << " double red!" << endl;
			system("pause");
			exit(1);
		}
		if (r&&r->c == RED)
		{
			cout << " double red!" << endl;
			system("pause");
			exit(1);
		}
	}
	if (!l && !r) vt.push_back(t);
}


/*
申请一个数组vt，
如果节点在judge_node中判断1和2，如果为根节点，就存入数组，
统一判断3
*/
template<typename T>
bool br_tree<T>::br_tree_judge( void(*f)(bst_BinNodePosi<T>, vector<bst_BinNodePosi<T>>) )
{
	vector<bst_BinNodePosi<T>> vt;
	TravPre(bst_tree<T>::_root, f, vt);
	if (bst_tree<T>::_root -> c == RED)
	{
		cout << " root color is red " << endl;
		system("pause");
		exit(1);
	}

	int tree_height = 0;
	
	if (!vt.empty())
	{
		bst_BinNodePosi<T> x = vt.back();
		while (x)
		{
			if (x->c == BLACK) tree_height++;
			x = x->parent;

		}
	}
	while (!vt.empty())
	{
		int node_height = 0;
		bst_BinNodePosi<T> x = vt.back();
		vt.pop_back();
		while (x)
		{
			if (x->c == BLACK) node_height++;
			x = x->parent;
		}
		if (node_height != tree_height)
		{
			cout << "根节点黑高度 不一致" << " 发生点为 " << x->get_value() << endl;
			system("pause");
			exit(1);
		}
	}

	return 1;
}


/*
删除逻辑：
1.进行seach ， pos=search(t);
2.pos为真
  r=succ(pos),用r和pos 进行替换,并删除pos内容；
  s = (p==_hot->lChild)? _hot->rChild:_hot->lChild;
  p_color = p.c;
  r_color = r ? r->c RED;
  s_color = s.c;

3.根据pos,r,s颜色进行识别，进行相应的调整
  如果pos或者r 其中一个为红色,if(r) r->c = RED 即可；
  如果双黑色，则进行双黑处理流程，调整单元格与颜色

4.执行实际删除操作

*/
template<typename T>
bool br_tree<T>::erase(const T& t)
{
	bst_BinNodePosi<T> pos = bst_tree<T>::search(t);
	if (!pos) return false; //未找到数据删除失败

	bst_BinNodePosi<T> r = pos->succ(); //replacement 替代者
	bst_BinNodePosi<T> p = pos->parent;
	color pos_color, r_color;
	pos_color = pos->c;
	r_color = r ? r->c : BLACK;
	
	if (pos_color == RED || r_color == RED) //简单情况 pos 和 r 其中一个为红色，删除后将后继（如果）置为红色即可，即删除侧高度不变
	{
		bst_tree<T>::erase(t);
		if(r) r->c = RED;
	}
	else if (bst_tree<T>::size <= 3) //双黑问题，且小于等于3
	{
		bst_tree<T>::erase(t);
	}
	else
	{
		DoubleBlack(pos);
		bst_tree<T>::erase(t);
	}

	return true;
}

template<typename T>
void br_tree<T>::DoubleBlack(bst_BinNodePosi<T> pos)
{
	T erase_t = pos->get_value();
	auto temp = pos;
	//特殊情况，删除节点是根节点 那么pos=pos->succ();
	if (pos == bst_tree<T>::_root)
		pos = pos->succ();

	//bst_BinNodePosi<T> r = pos->succ();
	bst_BinNodePosi<T> p = pos->parent;
	bst_BinNodePosi<T> s;
	if (!p) s = NULL;
	else s = (pos == p->lChild) ? p->rChild : p->lChild;
	//特殊情况s如果不存在，说明流程进入到了 t->succ() 流程
	//那么
	if (!s)
	{
		if (p->c == RED)
		{
			p->c = BLACK;
			pos -> c = RED;
		}
		else
		{
			s = (pos == p->lChild) ? p->rChild : p->lChild;
			bst_BinNodePosi<T> g = p->parent;
			if (g)
			{
				if (p == g->lChild) g->lChild = s;
				else g->rChild = s;
			}
			bst_BinNodePosi<T> T0;
			if (s == p->lChild)
			{
				T0 = s->rChild;
				s->rChild = p;
				p->lChild = T0;
				p->parent = s;
				p->c == RED;
				if (T0) T0->parent = p;
			}
			else
			{
				T0 = s->lChild;
				s->lChild = p;
				p->rChild = T0;
				p->parent = s;
				p->c == RED;
				if (T0) T0->parent = p;
			}
			if (bst_tree<T>::_root == p)
			{
				bst_tree<T>::_root = s;
				if (s->c == RED) s->c == BLACK;
			}
		}
	}

	else if(s&&s->c == BLACK)
	{
		
		color s_l_color = s->lChild ? s->lChild->c : BLACK;
		color s_r_color = s->rChild ? s->rChild->c : BLACK;
		//类型1.S为黑，且S得孩子有红色的
		if (s_l_color == RED || s_r_color == RED)
		{
			bst_BinNodePosi<T> T0, T1, T2, T3;
			color p_color = p->c;
			bst_BinNodePosi<T> t = (s_l_color == RED) ? s->lChild : s->rChild;
			if (s == p->lChild)
			{
				if (t == s->lChild)
				{
					T0 = t->lChild;
					T1 = t->rChild;
					T2 = s->rChild;
					T3 = p->rChild;
					link34_erase(t, s, p, p->parent,T0, T1, T2, T3);
					s->c = p_color;
					t->c = p->c = BLACK;
				}
				else
				{
					
					T0 = s->lChild;
					T1 = t->lChild;
					T2 = t->rChild;
					T3 = p->rChild;
					link34_erase(s, t, p, p->parent, T0, T1, T2, T3);
					t->c = p_color;
					s->c = p->c = BLACK;
				}
			}
			else
			{
				if (t == s->rChild)
				{
					T0 = p->lChild;
					T1 = s->lChild;
					T2 = t->lChild;
					T3 = t->rChild;
					link34_erase(p, s, t, p->parent, T0, T1, T2, T3);
					s->c = p_color;
					t->c = p->c = BLACK;
				}
				else
				{
					T0 = p->lChild;
					T1 = t->lChild;
					T2 = t->rChild;
					T3 = s->rChild;
					link34_erase(p, t, s, p->parent, T0, T1, T2, T3);
					t->c = p_color;
					s->c = p->c = BLACK;
				}
			}
		}
		//S为黑，且S得孩子没有红色
		else if (s_l_color == BLACK && s_r_color == BLACK)
		{
			//类型2.S为黑，S子节点都黑，P为红
			if (p->c == RED)
			{
				s->c = RED;
				p->c = BLACK;
			}
			//类型3，S为黑，S子节点都黑，P为黑
			else
			{
				s->c = RED;
				bst_BinNodePosi<T> g = p->parent;
				if (g)
				{
					g->c = BLACK;
					DoubleBlack(p);
				}
			}
		}
	}
	//情况4 S为红，其他为黑色
	else
	{
		bst_BinNodePosi<T> T0, T1, T2;
		bst_BinNodePosi<T> g = (p) ? p->parent : NULL;
		if (s == p->lChild)
		{
			T0 = s->lChild;
			T1 = s->rChild;
			T2 = p->rChild;

			s->rChild = p;
			s->parent = g ? g : NULL;
			p->lChild = T1;
			if (T1) T1->parent = p;
			p->parent = s;

			s->c = BLACK;
			p->c = RED;
			if (bst_tree<T>::_root == p) 
				bst_tree<T>::_root = s;

		}
		else
		{
			T0 = p->lChild;
			T1 = s->lChild;
			T2 = s->rChild;

			s->lChild = p;
			s->parent = g ? g : NULL;
			p->rChild = T1;
			if (T1) T1->parent = p;
			p->parent = s;

			s->c = BLACK;
			p->c = RED;
			if (bst_tree<T>::_root == p) bst_tree<T>::_root = s;
				
		}
		if (g)
		{
			if (p == g->lChild) 
				g->lChild = s;
			else g->rChild = s;
		}
		bst_tree<T>::_size++;
	}
}


/*

插入操作逻辑：

首先进行search 操作，获得pos;

#极限情况 树不存在节点，_hot = NULL; 则直接new 一个黑色节点作为根节点#

pos 为真，节点已经存在，则返回；pos = Null 则节点不存在，在_hot进行插入：
1.new 一个新的节点（默认为红）进行插入，到_hot下面；
2.进行双红检测
3.反回新插入的节点

*/
template<typename T>
bst_BinNodePosi<T> br_tree<T>::insert(const T&t)
{
	//极限情况 树为空
	if (!bst_tree<T>::_root)
	{
		bst_tree<T>::size++;
		bst_tree<T>::_root = new bst_BinNode<T>(t, NULL, BLACK);
		return bst_tree<T>::_root;
	}

	bst_BinNodePosi<T>  pos = bst_tree<T>::search(t);
	if (pos) return pos; //节点存在

	pos = new bst_BinNode<T>(t, bst_tree<T>::_hot, RED);
	if (pos->get_value() > bst_tree<T>::_hot->get_value())
		bst_tree<T>::_hot->rChild = pos;
	else bst_tree<T>::_hot->lChild = pos;
	bst_tree<T>::size++;
	DoubleRed(pos); //双红判断并处理函数
	return pos;
}

/*

双红问题解决流程：
#极限情况 
1.检测 p = x->parent if(p->col==RED) 则进入双红排除流程
2.双红排除流程正式开始：
	 g = p->parent;
     u = g->child;
2.1 u->color == BLACK ; 
    将x,p,g 进行3+4重构
	p为黑色，x,g为红
2.2 u->color == RED;
    将x,p,g,u 视为一个超级节点，将g染红提升到上一级
	将p和u染黑即可
	并向上追踪，x=g；
	if x==_root;调整x 为黑。
	else 循环进行双红检测
	直到 x == NULL结束
*/

template<typename T>
void br_tree<T>::DoubleRed(bst_BinNodePosi<T> x)
{
	bst_BinNodePosi<T> p = x->parent;  //无需处理情况
	if (p->c == BLACK) return;

	bst_BinNodePosi<T> g = p->parent;
	bst_BinNodePosi<T> u = (p == g->lChild) ? g->rChild : g->lChild;
	color u_c;
	if (u) u_c = u->c;
	else u_c = BLACK;

	if (u_c == BLACK) DoubleRed_R1(x); // 双红问题的一般情况，通过重构分给u节点一个红色的节点
	else DoubleRed_R2(x); // 双红问题简单情况，将p和u都染黑，g变成红，并且向上继续判断
	return;
}

template<typename T>
void br_tree<T>::DoubleRed_R1(bst_BinNodePosi<T> x)
{
	bst_BinNodePosi<T> p = x->parent;
	bst_BinNodePosi<T> g = p->parent;
	bst_BinNodePosi<T> T0, T1, T2, T3;

	if (p == g->lChild)
	{
		//link34_insert 说明： 将 x>y>z 三个节点传入；
		//x和z接到 y 下方，并且为红色， y 为黑；并将 gg 节点传入
		//使得gg 指向新调整后的节点
		// T0<T1<T2<T3 ,分别是 节点x , p， g 的另外部分子节点；
		if (x == p->lChild)
		{
			T0 = x->lChild;
			T1 = x->rChild;
			T2 = p->rChild;
			T3 = g->rChild;
			link34_insert(x, p, g,g->parent, T0, T1, T2, T3);
		}
		else
		{
			T0 = p->lChild;
			T1 = x->lChild;
			T2 = x->rChild;
			T3 = g->rChild;
			link34_insert(p, x, g, g->parent, T0, T1, T2, T3);
		}
	}
	else
	{
		if (x == p->rChild)
		{
			T0 = g->lChild;
			T1 = p->lChild;
			T2 = x->lChild;
			T3 = x->rChild;
			link34_insert(g, p, x, g->parent, T0, T1, T2, T3);
		}
		else
		{
			T0 = g->lChild;
			T1 = x->lChild;
			T2 = x->rChild;
			T3 = p->rChild;
			link34_insert(g, x, p, g->parent, T0, T1, T2, T3);
		}
	}
	
	return;
}

template<typename T>
void br_tree<T>::DoubleRed_R2(bst_BinNodePosi<T> x)
{
	bst_BinNodePosi<T> p = x->parent;
	bst_BinNodePosi<T> g = p->parent;
	bst_BinNodePosi<T> u = (p == g->lChild) ? g->rChild : g->lChild;

	//极限情况 g == _root;
	if (g == bst_tree<T>::_root) g->c = BLACK;
	else g->c = RED;
	p->c = BLACK;
	if(u) u->c = BLACK;
	
	x = g;
	bst_BinNodePosi<T> gg = g->parent;
	if(gg&&gg->c==RED) DoubleRed(x);
	return;
}


template<typename T>
void br_tree<T>::link34_insert(bst_BinNodePosi<T> lower_node, bst_BinNodePosi<T> new_parent, bst_BinNodePosi<T> higher_node, bst_BinNodePosi<T>gg,
	bst_BinNodePosi<T>T0, bst_BinNodePosi<T>T1, bst_BinNodePosi<T>T2, bst_BinNodePosi<T>T3)
{
	//极限情况 g 为_root
	if (gg)
	{
		if (higher_node == gg->lChild) gg->lChild = new_parent;
		else gg->rChild = new_parent;
	}
	else bst_tree<T>::_root = new_parent;

	new_parent->lChild = higher_node;
	new_parent->rChild = lower_node;
	new_parent->parent = gg;
	new_parent->c = BLACK;
	higher_node->lChild = T0;
	higher_node->rChild = T1;
	higher_node->parent = higher_node;
	higher_node->c = RED;
	lower_node->lChild = T2;
	lower_node->rChild = T3;
	lower_node->parent = higher_node;
	lower_node->c = RED;

	if (T0) T0->parent = higher_node;
	if (T1) T1->parent = higher_node;
	if (T2) T2->parent = lower_node;
	if (T3) T3->parent = lower_node;
}

template<typename T>
void br_tree<T>::link34_erase(bst_BinNodePosi<T>g, bst_BinNodePosi<T>p, bst_BinNodePosi<T>v, bst_BinNodePosi<T>gg,
	bst_BinNodePosi<T>T0, bst_BinNodePosi<T>T1, bst_BinNodePosi<T>T2, bst_BinNodePosi<T>T3)
{

	//极限情况 g 为_root
	if (gg)
	{
		if (g == gg->lChild) gg->lChild = p;
		else gg->rChild = p;
	}
	else bst_tree<T>::_root = p;

	p->lChild = g;
	p->rChild = v;
	p->parent = gg;
	g->lChild = T0;
	g->rChild = T1;
	g->parent = p;
	v->lChild = T2;
	v->rChild = T3;
	v->parent = p;

	if (T0) T0->parent = g;
	if (T1) T1->parent = g;
	if (T2) T2->parent = v;
	if (T3) T3->parent = v;
}
