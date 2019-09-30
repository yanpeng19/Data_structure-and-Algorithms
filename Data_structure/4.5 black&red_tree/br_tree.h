#pragma once

#include "bst_tree.h"
#include <queue>
#include <math.h>
#include <sstream>

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
3.��⣺������ǻ��ڽڵ㡰�ڡ��߶Ƚ���ƽ��ģ���ɫ�Ľڵ������Ϊ�߶�Ϊ��0��

�����ͽӿڣ�
1.������� insert()
2.ɾ������ erase()
3.����������¿��ܳ��ֵġ�˫�����⡱�����ӿ� DoubleRed()
4.ɾ���������¿��ܳ��ֵġ�˫�����⡱�����ӿ� DoubleBlack()
5.3+4�ع��ӿ� p,g,T0,T1,T2,T3  link34()
6.�����������ɼ̳�

ʵ���߼���
1.����  �����ҵ�����ڵ�λ�� pos��Ȼ�󽫽ڵ� x �Ժ�ɫ����ʽ������Ӱ����ԭ�ȸ߶ȵ���ʽ��
        ���뵽���У�
		Ȼ�����˫���⣬�鿴�ڵ�x���丸�ڵ��Ƿ��Ǻ�ɫ���Ӷ�Υ��ԭ��3��
		�����˫������������ DoubleRed ����

2.ɾ��  �����жϽڵ��Ƿ���ڣ�����������򷵻ء�������ڣ������˫���жϵ�������
        ��������������ɾ�������� r �̳� p ����ɫ���ɡ�
		

3.˫�����⴦����DoubleRed()  
	˫�������ʵ�ʣ�˫�������������丸�׽ڵ��Ǻ�ɫ������Υ��ԭ��3��
	                ��ô��Ҫͨ��������ɫ�ͽṹ���÷�֧�ĺڸ߶Ȳ���

	�������ı��������������˫�����⣬��ô˵��ɾ���� f Ϊ��ɫ��ͬ��Ҳ˵�� �游�ڵ� g һ��������Ϊ��ɫ��
	                ���Խ������ı������� x ,f��g �����ڵ�����½��еģ�

	��������˼·��(1).����Ѱ�ҵ����ڵ�f���游�ڵ�g�Լ��鸸�ڵ������һ���ӽڵ� s�������Ϊ��ڵ㣬�����׵��ֵܣ�  
	                (2).��ڵ�g�ǹ�ע�Ľڵ㣬���ǵ����е�����Ŀ�Ķ��ǣ��ýڵ�g����������ӽڵ�ڸ߶Ȳ��䣻
					(3.1).�������x:��ɫ  f:��ɫ g:��ɫ �������Ѿ�ȷ����
					      ����ڵ� s �Ǻ�ɫ�ģ���ô���ǿ���ͨ����s��fͬʱȾ�ڣ�
						  Ȼ�� g ȾΪ��ɫ���Ӷ�ʹ��g�ڵ���������нڵ�ڸ߶Ȳ��䣻
						  ��������һ�㣬�ж� gg = g->parent; �� g �Ƿ����˫�����⣻
						  #������������gΪ���ڵ㣬��ô����ֱ����Ϊ��ɫ�����ҷ���

					(3.2).��΢����һ�������� x:��ɫ f:��ɫ g:��ɫ ��������ȷ��
					      �ҽڵ� s Ϊ��ɫ����ʱ���ͨ�� f/x Ⱦ�ڣ����ʹ��f��֧�߶�����1
						  ����û�а취�ֲ� s ��֧�ĸ߶ȣ�
						  ���ԣ���Ҫ�� g��f, x,�����ڵ�����ع���ʹ������AVL�����ع�������
						  ����С���еĽڵ��������Ϸ� ��ɺ�ɫ��ͬʱ�� g �� f �ڵ㶼��Ϊ�죻
						  ����Ϊf�ڵ�ԭ�����Ǻ�ɫ�ģ����� f ������һ����֧�ض��Ǻ�ɫ������
						    ֱ�ӽ����ɫ�ĸ��ڵ㣬�������жϣ�ͬ�� s Ҳ�Ǻ�ɫ�������ж�˫�죩
						  ����֮��f��֧�ĸ߶Ⱦ�ʵ�����ֽ�����һ����λ���ڸ߶�û�б仯��
						  �������ʵ�ʣ���ĳ����ڵ���뵽f��֧������Ϊg�㼶�Ľڵ�Ϊ��ɫ�ޱ仯
						                �Ӷ�ʹ��ԭg�ڵ����µĲ��ָ߶Ȳ��䣩
						  ��ֱ�ӷ��أ�

4.˫�����⴦���жϺ��� DoubleBlack()
	˫�������ʵ�ʣ�˫������ĳ��֣�����Ϊɾ���ڵ� pos �ĺ�� r = succ(pos)�� �Լ� r �ĸ��� p
	                ���Ǻ�ɫ���Ӷ�����ɾ����� r ��֧ʵ�ʸ߶� -1��

					�����︴ϰһ�½ڵ� r �ĸ�� 
					  1.�� pos �������ڵ��ʱ��r ��һ�������� pos
					  2.�� pos ֻ��һ����֧��ʱ����pos->Child�������ӽڵ�ʱ��r �� pos �Ҳ��֧������
					  3.�� pos ����r ��r ֻ��һ���ӽڵ��ʱ����ô r �ĸ߶ȱ仯��ֻ��Ӱ�� r �ӽڵ�һ·�Ľڵ�

					  ����ɾ������ʵ�������� r �ĸ߶Ƚ����� 1 ����λ��ֻҪ�ҵ�r������ʹr�úڸ߶�+1��
					  �������ִ�������ø߶Ȳ�����

	                ���Ե�����ʵ�ʣ���ͨ�����ַ�ʽ��Ⱦɫ���ع��͵��� �ҵ��ڵ� r �ø��� p��_hot)
					r ���ֵܽڵ� s ������ �� r ����ӽڵ������£������֮�� pos->succ() �ͻ�仯��
					�ڲ�Ӱ�� pos �����˽ṹ������£���� pos ���������˽ṹ����ôpos->succ() Ҳ�ᷢ���仯��

					���Ե����� ��֤����֮��� r ��Ȼʹ pos �ú�̽ڵ㣬�� r �ĸ߶�+1
					�����ۺ͵����Ľڵ��ǣ�  r, p = r->parent ��s;

	˫�����ⱳ����  ���������˫�����⣬��ô r����ɫ��p����, s:��/�ڣ�
	                ��˫�����⣺ r/p ����һ����

	˫������˼·��  ˫������ı�����ͨ�������ڵ㣬ʹ�� r ��֧�ĺڸ߶� + 1��
	                ���Ը��� s �ڵ�Ĳ�ͬ����������е�����ʹ�� s ��֧�߶Ȳ����ͬʱ��
					r ��֧�߶� + 1��

					(*.*).������� posΪҶ�ӽڵ㣬��ô�������ֱ��ɾ������

					(0.0).��˫���������� r Ϊ��ɫ�������κβ���,�� r ����ɫ�滻�� pos ����ɫ

					(1.0).�������r:�ڣ�p:�ڣ���� s Ϊ��ɫ���Ҵ��ں�ɫ�ĺ��� s_child;
					      ��ô�� �� p,s,s_child ���� ������ת�ع���
						  a.�� �����ڵ��� ��ֵΪ�м��С�Ľڵ㣨ֻ������ s ���� s_child)
						    �滻�� p ��λ�ã����̳�p����ɫ��
						  b.�ٽ� ����һ���ڵ� �� p ����ɫȾΪ��ɫ,���뵽 new_p �·���
						    Ȼ�󣬽��� T0,T1,T2,T3��r ��ֻ֧������T0)

						  ��������������߶��Ƿ�Ϸ���						  
						  ��ʱ �µ� new_p �ڵ���ɫ���֣�s ��֧ ��ȥһ��p���ڣ�
						  Ҳ�õ�һ�� new_p :�ڣ�
						  ��˸߶�û�б仯��
						  r �ڵ���Ϊ����ᶼ����һ�� ��ɫ�� p �ڵ㣬�߶Ȼ�+1��
						  ���� r �� ����������֧��T0,T1,T2,T3 ��r������������֧)��
						  ��Ϊ֮ǰ�ǽ����� ��s ���� �� s_child+��s�У��ڸ߶���+1��
						  �ڵ���������Ȼ����뵽 ��ɫ�Ľڵ�p���� s_min �У��ڸ߶���Ȼ��+1���䣩
						 
						  ���⣺����֮�� r ��Ȼ�� pos->succ() ��
						  �ش𣺷�����������ۣ�p = pos��ʱ���Լ�p!=pos��
						  1.p = pos ��ʱ����ô r ֻ���� pos ���Ҳ࣬��ֻ��һ������
						    ��Ϊ s ��֧��������ֵ�� ��С�� pos ��r > pos 
							���������������� r ֻ�������� T3 ��Ȼ���뵽 p �·���
							����Ϊ r �µķ�֧��û�б仯�ģ���Ȼ��ֻ��һ���Һ��ӣ�
							���� r = p->succ() ��Ȼ�ǳ����ģ�

						  2.�� p ������ pos��ʱ��r ֻ���� pos �����ӣ���ô������
						    ֻ������Ϊ T0 ���뵽 P ���·���ͬʱ��PҲֻ���� new_p��
							���ӣ����� r ��Ȼ�� pos->succ();
						    
							  
					(2.0).һ�����1����� s Ϊ��ɫ����û�к�ɫ�ĺ��ӣ����ڵ� g �Ǻ�ɫ�� 
					      ��ô�� s Ⱦ�죬�ٰ� g Ⱦ�ڣ����ؼ��ɣ�
						  �������������ʱ s ��֧�ĸ߶ȣ� uȾ��-1��gȾ��+1  �߶����岻��
						              ��ʱ r ��֧�ĸ߶ȣ� gȾ�ڣ��߶�+1������˵�����

				    (3.0).һ�����2����� s Ϊ��ɫ����û�к�ɫ���ӣ��ҽڵ� g ��ɫ��
					      ��ô��ʱ �� s Ⱦ�죬�Ӷ�ʹ�� s ��֧ �� r ��֧�߶ȶ������½���
						  1����λ��
						  Ȼ���ٽ�p��Ϊ�µ�r���ϼ��������͵����� (�����ܻ���� h �νṹ��

				    (4.0).һ�����3:�ڵ� s Ϊ��ɫ����ô g �� r �� s ���ӽڵ�һ����Ϊ��ɫ
					
						 a. ��ô�� �ڵ� s Ⱦ��,���������� p ���Ϸ������� p Ⱦ���� s ���·���
						    Ȼ�� s ��һ�� p ����ӷ�֧  s_p_child = (s.data > p.data) ? s->lChild : s->rChild;
						    �� s_p_child Ⱦ�� ���뵽 p ���·���
						  
						    ���������s��������֧��s_no_p_child һֱ������ s, ʧȥ�� p�ĺڸ߶ȣ����ǵõ���
						              sȾ�ں�ĸ߶ȣ����Ը߶Ȳ��䣻
									  ͬ�� r��֧ �߶�Ҳû�б仯
									  s_p_child ����Ӻ�ɫ��ɺ�ɫ��������Ϊ������ p ���Ը߶�Ҳû�б仯��
									  p �ᱻɾ���������ע����
					     b.��ִ��һ�ε����жϣ���ʱ �������龰 �Ѿ��������� 2 �� s Ϊ�� �� g Ϊ�죻
						   ������Ӧ�ĵ�������

						   ����������˴ε����Ƿ��Ӱ�����˽ṹ��
						             r��Ȼ�� pos->succ() ��
						   �ش��ǵġ����Ƿ�Ϊ p = pos �Լ�  p!=pos ���������
						   1.p = pos ��ʱ��
						     ��ô r ��Ϊһ��������������p���Ҳࣻ
							 ������pos����s���Ҳ�,pos�µĽṹδ�յ�Ӱ�죬���� r ��Ȼ�� pos->succ()
						   2.p != pos ��ʱ��
						     ��Ϊ r < p ��һ���ģ���ô s > p Ҳ��һ���ģ�
							 ���� p ����뵽 s ����࣬r ��Ȼ�� p ���� ��·�ڵ㣻
							 r ��Ȼ�� p->succ();

							
					#���ۣ� �ڵ� s �����ڵ����
					 ���Ƚڵ� s ���п��ܲ����ڵģ�����Ҳ�����׹����������ʽ�ĺ������
					 ����������ϣ��������������������
					 1.r �� pos ������ʱ����ô�ͼ������Ϲ��ޣ�pos = r��ֱ�����ҵ� s �ڵ��ʱ��
					   �ٽ��е������� pos-r ��ߵķ�֧�߶�����1
					 2.r �� pos �������ӵ�ʱ����� s �����ڣ���ô�ͼ��� �� p = r ���Ϲ��ޣ�ֱ�����ҵ�
					   s ���ߵ�����ڵ��ʱ���ٽ��е���
					  
					 �ܽ᣺���������� ����Ѱ�� ��s�ķ�֧�������е�����ʹ�� r ��֧�ĸ߶�+1��
					       ����������ֵ�����ڵ���Ȼû��s �Ļ���˵����һ����������
						   ����ֱ�ӽ���ɾ�� pos ������Ӱ�������ڵ�߶�
					 ���������ַ�ʽ��Ӱ��pos��r�Ĺ�ϵ�� 
					       r �Ƿ���Ȼ�� pos->succ()?
				     �ش���Ȼ�ǵģ��������еĵ�����ʽ��������Ӱ�� r ��֧ �� p ��֧�Ĺ�ϵ��
					       ���ǵĹ�ϵ�ǲ����ܵ������ı仯�ģ�
						   ���1.0�Ѿ���������
						   ���2.0û�е������˽ṹ
						   ���3.0û�е������˽ṹ
						   ���4.0Ҳ������������Ӱ�� r = p->succ();

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

	void DoubleRed_R1(bst_BinNodePosi<T> x);  // ����ڵ�uΪ��ɫ������
	void DoubleRed_R2(bst_BinNodePosi<T> x); // ����ڵ�uΪ��ɫ������
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
		while (!q1.empty()) //û�и��ڵ����ø߶���Ϣ��ֻ�����۱���һ���ȡ�߶���
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

		//�հ׷�������ÿ�п�ȣ�ÿ����Ԫ��  |R��1| 8���ַ� �ټ��� 5���ַ�������
		int t = 1;
		high--;

		while (high)
		{
			t = t * 2;
			high--;
		}

		vector<bst_BinNodePosi<T>> vs(t * 2 + 1, NULL);//�洢ÿ���ڵ�

		int rank = 0;
		vs[0] = bst_tree<T>::_root;

		while (rank < t * 2) //�����ڵ�,��������һ����ȫ����������ô������Ϊ ��ײ�ڵ���*2
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
		//���нڵ��Ѿ�����

		//�������
		rank = 0;
		int sum = 0;
		int line = 0; //���
		for (int i = 1; i <= t; i =  i * 2) //ÿ�� i ����Ԫ��,t��ײ��е�Ԫ������
		{
			line = (t - i) * 8 / i / 2;  // ÿ�л��� ��t-i���Ŀռ���Խ��з��䣬
			sum += i;  //��Ԫ���ܸ������������� rank
			
			while (rank < sum) //���ÿ�е�����
			{
				if (vs[rank]) //���ÿ����Ԫ��
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
2.posΪ��,ֱ�ӽ��� doubleBlack ��������
3.ִ��ʵ��ɾ������
*/
template<typename T>
bool br_tree<T>::erase(const T& t)
{
	bst_BinNodePosi<T> pos = bst_tree<T>::search(t);
	if (!pos) return false; //δ�ҵ�����ɾ��ʧ��

	DoubleBlack(pos);
	bst_tree<T>::erase(t);
	if (bst_tree<T>::_root && bst_tree<T>::_root->c == RED) bst_tree<T>::_root->c = BLACK;
	return true;
}

template<typename T>
void br_tree<T>::DoubleBlack(bst_BinNodePosi<T> pos)
{
	bst_BinNodePosi<T> & _root = bst_tree<T>::_root;

	//���������pos ΪҶ�ڵ㣬���ۺ�ڣ����������
	if (!pos->lChild&&!pos->rChild) return;
	//���ڵ������ֱ������ɾ������
	if (pos == _root && bst_tree<T>::_size==1) return;


	bst_BinNodePosi<T> r = pos->succ();
	// pos ������r���������ôpos��ֻ���󺢣�û���ұߺ��ӣ����succ = pos->rChild;
	// r��ֱ�Ӷ���pos ,��������к�ɫ����ôֱ��Ⱦ�ɺ�ɫ���ɡ�
	// ������߶��Ǻ�ɫ���ҵ�������ô��Ҫ��pos->parent ��Ϊpos ���ϵ�������pos��֧һ������߶�
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

	//�����0��rΪ��
	//���r �Ǻ�ɫ�ģ���ô����Ų�߲����̽ڵ�ĸ߶ȣ�ֱ�Ӱ�������ɫ�滻�� pos ��
	//��ɫ��ʹ��������pos�����ĸ߶Ȳ���Ӱ�켴�ɣ�
	if (r->c == RED)
	{
		r->c = pos->c;
		return;
	}

	//s�����ڵ�������������Ϲ���
	if (!s)
	{
		//�������Ϲ��޵���,����������ڵ㣬ʲô��������������Ǹ�����ô���������
    	DoubleBlack(p);
		return;
	}

	if (s->c == BLACK)
	{
		color s_l_color = s->lChild ? s->lChild->c : BLACK;
		color s_r_color = s->rChild ? s->rChild->c : BLACK;
		//���1.0 s �к�ɫ���ӣ���p��ɫ��Ҫ��
		if (s_l_color == RED || s_r_color == RED)
		{
			color p_color = p->c;
			bst_BinNodePosi<T> T0, T1, T2, T3;
			bst_BinNodePosi<T> t = (s_l_color == RED) ? s->lChild : s->rChild; //t�� s �ĺ��ӽڵ�

			//����AVL��������4�������ͬ���
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
		else if (p->c == RED) //���2.0 p����ɫҪ���Ǻ�ɫ���򵥵�������
		{
			s->c = RED;
			p->c = BLACK;
			return;
		}
		else //���3.0 ȫ���ɫ�� sȾ�죬����Ѱ�ҽ��;��
		{
			s->c = RED;
			DoubleBlack(p);
			return;
		}
	}
	else//���4.0 s ��ɫΪ��ɫ���� p �ӵ� s �·���Ȼ��Խڵ� r ������һ�ε���
	{
		bst_BinNodePosi<T> g = (p) ? p->parent : NULL;

		// p = pos ��ʱ��s �� p ����ߣ����� s �� pos ���ұ�
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
		else // p != pos ��ʱ��s �� p ���ұ�
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
	DoubleRed(pos); //˫���жϲ�������
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
	bst_BinNodePosi<T> gg = g->parent;
	bst_BinNodePosi<T> new_parent;

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
