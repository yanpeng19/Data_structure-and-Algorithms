#pragma once

#include "bst_tree.h"
#include <queue>
#include <math.h>
#include <sstream>

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
1.插入  首先找到插入节点位置 pos，然后将节点 x 以红色的形式（即不影响树原先高度的形式）
        插入到树中；
		然后进行双红检测，查看节点x和其父节点是否都是红色，从而违反原则3；
		如果是双红情况，则进行 DoubleRed 处理

2.删除  首先判断节点是否存在，如果不存在则返回。如果存在，则进入双黑判断调整流程
        调整后正常进行删除，并让 r 继承 p 的颜色即可。
		

3.双红问题处理函数DoubleRed()  
	双红问题的实质：双红问题是由于其父亲节点是红色，导致违反原则3；
	                那么需要通过调整颜色和结构，该分支的黑高度不变

	解决问题的背景：如果产生了双红问题，那么说明删除节 f 为红色，同样也说明 祖父节点 g 一定存在且为黑色；
	                所以解决问题的背景，是 x ,f，g 都存在的情况下进行的；

	解决问题的思路：(1).首先寻找到父节点f，祖父节点g以及组父节点的另外一个子节点 s（下面称为叔节点，即父亲的兄弟）  
	                (2).祖节点g是关注的节点，我们的所有调整，目的都是：让节点g下面的所有子节点黑高度不变；
					(3.1).简单情况，x:红色  f:红色 g:黑色 这三个已经确定；
					      如果节点 s 是红色的，那么我们可以通过将s和f同时染黑，
						  然后将 g 染为红色，从而使得g节点下面的所有节点黑高度不变；
						  并且想上一层，判断 gg = g->parent; 和 g 是否存在双红问题；
						  #特殊情况，如果g为根节点，那么可以直接置为黑色，并且返回

					(3.2).稍微复杂一点的情况： x:红色 f:红色 g:黑色 这三个已确定
					      且节点 s 为黑色，此时如果通过 f/x 染黑，则会使得f分支高度增加1
						  并且没有办法弥补 s 分支的高度；
						  所以，需要对 g，f, x,三个节点进行重构，使用类似AVL树的重构方法，
						  将大小居中的节点提升到上方 变成黑色，同时将 g 和 f 节点都变为红；
						  （因为f节点原来就是红色的，所以 f 的另外一个分支必定是黑色，可以
						    直接接入红色的父节点，而无需判断；同理 s 也是黑色的无需判断双红）
						  这样之后，f分支的高度就实际性又降低了一个单位，黑高度没有变化；
						  （解决的实质：将某个红节点插入到f分支，且因为g层级的节点为黑色无变化
						                从而使得原g节点以下的部分高度不变）
						  并直接返回；

4.双黑问题处理判断函数 DoubleBlack()
	双黑问题的实质：双黑问题的出现，是因为删除节点 pos 的后继 r = succ(pos)， 以及 r 的父亲 p
	                都是黑色，从而导致删除后的 r 分支实际高度 -1；

					（这里复习一下节点 r 的概念： 
					  1.当 pos 有两个节点的时候，r 不一定紧邻着 pos
					  2.当 pos 只有一个分支的时候，且pos->Child有两个子节点时，r 在 pos 右侧分支最左下
					  3.当 pos 紧邻r 且r 只有一个子节点的时候，那么 r 的高度变化，只会影响 r 子节点一路的节点

					  所以删除操作实际上是让 r 的高度降低了 1 个单位，只要找到r，并且使r得黑高度+1；
					  即可随后执行正常得高度操作；

	                所以调整的实质，是通过各种方式的染色、重构和调整 找到节点 r 得父亲 p（_hot)
					r 得兄弟节点 s ，来在 给 r 添加子节点的情况下（添加了之后 pos->succ() 就会变化）
					在不影响 pos 的拓扑结构的情况下（如果 pos 调整了拓扑结构，那么pos->succ() 也会发生变化）

					所以调整： 保证调整之后得 r 依然使 pos 得后继节点，且 r 的高度+1
					而着眼和调整的节点是：  r, p = r->parent ，s;

	双黑问题背景：  如果出现了双黑问题，那么 r：黑色，p：黑, s:红/黑；
	                非双黑问题： r/p 中有一个红

	双黑问题思路：  双黑问题的本质是通过调整节点，使得 r 分支的黑高度 + 1；
	                所以根据 s 节点的不同情况，来进行调整，使得 s 分支高度不变的同时；
					r 分支高度 + 1；

					(*.*).极限情况 pos为叶子节点，那么无需调整直接删除即可

					(0.0).非双黑情况，如果 r 为红色，无需任何操作,将 r 的颜色替换成 pos 的颜色

					(1.0).简单情况：r:黑，p:黑，如果 s 为黑色，且存在红色的孩子 s_child;
					      那么将 将 p,s,s_child 三者 进行旋转重构；
						  a.将 三个节点中 数值为中间大小的节点（只可能是 s 或者 s_child)
						    替换到 p 的位置，并继承p的颜色；
						  b.再将 另外一个节点 和 p 的颜色染为黑色,接入到 new_p 下方；
						    然后，接入 T0,T1,T2,T3（r 分支只可能是T0)

						  情况分析：调整高度是否合法？						  
						  此时 新的 new_p 节点颜色保持，s 分支 减去一个p：黑，
						  也得到一个 new_p :黑；
						  因此高度没有变化，
						  r 节点因为上面会都接入一个 黑色的 p 节点，高度会+1；
						  而非 r 的 另外三个分支（T0,T1,T2,T3 非r的另外三个分支)，
						  因为之前是接入在 黑s 或者 红 s_child+黑s中，黑高度是+1；
						  在调整后其依然会接入到 黑色的节点p或者 s_min 中，黑高度依然是+1不变）
						 
						  问题：调整之后 r 依然是 pos->succ() 吗？
						  回答：分两种情况讨论，p = pos的时候，以及p!=pos；
						  1.p = pos 的时候，那么 r 只会在 pos 的右侧，且只有一个孩子
						    因为 s 分支的所有数值都 都小于 pos ；r > pos 
							所以在上述调整中 r 只能是最大的 T3 依然接入到 p 下方，
							又因为 r 下的分支是没有变化的，依然是只有一个右孩子；
							所以 r = p->succ() 依然是成立的；

						  2.当 p 不等于 pos的时候，r 只会是 pos 的左子；那么调整后
						    只会是作为 T0 接入到 P 的下方，同时，P也只会是 new_p的
							左子，所以 r 依然是 pos->succ();
						    
							  
					(2.0).一般情况1：如果 s 为黑色，且没有红色的孩子；但节点 g 是红色的 
					      那么将 s 染红，再把 g 染黑；返回即可；
						  （情况分析：此时 s 分支的高度： u染红-1，g染黑+1  高度整体不变
						              此时 r 分支的高度： g染黑，高度+1，完成了调整）

				    (3.0).一般情况2：如果 s 为黑色，且没有红色孩子；且节点 g 黑色的
					      那么此时 将 s 染红，从而使得 s 分支 和 r 分支高度都整体下降了
						  1个单位；
						  然后再将p作为新的r向上继续传导和调整； (最多可能会调整 h 次结构）

				    (4.0).一般情况3:节点 s 为红色，那么 g 和 r 和 s 的子节点一定都为黑色
					
						 a. 那么将 节点 s 染黑,并且提升到 p 得上方，并将 p 染接入 s 得下方。
						    然后将 s 的一个 p 侧的子分支  s_p_child = (s.data > p.data) ? s->lChild : s->rChild;
						    将 s_p_child 染红 接入到 p 得下方；
						  
						    情况分析：s得两个分支，s_no_p_child 一直紧跟着 s, 失去了 p的黑高度，但是得到了
						              s染黑后的高度，所以高度不变；
									  同理 r分支 高度也没有变化
									  s_p_child 本身从黑色变成红色，但是因为接入了 p 所以高度也没有变化；
									  p 会被删除，无需关注他；
					     b.再执行一次调整判断，此时 调整的情景 已经变成了情况 2 ： s 为黑 且 g 为红；
						   进行相应的调整即可

						   问题分析：此次调整是否会影响拓扑结构？
						             r依然是 pos->succ() 吗？
						   回答：是的。还是分为 p = pos 以及  p!=pos 的情况讨论
						   1.p = pos 的时候：
						     那么 r 作为一条单独链，接入p的右侧；
							 调整后，pos接入s的右侧,pos下的结构未收到影响，所以 r 依然是 pos->succ()
						   2.p != pos 的时候
						     因为 r < p 是一定的，那么 s > p 也是一定的，
							 所以 p 会接入到 s 的左侧，r 依然是 p 下最 左路节点；
							 r 依然是 p->succ();

							
					#讨论： 节点 s 不存在的情况
					 首先节点 s 是有可能不存在的，我们也很容易构造出这种形式的红黑树；
					 在这个基础上，我们再区分两种情况：
					 1.r 和 pos 相连的时候，那么就继续向上归宿，pos = r，直到能找到 s 节点的时候；
					   再进行调整，把 pos-r 这边的分支高度增加1
					 2.r 和 pos 不相连接的时候，如果 s 不存在，那么就继续 用 p = r 向上归宿，直到能找到
					   s 或者到达根节点的时候。再进行调整
					  
					 总结：处理方法都是 向上寻找 有s的分支，来进行调整，使得 r 分支的高度+1；
					       如果以上两种到达根节点依然没有s 的话，说明是一条单链，、
						   可以直接进行删除 pos ，不会影响其他节点高度
					 分析：这种方式会影响pos和r的关系吗？ 
					       r 是否依然是 pos->succ()?
				     回答：依然是的，上述所有的调整方式，都不会影响 r 分支 和 p 分支的关系；
					       他们的关系是不会受到调整的变化的；
						   情况1.0已经分析过；
						   情况2.0没有调整拓扑结构
						   情况3.0没有调整拓扑结构
						   情况4.0也分析过，不会影响 r = p->succ();

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
	void link34_insert(bst_BinNodePosi<T>g, bst_BinNodePosi<T>p, bst_BinNodePosi<T>v,
		bst_BinNodePosi<T>T0,bst_BinNodePosi<T>T1, bst_BinNodePosi<T>T2, bst_BinNodePosi<T>T3);
	void link34_erase(bst_BinNodePosi<T>g, bst_BinNodePosi<T>p, bst_BinNodePosi<T>v, bst_BinNodePosi<T> gg, bst_BinNodePosi<T>T0,
		bst_BinNodePosi<T>T1, bst_BinNodePosi<T>T2, bst_BinNodePosi<T>T3);

	bool br_tree_judge(void(*f)(bst_BinNodePosi<T>, vector<bst_BinNodePosi<T>>));
	static void judge_node(bst_BinNodePosi<T> t, vector<bst_BinNodePosi<T>> vt);
	void judeg()
	{
		br_tree_judge(judge_node);
	}
	color get_root_c() { return bst_tree<T>::_root->c; };
	void out()
	{
		int _size = bst_tree<T>::_size;
		int high = 0;
		if (!_size||!bst_tree<T>::_root) return;
		
		queue<bst_BinNodePosi<T>> q1;
		q1.push(bst_tree<T>::_root);
		while (!q1.empty()) //没有给节点设置高度信息，只有受累遍历一遍获取高度了
		{
			high++;
			queue<bst_BinNodePosi<T>> q_line;
			while (!q1.empty())
			{
				auto temp = q1.front();
				q1.pop();
				q_line.push(temp);
			}
			while (!q_line.empty())
			{
				auto temp = q_line.front();
				q_line.pop();
				if (temp->lChild) q1.push(temp->lChild);
				if (temp->rChild) q1.push(temp->rChild);
			}
		}

		//空白符，控制每行宽度；每个单元格  |R：1| 8个字符 再加上 5个字符的留白
		int t = 1;
		high--;

		while (high)
		{
			t = t * 2;
			high--;
		}

		vector<bst_BinNodePosi<T>> vs(t * 2 + 1, NULL);//存储每个节点

		int rank = 0;
		vs[0] = bst_tree<T>::_root;

		while (rank < t * 2) //遍历节点,假设其是一个完全二叉树，那么总数量为 最底层节点数*2
		{
			if (vs[rank])
			{
				if (vs[rank]->lChild)
					vs[rank * 2 + 1] = vs[rank]->lChild;
				if (vs[rank]->rChild)
					vs[rank * 2 + 2] = vs[rank]->rChild;
			}
			rank++;
		}
		//所有节点已经存入

		//按行输出
		rank = 0;
		int sum = 0;
		int line = 0; //间隔
		for (int i = 1; i <= t; i =  i * 2) //每行 i 个单元格,t最底层行单元格数量
		{
			line = (t - i) * 8 / i / 2;  // 每行还有 （t-i）的空间可以进行分配，
			sum += i;  //单元格总个数，用来控制 rank
			
			while (rank < sum) //输出每行的内容
			{
				if (vs[rank]) //输出每个单元格
				{
					cout << string(line, ' ');
					stringstream ss;
					ss << " |";
					vs[rank]->c == RED ? ss << "R:" : ss << "B:";
					ss << vs[rank]->get_value();
					if (vs[rank]->get_value() < 10)
						cout << " ";
					ss << "| ";
					cout << ss.str();
					cout << string(line, ' ');
				}
				else
				{
					cout << string(line, ' ');
					cout << string(8,' ') ;
					cout << string(line, ' ');
				}
				rank++;
			}
			cout << endl;
		}
		cout << endl;
	}
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
2.pos为真,直接进入 doubleBlack 调整流程
3.执行实际删除操作
*/
template<typename T>
bool br_tree<T>::erase(const T& t)
{
	bst_BinNodePosi<T> pos = bst_tree<T>::search(t);
	if (!pos) return false; //未找到数据删除失败

	DoubleBlack(pos);
	bst_tree<T>::erase(t);
	if (bst_tree<T>::_root && bst_tree<T>::_root->c == RED) bst_tree<T>::_root->c = BLACK;
	return true;
}

template<typename T>
void br_tree<T>::DoubleBlack(bst_BinNodePosi<T> pos)
{
	bst_BinNodePosi<T> & _root = bst_tree<T>::_root;

	//极限情况：pos 为叶节点，无论红黑，都无需调整
	if (!pos->lChild&&!pos->rChild) return;
	//根节点情况，直接正常删除即可
	if (pos == _root && bst_tree<T>::_size==1) return;


	bst_BinNodePosi<T> r = pos->succ();
	// pos 不存在r的情况，那么pos，只有左孩，没有右边孩子；左边succ = pos->rChild;
	// r会直接顶替pos ,如果二者有红色，那么直接染成黑色即可。
	// 如果二者都是黑色，且单链，那么需要将pos->parent 作为pos 向上调整，给pos分支一个额外高度
	if(r==NULL)
	{
		r = pos->lChild;
		if (r->c == RED)
		{
			r->c = BLACK;
			return;
		}
		r = pos->parent;
		return;
	}
	bst_BinNodePosi<T> p = r->parent;
	bst_BinNodePosi<T> s = (r == p->lChild) ? p->rChild : p->lChild;

	//简单情况0：r为红
	//如果r 是红色的，那么他的挪走不会后继节点的高度，直接把他的颜色替换成 pos 的
	//颜色，使得他顶替pos后，树的高度不受影响即可；
	if (r->c == RED)
	{
		r->c = pos->c;
		return;
	}

	//s不存在的情况，继续向上归宿
	if (!s)
	{
		//继续向上归宿调整,如果遇到根节点，什么都不做，如果不是根，那么会继续调整
    	DoubleBlack(p);
		return;
	}

	if (s->c == BLACK)
	{
		color s_l_color = s->lChild ? s->lChild->c : BLACK;
		color s_r_color = s->rChild ? s->rChild->c : BLACK;
		//情况1.0 s 有红色孩子，对p颜色无要求
		if (s_l_color == RED || s_r_color == RED)
		{
			color p_color = p->c;
			bst_BinNodePosi<T> T0, T1, T2, T3;
			bst_BinNodePosi<T> t = (s_l_color == RED) ? s->lChild : s->rChild; //t是 s 的红子节点

			//类似AVL树调整的4种情况不同情况
			if (s == p->lChild)
			{
				if (t == s->lChild)
				{
					T0 = t->lChild;
					T1 = t->rChild;
					T2 = s->rChild;
					T3 = p->rChild;
					link34_erase(t, s, p, p->parent, T0, T1, T2, T3);
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
			return;
		}
		else if (p->c == RED) //情况2.0 p的颜色要求是红色，简单调整即可
		{
			s->c = RED;
			p->c = BLACK;
			return;
		}
		else //情况3.0 全体黑色， s染红，向上寻找解决途径
		{
			s->c = RED;
			DoubleBlack(p);
			return;
		}
	}
	else//情况4.0 s 颜色为红色，把 p 接到 s 下方，然后对节点 r 进行下一次调整
	{
		bst_BinNodePosi<T> g = (p) ? p->parent : NULL;

		// p = pos 的时候，s 在 p 的左边；否则 s 在 pos 的右边
		if (s == p->lChild)
		{
			bst_BinNodePosi<T> temp = s->rChild;

			s->rChild = p;
			s->parent = g ? g : NULL;
			s->c = BLACK;

			p->lChild = temp;
			p->parent = s;
			p->c = RED;

			if (temp) temp->parent = p;

			if (bst_tree<T>::_root == p)
				bst_tree<T>::_root = s;
		}
		else // p != pos 的时候，s 在 p 的右边
		{
			bst_BinNodePosi<T> temp = s->lChild;

			s->lChild = p;
			s->parent = g ? g : NULL;
			s->c = BLACK;

			p->rChild = temp;
			p->parent = s;
			p->c = RED;

			if (temp) temp->parent = p;

			if (bst_tree<T>::_root == p) bst_tree<T>::_root = s;
		}
		if (g)
		{
			if (p == g->lChild)
				g->lChild = s;
			else g->rChild = s;
		}
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
	bst_BinNodePosi<T> gg = g->parent;
	bst_BinNodePosi<T> new_parent;

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
			link34_insert(x, p, g, T0, T1, T2, T3);
			new_parent = p;
		}
		else
		{
			T0 = p->lChild;
			T1 = x->lChild;
			T2 = x->rChild;
			T3 = g->rChild;
			link34_insert(p, x, g, T0, T1, T2, T3);
			new_parent = x;
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
			link34_insert(g, p, x, T0, T1, T2, T3);
			new_parent = p;
		}
		else
		{
			T0 = g->lChild;
			T1 = x->lChild;
			T2 = x->rChild;
			T3 = p->rChild;
			link34_insert(g, x, p, T0, T1, T2, T3);
			new_parent = x;
		}
	}
	if (g == bst_tree<T>::_root)
	{
		bst_tree<T>::_root = new_parent;
		new_parent->parent = NULL;
	}
	if (gg)
	{
		if (g == gg->lChild) gg->lChild = new_parent;
		else gg->rChild = new_parent;
		new_parent->parent = gg;
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
void br_tree<T>::link34_insert(bst_BinNodePosi<T> lower_node, bst_BinNodePosi<T> new_parent, bst_BinNodePosi<T> higher_node,
	bst_BinNodePosi<T>T0, bst_BinNodePosi<T>T1, bst_BinNodePosi<T>T2, bst_BinNodePosi<T>T3)
{
	new_parent->lChild = lower_node;
	new_parent->rChild = higher_node;
	new_parent->c = BLACK;

	lower_node->lChild = T0;
	lower_node->rChild = T1;
	lower_node->parent = new_parent;
	lower_node->c = RED;

	higher_node->lChild = T2;
	higher_node->rChild = T3;
	higher_node->parent = new_parent;
	higher_node->c = RED;

	if (T0) T0->parent = lower_node;
	if (T1) T1->parent = lower_node;
	if (T2) T2->parent = higher_node;
	if (T3) T3->parent = higher_node;
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
