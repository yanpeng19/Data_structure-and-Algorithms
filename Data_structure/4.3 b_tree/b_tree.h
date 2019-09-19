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
		2.3 solveOverflow()     //�����뵼�µģ��ڵ����紦��
		2.4 solveUnderflow()    //��ɾ�����µģ��ڵ����紦��
		
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
	int _order;    //�״� ��max[child->size()]
};

template<typename T>
BTNodePosi<T> b_tree<T>::search(const T& t)             //�ɹ���return x,ʧ����return NULL
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

//�����߼��� 
//1.����seach 
//2.�ж�Ԫ���Ƿ���� ->  auto x = search(t);   auto i = x->key.search(t);
//                       if(x->key[i]==t) ����,ֱ��return ;  else  ������뻷��
//3.�������         ->  if(!x) ����_hot ���в��룬else x����
//                       x/_hot ��ʱһ����Ҷ�ڵ���߿սڵ�
//                   ->  rank i = x->key.search(t) ; x->key.insert(i,t); x->child.insert(i+1,NULL);
//                   ->  solveOverflow �ж��Ƿ񳬽״�
//                       if(x->key->capcity() >= _order) ���ģ���� ,��Ҫ����
//4.��������������   ->  a.RANK i = m/2+(m%2)?0;1;
//                   ->  b.�� key[i] �� *this->parent �ڵ�
//                       c.new BTnode l(key=0->i-1,child=0->i-1)  new BTNode r(key=i+1->end child= i+1->end)
//                       d.ʹ��t�������  ���µõ���l,r���뵽 parent->key[i] parent->key[i+1]
//                       e.ѭ����� parent ֱ��_root
//                       f.��������� _root �ڵ����磬��Ҫ����һ���µĽڵ�    solveOverflow() {if(x==_root) _root= new BTNode(t,l,r)}
template<typename T>
bool b_tree<T>::insert(const T& t)
{
	int rank;
	if (!_root)
	{
		_root = new BT_Node<T>(t, BTNodePosi<T>(NULL), BTNodePosi<T>(NULL)); //�����������������_root
		_size++;
		return true;
	}
	BTNodePosi<T> x = _root;
	

	while (x)
	{
		rank = x->key.search(t); //rank ��Χ  -1~ key.size()
		if (!x->key.empty() && x->key[rank] == t) return false; //�ؼ��ʷǿ���ֵ��ͬ ˵����ֵ����������в���
		_hot = x;
		x = x->child[rank+1];
	}
	//���򵽴� ˵�� x Ϊ�ⲿ�ڵ㣬_hot�Ѿ����˺���λ�ã�ֱ����hot[rank]λ�ý��в���
	_hot->key.insert(rank+1, t);
	_hot->child.push_back(NULL);

	solveOverflow(_hot);
	_size++;

	return true;
}

/*
����Ƿ��������
1. ���_hot->key.size() �Ƿ���� order;��������������һ��
2. ���ѽڵ�_hot ʹ֮ size() ����,���ѵ� pos = size()/2 + 1�ڵ�
3. ԭ�ڵ� ���� key[pos+1]-key[size()-1] �Լ� child[pos] �� child[size()-1]
   �� �½ڵ��� n_node = new(BT_Node<T>),����ִ�и���һһ����
4. ɾ��ԭ�ڵ� key[pos]��֮������ݣ�ɾ��ԭ�ڵ� child (key.size()+1) ֮�������
5. ����һ��_hot->parent�� ���в���
   a.���_hot->parentΪ�� ��rank = _hot->parent->seach(t);BN
     child.insert(rank,n_node)
   b.���_hot->parenΪ��,��new(child[pos],*this,n_node) = _root;
*/
template<typename T>
void b_tree<T>::solveOverflow(BTNodePosi<T> _hot)
{

	if (_hot->key.size() > _order)
	{
		// pilt_pos �� _hot[spilt_pos] ����ڵ�,��0��ʼ����
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
			// ��һ�ڵ����ȷ����λ�� ��praen_pos֮��
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
ɾ���߼���
1.seach(t) ����Ԫ���Ƿ�������У�������������ɾ������
2.ɾ�������̣� ������������key���ҵ� t��λ��pos��Ȼ��t��child[pos+1][0] ���� 
               ֱ��t����ײ㣬�� !child[pos+1],Ȼ�� ִ�� key.erase(t) child.pop_back()
			    auto x = _root;
				_hot = NULL;
			   while(x)
			   {
			        pos = x->key.seach(t);
					if(x->key[pos]==t) //�ҵ���
					{
					    while(x->child[pos+1])
						{
						    x->key[pos] = x->child[pos+1]->key[0];
							_hot = x;
						    x = x->child[pos+1]
						}
						����x Ϊ�ⲿ�ڵ�
						_hot->key.erase(t);
						_hot->child.pop_back();
						return ture;
					}
					
					_hot = x;
					x = x->child[pos+1];
			   }
			   û�ҵ����� 
			   return false;
3.���ڵ��Ƿ���������
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
			//������� x Ϊ���ڵ� �� size = 1

			while (x->child[pos + 1])
			{
				x->key[pos] = x->child[pos + 1]->key[0];
				_hot = x;
				x = x->child[pos + 1];
			}
			//������� x ΪҶ�ڵ�
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
	������߼���
	�ڵ�x����ɾ���󣬿���x->key.size() < (_order/2)-2  
	                     x->child.size() < (_order/2)-1;
	if(x->key.size < (_order/2)-2) ;

	Ϊ�˱�֤���ĸ߶��Լ������ʣ���Ҫ���� a.��ת b.�ϲ�

	���Ƚ���a.��ת����������������ҽڵ�
	��l,r && l.size()>=_roder/2)
	parent = x->parent;
	pos_parent = parent->key.search(x->key[0])  //  �ҵ����ڵ��� X��λ��
	Ȼ��l[size()-1] ���� r[0] �� parent->key[pos] ������ֵ,����
	parent->key[pos] ���뵽 x->key�� ͷ������β��
	x->child.push_back(NULL);
	
	������ҽڵ� �����ڣ�����key.size() ��λ����������Ҫ���кϲ�������
	���ڵ�x���ڸ��ڵ��Ӧ��/�� �ڵ���кϲ�������key[pos] �³��� x�У�
	ʹx->key.size() >(_order/2)-2;

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
	//������� x Ϊ keyΪ0
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

	//�ȳ�����ת
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