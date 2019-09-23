/*
	B�� - b_tree
	-----------------------------------------------------------------------------
	0.���B����һ����Ȼ�����νṹ��֯�Ĳ�����������
			����ڶ�������AVL���ȵȣ�B����ע���Ż��Ĳ��֣���IO���֣������ݶ�ȡ
			����Ӧ�ó����Ļ������ݿ��Լ�������֯�õĽ϶�

	1.��֯��ʽ�� B�������ݵ�λ BT_Node ��һ����ͨ�ڵ����������BT_Node = vecotor<Node*>
				 ���Խ� BT_Node ������һ�������ڵ㡣
				 ��ÿ�������ڵ����棬���洢�� N ���ڵ㣨֮����order��ʾn�Ĵ�С����
				 ��ӦҲ�洢��N+1���ӽڵ���Ϣ��

				 (order = 5��
				 key :                 2              5               6             8     
				 child:    BT_Node* a     BT_Node* b      BT_Node*  c    BT_Node* d    BT_Node* e

				 Ҳ��ͨ��������ʽ��B����ʵ��һ�λ�ȡ����ڵ�����ݣ�������IO�Ĳ���

				 BT_Node �ڵ�����ݣ�
				 {
					vector<T> key;						 //�洢�����ݵ�ֵ
					vector<BT_Node*<T>> child;			 //�洢�����ݵ��ӽڵ���Ϣ
					BT_Node<T>* parent;					 //�洢�Ÿ��ڵ����Ϣ
				 }

				 B���ڵ�Ĺ���
				 1. key.size() <= order ÿ���ڵ�����ݲ��ܳ��� roder-1������������ˣ��ͷ���
				                        �ڵ㷢�����������Ҫ�Խڵ���д���
				 2. key.size() > (order-1)/2 Ϊ�˱�������Ч�ʣ�Ҫ��ÿ���ڵ�Ҫ���� (order-1)/2��
											 ������㣬�������������ͬ��Ҫ���д���

	2.�����͹��ܣ�
		2.1 search()            // ���Ҳ��� 
		2.1 insert()            // �������
		2.2 erase()             // ɾ��������
		2.3 solveOverflow()     // ��鲢�������뵼�µģ��ڵ����磺
		2.4 solveUnderflow()    // ��鲢����ɾ�����µģ��ڵ����磺
		
	3.ʵ���߼�
		3.0 ���� search(const T& t)      (1).�Ӹ��ڵ㿪ʼ������ʹ�ö��ֲ��ң�ȷ�Ͻڵ�λ�ã�����ڵ��ڸò���ڣ���ֱ�ӷ���
											 ���������������� key.search() �᷵�����һ�������� t �Ľڵ��±� i;
											 ���ǽ��� child[i+1] �е�����
											 ����������������� Ѱ�� 3�� i = 0�� ���ǵ�Ŀ���� BT_Node* b ������i֮��ķ�֧
											 ���Խ��� BT_Node* b;
										 (2).ѭ�����ϳ�����ֱ�������ⲿ�ڵ㣬��ô�÷���NULL��
										 (*).α���룺
											 search(t)
											 {
												_hot = NULL; //ͬ���Ǵ游�ڵ㣬����֮����в���
												if(!BT_Node* x = _root) return NULL;

												while(x)
												{
													int i = key.search(t); // key �в���
													if(key[i]==t) return key[i];   //�ҵ���
													_hot = x;
													x = child[i+1];  //������һ��
												}
												return NULL;  //��������Ѿ������ⲿ�ڵ���
											 }

		3.1 ���� insert(const T& t)       (1).���в��Ҳ��� BT_Node* p = search(t);
										  (2.0) ���ҳɹ������д��ڸ�Ԫ�أ��������ӽڵ��������ʲô������
										  (2.1) ����ʧ�ܣ���ʱ _hot Ϊ ���һ��С�� t �ĳ����ڵ㣬��ô��
												_hot�� key �еĺ���λ�ò��� t;
										  (3) �ж�_hot�ڵ��Ƿ���Ϊ�²���Ԫ�أ����������磬��������������
											  ��ô��ʹ�ú���������
										  (*)α���룺
										  inser(const T&t)
										  {
											 //��Ϊ�գ���ֱ�ӽ��в���
											 using rank = int;
											 if(!_root)
											 {
												_root = new BT_Node(t);
												_size++;
												return _root.key[0];
											 }
											 //һ�����
											 BT_Node x = search(t);
											 if(x) return x;  //�ҵ��ˣ��ͷ��ػ����Զ���һЩ����
											 rank i = _hot.key.search(t); //�����ҵ����ʵ�λ�ã�����t
											 _hot.key.search(i+1,t);  
											 _hot.child.push_back(NULL);  //_hot ������±ض���Ҷ�ڵ㣬Ҷ�ڵ�����һ�������ⲿ�ڵ㣬
																			     ��ֱ������NULL��
											 solveOverflow(_hot);  //����Ƿ����������������ĺ���
										  }

		3.2 ��鲢�������� solveOverflow()		(1). ����ڵ� X �� x->size() >= _order;��ô�÷������������
												(2). ����Ѱ�Ҹýڵ���м�λ�õ�Ԫ�أ� mi =  key.size()/2;
													 �����ڵ��Ϊ�����½ڵ㣺
													 
													 ԭ���Ľڵ㲿�֣� key[0]-k[mi-1] ����
													 �����ڵ㣺 BT_Node new = BT_Node( key[mi+1]~key[key.size()-1])
													 
													 ����Ӧ��Ҳ�����ǵ� �����ָ��� ����Chlid ���з��ѣ� 
													 ����:  Child[0~mi]
													 �Ұ�ߣ� Child[mi+1~Child.size()-1] �����������ݸ� new;

												(3). ��key[mi] ����������һ���У�����  parent = x->parnet 
													 �� parent->key ����λ�ò��� key[mi]
													 ����λ�õ�Ϊ��insert_posi = parnet.search(key[mi]);
													 parent.key.insert(insert_posi+1,key[mi];   //����key[mi] �����ڵ���

													 //Ϊʲô����λ���� insert_posi+1 ? ��Ϊ�Ҷ���Ĳ��Һ������ص��� �����ڲ���ֵ�Ľڵ�RANK
													 //�����Զ���Ĳ������ ���� RANK ��λ���� ֱ�Ӳ����
													 // ���� rank = 0�� insret(0,new) ,��ô�ͻ��� key[0] ���� new
													 //������⣬�ش�����

													 ͬʱ��Ӧ��Ҳ�� paretn.Child ������Ӧ�Ĳ�����ѳ����ķ�֧��
													 parent->Child.insert( insert_posi+2,new)

													 // Ϊʲô �²���ڵ�new���û�н��� �ӽڵ���룿
													 // ��Ϊ parent->key[insert_posi+1] == key[mi]
													 // ��ô paretn->child[insert_posi+1] ����С�ڵ��� key[mi] ��֧Ҳ���� ԭ���� x �ڵ�
													 // ���������������

													 * ������� 1�� x-parnet �����ڣ�˵�� x �Ǹ��ڵ�
													 * ��ʱ���� �� _root = new BT_Node(key[mi],x,new);
													 * ����һ���½ڵ���� key[mi] ������ x �� new
												(4). ����new_node->child����������ָ��new_node
													 һ�����������ײ���в�������ģ����ǣ�����������ϴ���֮���������һ��ͬ���������������
													 ��ô��Ҫ�����·��ѳ����Ľڵ㣬������ָ�� new_node
												(5).��Ϊ�ڵ�������ܻᵼ�£����ڵ�parent �����������������������磬��ô���������һ���ڵ�
												(*).α���룺
												solveOverflow(BT_Node x)
												{
													if(x.key.size()>=_order)
													{
														//�����������
														using rank = unsigned;
														rank mi = x.key().size()/2;
														T value_mi = key[mi];

														BT_Node* new_node = BT_Node(x,mi+1,key.size()-1);
														// ��x.key[mi+1]~x.key[key.size()-1] �Լ� x.child[mi]~x.child[chiled.size()-1]
														// ��ʼ��new_node

														//ɾ����key��chid mi֮���ֵ
														for(int i = x.key().size()-1; i>=mi;i--)
														{
															x->key.pop();
															x->child.pop();
														}

														BT_Node* parnet = x->parnet;

														//�������1��xΪ���ڵ�,���ɽڵ㲢�ҷ��ؼ���
														if(!parent) 
														{
															_root = new BT_Node(value_mi,x,new);
															new_node -> parent = _root;
															_hot -> parent = _root;
															return;
														}

														//����value_mi������ parent��ȥ,������new_node;
														rank posi = parent->key.search(value_key);
														parent->key.insert(posi+1,value_key);
														parent->child.insert(posi+2,new);
														new_node->parnet = parent;

														//���Ҽ�������һ����
														solveOverflow(parent);
													}
												}


		3.3 ɾ�� erase(const T& t)				(1).�������� BT_Node* p = search(t);
												(2).��� p ������ֱ�ӷ��أ���� P Ϊ�������
												(3).�� P�ڵ��� �ҵ� t ��λ�� int pos = p.key.search(t);
												    Ϊ�˲�Ӱ��ṹ����ͣ�� �� Ѱ���������������µĽڵ� ������
												(4).��ʱpһ����Ҷ�ڵ㣬ֱ��ɾ��p.key[0],��ִ�� p->child.pop();
												(5).p��ɾ���������ܵ�������������������飬solveUnderflow(p);
												(*).α���룺
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
													//��t����Ҷ�ڵ㣬��ɾ��
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

		3.4 ��������		(1).���Ƚ����ж� �Ƿ������������ x->key.size() < ��order-1��/2; ���δ������ֱ�ӷ���
							(2).������������������ô�����ֽ���취���ֱ��ǣ� �ϲ�����ת�� ���ȳ�����ת���Ԫ��
							(2.1) �����������  �ҵ� �ڵ� x �����ҽڵ㣬�ֱ��� l��r;
												  l����r ���ڣ��� key.size()>=(order/2)+1;
												  ��ô�Ϳ���ͨ���������һ��Ԫ�أ������ڵ� x �������������硣
												  
												  a.�ҵ�l/r,��ȷ�Ͻ����Ԫ��ֵ�������l����ôvalue=l->key[0];
													�����r ��ô value = r->key[size()-1];
												  b.���� value �� parent �� ���ʵĽ���Ԫ��
												    int pos_parent = parent->key.search(vllue);
													��� �� l ��ô pos_parent = pos_parent + 1;
													��� �� r ��ô pos_parent ����ı�
												  c.��parent ���ڵ��� pos_parent��Ԫ���� value��������ҽ�parentԭ��ֵ�� x
													pos_value = parent->key[pos_parnet];
												    parent->key[pos_parnet] = value;

													����� l �����x->key.insert(0,pos_value);
													����� r �����x->key.push_back(pos_value);
												  d.��l����r�ڵ�� ֵɾ����
													erase(l,0) ���� erase(r,size()-1);
													##���ɾ�����ܵ������´���������ڵ����������
							(2.2) �ϲ�����취�� ���ڵ� x ������ l ���� r�ڵ㣬�Լ�l��r�ڵ�����������ʱ������Ҫ���кϲ�����
												 ˼·�� Ѱ����ߵ�l�ڵ㣬Ȼ���ٳ��� �ұߵ�r �ڵ㣬Ȼ���ҵ����ڵ��� ��Ӧ�� key[pos_parent]
												 �� parent->key[pos_parent]����������Ϊճ�ϼ�����l/r + parent->key[pos_parent] + x ����һ���½ڵ�
												 ���� ����parent.child

												 ##��� l/r key().size<(order-1)/2 ,�� x->key.size()<(order-1)/2
												 ##��ô l/r.key()+x.key()+1 һ�����ᷢ������������� �� (order-1�� ���
												 ## (order-1)/2*2 >= l.key().size() * 2
												 ## (order-1)/2*2 >= l.key().size() + (x.key.size()+1
												 ## �ù�ʽ�������½ڵ㲻������

												 a.��parent�ڵ��У�Ѱ�Ҷ�Ӧ�ĸ��ڵ�λ�ã� int pos_parent = parent->key.search(x->key[0])
												   l ����� value_parent = parent[pos_parent+1]
												   r ����� value_parent = parent[pos_parent];
												 b. ���� value_parent�½��� ��Ϊճ�ϼ� l/r �Լ� x �ڵ�ճ��������
												    �����l�ڵ��������ô�� new_node.key = l.key()+value_parent + x.key()
													�����r�ڵ��������ô�� new_node.key = x.key()+value_parnet+r;
													����Ӧ�ϲ� child ����
												 c.��parent�У�����Ӧ�� parnet->child[pos_parent]/parent->child[pos_paretn+1]�޳�
												 d.��Ϊparent ����ֵ������1����������Ҫ�ڶ� parent ���м���Ƿ�����
													solveUnderflow(parent);
												## �����������x.key.size==0.�Ҳ�����l��r�ڵ��ʱ��ֱ��ɾ���ڵ�x������parent�ж�Ӧchild��Ϊ��
												## �������2����x.key.size()==0 �� x = _root ʱ�����⴦�����̣�
												   a.Ѱ��x�Ƿ����ӽڵ㣬_root_lc = x->child[0] ����� _root_lcΪNULL��˵�����Ѿ��գ�ɾ��_root����
												   b.������� _root_lc ,��ô�� x-> key[0] �½����ڵ� _root_lc + x + _root_rc �У�����������ײ㣬��ɾ��
												     �ټ���Ƿ������

												(*) α���룺
												solveUnderflow(x)
												{
													���û�����磬��ô����
													if(x->key.size()>(order-1)/2) return ;
													//�������xΪ���ڵ㣬������ˣ���_root��Ϊ�գ�����������
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

													//��parent���ҵ� l��r
													for(pos_x=0;i<parent.child.size();i++)
													{
														if(parnet.child[pos_x]== x)
														{
															if(i-1>=0) l = parent->child[i-1];
															if(i+1<parent.child.size()) r = parent->child[i+1]
														}													
													}

													//���������x.size()==0��
													if(x->key.size()==0&&!l&&!r)
													{
														// l �� r �������ڣ�ֱ�Ӱѽڵ�ɾ��
														delete(x);
														parent[pos_x] = NULL;
														return;
													}
													//������ʽ���̣��ȳ�����ת,������ұ�

													//��ڵ���ڣ��ҿ�����ת���
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
													if(r&&r->key.size()>(roder-1)/2) //�ҽڵ���ڣ��ҿ�����ת���
													{
														pos_parent = parent->key.search(r->key[0]);
														value_par = parent->key[pos_parent];
														parent->key[pos_parent] = r.key[0];
														x->key.push_back(value_par);
														x->child.push_back(NULL);
														erase(r,0);
														return;
													}

													//��ô�������Ͳ�����
													if(l)
													{
														//ȷ�����ڵ��кϲ���ֵ
														pos_parent = parent->key.search(x->key[0]);
														value_par = paretn->key[pos_parent];
														l->key.push_back( value_par);
														//�ѽڵ�ϲ�
														for(int i=0;i<x->key.size();i++)
														{
															l->key.push_back(x->key[i]);
															l->child.push_back(x->child[i];
														}
														//ɾ������鸸�ڵ�
														delete(x);
														x = NULL;
														parent->key.erase(pos_parent);
														parent->child.erase(pos_parent+1);
														solveUnderflow(parent);
														return;
													}
													else
													{
														//ȷ�����ڵ��кϲ���ֵ
														pos_parent = parent->key.search(r->key[0]);
														value_par = paretn->key[pos_parent];
														x->key.push_back( value_par);
														for(int i=0;i<x->key.size();i++)
														{
															x->key.push_back(x->key[i]);
															x->child.push_back(x->child[i];
														}
														//ɾ���ڵ㲢��鸸�ڵ�
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

	void out() //���㼶�����
	{
		queue<BTNodePosi<T>> q, q_line;
		q.push(_root);
		int j=1;
		int z = 0;
		/*
			������ĺ���-���ղ㼶�����
			1.ʹ����������������㼶 �ֱ� q �ܶ��У�q_line �㼶����
			2.���ڵ�ѹ����ڵ���ж���һ���㼶�ж�����Ч�ڵ㣬������Щ�ڵ��q_line
			3.q_line �����
			4.q ���ƻ��з�
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

						//���ӽڵ㲻ƥ����
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
			cout << endl;//��ʱjΪ��һ����Ч�ڵ���

		}
		cout << "total : " << z << endl;
	}

protected:
	BTNodePosi<T> _root;
	BTNodePosi<T> _hot;
	int _size;     //
	int _order;    //�״� ��_order-1 = max[child->size()]
};

template<typename T>
BTNodePosi<T> b_tree<T>::search(const T& t)             //�ɹ���return x,ʧ����return NULL
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

//�����߼��� 
//1.����seach 
//2.�ж�Ԫ���Ƿ���� ->  auto x = search(t);   auto i = x->key.search(t);
//                       if(x->key[i]==t) ����,ֱ��return ;  else  ������뻷��
//3.�������         ->  if(!x) ����_hot ���в��룬else x����
//                       x/_hot ��ʱһ����Ҷ�ڵ���߿սڵ�
//                   ->  rank i = x->key.search(t) ; x->key.insert(i,t); x->child.insert(i+1,NULL);
//                   ->  solveOverflow �ж��Ƿ񳬽״�
//                       if(x->key->capcity() >= _order) ���ģ���� ,��Ҫ����
template<typename T>
bool b_tree<T>::insert(const T& t)
{
	int pos;
	if (!_root)
	{
		_root = new BT_Node<T>(t, BTNodePosi<T>(NULL), BTNodePosi<T>(NULL)); //�����������������_root
		_size++;
		return true;
	}
	BTNodePosi<T> x = search(t);
	//�ڵ����ֱ�ӷ���
	if (x) return false;

	pos = _hot->key.search(t);
	_hot->key.insert(pos + 1, t);
	_hot->child.push_back(NULL);

	solveOverflow(_hot);
	_size++;


	return true;
}

/*
����Ƿ��������
1. ���_hot->key.size() �Ƿ���� order;��������������һ��
2. �ҵ��м��Ԫ�� mi,�����ݷֳ������֣�l ,r 
3. ��mi��������һ�㣬��������l��r
*/
template<typename T>
void b_tree<T>::solveOverflow(BTNodePosi<T> _hot)
{

	if (_hot->key.size() > _order-1)
	{
		int mi = (_hot->key.size() / 2);

		//�½ڵ㿽��mi~size()�� key ��child
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
			//������� �ڵ�Ϊ_root;
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
		     
		// ��һ�ڵ����ȷ����λ�� ��praen_pos֮��
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
ɾ���߼���
1.seach(t) ����Ԫ���Ƿ�������У�������������ɾ������
2.ɾ�������̣� ������������key���ҵ� t��λ��pos��Ȼ��t��child[pos+1][0] ���� 
               ֱ��t����ײ㣬�� !child[pos+1],Ȼ�� ִ�� key.erase(t) child.pop_back()
3.���ڵ��Ƿ���������
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
		//������� x Ϊ���ڵ� �� size = 1 ɾ���ڵ㼴��
		if (_size == 1)
		{
			delete(_root);
			_root = NULL;
			return true;
		}
		//�������2 x Ϊ���ڵ��� ֻ��һ����
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
			//���ˣ�ԭ����_root�Ѿ��½��ɹ������������������ɣ�������Ϊ_root��size���꣬��Ҫ����һ����������

			pos = x->key.search(t);
			BTNodePosi<T> c = x->child[pos + 1];
			if (c)
			{
				while (c)
				{
					_hot = c;
					c = c->child[0];
				}
				//���� x ΪҶ�ڵ�
				x->key[pos] = _hot->key[0];
				_hot->key.erase(0);
				_hot->child.pop();
				solveUnderflow(_hot);
				solveOverflow(_root);
				return true;
			}
			else
			{
				//x ΪҶ�ڵ����
				x->key.erase(pos);
				x->child.pop();
				solveUnderflow(x);
				solveOverflow(_root);
				return true;
			}
			
		}
			
	}

	//һ���������x[pos]��succ(x) ������Ȼ��ɾ��
	pos = x->key.search(t);
	BTNodePosi<T> c = x->child[pos+1];
	
	if (c)
	{
		while (c)
		{
			_hot = c;
			c = c->child[0];
		}
		//���� x ΪҶ�ڵ�
		x->key[pos] = _hot->key[0];
		_hot->key.erase(0);
		_hot->child.pop();
		solveUnderflow(_hot);
		return true;
	}
	else
	{
		//x ΪҶ�ڵ����
		x->key.erase(pos);
		x->child.pop();
		solveUnderflow(x);
		return true;
	}
}

/*
	������߼���
	�ڵ�x����ɾ���󣬿���x->key.size() < (_order/2)-2  
	Ϊ�˱�֤���ĸ߶��Լ������ʣ���Ҫ���� a.��ת b.�ϲ�

	���Ƚ���
	a.��ת����������������ҽڵ����һ���ڵ�
	b.�ϲ������������ڵ���ж�Ӧ�����ݽ���������Ϊճ�ϼ�����x������ҽڵ�ճ��
	  ���Ҽ�鸸�ڵ��Ƿ�����
*/
template<typename T>
void b_tree<T>::solveUnderflow(BTNodePosi<T> x)
{
	//û�з�������
	if (x->key.size() >= (_order-1) / 2) return;
	if (x == _root) return;//������������ڵ�
	
	BTNodePosi<T> parent = x->parent;
	BTNodePosi<T> l = NULL;
	BTNodePosi<T> r = NULL;
	int pos_parent = 0;
	int pos_x;

	//ȷ��l��r
	for (pos_x = 0; pos_x < parent->child.size();pos_x++)
	{
		if (parent->child[pos_x] == x)
		{
			if (pos_x - 1 >= 0) l = parent->child[pos_x - 1];
			if (pos_x + 1 <= parent->child.size() - 1) r = parent->child[pos_x + 1];
			break;
		}
	}

	// �ȳ���L �ڵ���ת, #ע��Ҫ�� l����r���ӽڵ�Ҳ�ӹ���
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
	else if (r && r->key.size() > (_order - 1) / 2) //Ȼ������r�ڵ���ת
	{
		pos_parent = parent->key.search(r->key[0]);
		x->key.push_back(parent->key[pos_parent]);
		x->child.push_back(r->child[0]);
		if(r->child[0]) r->child[0]->parent = x;

		parent->key[pos_parent] = r->key[0];
		
		r->key.erase(0);
		r->child.erase(0);
	}

	//�����ת���ɹ�������кϲ�
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

		//������� parentΪ_root,�ҽ���֮�� û������
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