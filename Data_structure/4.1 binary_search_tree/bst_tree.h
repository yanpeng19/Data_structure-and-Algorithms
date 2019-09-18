/*
	���������� Binary Search Tree
	----------------------------------------------------------------------
	0.�ص㣺
		ÿ���ڵ�Ķ��ǡ�ֵ����С������֯������Ϊ x.lChild < x , x.rChild > x
		���ԣ��ڶ����������У����Խ�����Ч�ʵ���������������Ч��Ϊ O(logn��
		Node Ϊ�ڵ���
	
	0.5 ���������� 
		��Ϊ���ṹ�洢���� Node* ����ָ���Σ���Ҫͨ���������нڵ㣬�����ͷ�
		ÿ���ڵ��ڴ棬����������

	1.���ܺ�����
		1.1. insert()     ����ֵ/�ڵ�
		1.2. erase()      ɾ��ֵ/�ڵ�
		1.3. seach()      �����ض�ֵ��Ԫ��λ��

		1.4  Node* root   ���ڵ� ����Ϊ��
		1.5  Node* _rot   �������,������¼����ʱ�򣬽ڵ������һ��֮ǰ��λ�ã��Ա���в������
		1.6  size         ���Ľڵ�����

	2.�����߼���

		2.1 �����߼���	  
		Node* seach(const T& t)
		{
			Node* x = _root;
			Node* _hot = NULL;
			while(x)
			{
				if( x.data == t ) return x;//�ҵ��ڵ�

				//�������������ҷ�֧������¼��ǰ�ڵ�λ��
				_hot = x;
				if(t>x.data) x = x.lChild;
				else t = x.rChlid;
			}
			return NULL; //û���ҵ�
		}

		2.2 �����߼�: �����ǻ��ڲ��ҹ����Ͻ��еģ���Ҫһ������ Node* _hot,������¼�ڵ������һ��֮ǰ��λ�ã�
			          Ŀ����Ϊ�˵����ҽڵ㲻�����ǣ�������λ �����λ�á�
					  ������ֵ������ʱ���ڵ�xһ���ᵽ�������ⲿ��

			���������a.��Ϊ����         -> ֱ�Ӳ��뵽���ڵ�
					  b.���д��ڲ���ֵ   -> ������Ϊ�����߶Խڵ������Զ���������������
					  c.���в����ڲ���ֵ -> �����½ڵ㲢����

		Node* insert(const T& t)
		{
			if(!_root) _root = new Node(t);  //û�и��ڵ�����������������ڵ���Ϊ���ڵ�
			Node* x = seach(t);              //Ѱ�Ҳ���λ��
			if(_root&&x) return NULL;        //�ڵ���ڣ�����ֱ�ӷ��أ������Զ���һЩ�������������ӽڵ�����ļ�������
			else                             //xΪ�գ���ʾ����û�иýڵ㣬����һ���µĽڵ������hot������
			{	
				Node* posi = new Node(t);

				//���Ӳ���ڵ�͸��ڵ�
				if(t > _hot->lChlid) _hot->lChild = posi;
				else _hot->rChlid = posi;
				return posi;
			}
		}

		3.3 ɾ���߼������������в���ɾ��ֵ t �Ƿ���ڣ���������ڣ�ֱ�ӷ��ؼ��ɣ�
					  �����ڣ������ ɾ���ڵ� x �ķǿ��ӽڵ����������в���,
					  ͬ�������������

					  a. x û���ӽڵ�      -> ֱ�ӽ���ɾ���������� _hot ָ�� x �ķ�֧��Ϊ��
					  b. x ��һ���ӽڵ�    -> ɾ���ڵ�x�������� _hot ָ�� x ���ӽڵ�
					  c. x �������ӽڵ�    -> Ѱ�� x �ڵ����������µ� ��һ���ӽڵ� p��Ҳ�����к�xֵ��ӽ���һ���ڵ㣩,
					                          ��ʱpע����û�����ӽڵ�ģ����� x �� p ���ߵ�ֵ swap��x,p)
											  ��ʱ���ڵ� p->data = t; _hot = p->parent;
											  ��� p û���ӽڵ㣬��ôֱ��ɾ�� p , ���� p �ҽڵ��� _hot���� ���� ɾ��p

		α�����߼���
		void erase(const T& t)
		{
			//���ּ������
			if(!_root) return ;
			if(t == _root->data)
			{
				delete _root;
				_root = NULL;
			}

			Node* x = seach(t);
			if(!x->lChild&&!x->rChild) // ɾ���ڵ�û�к���
			{
				if(p == _hot->lChild) _hot->lChild = NULL;
				else _hot->rChild = NULL;
				delete p;
			}
			else if(x->lChild && x->rChild) //ɾ���ڵ�����������
			{
				Node* p = x;
				while (p->lChild) 
					p = p->lChild;

				swap(x,p);
				_hot = p->parent;

				if(!p->lChild)
				{
					//�ڵ�p�������Һ��ӣ�ֱ��ɾ���͵���_hot��ϵ
					if(p ==_hot->lChild) _hot->lChild = NULL;    //������pֻ���� ��_hot ������
					else _hot ->rChild = NULL;
					delete p;
				}
				else
				{
					if(p==_hot->lChild) _hot->lChild = p->rChild;
					delete p ;
				}
			}
			else     //ɾ���ڵ��һ������
			{
				Node* p = _hot->lChild ? _hot->lChild : _hot->rChild;
				if(x == _hot->lChild) _hot->lChild = p;
				else _hot->rCHild = p;
				p ->parent = _hot;
				delete p;
			}
		}
	----------------------------------------------------------------------
*/

#pragma once
#include "my_stack.h"

using namespace std;

template<typename T> class bst_BinNode;

template<typename T> using bst_BinNodePosi = bst_BinNode<T>* ;

template<typename T> class bst_tree;

template<typename T>
class bst_BinNode 
{
	template<typename B> friend void swap_bst_node(bst_BinNodePosi<B> a, bst_BinNodePosi<B> b);
	template<typename A, typename VST> friend void visitAlongLeftNode(bst_BinNodePosi<A>, VST visit, my_stack<bst_BinNodePosi<A>> &s);
	template<typename T2> friend class bst_tree;
public:
	bst_BinNode() = default;
	bst_BinNode(const T& t, bst_BinNodePosi<T> par) : data(t),parent(par)//value ��ʱ��string ��֯
	{
		stringstream ss;
		ss << t;
		value = t;
	}
	bst_BinNode(const T& t) :bst_BinNode(t, NULL) {}; //value ��ʱ��string ��֯

	bool operator >(const bst_BinNode&b) { return value > b.value; };
	bool operator <(const bst_BinNode&b) { return value < b.value; };
	bool operator ==(const bst_BinNode&b) { return value == b.value; };
	bool operator !=(const bst_BinNode&b) { return value != b.value; };
	bool operator <=(const bst_BinNode&b) { return value > b.value; };
	bool operator >=(const bst_BinNode&b) { return value < b.value; };

	bool operator >(const bst_BinNodePosi<T> b) { return value > b->value; };
	bool operator <(const bst_BinNodePosi<T> b) { return value < b->value; };
	bool operator ==(const bst_BinNodePosi<T> b) { return value == b->value; };
	bool operator !=(const bst_BinNodePosi<T> b) { return value != b->value; };

	
	bst_BinNodePosi<T> succ();
	T& get_value() { return value; };
	bool operator=(bst_BinNodePosi<T> b)
	{
		data = b->data;
		value = b->value;
		lChild = b->lChild;
		rChild = b->rChild;
		parent = b->parent;
	}
	
	T data;
	string key;
	T value;
	bst_BinNodePosi<T> lChild;
	bst_BinNodePosi<T> rChild;
	bst_BinNodePosi<T> parent;
};


template<typename T>
class bst_tree
{
public:
	bst_tree() = default;
	bst_tree(const T& t):_root(new bst_BinNode<T>(t,NULL)){};
	void LDR(void(*f)(bst_BinNodePosi<T>)); //������� ��ROOT��ʼ

	bst_BinNodePosi<T> seach(const T& t);
	bst_BinNodePosi<T> insert(const T&t);
	bst_BinNodePosi<T> erase(const T&);
	static void free(bst_BinNodePosi<T> t)
	{
		delete t;
	}

	~bst_tree()
	{
		LDR(free);
	}

	bst_BinNodePosi<T> _root; // ��
	bst_BinNodePosi<T> hot;
	int size;
};

template<typename T>
void bst_tree<T>::LDR(void(*f)(bst_BinNodePosi<T>)) //������� ����ִ��
{
	TravPre(_root, f);
}

//��������
template<typename T, typename VST>
void visitAlongLeftNode(bst_BinNodePosi<T> x, VST visit, my_stack<bst_BinNodePosi<T>> &s)
{
	while (x)
	{
		auto l = x->lChild;
		auto r = x->rChild;
		visit(x);
		s.push(r);
		x = l;
	}
}

template<typename T, typename VST>
void TravPre(bst_BinNodePosi<T> x, VST visit)
{
	my_stack<bst_BinNodePosi<T>> s;
 	visitAlongLeftNode(x, visit, s);
	while (!s.empty())
	{
		x = s.back();
		visitAlongLeftNode(x, visit, s);
	}
}
//�������� ����

template<typename T>
bst_BinNodePosi<T> bst_tree<T>::seach(const T&t)
{
	if (!_root) return _root;
	auto x = _root;
	hot = NULL;

	while(x)
	{
		if ( t == x->get_value()) return x;
		else if ( t > x->get_value())
		{
			hot = x;
			x = x->rChild;
		}
		else
		{
			hot = x;
			x = x->lChild;
		}
	}
	return x;   //
}

template<typename T>
bst_BinNodePosi<T> bst_tree<T>::insert(const T&t)
{
	
	bst_BinNodePosi<T> posi = seach(t);
	if (posi&&posi!=_root) return NULL;

	size++;
	posi = new bst_BinNode<T>(t, hot);
	
	if (!hot) return _root = posi;

	if (posi->get_value() > hot->get_value()) 
		hot->rChild = posi;
	else
		hot->lChild = posi;

	return posi;
}

template<typename T>
bst_BinNodePosi<T> bst_tree<T>::erase(const T&t)
{
	bst_BinNodePosi<T> posi = seach(t);
	if (!posi) return NULL;
	size--;

	if (!(posi->lChild) && !(posi->rChild))
	{
		bst_BinNode<T> temp(posi->data,NULL);
		if (hot)
		{
			if (t > hot->get_value())
				hot->rChild = NULL;
			else hot->lChild = NULL;
		}
		
		delete posi;
		if (!hot)
			posi = _root = NULL;
		return &temp;
	}

	else if(posi->lChild && posi->rChild)
	{
		auto p = posi->succ();
		bst_BinNode<T> temp(posi->data, NULL);

		//ɾ�����ڵ�ʱ��Ϊ��hot
		if(hot)
		{
			if (posi == hot->lChild)
			{
				hot->lChild = p;
			}
			else hot->rChild = p;
		}
		
		swap_bst_node(posi, p);

		if (p->rChild)
		{   
			p->parent->rChild = p->rChild;
		}

		delete p;
		return &temp;
	}

	else
	{
		bst_BinNode<T> temp(posi->data, NULL);
		bst_BinNodePosi<T> a = posi->rChild ? posi->rChild : posi->lChild;
		a->parent = hot;
		if (!hot) _root = a;
		else if (posi->get_value() > t) hot->rChild = a;
		else hot->lChild = a;
		delete posi;
		return &temp;

	}

}

template<typename T>
bst_BinNodePosi<T> bst_BinNode<T>::succ()
{
	auto posi = rChild;
	while (posi->lChild)
		posi = posi->lChild;
	return posi;
}

template<typename T>
void swap_bst_node(bst_BinNodePosi<T> a, bst_BinNodePosi<T> b) //ֻ���� data��value ������λ�ü� parent,lc,rc
{
	auto data = a->data;
	auto value = a->value;
	a->data = b->data;
	a->value = b->value;
	b->data = data;
	b->value = value;
}