#pragma once

#include "bst_tree.h"

using namespace std;

/*
������������ص㣺
�������br_tree),���ڶ�����������װ��ʵ�֡�
�������4��ԭ��
��1�����ڵ��Ǻ�ɫ��
��2��ÿ�����ӽڵ㣨NIL���Ǻ�ɫ��
��3�����һ���ڵ��Ǻ�ɫ�ģ��������ӽڵ�����Ǻ�ɫ�ġ�
��4�������ⲿ��㵽��·���ϰ�����ͬ��Ŀ�ĺڽڵ㡣

�ص㣺
1.�������ص㣨3�����ƣ�ʹ�ú���������з�֧�߶����ƽ��ģ������1��������Ҳ�������ƫб����
2.��Ч�ļ�����logn),���루logn1+1),ɾ��(logn).ͬʱ�����AVL����˵����ɾ�������������˽ṹ��Ӱ��
  ��������ֻ��Ҫһ�����˽ṹ����
3.���⣺������ǻ��ڽڵ㡰�ڡ��߶Ƚ���ƽ��ģ���ɫ�Ľڵ������Ϊ�߶�Ϊ��0��

�����ͽӿڣ�
1.������� insert()
2.ɾ������ erase()
3.����������¿��ܳ��ֵġ�˫�����⡱�����ӿ� DoubleRed()
4.ɾ���������¿��ܳ��ֵġ�˫�����⡱�����ӿ� DoubleBlack()
5.3+4�ع��ӿ� p,g,T0,T1,T2,T3  link34()
6.�����������ɼ̳�

ʵ���߼���
1.����  �����ҵ�����ڵ�λ�� p��Ȼ�󽫽ڵ� x �Ժ�ɫ����ʽ������Ӱ����ԭ�ȸ߶ȵ���ʽ��
        ���뵽���У�
		Ȼ�����˫���⣬�鿴�ڵ�x���丸�ڵ��Ƿ��Ǻ�ɫ���Ӷ�Υ��ԭ��3��
		�����˫������������ DoubleRed ����

2.ɾ��  �����жϽڵ��Ƿ���ڣ�����������򷵻ء�������ڣ��򽫽ڵ� x ɾ����
		���������΢���ӵ�˫���ж�����

3.˫�����⴦������DoubleRed()  
	˫�������ʵ�ʣ�˫�������������丸�׽ڵ��Ǻ�ɫ������Υ��ԭ��3��
	                ��ô��Ҫͨ��������ɫ�ͽṹ���÷�֧�ĺڸ߶Ȳ���

	�������ı��������������˫�����⣬��ô˵�����ڵ� p Ϊ��ɫ��ͬ��Ҳ˵�� �游�ڵ� p һ��������Ϊ��ɫ��
	                ���Խ������ı������� x,f��g �����ڵ�����½��еģ�
	��������˼·��(1).����Ѱ�ҵ����ڵ�f���游�ڵ�g�Լ��鸸�ڵ������һ���ӽڵ� u�������Ϊ��ڵ㣬�����׵��ֵܣ�  
	                (2).��ڵ�g�ǹ�ע�Ľڵ㣬���ǵ����е�����Ŀ�Ķ��ǣ��ýڵ�g����������ӽڵ�ڸ߶Ȳ��䣻
					(3.1).�������x:��ɫ  f:��ɫ g:��ɫ �������Ѿ�ȷ����
					      ����ڵ� u �Ǻ�ɫ�ģ���ô���ǿ���ͨ����u��fͬʱȾ�ڣ�
						  Ȼ�� g ȾΪ��ɫ���Ӷ�ʹ��g�ڵ���������нڵ�ڸ߶Ȳ��䣻
						  ��������һ�㣬�ж� gg = g->parent; �� g �Ƿ����˫�����⣻
						  #������������gΪ���ڵ㣬��ô����ֱ����Ϊ��ɫ�����ҷ���

					(3.2).��΢����һ�������� x:��ɫ f:��ɫ g:��ɫ ��������ȷ��
					      �ҽڵ� u Ϊ��ɫ����ʱ���ͨ�� f/x Ⱦ�ڣ����ʹ��f��֧�߶�����1
						  ����û�а취�ֲ� u ��֧�ĸ߶ȣ�
						  ���ԣ���Ҫ�� g��f, x,�����ڵ�����ع���ʹ������AVL�����ع�������
						  ����С���еĽڵ��������Ϸ� ��ɺ�ɫ��ͬʱ�� g �� f �ڵ㶼��Ϊ�죻
						  ����Ϊf�ڵ�ԭ�����Ǻ�ɫ�ģ����� f ������һ����֧�ض��Ǻ�ɫ������
						    ֱ�ӽ����ɫ�ĸ��ڵ㣬�������жϣ�ͬ�� u Ҳ�Ǻ�ɫ�������ж�˫�죩
						  ����֮��f��֧�ĸ߶Ⱦ�ʵ�����ֽ�����һ����λ���ڸ߶�û�б仯��
						  �������ʵ�ʣ���ĳ����ڵ���뵽f��֧������Ϊg�㼶�Ľڵ�Ϊ��ɫ�ޱ仯
						                �Ӷ�ʹ��ԭg�ڵ����µĲ��ָ߶Ȳ��䣩
						  ��ֱ�ӷ��أ�

4.˫�����⴦���жϺ��� DoubleBlack()
	˫�������ʵ�ʣ�˫������ĳ��֣�����Ϊĳ����֧��ɾ�������¸÷�֧�ĸ߶ȼ���1��
	                ����ֻ��ͨ�����ַ�ʽ���ع� p ,g ʹ�� g �µĸ�����֧�߶Ȳ��䣻
					���ۺ͵����Ľڵ��� g��

	˫�����ⱳ����  ���������˫�����⣬��ô f����ɫ��g: ��/�ڣ�gg����/��,u:��/��

	˫������˼·��  ˫������ı�����ͨ�������ڵ㣬ʹ�� f ��֧�ĺڸ߶� + 1��
	                ���Ը��� �� �ڵ�Ĳ�ͬ����������е�����ʹ�� u��֧ �߶Ȳ����ͬʱ��
					f ��֧�߶� + 1��

					(1.0).���������� u Ϊ��ɫ���Ҵ��ں�ɫ�ĺ��� u_child;
					      ��ô�� �� g,u,u_child ���� ������ת�ع���
						  a.���ڵ�Ĵ�С��ϵ x>y>z �е� y �滻�� g ��λ�ã�ͬʱ��ɫ���䣻
						  b.���ڵ� x ����ɫ ȾΪ ��ɫ�����ڵ� u_child ҲȾΪ��ɫ��
						 (�����������ʱ �µ�g�ڵ���ɫû�б仯��u��֧ ��ȥһ���ڣ��õ�һ����
						  �߶�û�б仯��f��֧���ӵ����µĺ�ɫ�ڵ��£��Ӷ�ʹ�ø߶�+1���Ӷ��γ�
						  �Ϸ��ĺ������
						 (���ع���ʽ���Ƿ�Ϸ�������
						  T0<T1<T2<T3,���ĸ����ع��ķ�֧����Ϊ���ᱻ�ӵ���ɫ�Ľڵ��ϣ�����
						  �����Ǻ��Ǻڣ�����������˫��Ĳ��Ϸ����������ֱ�ӷ��أ�



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

	void DoubleRed_R1(bst_BinNodePosi<T> x);  // ����ڵ�uΪ��ɫ��������
	void DoubleRed_R2(bst_BinNodePosi<T> x); // ����ڵ�uΪ��ɫ��������
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
�������⣺
1.���ڵ��Ƿ��Ǻ�ɫ��
2.������ɫ�ڵ��Ƿ�������
3.ÿ��Ҷ�ڵ�ĺڸ߶��Ƿ�һ�£�

˼·��
1.�����ڵ���ɫ  
2.�����������ĳ���ڵ��Ǻ�ɫ�����ж��Ƿ��к�ɫ�ĸ�ĸ���ӽڵ�
3.�������ҵ����и��ڵ� ��l->child=NULL r->child = NULL��
  ������һ�����ڵ㵽 ���׵ĺھ����Ϊ��black_height;
  ���ұ��������ڵ㣬����ʱ�򣬾��м�¼������

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
����һ������vt��
����ڵ���judge_node���ж�1��2�����Ϊ���ڵ㣬�ʹ������飬
ͳһ�ж�3
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
			cout << "���ڵ�ڸ߶� ��һ��" << " ������Ϊ " << x->get_value() << endl;
			system("pause");
			exit(1);
		}
	}

	return 1;
}


/*
ɾ���߼���
1.����seach �� pos=search(t);
2.posΪ��
  r=succ(pos),��r��pos �����滻,��ɾ��pos���ݣ�
  s = (p==_hot->lChild)? _hot->rChild:_hot->lChild;
  p_color = p.c;
  r_color = r ? r->c RED;
  s_color = s.c;

3.����pos,r,s��ɫ����ʶ�𣬽�����Ӧ�ĵ���
  ���pos����r ����һ��Ϊ��ɫ,if(r) r->c = RED ���ɣ�
  ���˫��ɫ�������˫�ڴ������̣�������Ԫ������ɫ

4.ִ��ʵ��ɾ������

*/
template<typename T>
bool br_tree<T>::erase(const T& t)
{
	bst_BinNodePosi<T> pos = bst_tree<T>::search(t);
	if (!pos) return false; //δ�ҵ�����ɾ��ʧ��

	bst_BinNodePosi<T> r = pos->succ(); //replacement �����
	bst_BinNodePosi<T> p = pos->parent;
	color pos_color, r_color;
	pos_color = pos->c;
	r_color = r ? r->c : BLACK;
	
	if (pos_color == RED || r_color == RED) //����� pos �� r ����һ��Ϊ��ɫ��ɾ���󽫺�̣��������Ϊ��ɫ���ɣ���ɾ����߶Ȳ���
	{
		bst_tree<T>::erase(t);
		if(r) r->c = RED;
	}
	else if (bst_tree<T>::size <= 3) //˫�����⣬��С�ڵ���3
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
	//���������ɾ���ڵ��Ǹ��ڵ� ��ôpos=pos->succ();
	if (pos == bst_tree<T>::_root)
		pos = pos->succ();

	//bst_BinNodePosi<T> r = pos->succ();
	bst_BinNodePosi<T> p = pos->parent;
	bst_BinNodePosi<T> s;
	if (!p) s = NULL;
	else s = (pos == p->lChild) ? p->rChild : p->lChild;
	//�������s��������ڣ�˵�����̽��뵽�� t->succ() ����
	//��ô
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
		//����1.SΪ�ڣ���S�ú����к�ɫ��
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
		//SΪ�ڣ���S�ú���û�к�ɫ
		else if (s_l_color == BLACK && s_r_color == BLACK)
		{
			//����2.SΪ�ڣ�S�ӽڵ㶼�ڣ�PΪ��
			if (p->c == RED)
			{
				s->c = RED;
				p->c = BLACK;
			}
			//����3��SΪ�ڣ�S�ӽڵ㶼�ڣ�PΪ��
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
	//���4 SΪ�죬����Ϊ��ɫ
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

��������߼���

���Ƚ���search ���������pos;

#������� �������ڽڵ㣬_hot = NULL; ��ֱ��new һ����ɫ�ڵ���Ϊ���ڵ�#

pos Ϊ�棬�ڵ��Ѿ����ڣ��򷵻أ�pos = Null ��ڵ㲻���ڣ���_hot���в��룺
1.new һ���µĽڵ㣨Ĭ��Ϊ�죩���в��룬��_hot���棻
2.����˫����
3.�����²���Ľڵ�

*/
template<typename T>
bst_BinNodePosi<T> br_tree<T>::insert(const T&t)
{
	//������� ��Ϊ��
	if (!bst_tree<T>::_root)
	{
		bst_tree<T>::size++;
		bst_tree<T>::_root = new bst_BinNode<T>(t, NULL, BLACK);
		return bst_tree<T>::_root;
	}

	bst_BinNodePosi<T>  pos = bst_tree<T>::search(t);
	if (pos) return pos; //�ڵ����

	pos = new bst_BinNode<T>(t, bst_tree<T>::_hot, RED);
	if (pos->get_value() > bst_tree<T>::_hot->get_value())
		bst_tree<T>::_hot->rChild = pos;
	else bst_tree<T>::_hot->lChild = pos;
	bst_tree<T>::size++;
	DoubleRed(pos); //˫���жϲ���������
	return pos;
}

/*

˫�����������̣�
#������� 
1.��� p = x->parent if(p->col==RED) �����˫���ų�����
2.˫���ų�������ʽ��ʼ��
	 g = p->parent;
     u = g->child;
2.1 u->color == BLACK ; 
    ��x,p,g ����3+4�ع�
	pΪ��ɫ��x,gΪ��
2.2 u->color == RED;
    ��x,p,g,u ��Ϊһ�������ڵ㣬��gȾ����������һ��
	��p��uȾ�ڼ���
	������׷�٣�x=g��
	if x==_root;����x Ϊ�ڡ�
	else ѭ������˫����
	ֱ�� x == NULL����
*/

template<typename T>
void br_tree<T>::DoubleRed(bst_BinNodePosi<T> x)
{
	bst_BinNodePosi<T> p = x->parent;  //���账�����
	if (p->c == BLACK) return;

	bst_BinNodePosi<T> g = p->parent;
	bst_BinNodePosi<T> u = (p == g->lChild) ? g->rChild : g->lChild;
	color u_c;
	if (u) u_c = u->c;
	else u_c = BLACK;

	if (u_c == BLACK) DoubleRed_R1(x); // ˫�������һ�������ͨ���ع��ָ�u�ڵ�һ����ɫ�Ľڵ�
	else DoubleRed_R2(x); // ˫��������������p��u��Ⱦ�ڣ�g��ɺ죬�������ϼ����ж�
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
		//link34_insert ˵���� �� x>y>z �����ڵ㴫�룻
		//x��z�ӵ� y �·�������Ϊ��ɫ�� y Ϊ�ڣ����� gg �ڵ㴫��
		//ʹ��gg ָ���µ�����Ľڵ�
		// T0<T1<T2<T3 ,�ֱ��� �ڵ�x , p�� g �����ⲿ���ӽڵ㣻
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

	//������� g == _root;
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
	//������� g Ϊ_root
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

	//������� g Ϊ_root
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